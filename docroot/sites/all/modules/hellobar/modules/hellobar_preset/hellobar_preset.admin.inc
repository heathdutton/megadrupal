<?php
/**
 * @file
 * Forms for admin page.
 */

/**
 * Preset lists form.
 *
 * @param array $form
 *   A nested array form elements comprising the form.
 * @param array $form_state
 *   An associative array containing the current state of the form.
 *
 * @return array
 *   The array containing the complete form.
 */
function hellobar_preset_list_form($form, $form_state) {
  $form = array();

  $path = 'admin/config/media/hellobar';
  $header = array('Name', 'Machine name', 'Edit', 'Delete');

  foreach (hellobar_preset_load_list() as $pid => $preset) {
    $rows_by_type[$preset->type][$preset->pid] = array(
      $preset->title,
      $preset->name,
      l(t('Edit'), "$path/$preset->type/$preset->name/edit"),
      l(t('Delete'), "$path/$preset->type/$preset->name/delete"),
    );
  }

  foreach ($rows_by_type as $type => $rows) {
    $form[$type] = array(
      '#markup' => theme('table', array(
        'caption' => "<h3>$type</h3>",
        'header' => $header,
        'rows' => $rows,
      )),
    );
  }

  return $form;
}

/**
 * Preset edit Form.
 *
 * @param array $form
 *   A nested array form elements comprising the form.
 * @param array $form_state
 *   An associative array containing the current state of the form.
 * @param string $type
 *   A type of preset.
 * @param array $preset
 *   A preset options array.
 *
 * @return array
 *   The array containing the complete form.
 */
function hellobar_preset_edit_form($form, $form_state, $type, $preset = NULL) {
  $form = array();

  if (in_array($type, array('basic', 'more', 'advanced'))) {
    module_load_include('inc', 'hellobar_preset', "forms/hellobar_preset.{$type}_form");
    $callback = "hellobar_preset_{$type}_form";

    if (function_exists($callback)) {
      $form = $callback($form, $form_state, $preset);

      // Add required fields.
      $form += _hellobar_preset_required_form($preset, $type);
      return $form;
    }
  }

  return $form;
}

/**
 * General field for preset edit form.
 *
 * @param array $preset
 *   A preset options array.
 * @param string $type
 *   A type of preset.
 *
 * @return array
 *   The array containing the complete form.
 */
function _hellobar_preset_required_form($preset, $type) {
  // Add Title & name fields.
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Human name'),
    '#default_value' => isset($preset['title']) ? $preset['title'] : '',
    '#weight' => -2,
    '#required' => TRUE,
  );

  if (isset($preset['name'])) {
    $form['name'] = array(
      '#type' => 'value',
      '#value' => $preset['name'],
    );
  }
  else {
    $form['name'] = array(
      '#type' => 'machine_name',
      '#title' => t('Machine name'),
      '#machine_name' => array('exists' => 'hellobar_preset_name_field_validate'),
      '#weight' => -1,
      '#required' => TRUE,
    );
  }

  // Add system fields.
  if (isset($preset['pid'])) {
    $form['pid'] = array('#type' => 'value', '#value' => $preset['pid']);
  }

  $form['type'] = array('#type' => 'value', '#value' => $type);
  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));

  return $form;
}

/**
 * Preset delete confirm form.
 *
 * @param array $form
 *   A nested array form elements comprising the form.
 * @param array $form_state
 *   An associative array containing the current state of the form.
 *
 * @return array
 *   The array containing the complete form.
 */
function hellobar_preset_delete_confirm($form, &$form_state, $preset) {
  $form['pid'] = array('#type' => 'value', '#value' => $preset['pid']);

  return confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => $preset['title'])),
    'admin/config/media/hellobar/list',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Preset delete confirm form submit.
 *
 * @param array $form
 *   A nested array form elements comprising the form.
 * @param array $form_state
 *   An associative array containing the current state of the form.
 */
function hellobar_preset_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    hellobar_preset_delete($form_state['values']['pid']);
    drupal_set_message(t('Preset has been deleted.'));
  }

  $form_state['redirect'] = 'admin/config/media/hellobar/list';
}

/**
 * Validate machine name field.
 *
 * @param string $name
 *   A field name.
 * @param array $element
 *   The structured array for the widget.
 * @param array $form_state
 *   An associative array containing the current state of the form.
 *
 * @return bool
 *   TRUE or FALSE.
 */
function hellobar_preset_name_field_validate($name, $element, $form_state) {
  $type = $form_state['values']['type'];
  return hellobar_preset_load($name, $type) ? TRUE : FALSE;
}

/**
 * Preset edit form validate.
 *
 * @param array $form
 *   A nested array form elements comprising the form.
 * @param array $form_state
 *   An associative array containing the current state of the form.
 */
function hellobar_preset_edit_form_validate($form, &$form_state) {

  // Check numeric fields.
  foreach (_hellobar_preset_formats('numeric') as $field) {
    if (isset($form_state['values'][$field])) {
      $value = $form_state['values'][$field];

      if (!empty($value) && !is_numeric($value)) {
        form_set_error($field, t('%field: %value is not numeric!', array('%field' => $field, '%value' => $value)));
      }
    }
  }
}

/**
 * Preset edit form submit.
 *
 * @param array $form
 *   A nested array form elements comprising the form.
 * @param array $form_state
 *   An associative array containing the current state of the form.
 */
function hellobar_preset_edit_form_submit($form, &$form_state) {
  form_state_values_clean($form_state);

  if (hellobar_preset_save($form_state['values'])) {
    drupal_set_message(t('Preset has been saved!'), 'status');
    $form_state['redirect'] = 'admin/config/media/hellobar/list';
  }
  else {
    drupal_set_message(t("Error! :'("), 'error');
  }
}
