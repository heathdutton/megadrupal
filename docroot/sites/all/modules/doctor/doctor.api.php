<?php
/**
 * @file
 * API file for the doctor module.
 */

/**
 * Implements hook_doctor_listener_info().
 */
function hook_doctor_listener_info() {
  return array(
    'doctor' => array(
      'callback' => '_hook_doctor_listener_info',
    ),
  );
}

/**
 * Doctor listener callback function.
 *
 * @param $data
 *  The un-serialized data listener item.
 */
function _hook_doctor_listener_info($data) {
  drush_print_r(dt('Your recorded info: @data', array('@data' => $data)));
}

/**
 * Implements hook_doctor_listener_info_alter().
 *
 * Allow you to alter the listener callback functions.
 */
function hook_doctor_listener_info_alter(&$data) {
  foreach ($data as $module_name => &$callbacks_info) {
    if ($module_name == 'doctor') {
      $callbacks_info['callback'] = '__hook_doctor_listener_info';
    }
  }
}

/**
 * Implements hook_doctor_listener_ui_info().
 *
 * Unlike the normal listen item, this hook will define which hook will be
 * implements when watching the reports in the admin page.
 */
function hook_doctor_listener_ui_info() {
  return array(
    'doctor' => array(
      'callback' => '_hook_doctor_listener_ui_info',
    ),
  );
}

/**
 * Doctor listener callback function of the UI.
 *
 * The first module that will return an output will win the privilege to present
 * his styled doctor item.
 *
 * @param $data
 *  The un-serialized data listener item.
 *
 * @return string
 *  The string to output in the reports page.
 */
function _hook_doctor_listener_ui_info($data) {
  return t('Your recorded info: @data', array('@data' => $data));
}

/**
 * Implements hook_listener_ui_info_alter().
 *
 * Allow you to alter the listener callback functions.
 */
function hook_listener_ui_info_alter(&$data) {
  foreach ($data as $module_name => &$callbacks_info) {
    if ($module_name == 'doctor') {
      $callbacks_info['callback'] = '__hook_doctor_listener_info';
    }
  }
}

/**
 * Implements hook_doctor_bundle_info().
 */
function hook_doctor_bundle_info() {
  return array(
    'foo' => array(
      'name' => 'Foo',
      'type' => 'foo',
      'description' => 'Bar!! You expected foo but no... surprise ;)',
    ),
  );
}
