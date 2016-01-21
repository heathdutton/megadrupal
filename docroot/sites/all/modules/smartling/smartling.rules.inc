<?php

/**
 * @file
 * Rules functions for Smartling module.
 */

/**
 * Implements hook_rules_event_info().
 */
function smartling_rules_event_info() {
  return array(
    'smartling_after_submission_check_status_event' => array(
      'label' => t('After the status of Smartling submission has been checked'),
      'module' => 'smartling',
      'group' => 'smartling',
      'variables' => array(
        'eid' => array(
          'type' => 'integer',
          'label' => t('Smartling submission id')
        ),
      ),
    ),
    'smartling_after_submission_download_event' => array(
      'label' => t('After Smartling submission was downloaded'),
      'module' => 'smartling',
      'group' => 'smartling',
      'variables' => array(
        'eid' => array(
          'type' => 'integer',
          'label' => t('Smartling submission id')
        ),
      ),
    ),
    'smartling_after_submission_upload_event' => array(
      'label' => t('After Smartling submission was uploaded'),
      'module' => 'smartling',
      'group' => 'smartling',
      'variables' => array(
        'eid' => array(
          'type' => 'integer',
          'label' => t('Smartling submission id')
        ),
      ),
    ),
  );
}

///**
// * Implements hook_rules_condition_info().
// */
//function smartling_rules_condition_info() {
//  return array(
//    'smartling_locale_condition' => array(
//      'label' => t('Locale comparison'),
//      'parameter' => array(
//        'data' => array(
//          'type' => '*',
//          'label' => t('Locale to compare'),
//          'description' => t('The locale to be compared, specified by using a data selector, e.g. "locale".'),
//          'allow null' => TRUE,
//        ),
//        'op' => array(
//          'type' => 'text',
//          'label' => t('Operator'),
//          'description' => t('The comparison operator.'),
//          'optional' => TRUE,
//          'default value' => '==',
//          'options list' => 'rules_condition_data_is_operator_options',
//          'restriction' => 'input',
//        ),
//        'value' => array(
//          'type' => 'text',
//          'label' => t('Locale value'),
//          'description' => t('The value to compare the locale with.'),
//        ),
//      ),
//      'group' => t('Smartling'),
//    ),
//  );
//}
//
///**
// * Smartling locale condition.
// *
// * @param int|string|array|bool $data
// *   Data.
// * @param string $op
// *   Operations.
// * @param int|string|array|bool $value
// *   Value.
// *
// * @return bool
// *   Return operation result.
// */
//function smartling_locale_condition($data, $op, $value) {
//  switch ($op) {
//    default:
//    case '==':
//      // In case both values evaluate to FALSE, further differentiate between
//      // NULL values and values evaluating to FALSE.
//      if (!$data && !$value) {
//        return (isset($data) && isset($value)) || (!isset($data) && !isset($value));
//      }
//      return $data == $value;
//
//    case '<':
//      return $data < $value;
//
//    case '>':
//      // Note: This is deprecated by the text comparison condition and IN below.
//      return $data > $value;
//
//    case 'contains':
//      return is_string($data) && strpos($data, $value) !== FALSE || is_array($data) && in_array($value, $data);
//
//    case 'IN':
//      return is_array($value) && in_array($data, $value);
//  }
//}