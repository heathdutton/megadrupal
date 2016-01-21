<?php

/**
 * @file
 * Defines code field type that can be used with the Options module.
 */

/**
 * Implements hook_field_info().
 */
function codes_field_info() {
  return array(
    'codes' => array(
      'label' => t('Code'),
      'description' => t('This field stores code.'),
      'settings' => array(
        'module' => '',
        'codeset' => '',
      ),
      'default_widget' => 'options_select',
      'default_formatter' => 'codes_default',
      'default_token_formatter' => 'codes_code',
    ),
  );
}

/**
 * Implements hook_field_schema().
 */
function codes_field_schema($field) {
  switch ($field['type']) {
    case 'codes':
      $columns = array(
        'value' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => FALSE,
        ),
      );
      break;
  }

  return array(
    'columns' => $columns,
    'indexes' => array(
      'value' => array('value'),
    ),
  );
}

/**
 * Implements hook_field_settings_form().
 */
function codes_field_settings_form($field, $instance, $has_data) {
  $settings = $field['settings'];
  $module_info = system_get_info('module');
  $modules = codes_get_module_apis();

  foreach ($modules as $module => $info) {
    $module_names[$module] = check_plain($module_info[$module]['name']);
  }
  $module = !empty($settings['module']) ? $settings['module'] : key($module_names);
  $form['module'] = array(
    '#type' => 'select',
    '#title' => t('Module'),
    '#options' => $module_names,
    '#default_value' => $module,
    '#required' => TRUE,
    '#description' => t('The module implementing the codeset.'),
    '#ajax' => array(
      'callback' => 'codes_dropdown_codeset_callback',
      'wrapper' => 'dropdown-codeset-replace',
    ),
  );

  $codesets = codes_get_codesets($module);
  $form['codeset'] = array(
    '#type' => 'select',
    '#title' => t('Codeset'),
    '#options' => $codesets,
    '#default_value' => $settings['codeset'],
    '#required' => TRUE,
    '#description' => t('The codeset to use.'),
    '#prefix' => '<div id="dropdown-codeset-replace">',
    '#suffix' => '</div>',
    '#validated' => TRUE, // the ajax callback triggers validation errors.
  );

  return $form;
}

function codes_dropdown_codeset_callback($form, $form_state) {
  $codesets = codes_get_codesets($form_state['triggering_element']['#value']);
  $form['field']['settings']['codeset']['#options'] = $codesets;
  return $form['field']['settings']['codeset'];
}

/**
 * Implements hook_field_is_empty().
 */
function codes_field_is_empty($item, $field) {
  if (empty($item['value'])) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Implements hook_field_widget_info_alter().
 */
function codes_field_widget_info_alter(&$info) {
  $widgets = array(
    'options_select' => array('codes'),
    'options_buttons' => array('codes'),
  );

  foreach ($widgets as $widget => $field_types) {
    if (isset($info[$widget])) {
      $info[$widget]['field types'] = array_merge($info[$widget]['field types'], $field_types);
    }
  }
}

/**
 * Implements hook_options_list().
 */
function codes_options_list($field) {
  $allowed_values = &drupal_static(__FUNCTION__, array());

  // @todo: language dependent cache.
  if (!isset($allowed_values[$field['id']])) {
    $module = $field['settings']['module'];
    $codeset = $field['settings']['codeset'];
    $allowed_values[$field['id']] = codes_get_all($module, $codeset);
  }

  return $allowed_values[$field['id']];
}

/**
 * Implements hook_field_formatter_info().
 */
function codes_field_formatter_info() {
  return array(
    'codes_default' => array(
      'label' => t('Default'),
      'field types' => array('codes'),
    ),
    'codes_div' => array(
      'label' => t('Div with class'),
      'field types' => array('codes'),
    ),
    'codes_code' => array(
      'label' => t('Code'),
      'field types' => array('codes'),
    ),
  );
}

/**
 * Implements hook_field_formatter_view().
 */
function codes_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();

  switch ($display['type']) {
    case 'codes_default':
    case 'codes_div':
      $allowed_values = codes_options_list($field);
      foreach ($items as $delta => $item) {
        if (isset($allowed_values[$item['value']])) {
          $output = field_filter_xss($allowed_values[$item['value']]);
        }
        else {
          // If no match was found in allowed values, fall back to the key.
          $output = field_filter_xss($item['value']);
        }
        if ($display['type'] == 'codes_div') {
          $code = field_filter_xss($item['value']);
          $output = '<div class="' . drupal_html_class($code) . '">' . $output . '</div>';
        }

        $element[$delta] = array('#markup' => $output);
      }
      break;

    case 'codes_code':
      foreach ($items as $delta => $item) {
        $element[$delta] = array('#markup' => field_filter_xss($item['value']));
      }
      break;
  }

  return $element;
}
