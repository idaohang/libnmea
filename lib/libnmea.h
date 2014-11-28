/**
 * Main header for libnmea: prototypes of the publicly exposed message parsing
 * functions, as well as definitions of the relevant structures.
 *
 * Since the message type may not be known beforehand, these functions will
 * allocate memory for their structures, and return a pointer to this memory. It
 * is the responsibility of the caller to free() this memory once they are done
 * with using the structure.
 *
 * Strings passed into functions are NOT modified, nor deallocated. They are
 * copied to an internal buffer for parsing.
 */
#ifndef LIBNMEA_H
#define LIBNMEA_H

#include "libnmea_types.h"

/**
 * Determines the type of message from the given string buffer.
 */
nmea_message_type_t nmea_get_message_type(char *message);

/**
 * Parses a message. If the message could not be parsed, NULL is returned. The
 * return value be cast to the proper struct, based on the determined message
 * type. 
 *
 * If the message could not be parsed, and if the outError pointer is non-NULL,
 * its contents will be set to the appropriate error code.
 */
nmea_message_t *nmea_parse_message(char *message, nmea_error_t *outError);

#endif