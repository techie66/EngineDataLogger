/** @file cmdline.h
 *  @brief The header file for the command line option parser
 *  generated by GNU Gengetopt version 2.23
 *  http://www.gnu.org/software/gengetopt.
 *  DO NOT modify this file, since it can be overwritten
 *  @author GNU Gengetopt */

#ifndef CMDLINE_H
#define CMDLINE_H

/* If we use autoconf.  */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h> /* for FILE */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef CMDLINE_PARSER_PACKAGE
/** @brief the program name (used for printing errors) */
#define CMDLINE_PARSER_PACKAGE PACKAGE
#endif

#ifndef CMDLINE_PARSER_PACKAGE_NAME
/** @brief the complete program name (used for help and version) */
#ifdef PACKAGE_NAME
#define CMDLINE_PARSER_PACKAGE_NAME PACKAGE_NAME
#else
#define CMDLINE_PARSER_PACKAGE_NAME PACKAGE
#endif
#endif

#ifndef CMDLINE_PARSER_VERSION
/** @brief the program version */
#define CMDLINE_PARSER_VERSION VERSION
#endif

enum enum_verbose { verbose__NULL = -1, verbose_arg_NONE = 0, verbose_arg_ERROR, verbose_arg_WARN, verbose_arg_INFO, verbose_arg_DEBUG };

