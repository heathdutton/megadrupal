<?php

/**
 * @file
 * This file holds functions for handling sn_coder_plus Admin threshold form.
 *
 * @SNDCP sn_coder_plus
 */

/**
 * Function to create test file on specific module.
 *
 * @param string $module_abs_path
 * @param string $module_name
 */
function sn_simpletest_plus_create_test_file($module_abs_path, $module_name) {
  if (is_writable($module_abs_path)) {
    $test_file_path = $module_abs_path . DIRECTORY_SEPARATOR . $module_name . '.test';
    touch($test_file_path);
    sn_simpletest_plus_generate_test_template($module_name);
  }
  else {
    drupal_set_message(t('Directory %module_abs_path is not accessible.', array('%module_abs_path' => $module_abs_path)), 'error');
  }
}

/**
 * Edit info file.
 * 
 * Appends the file[] = module.test string
 * in .info file, so the test cases can 
 * be identified.
 * 
 * @param string $module_abs_path
 *   Module's absolute installed path
 * @param string $module_name
 *   Module name for which test cases are 
 *   being generated.
 */
function sn_simpletest_plus_edit_info_file($module_abs_path, $module_name) {
  // Module .info file path.
  $module_info_file_path = $module_abs_path . DIRECTORY_SEPARATOR . $module_name . '.info';
  //$file_open = fopen($module_info_file_path, 'r');
  $file_read = file($module_info_file_path);
  $filename_tocompare = "files[] = " . $module_name . ".test";
  foreach ($file_read as $key => $value) {
    if ($value != $filename_tocompare) {
      $info_file_str = "\nfiles[] = " . $module_name . ".test";
    }
  }
  // Getting info file data string to be appended.
  $test_file = $module_abs_path . DIRECTORY_SEPARATOR . $module_name . ".test";
  if (!file_exists($test_file)) {
    // Writing required data in file
    sn_simpletest_plus_file_write($module_info_file_path, $info_file_str);
  }
  
}

/**
 * To append data in a file.
 *
 * @param Sring  $filepath
 *   Path of file
 * @param string $data_str
 *   data to be appended
 *
 * @return bool
 *   TRUE if file is sucessfully written;
 *   FALSE otherwise.
 */
function sn_simpletest_plus_file_write($filepath, $data_str, $mode = "w+") {
  // Checking file for writing.
  if (file_exists($filepath)) {
    $read_file = fopen($filepath, $mode);
    if ($read_file) {
      if (fwrite($read_file, $data_str)) {
        fclose($read_file);
        return TRUE;
      }
      else {
        drupal_set_message(t("Unable to write data in file '%filepath'", array('%filepath' => $filepath)), 'error');
        return FALSE;
      }
    }
    else {
      drupal_set_message(t("Unable to access file '%filepath'", array('%filepath' => $filepath)), 'error');
      return FALSE;
    }
  }
  else {
    drupal_set_message(t("File '%filepath', does not exists", array('%filepath' => $filepath)), 'error');
    return FALSE;
  }
}

/**
 * Function to write test file
 *
 * @param string $module_name
 */
function sn_simpletest_plus_generate_test_template($module_name) {
  $keys = array('##_module', '##_upper', '##_class');
  $test_group = ucwords(str_replace('_', ' ', $module_name));
  $class_name = str_replace(' ', '',  $test_group . 'TestCase');
  $value = array($module_name, $test_group, $class_name);
  $file_content = file_get_contents(sn_simpletest_plus_module_abs_path('sn_simpletest_plus') . DIRECTORY_SEPARATOR . 'sample_test.txt');
  $string_replace = str_replace($keys, $value, $file_content);

  // Module test file path.
  $module_abs_path = sn_simpletest_plus_module_abs_path($module_name);
  $module_test_file_path = $module_abs_path . DIRECTORY_SEPARATOR . $module_name . '.test';
  if (file_exists($module_test_file_path)) {
    // Writing required data in file.
    sn_simpletest_plus_file_write($module_test_file_path, $string_replace);
    sn_simpletest_plus_module_complexity($module_name);
  }
}

/**
 * Function retyrns file name and functions with table complexity.
 *
 * @param array $data
 *   File data with complexity.
 *
 * @return array
 *   Table display of complexity in array format. 
 */
