<?php

/**
 * @file
 * Hooks provided by the quail api.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Define user permissions associated with the quail api.
 *
 * Normally other modules can implement their own hook_permission.
 * However, in order to help improve organization of the permission page,
 * this hook is provided so that all sub-modules may have their permissions
 * appear under the parent group.
 * This is provided for structural organizational purposes and its usage is
 * not required.
 *
 * @see hook_permission()
 */
function hook_quail_api_permission(&$permissions) {
  $permissions['utilize my quail api extension'] = array(
    'title' => t("Utilize my quail api extension"),
    'description' => t("Specify whether or not my quail api extension may be utilized by some user or role."),
  );
}

/**
 * Define quail api standards.
 *
 * This allows for custom modules to define their own quail api standards or
 * variations of a given standard.
 *
 * @param $standards
 *   An array of standards that is returned to the caller.
 * @param $standard
 *   (optional) A machine name string of the standard to use.
 *   When defined, only the standard that matches this string will loaded into
 *   the standards array.
 *   When undefined, all standards will be loaded into the standards array.
 * @param $other_arguments
 *   (optional) Array of additional arguments.
 *
 * The standards array has the following structure:
 * - standard: An array (whose key is the machine name of the standard)
 *   defining a given standard using the following keys:
 *   - human_name: Human friendly name of the standard.
 *   - module: Machine name of the primary module providing this standard.
 *   - description: A description of the standard, wrapped in t().
 *   - guideline: Machine name of the guideline.
 *     This is used to load the guideline class whose class object must be
 *     defined as follows:
 *     - ${guideline}_${standard}Guideline extends quailGuideline { ...
 *     If the standard were called 'wcag7_4d' and the guideline were called
 *     'hypercube', then the class object would be:
 *     - wcag7_4d_hypercubeGuideline extends quailGuideline { ...
 *   - reporter: Machine name of the reporter.
 *     This is used to load the reporter class whose class object must be
 *     defined as follows:
 *     - report${reporter} extends quailReporter { ...
 *     If the guideline were called 'hypercube', then the class object would
 *     be:
 *     - reportHypercube extends quailReporter { ...
 *     - Take special note of the upper-case H, the quail library requires that
 *       the first character of the guideline name be uppercase.
 *   - target: Target allows for selecting standards based on some category or
 *     purpose.
 *     - The target, in general, represents the scope in which the standards
 *       will be applied.
 *     - The following targets are directly supported: 'snippet', 'page'.
 *
 * The other_arguments array has the following structure:
 * - target: A machine name string of a target to use.
 *   Providing a target allows for limiting standards by some category.
 *   The target, in general, represents the scope in which the standards will
 *   be applied.
 *   The following targets are directly supported: 'snippet', 'page'.
 *
 * @see quail_api_get_standards()
 * @see quail_api_get_standards_list()
 */
function hook_quail_api_get_standards_alter(&$standards, $standard, $other_arguments) {
  if ($other_arguments['target'] == 'tesseract' || $standard == 'wcag7_4d') {
    $standards['wcag7_4d'] = array(
      'human_name' => t("WCAG 7.0 - 4D"),
      'module' => 'wcag7',
      'description' => t(
        "Validate using WCAG 7.0. This provides 4th dimensional content in such a way that users who can only perceive 3 dimensions may appropriately navigate and understand said content."
      ),
      'guideline' => 'hypercube',
      'reporter' => 'hypercube',
      'target' => $other_arguments['target'],
    );
  }
}

/**
 * Define quail api display levels.
 *
 * This allows for custom modules to define their own quail api display levels
 * or alter an existing display level.
 *
 * @param $display_levels
 *   An array of standards that is returned to the caller.
 * @param $display_level
 *   (optional) A number representing the display level.
 *   When defined, the return value to only contain the display level that
 *   matches this string.
 *   When undefined, all display levels will be loaded into the display_levels
 *   array.
 * @param $other_arguments
 *   (optional) Array of additional arguments.
 *
 * The display_levels array has the following structure:
 * - display level: An array (whose key is the numerical id of the display
 *   level) defining a given display level using the following keys:
 *   - machine name: Machine name of the standard.
 *   - human name: Human friendly name of the standard.
 *   - module: Machine name of the primary module providing this display level.
 *   - description: A description of the standard, wrapped in t().
 *   - id: Numerical id of the display level, this must match the array key.
 *   - default: A boolean representing whether or not to have this display
 *     level enabled by default.
 *
 * @see quail_api_get_display_levels()
 * @see quail_api_get_display_levels_list()
 */
function hook_quail_api_get_display_levels(&$display_levels, $display_level) {
  if (!is_numeric($display_level) || $display_level == 4) {
    $display_levels[4] = array(
      'machine_name' => 'quail_test_other',
      'human_name' => t("Other Problems"),
      'module' => 'wcag7',
      'description' => t(
        "Other problems represent failures in accessibility compliance that do not fall in any of the three primary categories: Major, Minor, and Suggestion."
      ),
      'id' => 4,
      'default' => TRUE,
    );
  }
}

/**
 * Define quail api valodation methods.
 *
 * This allows for custom modules to define their own quail api validation
 * methods or alter an existing validation method.
 *
 * @param $validation_methods
 *   An array of validation methods that is returned to the caller.
 * @param validation_method
 *   (optional) A machine name representing of the validation method.
 *   When defined, the return value to only contain the validation method that
 *   matches the given id.
 *   When undefined, all validation methods will be loaded into the
 *   validation_method array.
 * @param $other_arguments
 *   (optional) Array of additional arguments.
 *
 * The validation_methods array has the following structure:
 * - validation method: An array (whose key is the machine name of the
 *   validation method) defining a given validation method using the
 *   following keys:
 *   - machine name: Machine name of the standard.
 *   - human name: Human friendly name of the standard.
 *   - module: Machine name of the primary module providing this display level.
 *   - description: A description of the standard, wrapped in t().
 *   - id: Numerical id of the display level, this must match the array key.
 *   - default: A boolean representing whether or not to have this display
 *     level enabled by default.
 *
 * @see quail_api_get_validation_methods()
 * @see quail_api_get_validation_methods_list()
 */
function hook_quail_api_get_validation_methods(&$validation_methods, $validation_method) {
  if (empty($validation_method) || $validation_methods == 'quail_api_method_stare') {
    $validation_methods['quail_api_method_stare'] = array(
      'human_name' => t("Immediately Stare"),
      'module' => 'quail_api',
      'description' => t(
        "Make the validator stare at you when performing validation. Stare results are never saved."
      ),
      'database' => FALSE,
      'automatic' => TRUE,
    );
  }
}

/**
 * @} End of "addtogroup hooks".
 */
