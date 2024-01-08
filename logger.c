/*
 * logger.c
 *
 *  Created on: Dec 26, 2023
 *      Author: İlyas Başaran
 */

#include "logger.h"

/**
 * @brief This variable represents the maximum length that can be logged.
 */
#define LOGGER_BUFFER_MAX_LENGTH                                						256

/**
 * @brief Size of the application name buffer.
 *
 * @details This constant defines the size of the character array used to store
 *          the application name. Adjust this value based on the maximum length
 *          your application supports for the application name.
 */
#define APP_NAME_SIZE 50

/**
 * @brief Contains the string form of the logs.
 *
 * The constant used by @ref LOG when the user wants to print log.
 */
static char logger_buffer[LOGGER_BUFFER_MAX_LENGTH];

/**
 * @brief Current log level for the application.
 *
 * @details This global variable represents the current log level for the application.
 *          It is initialized to WARN, indicating that log messages with a level
 *          of WARN or higher will be printed by default.
 */
logger_levels_t currentLogLevel = WARN;

/**
 * @brief Buffer to store the application name.
 *
 * @details This character array is used to store the application name. The size of
 *          the array is determined by the APP_NAME_SIZE constant. The array is
 *          initialized with the default value "MyApp". Adjust the size and initial
 *          value based on your application requirements.
 */
static char app_name[APP_NAME_SIZE] = "MyApp";

/**
 * @brief Function pointer to obtain the elapsed time in milliseconds.
 *
 * @details This function pointer is used to hold the address of a function that returns
 *          the elapsed time in milliseconds as a floating-point value (float).
 *          The function is typically set by calling LoggerRegisterAppFunctions.
 */
static GetMillisecondsFunction pfGetMilliseconds = NULL;

/**
 * @brief Function pointer to print formatted log messages.
 *
 * @details This function pointer is used to hold the address of a function that prints
 *          formatted log messages. It takes a pointer to a buffer (uint8_t*) and the
 *          length of the buffer (uint8_t) as parameters. The function is typically set
 *          by calling LoggerRegisterAppFunctions.
 */
static LoggerPrintfFunction pfLoggerPrintf = NULL;

/**
 * @brief   Get the elapsed time in milliseconds.
 *
 * @details This function retrieves the elapsed time in milliseconds through the function
 *          pointer pfGetMilliseconds. If pfGetMilliseconds is set (non-NULL), it calls
 *          the corresponding function to obtain the elapsed time; otherwise, it returns
 *          a default error value of -1.0f.
 *
 * @return  The elapsed time in milliseconds, or -1.0f if the function pointer is not set
 *          or the corresponding function fails to provide a valid value.
 */
__inline float GetMilliseconds(void)
{
    if (pfGetMilliseconds != NULL) {
        return pfGetMilliseconds();
    }
    return -1.0f;
}

/**
 * @brief   Print a formatted log message.
 *
 * @details This function prints a formatted log message using the function pointer
 *          pfLoggerPrintf. If pfLoggerPrintf is set (non-NULL), it calls the corresponding
 *          function to print the log message; otherwise, it does nothing.
 *
 * @param[in] p   A pointer to the buffer containing the log message.
 * @param[in] len The length of the log message buffer.
 */
__inline void LoggerPrintf(const uint8_t* p, uint8_t len)
{
    if (pfLoggerPrintf != NULL) {
        pfLoggerPrintf(p, len);
    }
}

/**
 * @brief   Register application-specific functions for logging and information retrieval.
 *
 * @details This function is used to register application-specific functions for information
 *          retrieval and logging. It takes function pointers to the GetMilliseconds and
 *          LoggerPrintf functions as parameters and assigns them to
 *          corresponding internal function pointers.
 *
 * @param[in] pGetMilliseconds      A function pointer to obtain the elapsed time in milliseconds.
 * @param[in] pLoggerPrintf         A function pointer to print a formatted log message.
 */
void LoggerRegisterAppFunctions(GetMillisecondsFunction pGetMilliseconds, LoggerPrintfFunction pLoggerPrintf)
{
    pfGetMilliseconds = pGetMilliseconds;
    pfLoggerPrintf = pLoggerPrintf;
}

/**
 * @brief   Get the application name.
 *
 * @details This function retrieves the application name through the global
 *          character array `app_name`. It returns a pointer to a constant
 *          character string representing the application name.
 *
 * @return  A pointer to a constant character string representing the application name.
 */
const char* GetAppName(void) {
    return app_name;
}

/**
 * @brief   Set the application name.
 *
 * @details This function sets the application name using the provided string `appName`.
 *          It copies the string into the global character array `app_name`, ensuring
 *          it does not exceed the maximum size defined by `sizeof(app_name) - 1`.
 *          The function null-terminates the array to ensure proper string termination.
 *
 * @param[in] appName   A pointer to the string representing the new application name.
 */
