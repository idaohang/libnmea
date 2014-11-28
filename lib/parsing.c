/**
 * Implementations of actual parsing functions. Their prototypes are defined in
 * libnmea.c.
 */
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "libnmea.h"

/**
 * Calculates the NMEA checksum. s should point to the message, directly after
 * the initial dollar sign.
 */
static int do_nmea_checksum(char *s) {
    int c = 0;
 
    while(*s)
        c ^= *s++;
 
    return c;
}

/**
 * Parses a GGA Message:
 *
 * $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
 * 0: UTC Time (hhmmss)
 * 1/2: Latitude + Direction
 * 3/4: Longitude + Direction
 * 5: Fix quality
 * 6: Satellites tracked
 * 7: Horizontal DOP
 * 8: Altitude above MSL
 * 9: Height of geoid (MSL) above WGS84
 * 10/11: DGPS (Ignored)
 * 12: Checksum
 */
nmea_message_gga_t* parse_gga(char *msg, nmea_error_t *outErr) {
	// allocate a struct pls
	nmea_message_gga_t *gga = calloc(1, sizeof(nmea_message_gga_t));

	// Split message with commas.
	char *firstValue = msg+7;
	char *pch = strtok(firstValue, ",");
	int i = 0;

	// Evaluate each token.
	while(pch != NULL) {
		switch(i) {
			// UTC: separate into hh, mm, and ss.
			case 0: {
				char hh[3] = {pch[0], pch[1], 0};
				char mm[3] = {pch[2], pch[3], 0};
				char ss[3] = {pch[4], pch[5], 0};

				// convert to integers
				int h = atoi(hh);
				int m = atoi(mm);
				int s = atoi(ss);

				// convert to secs
				gga->utc = s + (m * 60) + (h * 3600);

				break;
			}

			// latitude and longitude
			case 1:
				gga->lat = strtod(pch, NULL);
				break;
			case 3:
				gga->lng = strtod(pch, NULL);
				break;

			case 2:
				gga->lat *= (*pch == 'N') ? 1 : -1;
				break;
			case 4:
				gga->lng *= (*pch == 'E') ? 1 : -1;
				break;

			// Fix quality
			case 5:
				gga->quality = atoi(pch);
				break;

			// Satellites tracked
			case 6:
				gga->satellites = atoi(pch);
				break;

			// Horizontal DOP
			case 7:
				gga->hdop = strtod(pch, NULL);
				break;

			// Altitude
			case 8:
				gga->altitude = strtod(pch, NULL);
				break;
			case 9:
				gga->geoid_height = strtod(pch, NULL);
				break;

			default:
				break;
		}

		// go to the next token
		pch = strtok(NULL, ",");
		i++;
	}

	// Done.
	return gga;
}

/// Parses a GSA Message.
nmea_message_gsa_t* parse_gsa(char *msg, nmea_error_t *outErr) {
	return NULL;
}

/// Parses a GSV Message.
nmea_message_gsv_t* parse_gsv(char *msg, nmea_error_t *outErr) {
	return NULL;
}

/**
 * Parses a VTG Message:
 *
 * $GPVTG,054.7,T,034.4,M,005.5,N,010.2,K*48
 * 0: True heading
 * 1: Reference ("T")
 * 2: Magnetic Heading
 * 3: Reference("M")
 * 4: Ground Speed (Knots)
 * 5: Unit ("N")
 * 6: Ground Speed (km/h)
 * 7: Unit ("K")
 */
nmea_message_vtg_t* parse_vtg(char *msg, nmea_error_t *outErr) {
	// allocate a struct pls
	nmea_message_vtg_t *vtg = calloc(1, sizeof(nmea_message_vtg_t));

	// Split message with commas.
	char *firstValue = msg+7;
	char *pch = strtok(firstValue, ",");
	int i = 0;

	// Evaluate each token.
	while(pch != NULL) {
		switch(i) {
			// true and magnetic heading
			case 0:
				vtg->course_true = strtod(pch, NULL);
				break;
			case 2:
				vtg->course_magnetic = strtod(pch, NULL);
				break;

			// ground speed (knots and kmh)
			case 4:
				vtg->h_speed_knots = strtod(pch, NULL);
				break;
			case 6:
				vtg->h_speed_kmh = strtod(pch, NULL);
				break;

			default:
				break;
		}

		// Verify the checksum
		char *checksumStart = msg+1;

		char *checksumEnd = strchr(msg, '*');
		*checksumEnd = 0x00;

		int checksum = do_nmea_checksum(checksumStart);

		// read the actual checksum
		checksumEnd++;
		int parsed = strtol(checksumEnd, NULL, 16);

		if(parsed != checksum) {

		}

		// go to the next token
		pch = strtok(NULL, ",");
		i++;
	}

	// Done.
	return vtg;
}