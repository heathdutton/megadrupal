<?php

/**
 * @file
 * Mock test class for MaPS Import request class.
 */

use Drupal\maps_import\Request\Request;
use Drupal\maps_import\Request\Webservice;
use Drupal\maps_import\Profile\Profile;
use Drupal\maps_import\Exception\Exception;

/**
 * Mock class.
 */
class MapsImportRequestMock extends Request {

  /**
   * This function does not call any Web Service, but loads a local file
   * with the expected data.
   */
  public static function getData(Profile $profile, array $args, array $options = array()) {
  	$testFile = $args['file'];
    
    if ($diff = variable_get('test_differential', 0)) {
      $testFile = $testFile . '_diff_' . $diff;      
    }

    $file = drupal_get_path('module', 'maps_import') . "/tests/files/$testFile.xml";
      
    if (!is_file($file) || !is_readable($file)) {
      return FALSE;
    }

    try {
      $instance = new Webservice($profile, array(), array());
      $instance->result = new \stdClass();
      $instance->result->data = file_get_contents($file);
      return $instance->parse();
    }
    catch(MapsImportException $e) {
      return FALSE;
    }
  }

}