void SetAppName(const char* appName) {
    // Copy the provided string into the global character array, limiting the size.
    strncpy(app_name, appName, sizeof(app_name) - 1);

    // Null-terminate the array to ensure proper string termination.
    app_name[sizeof(app_name) - 1] = '\0';
}

/**
 * @brief   Get the current log level.
 *
 * @details This function retrieves the current log level from the global variable
 *          `currentLogLevel` and returns it. If the variable is not set, it defaults
 *          to the debug log level (DBG).
 *
 * @return  The current log level, typically of the type logger_levels_t.
 */
logger_levels_t GetCurrentLogLevel(void) {
    // Retrieve the current log level from the global variable.
    return currentLogLevel;
}

/**
 * @brief   Set the current log level.
 *
 * @details This function sets the current log level to the specified value `level`
 *          in the global variable `currentLogLevel`.
 *
 * @param[in] level   The new log level to be set, typically of the type logger_levels_t.
 */
void SetCurrentLogLevel(logger_levels_t level) {
    // Set the current log level to the specified value.
    currentLogLevel = level;
}

/**
 * @brief   Retrieve the current content of the logger buffer.
 *
 * @details This function copies the current content of the logger buffer into
 *          the provided buffer. The logger buffer stores formatted log messages
 *          and related information.
 *
 * @param[out] buffer   A pointer to the buffer where the logger content will be copied.
 */
void GetLoggerBuffer(char *buffer) {
    // Copy the content of the logger buffer into the provided buffer.
    memcpy(buffer, &logger_buffer, sizeof(logger_buffer));
}

/**
 * @brief   Extracts the file name from a given file path.
 *
 * @details This function takes a file path as input and identifies the last occurrence
 *          of '/' or '\\' to determine the file name. It returns a pointer to the substring
 *          containing the file name.
 *
 * @param[in] path A pointer to the file path string.
 *
 * @return  A pointer to the substring containing the file name, or the original path if
 *          no separator ('/' or '\\') is found.
 */
static const char* GetFileNameFromPath(const char* path) {
    // Find the last occurrence of '/' or '\\' in the path
    const char* lastSlash = strrchr(path, '/');
    const char* lastBackslash = strrchr(path, '\\');

    // Choose the last occurrence as the separator
    const char* lastSeparator = (lastSlash > lastBackslash) ? lastSlash : lastBackslash;

    // If a separator is found, return the substring after it; otherwise, return the whole path
    return (lastSeparator != NULL) ? lastSeparator + 1 : path;
}

#if defined(LOGGER_ENABLED)

/**
 * @brief Macro for adding an entity to the description array.
 *
 * Macro that helps to create a single entity in the description array.
 */
#define LOGGER_ENTITY(mnemonic)                                							{.level=mnemonic, .name=#mnemonic}

/**
 *  Macro element that describes an log level. @ref LOGGER_ENTITY
 */
typedef struct{
    logger_levels_t level;                                                      		/**< Log level */
    char const * name;                                                                  /**< Log level name */
}logger_entity_t;

/**
 * @brief Array entity element that describes an log.
 */
typedef struct
{
    logger_entity_t entity;                                                     		/**< @ref ogger_entity_t */
    char const * color;                                                                 /**< Log level color code */
}logger_desc_t;

/**
 * @brief Array with log level codes.
 *
 * Array that describes log levels.
 *
 * @note Log levels do not need to be in ascending order in this string.
 *       This is checked in automated unit testing prior to release.
 */
static logger_desc_t const logger_array[] = {
    {
        .entity = LOGGER_ENTITY(DBG),
        .color = "\x1b[37;1m" ///< White color for DEBUG log level.
    },
    {
        .entity = LOGGER_ENTITY(INFO),
        .color = "\x1b[32;1m" ///< Green color for INFO log level.
    },
    {
        .entity = LOGGER_ENTITY(WARN),
        .color = "\x1b[33;1m" ///< Yellow color for WARN log level.
    },
    {
        .entity = LOGGER_ENTITY(ERR),
        .color = "\x1b[31;1m" ///< Red color for ERR log level.
    },
    {
        .entity = LOGGER_ENTITY(FATAL),
        .color = "\x1b[31;1m" ///< Red color for FATAL log level.
    }
};

/**
 * @brief This variable determines the level of the logs to be printed.
 */
logger_levels_t current_logger_level = DBG;

/**
 * @brief A literal string containing the color reset and newline characters.
 */
const char* RESET_NEWLINE = "\n\r\x1b[0m";

