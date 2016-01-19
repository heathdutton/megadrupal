<?php

/**
 * @file
 * Frontend for Detector module library.
 */

module_load_include('inc', 'detector', 'detector.lib');

/**
 * UA property fetcher
 */
class DetectorProperty {
  function getProperty($property) {
    if (empty($property)) {
      return null;
    }
    
    $arr = self::listProperties();
    if (!is_array($arr) && empty($arr)) {
      return null;
    }
    
    // error_log(print_r($arr,1));
    // error_log(print_r($property,1));
    
    if (isset($arr["{$property}"])) {
      error_log($arr["{$property}"]);
      return $arr["{$property}"];
    }
    
    return null;
  }
  
  function listProperties() {
    global $ua;
    return _detector_flatten_list($ua);
  }
}
