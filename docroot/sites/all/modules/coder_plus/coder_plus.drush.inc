<?php

/**
 * @file
 * Command line utility support for Coder_review module.
 */

/**
 * Implements hook_drush_command().
 */
function coder_plus_drush_command() {
  $items['coder-plus'] = array(
    'description' => dt('Run coder plus complexity report using drush'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'aliases' => array('cp'),
    'arguments' => array(
        'module_name' => 'Check complexity for specified module',
    ),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function coder_plus_drush_help($section) {
  switch ($section) {
    case 'drush:coder-plus':
      $help = dt('This comand will execute Coder Plus to check cyclomatic complexity for a given module.');
      return $help;
  }
}

/**
 * Function to print result of complexity results through drush command.
 *
 * @param array $module_name
 * module_name param contains module name.
 */
function drush_coder_plus($module_name) {
  $results = array();
  module_load_include('inc', 'coder_plus', 'coder_plus_complexity_logic.admin');
  $results = _check_module_for_complexity($module_name);
  drush_print(coder_plus_prepare_drush_result_of_complexity($results));
}

/**
 * Function to return result with complexity results.
 *
 * @param array $results
 *   Result param conating function with their complexity
 *
 * @return result
 *   Result data with complexity.
 */
function coder_plus_prepare_drush_result_of_complexity($results) {
  $drush_output = '';
  if (isset($results) && count($results)) {
    foreach ($results as $file_name => $function_array) {
      if (count($function_array)) {
        $i = 0;
        $flag = 0;
        foreach ($function_array as $function_name => $function_details) {
          if ($flag == 0) {
            $file_name_raw = explode('/', $file_name);
            $file_name_final = end($file_name_raw);
            $drush_output .= "\nComplexity for file " . $file_name_final . "\n";
            $flag = 1;
          }
          $drush_output .= "function " . $function_name . "()\n";
          $drush_output .= "Complexity: " . $function_details['complexity'] . ", Severity: " . coder_plus_get_complexity_image($function_details['complexity'], 1) . "\n";
          $i++;
        }
      }
    }
  }
  return $drush_output;
}
