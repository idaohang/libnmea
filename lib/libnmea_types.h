#ifndef LIBNMEA_TYPES_H
#define LIBNMEA_TYPES_H

#include <time.h>

/**
 * Represent coordinates as double-precision floating point values. Direction is
 * indicated using the sign: negative indicates south/west, whereas positive is
 * indicative of north/east.
 */
typedef double nmea_degree_t;

/**
 * Enumeration describing each of the message types available.
 */
typedef enum {
	kNMEAMessageUnknown = -1,
	kNMEAMessageGGA = 1, // fix data providing 3D position
	kNMEAMessageGSA, // GPS dilution of precision and active satellites
	kNMEAMessageGSV, // Satellites in view
//	kNMEAMessageRMC, // position, velocity, time (NMEA version)
//	kNMEAMessageGLL, // Geographic latitude and longitude (old version)
	kNMEAMessageVTG, // Velocity Made Good
} nmea_message_type_t;

/**
 * Error codes output by functions.
 */
typedef enum {
	kNMEAErrorNone = 0,
	kNMEAErrorTypeNotUnderstood,
	kNMEAErrorTooManyArguments,
	kNMEAErrorChecksumInvalid,
	kNMEAErrorSentenceInvalid,
	kNMEAErrorFieldMisformat
} nmea_error_t;

/**
 * Basic type encompassing a decoded NMEA message. This indicates the type of
 * the message, and serves as a preamble that all message structs.
 */
typedef struct {
	nmea_message_type_t type;
} nmea_message_t;

/**
 * GGA (essential fix data providing 3D location+accuracy) message
 *
 * Contains information on the UTC time (time_t), lat and long in degrees, fix
 * quality, number of satellites, DOP, altitude above MSL, height of geoid above
 * WGS84.
 */
typedef struct {
	nmea_message_type_t type;

	int utc; // seconds since midnight of today

	nmea_degree_t lat, lng;
	int quality;
	int satellites;
	float hdop;
	float altitude, geoid_height;
} nmea_message_gga_t;

/**
 * GSA (GPS Dilution of Precision and Active Satellites) message
 *
 * Contains information on the current fix state (none, 2D or 3D), satellite PRN,
 * as well as the position dilution  of precision, split in both horizontal and 
 * vertical components.
 */
typedef struct {
	nmea_message_type_t type;

	int fix_type; // 1 = none, 2 = 2D, 3 = 3D
	int satellite_prn[12];
	float pdop, hdop, vdop;
} nmea_message_gsa_t;

/**
 * GSV (Satellites In View) message
 *
 * These messages can be split into multiple parts, and indicate the satellite's
 * PRN, elevation and azimuth, as well as its SNR, for up to four satellites.
 */
typedef struct {
	nmea_message_type_t type;
	int msg_total, msg_num;
	
	int satellites_in_vies;

	struct {
		int prn;
		int elevation;
		int azimuth;
		int snr;
	} satellite[4];
} nmea_message_gsv_t;

/**
 * VTG (Velocity Made Good) message
 *
 * Provides information on the velocity of the receiver, in both knots as well
 * as kilometers; if the receiver supports it, it can also provide the measured
 * heading, both as a true and magnetic reference.
 */
typedef struct {
	nmea_message_type_t type;
	
	float course_true, course_magnetic;

	float h_speed_knots, h_speed_kmh;
} nmea_message_vtg_t;

#endif