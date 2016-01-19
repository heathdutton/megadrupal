<?php
/**
 * @file
 *  A helper class for working with arrays
 * @copyright Copyright(c) 2012 Christopher Skene
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 */
namespace Xu\Component\Helpers;

class ArrayHelper {

  /**
   * Check all items in an array via a callback
   */
  static function allTrue(array $data, $callback) {
    if (!function_exists($callback)) {
      // @todo should throw an error
      return FALSE;
    }

    if (is_array($data) && !empty($data)) {
      foreach($data as $key => $datum) {
        if (call_user_func($callback, $datum) == FALSE) {
          return array(
            'result' => FALSE,
            'error' => $datum,
          );
        }
      }
    }

    return array(
      'result' => TRUE,
    );
  }

  /**
   * Check that a given value is TRUE
   */
  static function assertTrue($data, $key) {
    if (isset($data['key'])) {
      if ($data[$key] == TRUE) {
        return TRUE;
      }
    }

    return FALSE;
  }

  /**
   * Check that a given value is FALSE
   */
  static function assertFalse($data, $key) {
    if (isset($data['key'])) {
      if ($data[$key] == FALSE) {
        return TRUE;
      }
    }

    return FALSE;
  }
}