/** @brief Where the command line options are stored */
struct gengetopt_args_info
{
  const char *help_help; /**< @brief Print help and exit help description.  */
  const char *detailed_help_help; /**< @brief Print help, including all details and hidden options, and exit help description.  */
  const char *full_help_help; /**< @brief Print help, including hidden options, and exit help description.  */
  const char *version_help; /**< @brief Print version and exit help description.  */
  char * config_file_arg;	/**< @brief Configuration file..  */
  char * config_file_orig;	/**< @brief Configuration file. original value given at command line.  */
  const char *config_file_help; /**< @brief Configuration file. help description.  */
  char * output_file_arg;	/**< @brief Output file for CSV logging (default='/dev/null').  */
  char * output_file_orig;	/**< @brief Output file for CSV logging original value given at command line.  */
  const char *output_file_help; /**< @brief Output file for CSV logging help description.  */
  char * gpx_file_arg;	/**< @brief Output file for GPX logging (default='/dev/null').  */
  char * gpx_file_orig;	/**< @brief Output file for GPX logging original value given at command line.  */
  const char *gpx_file_help; /**< @brief Output file for GPX logging help description.  */
  const char *output_file_date_help; /**< @brief Insert date and time into output filename. help description.  */
  char * output_file_format_arg;	/**< @brief Format string of output CSV file.  */
  char * output_file_format_orig;	/**< @brief Format string of output CSV file original value given at command line.  */
  const char *output_file_format_help; /**< @brief Format string of output CSV file help description.  */
  char * gear_ratios_arg;	/**< @brief RPM/Speed ratios. Comma separated. (Eg. -g \"175,122,95,78,67\").  */
  char * gear_ratios_orig;	/**< @brief RPM/Speed ratios. Comma separated. (Eg. -g \"175,122,95,78,67\") original value given at command line.  */
  const char *gear_ratios_help; /**< @brief RPM/Speed ratios. Comma separated. (Eg. -g \"175,122,95,78,67\") help description.  */
  int weight_arg;	/**< @brief Weight of vehicle, in kg, to use for calculations. (default='300').  */
  char * weight_orig;	/**< @brief Weight of vehicle, in kg, to use for calculations. original value given at command line.  */
  const char *weight_help; /**< @brief Weight of vehicle, in kg, to use for calculations. help description.  */
  unsigned int v_min; /**< @brief Verbose output. Specify multiple times for increasing verbosity.'s minimum occurreces */
  unsigned int v_max; /**< @brief Verbose output. Specify multiple times for increasing verbosity.'s maximum occurreces */
  const char *v_help; /**< @brief Verbose output. Specify multiple times for increasing verbosity. help description.  */
  enum enum_verbose verbose_arg;	/**< @brief Set level of verbosity explicitly. 
  (Overrides -v)
 (default='ERROR').  */
  char * verbose_orig;	/**< @brief Set level of verbosity explicitly. 
  (Overrides -v)
 original value given at command line.  */
  const char *verbose_help; /**< @brief Set level of verbosity explicitly. 
  (Overrides -v)
 help description.  */
  const char *quiet_help; /**< @brief Suppress output. Same as --verbose=NONE (Overrides both -v and --verbose) help description.  */
  char * front_controls_arg;	/**< @brief Front controls device.  */
  char * front_controls_orig;	/**< @brief Front controls device original value given at command line.  */
  const char *front_controls_help; /**< @brief Front controls device help description.  */
  char * ignitech_arg;	/**< @brief Ignitech ignition device.  */
  char * ignitech_orig;	/**< @brief Ignitech ignition device original value given at command line.  */
  const char *ignitech_help; /**< @brief Ignitech ignition device help description.  */
  char * lc2_arg;	/**< @brief Innovate LC-2 device.  */
  char * lc2_orig;	/**< @brief Innovate LC-2 device original value given at command line.  */
  const char *lc2_help; /**< @brief Innovate LC-2 device help description.  */
  int lc2_delay_arg;	/**< @brief Delay before powering up LC-2. (default='15').  */
  char * lc2_delay_orig;	/**< @brief Delay before powering up LC-2. original value given at command line.  */
  const char *lc2_delay_help; /**< @brief Delay before powering up LC-2. help description.  */
  int lc2_pin_arg;	/**< @brief GPIO pin that controls power for LC-2 (default='26').  */
  char * lc2_pin_orig;	/**< @brief GPIO pin that controls power for LC-2 original value given at command line.  */
  const char *lc2_pin_help; /**< @brief GPIO pin that controls power for LC-2 help description.  */
  char * sleepy_arg;	/**< @brief I2C device to communicate with Sleepy Pi..  */
  char * sleepy_orig;	/**< @brief I2C device to communicate with Sleepy Pi. original value given at command line.  */
  const char *sleepy_help; /**< @brief I2C device to communicate with Sleepy Pi. help description.  */
  char * sleepy_addr_arg;	/**< @brief Address for Sleepy Pi. (default='0x04').  */
  char * sleepy_addr_orig;	/**< @brief Address for Sleepy Pi. original value given at command line.  */
  const char *sleepy_addr_help; /**< @brief Address for Sleepy Pi. help description.  */
  char * can_arg;	/**< @brief CAN device to bind to. CAN disabled if not set..  */
  char * can_orig;	/**< @brief CAN device to bind to. CAN disabled if not set. original value given at command line.  */
  const char *can_help; /**< @brief CAN device to bind to. CAN disabled if not set. help description.  */
  char * ignitech_dump_file_arg;	/**< @brief File to dump raw responses from Ignitech.  */
  char * ignitech_dump_file_orig;	/**< @brief File to dump raw responses from Ignitech original value given at command line.  */
  const char *ignitech_dump_file_help; /**< @brief File to dump raw responses from Ignitech help description.  */
  int ignitech_servo_as_iap_flag;	/**< @brief Treat servo reading as IAP. Requires calibration options. (default=off).  */
  const char *ignitech_servo_as_iap_help; /**< @brief Treat servo reading as IAP. Requires calibration options. help description.  */
  int ignitech_sai_low_arg;	/**< @brief Low kpa reading. Eg: 21.  */
  char * ignitech_sai_low_orig;	/**< @brief Low kpa reading. Eg: 21 original value given at command line.  */
  const char *ignitech_sai_low_help; /**< @brief Low kpa reading. Eg: 21 help description.  */
  int ignitech_sai_low_mv_arg;	/**< @brief Low mv value. Eg: 708.  */
  char * ignitech_sai_low_mv_orig;	/**< @brief Low mv value. Eg: 708 original value given at command line.  */
  const char *ignitech_sai_low_mv_help; /**< @brief Low mv value. Eg: 708 help description.  */
  int ignitech_sai_high_arg;	/**< @brief High kpa reading. Eg: 102.  */
  char * ignitech_sai_high_orig;	/**< @brief High kpa reading. Eg: 102 original value given at command line.  */
  const char *ignitech_sai_high_help; /**< @brief High kpa reading. Eg: 102 help description.  */
  int ignitech_sai_high_mv_arg;	/**< @brief High mv value. Eg: 4252.  */
  char * ignitech_sai_high_mv_orig;	/**< @brief High mv value. Eg: 4252 original value given at command line.  */
  const char *ignitech_sai_high_mv_help; /**< @brief High mv value. Eg: 4252 help description.  */
  float mount_offset_roll_arg;	/**< @brief Offset to zero imu reading.  */
  char * mount_offset_roll_orig;	/**< @brief Offset to zero imu reading original value given at command line.  */
  const char *mount_offset_roll_help; /**< @brief Offset to zero imu reading help description.  */
  float mount_offset_pitch_arg;	/**< @brief Offset to zero imu reading.  */
  char * mount_offset_pitch_orig;	/**< @brief Offset to zero imu reading original value given at command line.  */
  const char *mount_offset_pitch_help; /**< @brief Offset to zero imu reading help description.  */
  int roll_pitch_swap_flag;	/**< @brief Swap roll and pitch. Used depending on physical sensor orientation. (default=off).  */
  const char *roll_pitch_swap_help; /**< @brief Swap roll and pitch. Used depending on physical sensor orientation. help description.  */
  int test_mode_arg;	/**< @brief Secret test mode. Does not continue running (default='0').  */
  char * test_mode_orig;	/**< @brief Secret test mode. Does not continue running original value given at command line.  */
  const char *test_mode_help; /**< @brief Secret test mode. Does not continue running help description.  */
  
