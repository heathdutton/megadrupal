<?php

/**
 * @file
 * Administration form file.
 *
 * Contains form functions for all admin pages.
 */

/**
 * A form wrapper functions that returns the form or echo the rendered form.
 *
 * @param string $form_id
 *   the form id
 *
 * @return array
 *   form array
 */
function extra_columns_fields($form_id) {
  $form = array();

  $form = drupal_get_form('extra_columns_fields_form', $form_id);

  if (isset($_GET['ajax'])) {
    echo render($form);
    drupal_exit();
  }

  return $form;
}

/**
 * Returns a field checkboxes form.
 *
 * @param array $form
 *   form
 * @param array $form_state
 *   form state
 * @param string $form_id
 *   the form id
 *
 * @return array
 *   field form
 */
function extra_columns_fields_form($form, $form_state, $form_id) {
  module_load_include('inc', 'extra_columns', 'includes/data');

  $field_options = array();
  $fields = field_info_field_map();
  $checked_fields = extra_columns_get_form_columns($form_id);

  // Get fields.
  foreach ($fields as $name => $field) {
    if ($form_id == EC_FORM_NODE && !empty($field['bundles']['node'])) {
      $field_options[$name] = extra_columns_get_field_label($name, ', ');
    }
    elseif ($form_id == EC_FORM_COMMENT && isset($field['bundles']['comment'])) {
      $field_options[$name] = extra_columns_get_field_label($name, ', ');
    }
  }

  // Get custom fields by this module.
  switch ($form_id) {
    case EC_FORM_NODE:
      $field_options[EC_FIELD_COMMENT_COUNT] = t('Comment Count');
      break;

    case EC_FORM_COMMENT:
      $field_options[EC_FIELD_COMMENT_HOST] = t('Comment Host');
      $field_options[EC_FIELD_COMMENT_MAIL] = t('Comment Mail');
      break;

    case EC_FORM_USER:
      $field_options[EC_FIELD_COMMENT_COUNT] = t('Comment Count');
      $field_options[EC_FIELD_NODE_COUNT] = t('Node Count');
      break;

  }

  $form['options'] = array(
    '#title'         => t('Fields'),
    '#type'          => 'checkboxes',
    '#options'       => $field_options,
    '#default_value' => $checked_fields,
  );

  $form['which_form'] = array(
    '#type'  => 'hidden',
    '#value' => $form_id,
  );

  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Add Fields'),
  );

  return $form;
}

/**
 * Handles form field submission.
 *
 * @param array $form
 *   form
 * @param array $form_state
 *   form state
 */
function extra_columns_fields_form_submit(&$form, $form_state) {
  module_load_include('inc', 'extra_columns', 'includes/data');

  $values = $form_state['values'];

  if ($values['op'] == t('Add Fields')) {
    $the_form = $values['which_form'];

    if (!empty($the_form)) {
      $options = array();

      foreach ($values['options'] as $field => $checked) {
        if (!empty($checked)) {
          $options[] = $field;
        }
      }

      extra_columns_set_form_columns($the_form, $options);
      drupal_set_message(t('Extra columns saved successfully.'));
    }
    else {
      drupal_set_message(t('Failed to save extra columns'), 'error');
    }
  }
}

/**
 * Form render function for configuration page.
 *
 * @param array $form
 *   form
 * @param array $form_state
 *   form state
 *
 * @return array
 *   config form
 */
function extra_columns_config_form($form, $form_state) {
  module_load_include('inc', 'extra_columns', 'includes/data');

  $form['table'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Table Styles'),
  );

  $form['table']['header_no_wrap'] = array(
    '#type'          => 'checkbox',
    '#title'         => t("Don't wrap table header text"),
    '#description'   => t('For readability, avoid wrapping table header text if it gets too long.'),
    '#default_value' => variable_get(EC_SETTING_HEADER_NO_WRAP, FALSE),
  );

  $form['table']['cell_no_wrap'] = array(
    '#type'          => 'checkbox',
    '#title'         => t("Don't wrap table cell text"),
    '#description'   => t('For readability, avoid wrapping table cell text if it gets too long.'),
    '#default_value' => variable_get(EC_SETTING_CELL_NO_WRAP, FALSE),
  );

  $date_options = array();
  $date_formats = system_get_date_types();
  foreach ($date_formats as $key => $type) {
    $date_options[$key] = $type['title'];
  }

  $form['table']['date_format'] = array(
    '#type'          => 'select',
    '#title'         => t('Date field format'),
    '#description'   => t('The datetime format for date field.'),
    '#options'       => $date_options,
    '#default_value' => variable_get(EC_SETTING_DATE_FORMAT, 'medium'),
  );

  $form['table']['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Save Table Settings'),
  );

  $form['forms'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Forms'),
    '#description' => t('Enable extra columns on these forms.'),
  );

  $form['forms']['form_node'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Node/Content Form'),
    '#description'   => t('Enable Extra Columns on') . ' ' . l(url('admin/content'), 'admin/content'),
    '#default_value' => variable_get(EC_SETTING_FORM_NODE, FALSE),
  );

  $form['forms']['form_comment'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Comment Form'),
    '#description'   => t('Enable Extra Columns on') . ' ' . l(url('admin/content/comment'), 'admin/content/comment'),
    '#default_value' => variable_get(EC_SETTING_FORM_COMMENT, FALSE),
  );

  $form['forms']['form_user'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('User Form'),
    '#description'   => t('Enable Extra Columns on') . ' ' . l(url('admin/people'), 'admin/people'),
    '#default_value' => variable_get(EC_SETTING_FORM_USER, FALSE),
  );

  $form['forms']['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Save Forms'),
  );

  $form['reset'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Reset Forms'),
    '#description' => t('Remove all added columns from all forms.'),
  );

  $form['reset']['do_reset'] = array(
    '#type'  => 'submit',
    '#value' => t('Reset'),
  );

  return $form;
}

/**
 * Handles main configuration form submission.
 *
 * @param array $form
 *   form
 * @param array $form_state
 *   form state
 */
function extra_columns_config_form_submit($form, $form_state) {
  module_load_include('inc', 'extra_columns', 'includes/data');

  $values = $form_state['values'];
  if ($values['op'] == t('Save Table Settings')) {
    variable_set(EC_SETTING_HEADER_NO_WRAP, $values['header_no_wrap']);
    variable_set(EC_SETTING_CELL_NO_WRAP, $values['cell_no_wrap']);

    variable_set(EC_SETTING_DATE_FORMAT, $values['date_format']);

    drupal_set_message(t('Saved configurations.'));
  }
  elseif ($values['op'] == t('Save Forms')) {
    variable_set(EC_SETTING_FORM_NODE, $values['form_node']);
    variable_set(EC_SETTING_FORM_COMMENT, $values['form_comment']);
    variable_set(EC_SETTING_FORM_USER, $values['form_user']);

    drupal_set_message(t('Saved configurations.'));
  }
  elseif ($values['op'] == t('Reset')) {
    variable_del('extra_columns_fields_' . EC_FORM_NODE);
    variable_del('extra_columns_fields_' . EC_FORM_COMMENT);
    variable_del('extra_columns_fields_' . EC_FORM_USER);

    drupal_set_message(t('Removed added columns from all forms.'));
  }
}