function sn_simpletest_plus_module_complexity($module_name) {
  $results = array();
  if (isset($module_name)) {
    $mod_path = drupal_get_path('module', $module_name);
    $dirs = file_scan_directory($mod_path, '/.*\.(inc|module|install)$/');
    foreach ($dirs as $val) {
      $results[$val->filename] = sn_simpletest_plus_check_complexity($module_name, $val->uri);
    }
  }

  foreach ($results as $key => $value) {
    $functions[] = $value;
  }

  $functions = implode($functions);
  $module_abs_path = sn_simpletest_plus_module_abs_path($module_name);
  $module_test_file_path = $module_abs_path . DIRECTORY_SEPARATOR . $module_name . '.test';
  $file_content = file_get_contents($module_test_file_path);
  $func_keys = array('##_test_functions');
  $func_value = array($functions);
  $string_replace = str_replace($func_keys, $func_value, $file_content);

  if (file_exists($module_test_file_path)) {
    sn_simpletest_plus_file_write($module_test_file_path, $string_replace, "w");
    drupal_set_message(t("Your file has been written successfully"));
  }
  drupal_goto('admin/config/system/add-simple-test/modules-list');
}

/**
 * Function to check cyclometric complexity for a module.
 *
 * @param string $file
 *   File path
 *
 * @return array
 *   function with their complexity
 */
function sn_simpletest_plus_check_complexity($module_name, $file) {
  $functions = array();
  $in_comment = FALSE;
  $in_php = FALSE;
  $is_class = FALSE;
  $handle = fopen($file, 'r');
  $function_array = array();
  while (!feof($handle)) {
    $line = sn_simpletest_plus_escape_slashes(fgets($handle));
    if (1 == preg_match('/\<\?(php)?/', $line)) {
      $in_php = TRUE;
    }
    if (1 === preg_match('/\?\>/', $line)) {
      $in_php = FALSE;
    }
    if (1 === preg_match('/\/\*/', $line)) {
      $in_comment = TRUE;
    }
    if (1 === preg_match('/\*\//', $line)) {
      $in_comment = FALSE;
    }
    if ($in_comment || !$in_php) {
      continue;
    }
    if (FALSE !== ($function_name = sn_simpletest_plus_find_function($line))) {
      $function_array[] = $function_name;
    }
    if (FALSE !== sn_simpletest_plus_find_class($line)) {
      $is_class = TRUE;
      break;
    }
  }

  fclose($handle);
  if (!$is_class) {
    $functions = sn_simpletest_plus_process_nodes_edges($module_name, $function_array, $file);
  }
  return $functions;
}

/**
 * Function to calculate cyclometric complexity for a module.
 * 
 * @param array $function_array
 *   Array of function in a file.
 * @param array $file
 *   File data in array format
 *
 * @return array
 *   Final out with calculated nodes and edges.
 */
function sn_simpletest_plus_process_nodes_edges($module_name, $function_array, $file) {
  include_once $file;
  $counts = array();
  
  foreach ($function_array as $fun_key => $data_function) {
    $reflector = new ReflectionFunction($data_function['fnc']);
    $line_func = $reflector->getStartLine();

    $body = array_slice(
            file($file),
            $reflector->getStartLine(),
            $reflector->getEndLine() - $reflector->getStartLine()
    );
    
    foreach ($body as $key => $value) {

      $test_switch = preg_match_all('/switch.*\(.*\)\{?/', $value, $matches);
      if ($test_switch === 1) {
        $counts[] = sn_simpletest_plus_create_functions('switch', $data_function['fnc'], $key, $value, $module_name, $line_func);
      }
      
      $test_foreach = preg_match_all('/foreach.*\(.*\){?/',$value,$matches);
      if ($test_foreach === 1) {
      $counts[] = sn_simpletest_plus_create_functions('foreach', $data_function['fnc'], $key, $value, $module_name, $line_func);  
      }
      
      $test_for = preg_match_all('/for.*\(.*\){?/',$value,$matches);
      if ($test_for === 1) {
      $counts[] = sn_simpletest_plus_create_functions('for', $data_function['fnc'], $key, $value, $module_name, $line_func);  
      }
      
      $test_while = preg_match_all('/while.*\(.*\){?/',$value,$matches);
      if ($test_while === 1) {
      $counts[] = sn_simpletest_plus_create_functions('while', $data_function['fnc'], $key, $value, $module_name, $line_func);  
      }
      
      $test_dowhile = preg_match_all('/dowhile.*\(.*\){?/',$value,$matches);
      if ($test_dowhile === 1) {
      $counts[] = sn_simpletest_plus_create_functions('dowhile', $data_function['fnc'], $key, $value, $module_name, $line_func);  
      }
     
      $test_if = preg_match_all('/(else)?if.*\(.+/', $value, $matches);
      if ($test_if === 1) {
        $if_or_condition = preg_match_all('/\|\|/', $value, $matches);
        $if_and_condition = preg_match_all('/&&/', $value, $matches);
        if ($if_or_condition === 1 || $if_and_condition === 1) {
          if ($if_or_condition === 1) {
            $counts[] = sn_simpletest_plus_create_functions('if_or_condition', $data_function['fnc'], $key, $value, $module_name, $line_func);
          }
          if ($if_and_condition === 1) {
            $counts[] = sn_simpletest_plus_create_functions('if_and_condition', $data_function['fnc'], $key, $value, $module_name, $line_func);
          }
        } 
        else {
          $counts[] = sn_simpletest_plus_create_functions('if', $data_function['fnc'], $key, $value, $module_name, $line_func);
        }
      }
    }
  }
  $functions = implode($counts);
  return $functions;
}


