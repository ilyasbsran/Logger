/*
 * logger.h
 *
 *  Created on: Dec 26, 2023
 *      Author: ilyas
 */

#ifndef LOGGER_LOGGER_H_
#define LOGGER_LOGGER_H_

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Level of logger
 */
typedef enum
{
    /**
     * Logs that are used for interactive investigation during development. These logs should primarily contain
     * information useful for debugging and have no long-term value.
     */
	DBG,
	/**
	 * Logs that track the general flow of the application. These logs should have long-term value.
	 */
	INFO,
	/**
	 * Logs that highlight an abnormal or unexpected event in the application flow, but do not otherwise cause the
	 * application execution to stop.
	 */
	WARN,
	/**
	 * Logs that highlight when the current flow of execution is stopped due to a failure. These should indicate a
	 * failure in the current activity, not an application-wide failure.
	 */
	ERR,
	/**
	 * Logs that describe an unrecoverable application or system crash, or a catastrophic failure that requires
	 * immediate attention.
	 */
	FATAL,
}logger_levels_t;

/**
 * @brief Function pointer type to obtain the elapsed time in milliseconds.
 *
 * @details This function pointer type is used to define the signature of a function
 *          that retrieves the elapsed time in milliseconds. The function should take
 *          no parameters and return a floating-point value (float).
 */
typedef float (*GetMillisecondsFunction)(void);

/**
 * @brief Function pointer type to print formatted log messages.
 *
 * @details This function pointer type is used to define the signature of a function
 *          that prints formatted log messages. The function should take a pointer
 *          to a buffer (const uint8_t*) and the length of the buffer (uint8_t) as parameters.
 *          The function does not return a value.
 */
typedef void (*LoggerPrintfFunction)(const uint8_t* p, uint8_t len);

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
float GetMilliseconds(void);

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
void LoggerPrintf(const uint8_t* p, uint8_t len);

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
void LoggerRegisterAppFunctions(GetMillisecondsFunction pGetMilliseconds, LoggerPrintfFunction pLoggerPrintf);

/**
 * @brief   Get the application name.
 *
 * @details This function retrieves the application name through the global
 *          character array `app_name`. It returns a pointer to a constant
 *          character string representing the application name.
 *
 * @return  A pointer to a constant character string representing the application name.
 */
const char* GetAppName(void);

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
void SetAppName(const char* appName);

/**
 * @brief   Get the current log level.
 *
 * @details This function retrieves the current log level from the global variable
 *          `currentLogLevel` and returns it. If the variable is not set, it defaults
 *          to the debug log level (DBG).
 *
 * @return  The current log level, typically of the type logger_levels_t.
 */
logger_levels_t GetCurrentLogLevel(void);

/**
 * @brief   Set the current log level.
 *
 * @details This function sets the current log level to the specified value `level`
 *          in the global variable `currentLogLevel`.
 *
 * @param[in] level   The new log level to be set, typically of the type logger_levels_t.
 */
void SetCurrentLogLevel(logger_levels_t level);

/**
 * @brief   Retrieve the current content of the logger buffer.
 *
 * @details This function copies the current content of the logger buffer into
 *          the provided buffer. The logger buffer stores formatted log messages
 *          and related information.
 *
 * @param[out] buffer   A pointer to the buffer where the logger content will be copied.
 */
void GetLoggerBuffer(char *buffer);

#if defined(LOGGER_ENABLED)

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
void LOG(int level, const char *file, const char *function, int line, const char *fmt, ...);

/**
 * @brief Macro for simplified logging with log level, file, function, and line information.
 *
 * @details This macro provides a convenient way to log messages with specified log levels.
 *          It uses the underlying LOG macro, passing the provided log level along with
 *          the current file name (__FILE__), function name (__FUNCTION__), and line number
 *          (__LINE__) information. Additional formatting and variable arguments are also supported.
 *
 * @param level Log level to be used for the message (e.g., DBG, INFO, WARN, ERR, FATAL).
 * @param ...   Variable arguments to be formatted and included in the log message.
 *
 * @note This macro internally calls the LOG macro and passes the relevant information.
 *       The format and usage are similar to the LOG macro.
 */
#define LOGGER(level, ...)  LOG(level, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)

#elif defined(HARD_FAULT_LOGGER_ENABLED)

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
void LOG(const char *file, int line);

/**
 * @brief This function directs the strings and arguments to be logged.
 */
#define LOGGER(level, ...)  LOG(__FILE__, __LINE__)

#else

/**
 * @brief This function does not direct the logs to be recorded so that no action is taken and they do not take up
 *        space in the memory.
 */
#define LOGGER(level, ...)

#endif

#endif /* LOGGER_LOGGER_H_ */
