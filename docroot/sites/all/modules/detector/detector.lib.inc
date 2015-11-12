<?php

/**
 * @file
 * Library commands for Detector module.
 */

/**
 *
 */
function _detector_libraries_detect() {
  if (function_exists('libraries_detect')) {
    return libraries_detect('Detector');
  }
  
  $path = libraries_get_path('Detector');
  $detector_src = $path . '/lib/Detector/Detector.php';

  $library['path'] = $path;  
  $library['main_file'] = $detector_src;
  $library['installed'] = file_exists($detector_src);
  
  return $library;
}

/**
 *
 */
function _detector_libraries_load() {
  global $ua;
  
  if (function_exists('libraries_load')) {
    return libraries_load('Detector');
  }
  
  $library = _detector_libraries_detect();
  if (!empty($library['installed'])) {
    require_once(DRUPAL_ROOT . '/' . $library['main_file']);
    $library['loaded'] = true;
  }
  
  /* Execute rules and contexts */
  
  $arr = DetectorProperty::listProperties();
  
  if (function_exists('rules_invoke_event')) {
    rules_invoke_event('detector_loaded', $arr);
  }
  
  if (function_exists('context_get_plugin')) {
    if ($plugin = context_get_plugin('condition', 'detector_feature')) { 
      foreach ($arr as $key => $val) {
        $plugin->execute($key, $val);
      }
    }
  }
  
  return $library;
}

/**
 * Includes the Detector libraries.
 * 
 * @param  Prompt: TRUE to print a message if the library was not found.
 *         FALSE by default, because this is just a detection function.
 * 
 * @return TRUE if library was found.
 *         FALSE otherwise.
 */
function _detector_check_lib($prompt = false) {
  $found = false;
  
  if (($library = _detector_libraries_detect()) && !empty($library['installed'])) {
    $found = true;
  }
  
  if (!$found) {
    _detector_lib_warning();
  }
  
  return $found;
}

/**
 * Includes the Detector libraries.
 *
 * @param  Prompt: TRUE to print a message if the library was not found.
 *         TRUE by default, because this is a loading function.
 * 
 * @return TRUE if library was found.
 *         FALSE otherwise.
 */
function _detector_get_lib($prompt = true) {
  $loaded = false;
  
  if (($library = _detector_libraries_load()) && !empty($library['loaded'])) {
    $loaded = true;
  }
  
  if (!$loaded) {
    _detector_lib_warning();
  }
  
  return $loaded;
}

/**
 * Warns the user that the Detector library was not found.
 */
function _detector_lib_warning() {
  $detector_src = 'https://github.com/dmolsen/Detector';
  
  $msg = t("The !detector library was not found.  Please download it and install it in your site's Libraries directory.", array(
    '!detector' => l(t('Detector'), $detector_src),
  ));
  drupal_set_message($msg, 'error');
}

/**
 * Loads special external JS files for Java and Flash detection.
 */
function _detector_load_special() {
  $special_detection = variable_get('detector_special_detection', FALSE);

  if ($special_detection) {
    $java = variable_get('detector_special_detection_java', '');
    if (empty($java)) {
      $java = 'http://java.com/js/deployJava.js';
    }
    $external_java = parse_url($java);
    
    $flash = variable_get('detector_special_detection_flash', '');
    if (empty($flash)) {
      $flash = 'http://ajax.googleapis.com/ajax/libs/swfobject/2.2/swfobject.js';
    }
    $external_flash = parse_url($flash);
    
    $abs = array('absolute' => true);
    
    drupal_add_js(url($java, $abs), ($external_java ? 'external' : 'file'));
    drupal_add_js(url($flash, $abs), ($external_flash ? 'external' : 'file'));
  }
}

/**
 * Flattens an object into a specially formatted array.
 *
 * @param The object to flatten.
 * 
 * @return The resulting array.
 */
function _detector_flatten_list($obj, $prefix = '') {
  $arr = array();
  if (!is_object($obj)) {
    return $arr;
  }
  
  if (!is_object($obj)) {
    return $arr;
  }
  
  $vars = get_object_vars($obj);
  
  foreach ($vars as $key => $val) {
    $arr_key = (!empty($prefix) ? ($prefix . '->' . $key) : $key);
    
    if (is_object($val)) {
      $result = _detector_flatten_list($val, $arr_key);
      
      foreach ($result as $k => $v) {
        $arr[$k] = $v;
      }
    }
    else if (is_array($val)) {
      $arr[$arr_key] = print_r($val, 1);
    }
    else if (is_bool($val)) {
      $arr[$arr_key] = $val ? 'TRUE' : 'FALSE';
    }
    else {
      $arr[$arr_key] = $val . '';
    }
  }
  
  return $arr;
}