/**
 * @brief This function prints the string to be printed and the argument with the information of the place where it
 *        was printed.
 *
 * @param level         Log level
 * @param file          The name of the file in which the file is used.
 * @param function      The name of the file in which the function is used.
 * @param line          The name of the file in which the line is used.
 * @param fmt           The string to be printed.
 * @param ...           The argument to be printed.
 *
 */
void ZB_printf(int level, const char *file, const char *function, int line, const char *fmt, ...){
    // Check if the log will be printed.
    if (level >= GetCurrentLogLevel()){
    	// Clear buffer before write to log.
        memset(logger_buffer, 0x00, sizeof(logger_buffer));
        // Set the default log syntax.
        int length = snprintf(logger_buffer, sizeof(logger_buffer), "%s%s[%0.1f] : %s : %s : %s : %d -> ",
        		logger_array[level].color,
				GetAppName(),
				GetMilliseconds(),
				logger_array[level].entity.name,
				GetFileNameFromPath(file),
				function,
				line);
        va_list args;
        // Enables access to the variable arguments
        va_start(args, fmt);
        // Calculate the remaining space in the buffer after the initial log message.
        size_t size = sizeof(logger_buffer) - sizeof(RESET_NEWLINE) - length;
        // Check if there is space available in the buffer for additional characters.
        if (size > 0) {
            // Try to write the formatted log message to the buffer with the remaining space.
            int written = vsnprintf(logger_buffer + length, size, fmt, args);
            // Update the length with the actual number of characters written.
            if (written > 0) {
                // If more characters were written than the remaining space, update the length accordingly.
                if (written > size)
                    length += size;
                else
                    length += written;
            }
        }
        // Performs cleanup for an ap object initialized by a call to va_start().
        va_end(args);
        // Delete the color of the log level for the next log.
        snprintf(logger_buffer + length, sizeof(logger_buffer) - length, "%s", RESET_NEWLINE);
        // Print the all logs.
        LoggerPrintf((uint8_t *) logger_buffer, length + sizeof(RESET_NEWLINE));
	}
}

#elif defined(HARD_FAULT_LOGGER_ENABLED)

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/**
 * @brief   Log a message with file and line information.
 *
 * @details This function logs a message with information about the source file
 *          and line number where the log occurred. It appends the file name,
 *          line number, and a separator to the logger buffer. The stored logs
 *          in the buffer are managed to prevent overflow.
 *
 * @param[in] file   A pointer to the source file name.
 * @param[in] line   The line number in the source file.
 */
void ZB_printf(const char *file, int line) {
    const char separator[1] = "-";  ///< Separator character between file and line
    const int length_of_line = (line == 0 ? 1 : ((int)(log10(fabs(line)) + 1) + (line < 0 ? 1 : 0)));
    const int new_log_length = strlen(GetFileNameFromPath(file)) + length_of_line + sizeof(separator);

    int length_of_stored_log = (strrchr(logger_buffer, separator[0]) == NULL ? 0 : strrchr(logger_buffer, separator[0]) - logger_buffer + 1);

    // Clear buffer before saving the first log.
    if (length_of_stored_log == 0) {
        memset(logger_buffer, 0x00, sizeof(logger_buffer));
    }
    // Unhandled statement
    else if (length_of_stored_log > sizeof(logger_buffer) / sizeof(logger_buffer[0])) {
        memset(logger_buffer, 0x00, sizeof(logger_buffer));
        return;
    }
    // If there is not enough space for a new log in the buffer
    else if (new_log_length > (sizeof(logger_buffer) / sizeof(logger_buffer[0])) - length_of_stored_log) {
        do {
            int latest_stored_log_length = (strchr(logger_buffer, separator[0]) == NULL ? 0 : strchr(logger_buffer, separator[0]) - logger_buffer + 1);
            memcpy(logger_buffer, logger_buffer + latest_stored_log_length, (sizeof(logger_buffer) / sizeof(logger_buffer[0])) - latest_stored_log_length);
            memset(logger_buffer + (sizeof(logger_buffer) / sizeof(logger_buffer[0])) - latest_stored_log_length, 0x00, latest_stored_log_length);
            length_of_stored_log = (strrchr(logger_buffer, separator[0]) == NULL ? 0 : strrchr(logger_buffer, separator[0]) - logger_buffer + 1);
        } while (new_log_length > (sizeof(logger_buffer) / sizeof(logger_buffer[0])) - length_of_stored_log);
    }

    // Append the file name, line number, and separator to the logger buffer.
    snprintf(logger_buffer + length_of_stored_log, sizeof(logger_buffer) - length_of_stored_log, "%s%u%s", GetFileNameFromPath(file), line, separator);
}

#else

#endif
