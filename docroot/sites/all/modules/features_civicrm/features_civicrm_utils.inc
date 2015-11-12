<?php

/**
 * @file
 * Features CiviCRM module's common utility functions.
 */

/**
 * Wrapper for all CiviCRM API calls.
 *
 * For consistency, future-proofing, and error handling
 * Copied form webform_civicrm webform_civicrm_utils.inc file
 *
 * @param string $entity
 *   API entity
 * @param string $operation
 *   API operation
 * @param array $params
 *   API params
 *
 * @return array
 *   Result of API call
 */
function features_civicrm_api($entity, $operation, $params = array()) {
  civicrm_initialize();

  $params += array(
    'check_permissions' => FALSE,
    'version' => 3,
  );
  $result = civicrm_api($entity, $operation, $params);
  if (!empty($result['is_error'])) {
    $bt = debug_backtrace();
    $file = explode('/', $bt[0]['file']);
    watchdog(
      'features_civicrm',
      'The CiviCRM "%function" API function returned the error: "%msg" when called by line !line of !file with the following parameters: "!params"',
      array(
        '%function' => $entity . ' ' . $operation,
        '%msg' => $result['error_message'],
        '!line' => $bt[0]['line'],
        '!file' => array_pop($file),
        '!params' => print_r($params, TRUE)),
      WATCHDOG_ERROR);
    drupal_set_message(
      t('The CiviCRM "%function" API function returned the error: "%msg".',
        array(
          '%function' => $entity . ' ' . $operation,
          '%msg' => $result['error_message'])),
      'warning');
    if (module_exists('devel')) {
      dsm('Message from: (' . $entity . ', ' . $operation . ')');
      dsm($params);
      dsm($result);
    }
  }
  return $result;
}

/**
 * This function merges arrays, but combines the same values.
 *
 * array_merge_recursive does indeed merge arrays, but it converts values with
 * duplicate keys to arrays rather than overwriting the value in the first
 * array with the duplicate value in the second array, as array_merge does.
 * I.e., with array_merge_recursive, this happens (documented behavior):
 *
 * array_merge_recursive(
 *   array('key' => 'org value'),
 *   array('key' => 'new value'));
 *     => array('key' => array('org value', 'new value'));
 *
 * array_merge_recursive_distinct does not change the datatypes of the values
 * in the arrays. Matching keys' values in the second array overwrite those
 * in the first array, as is the case with array_merge, i.e.:
 *
 * array_merge_recursive_distinct(
 *   array('key' => 'org value'),
 *   array('key' => 'new value'));
 *     => array('key' => array('new value'));
 *
 * Parameters are passed by reference, though only for performance reasons.
 * They're not altered by this function.
 *
 * @param array $array1
 *   Array 1
 * @param array $array2
 *   Array 2
 *
 * @return array
 *   Merged Array
 */
function array_merge_recursive_distinct(array $array1, array $array2) {
  $merged = $array1;

  foreach ($array2 as $key => &$value) {
    if (is_array($value) && isset($merged[$key]) && is_array($merged[$key])) {
      $merged[$key] = array_merge_recursive_distinct($merged[$key], $value);
    }
    else {
      $merged[$key] = $value;
    }
  }

  return $merged;
}