/**
 * Function to create dynamic functions.
 *
 * @param array $source
 *   Array format of file to be processed
 *
 * @return array
 *   Array of count for different complexities.
 */
function sn_simpletest_plus_create_functions($type, $function, $func_key, $condition, $module_name,$line_func) {
  $keys = array('##function_name', '##condition', '##group', '##funcline','##assert');
  $function_name = 'test' . ucwords($function) . '_' . $type  . '_'. $func_key;
  $test_group = ucwords(str_replace('_', ' ', $module_name));
  $condition = trim($condition);
  $line_no = $line_func;
  $assert_condition = 'false'; 
  $value = array($function_name, rtrim($condition, '{'), $test_group, $line_no,$assert_condition);
  $file_content = file_get_contents(sn_simpletest_plus_module_abs_path('sn_simpletest_plus') . DIRECTORY_SEPARATOR . 'sample_test_functions.txt');
  $string_replace = str_replace($keys, $value, $file_content);
  
  return $string_replace;
}

/**
 * Function to perform regular expression search and replace.
 *
 * @param string $line
 *   Line written in function.
 *
 * @return string
 *   Formatted string.
 */
function sn_simpletest_plus_escape_slashes($line) {
  $pattern = array(
    "/\\\'/",
    '/\\\"/',
    '/\'{1}[.]*[^\']*\'{1}/',
    '/"{1}[.]*[^"]*"{1}/',
    '/\/\*.*\*\//',
    '/(#|\/\/).*/');

  $replacement = array(
    'ESCAPED_SINGLE_QUOTE',
    'ESCAPED_DOUBLE_QUOTE',
    'SINGLE_QUOTE',
    'DOUBLE_QUOTE',
    'SLST_COMMENT',
    'ONLI_COMMENT');

  return preg_replace($pattern, $replacement, $line);
}

/**
 * Function to find function in a file.
 * 
 * @param string $line
 *   Line written in function.
 */
function sn_simpletest_plus_find_function($line) {
  if (0 === preg_match('/function[\s]+&?([a-zA-Z_\x7f-\xff][a-zA-Z0-9_\x7f-\xff]*)\(.*\)(?!;)/', $line, $match)) {
    return FALSE;
    
  }
  $units['fnc'] = $match[1];
  return $units;
}

/**
 * Function to find class in a file.
 *
 * @param string $line
 *   Line written in function.
 *
 * @return bool
 *   True or false.
 */
function sn_simpletest_plus_find_class($line) {
  if (0 === preg_match('#^(\s*)((?:(?:abstract|final|static)\s+)*)class\s+([-a-zA-Z0-9_]+)(?:\s+extends\s+([-a-zA-Z0-9_]+))?(?:\s+implements\s+([-a-zA-Z0-9_,\s]+))?#', $line, $match)) {
    return FALSE;
  }

  return TRUE;
}