  unsigned int help_given ;	/**< @brief Whether help was given.  */
  unsigned int detailed_help_given ;	/**< @brief Whether detailed-help was given.  */
  unsigned int full_help_given ;	/**< @brief Whether full-help was given.  */
  unsigned int version_given ;	/**< @brief Whether version was given.  */
  unsigned int config_file_given ;	/**< @brief Whether config-file was given.  */
  unsigned int output_file_given ;	/**< @brief Whether output-file was given.  */
  unsigned int gpx_file_given ;	/**< @brief Whether gpx-file was given.  */
  unsigned int output_file_date_given ;	/**< @brief Whether output-file-date was given.  */
  unsigned int output_file_format_given ;	/**< @brief Whether output-file-format was given.  */
  unsigned int gear_ratios_given ;	/**< @brief Whether gear-ratios was given.  */
  unsigned int weight_given ;	/**< @brief Whether weight was given.  */
  unsigned int v_given ;	/**< @brief Whether v was given.  */
  unsigned int verbose_given ;	/**< @brief Whether verbose was given.  */
  unsigned int quiet_given ;	/**< @brief Whether quiet was given.  */
  unsigned int front_controls_given ;	/**< @brief Whether front-controls was given.  */
  unsigned int ignitech_given ;	/**< @brief Whether ignitech was given.  */
  unsigned int lc2_given ;	/**< @brief Whether lc2 was given.  */
  unsigned int lc2_delay_given ;	/**< @brief Whether lc2-delay was given.  */
  unsigned int lc2_pin_given ;	/**< @brief Whether lc2-pin was given.  */
  unsigned int sleepy_given ;	/**< @brief Whether sleepy was given.  */
  unsigned int sleepy_addr_given ;	/**< @brief Whether sleepy-addr was given.  */
  unsigned int can_given ;	/**< @brief Whether can was given.  */
  unsigned int ignitech_dump_file_given ;	/**< @brief Whether ignitech-dump-file was given.  */
  unsigned int ignitech_servo_as_iap_given ;	/**< @brief Whether ignitech-servo-as-iap was given.  */
  unsigned int ignitech_sai_low_given ;	/**< @brief Whether ignitech-sai-low was given.  */
  unsigned int ignitech_sai_low_mv_given ;	/**< @brief Whether ignitech-sai-low-mv was given.  */
  unsigned int ignitech_sai_high_given ;	/**< @brief Whether ignitech-sai-high was given.  */
  unsigned int ignitech_sai_high_mv_given ;	/**< @brief Whether ignitech-sai-high-mv was given.  */
  unsigned int mount_offset_roll_given ;	/**< @brief Whether mount-offset-roll was given.  */
  unsigned int mount_offset_pitch_given ;	/**< @brief Whether mount-offset-pitch was given.  */
  unsigned int roll_pitch_swap_given ;	/**< @brief Whether roll-pitch-swap was given.  */
  unsigned int test_mode_given ;	/**< @brief Whether test-mode was given.  */

} ;

/** @brief The additional parameters to pass to parser functions */
struct cmdline_parser_params
{
  int override; /**< @brief whether to override possibly already present options (default 0) */
  int initialize; /**< @brief whether to initialize the option structure gengetopt_args_info (default 1) */
  int check_required; /**< @brief whether to check that all required options were provided (default 1) */
  int check_ambiguity; /**< @brief whether to check for options already specified in the option structure gengetopt_args_info (default 0) */
  int print_errors; /**< @brief whether getopt_long should print an error message for a bad option (default 1) */
} ;

/** @brief the purpose string of the program */
extern const char *gengetopt_args_info_purpose;
/** @brief the usage string of the program */
extern const char *gengetopt_args_info_usage;
/** @brief the description string of the program */
extern const char *gengetopt_args_info_description;
/** @brief all the lines making the help output */
extern const char *gengetopt_args_info_help[];
/** @brief all the lines making the full help output (including hidden options) */
extern const char *gengetopt_args_info_full_help[];
/** @brief all the lines making the detailed help output (including hidden options and details) */
extern const char *gengetopt_args_info_detailed_help[];

