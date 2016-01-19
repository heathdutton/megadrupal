<?php

/**
 * @file
 * Hooks provided by the diff module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Perform validations before the confirmation step of a VBO is fired.
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form. The arguments
 *   that drupal_get_form() was originally called with are available in the
 *   array $form_state['build_info']['args'].
 * @param $vbo_name
 *   String representing the name of the VBO action itself.
 */
function hook_vbo_validation_validate($form, $form_state, $vbo_name) {
  $name_len = strlen($vbo_name);
  if ($name_len % 2) {
    $vbo = _views_bulk_operations_get_field($form_state['build_info']['args'][0]);
    $field_name = $vbo->options['id'];
    form_set_error($field_name, t('You can only do this operation on odd-length VBO names. This is an even length VBO name.'));
  }
}

/**
 * Provide a VBO-specific validation instead of the global hook_vbo_validation_validate().
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form. The arguments
 *   that drupal_get_form() was originally called with are available in the
 *   array $form_state['build_info']['args'].
 */
function hook_vbo_validation_prc_trt_delete_schools_validate($form, $form_state) {
  $minute = date('i');
  if ($minute % 2) {
    $vbo = _views_bulk_operations_get_field($form_state['build_info']['args'][0]);
    $field_name = $vbo->options['id'];
    form_set_error($field_name, t('You can only do this operation on odd minutes. This is an even minute.'));
  }
}