/**
 * The command line parser
 * @param argc the number of command line options
 * @param argv the command line options
 * @param args_info the structure where option information will be stored
 * @return 0 if everything went fine, NON 0 if an error took place
 */
int cmdline_parser (int argc, char **argv,
  struct gengetopt_args_info *args_info);

/**
 * The command line parser (version with additional parameters - deprecated)
 * @param argc the number of command line options
 * @param argv the command line options
 * @param args_info the structure where option information will be stored
 * @param override whether to override possibly already present options
 * @param initialize whether to initialize the option structure my_args_info
 * @param check_required whether to check that all required options were provided
 * @return 0 if everything went fine, NON 0 if an error took place
 * @deprecated use cmdline_parser_ext() instead
 */
int cmdline_parser2 (int argc, char **argv,
  struct gengetopt_args_info *args_info,
  int override, int initialize, int check_required);

/**
 * The command line parser (version with additional parameters)
 * @param argc the number of command line options
 * @param argv the command line options
 * @param args_info the structure where option information will be stored
 * @param params additional parameters for the parser
 * @return 0 if everything went fine, NON 0 if an error took place
 */
int cmdline_parser_ext (int argc, char **argv,
  struct gengetopt_args_info *args_info,
  struct cmdline_parser_params *params);

/**
 * Save the contents of the option struct into an already open FILE stream.
 * @param outfile the stream where to dump options
 * @param args_info the option struct to dump
 * @return 0 if everything went fine, NON 0 if an error took place
 */
int cmdline_parser_dump(FILE *outfile,
  struct gengetopt_args_info *args_info);

/**
 * Save the contents of the option struct into a (text) file.
 * This file can be read by the config file parser (if generated by gengetopt)
 * @param filename the file where to save
 * @param args_info the option struct to save
 * @return 0 if everything went fine, NON 0 if an error took place
 */
int cmdline_parser_file_save(const char *filename,
  struct gengetopt_args_info *args_info);

/**
 * Print the help
 */
void cmdline_parser_print_help(void);
/**
 * Print the full help (including hidden options)
 */
void cmdline_parser_print_full_help(void);
/**
 * Print the detailed help (including hidden options and details)
 */
void cmdline_parser_print_detailed_help(void);
/**
 * Print the version
 */
void cmdline_parser_print_version(void);

/**
 * Initializes all the fields a cmdline_parser_params structure 
 * to their default values
 * @param params the structure to initialize
 */
void cmdline_parser_params_init(struct cmdline_parser_params *params);

/**
 * Allocates dynamically a cmdline_parser_params structure and initializes
 * all its fields to their default values
 * @return the created and initialized cmdline_parser_params structure
 */
struct cmdline_parser_params *cmdline_parser_params_create(void);

/**
 * Initializes the passed gengetopt_args_info structure's fields
 * (also set default values for options that have a default)
 * @param args_info the structure to initialize
 */
void cmdline_parser_init (struct gengetopt_args_info *args_info);
/**
 * Deallocates the string fields of the gengetopt_args_info structure
 * (but does not deallocate the structure itself)
 * @param args_info the structure to deallocate
 */
void cmdline_parser_free (struct gengetopt_args_info *args_info);

/**
 * The config file parser (deprecated version)
 * @param filename the name of the config file
 * @param args_info the structure where option information will be stored
 * @param override whether to override possibly already present options
 * @param initialize whether to initialize the option structure my_args_info
 * @param check_required whether to check that all required options were provided
 * @return 0 if everything went fine, NON 0 if an error took place
 * @deprecated use cmdline_parser_config_file() instead
 */
int cmdline_parser_configfile (const char *filename,
  struct gengetopt_args_info *args_info,
  int override, int initialize, int check_required);

/**
 * The config file parser
 * @param filename the name of the config file
 * @param args_info the structure where option information will be stored
 * @param params additional parameters for the parser
 * @return 0 if everything went fine, NON 0 if an error took place
 */
int cmdline_parser_config_file (const char *filename,
  struct gengetopt_args_info *args_info,
  struct cmdline_parser_params *params);

/**
 * Checks that all the required options were specified
 * @param args_info the structure to check
 * @param prog_name the name of the program that will be used to print
 *   possible errors
 * @return
 */
int cmdline_parser_required (struct gengetopt_args_info *args_info,
  const char *prog_name);

extern const char *cmdline_parser_verbose_values[];  /**< @brief Possible values for verbose. */


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CMDLINE_H */
