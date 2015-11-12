<?php

/**
 * Implements hook_field_info().
 */
function schema_reference_field_info() {
  $info['schema_reference'] = array(
    'label' => t('Schema reference'),
    'description' => t('Stores a reference to a database connection, table, and or column.'),
    'settings' => array(
      'allowed_connections' => array(),
      'select_table' => 1,
      'select_column' => 0,
    ),
    'default_widget' => 'schema_reference_default',
    'default_formatter' => 'hidden',
  );

  return $info;
}

/**
 * Implements hook_field_settings_form().
 */
function schema_reference_field_settings_form($field, $instance, $has_data) {
  $settings = $field['settings'];

  $form['allowed_connections'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Allowed connections'),
    '#description' => t('If none selected, they will all be available for the user to pick from.'),
    '#options' => schema_get_connection_options(),
    '#default_value' => $settings['allowed_connections'],
  );

  $form['select_table'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow selection of a table in the selected connection database.'),
    '#default_value' => $settings['select_table'],
    '#disabled' => $has_data,
  );

  $form['select_column'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow selection of a column in the selected table.'),
    '#default_value' => $settings['select_column'],
    '#disabled' => $has_data,
    '#states' => array(
      'invisible' => array(
        ':input[name="field[settings][select_table]"]' => array('checked' => FALSE),
      ),
    ),
  );

  return $form;
}

/**
 * Implements hook_field_is_empty().
 */
function schema_reference_field_is_empty($item, $field) {
  if (empty($item['connection'])) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Implements hook_field_widget_info().
 */
function schema_reference_field_widget_info() {
  $info['schema_reference_default'] = array(
    'label' => t('Default'),
    'field types' => array('schema_reference'),
  );

  return $info;
}

/**
 * Implements hook_field_widget_form().
 */
function schema_reference_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  if ($field['cardinality'] == 1) {
    $element['#type'] = 'fieldset';
  }

  $parents = $element['#field_parents'];
  $parents[] = $field['field_name'];
  $parents[] = $langcode;
  $parents[] = $delta;

  $state_parent = $parents[0] . '[' . implode('][', array_slice($parents, 1));

  $table_wrapper = drupal_html_id(implode('-', $parents) . '-table-wrapper');
  $column_wrapper = drupal_html_id(implode('-', $parents) . '-column-wrapper');

  $connection_options = schema_get_connection_options();
  $allowed_connections = array_filter($field['settings']['allowed_connections']);
  if (!empty($allowed_connections)) {
    $connection_options = array_intersect_key($connection_options, $allowed_connections);
  }
  $element['connection'] = array(
    '#type' => 'select',
    '#title' => t('Connection'),
    '#options' => $connection_options,
    '#default_value' => isset($items[$delta]['connection']) ? $items[$delta]['connection'] : NULL,
    '#empty_value' => '',
    '#required' => $element['#required'],
  );
  if (!empty($field['settings']['select_table'])) {
    $element['connection']['#ajax'] = array(
      'callback' => 'schema_reference_widget_ajax_callback',
      'wrapper' => $table_wrapper,
    );
  }

  $connection = drupal_array_get_nested_value($form_state, array_merge(array('values'), $parents, array('connection')));
  if (!isset($connection) && !empty($element['connection']['#default_value'])) {
    $connection = $element['connection']['#default_value'];
  }

  if (!empty($connection) && !empty($field['settings']['select_table'])) {
    $element['table'] = array(
      '#type' => 'select',
      '#title' => t('Table/View'),
      '#options' => !empty($connection) ? schema_reference_get_table_options($connection) : array(),
      '#default_value' => isset($items[$delta]['table']) ? $items[$delta]['table'] : NULL,
      '#empty_value' => '',
      '#required' => $element['#required'],
      '#prefix' => '<div id="' . $table_wrapper . '">',
      '#suffix' => '</div>',
    );
    if (!empty($field['settings']['select_column'])) {
      $element['table']['#ajax'] = array(
        'callback' => 'schema_reference_widget_ajax_callback',
        'wrapper' => $column_wrapper,
      );
    }
  }
  else {
    $element['table'] = array(
      '#type' => 'hidden',
      '#value' => NULL,
      '#prefix' => '<div id="' . $table_wrapper . '">',
      '#suffix' => '</div>',
      '#access' => TRUE,
    );
  }

  $table = drupal_array_get_nested_value($form_state, array_merge(array('values'), $parents, array('table')));
  if (!isset($table) && !empty($element['table']['#default_value'])) {
    $table = $element['table']['#default_value'];
  }

  if (!empty($table) && !empty($field['settings']['select_table']) && !empty($field['settings']['select_column'])) {
    $element['column'] = array(
      '#type' => 'select',
      '#title' => t('Column'),
      '#options' => (!empty($connection) && !empty($table) ? schema_reference_get_column_options($connection, $table) : array()),
      '#default_value' => isset($items[$delta]['column']) ? $items[$delta]['column'] : NULL,
      '#empty_value' => '',
      '#empty_option' => 'None',
      '#required' => $element['#required'],
      '#prefix' => '<div id="' . $column_wrapper . '">',
      '#suffix' => '</div>',
    );
  }
  else {
    $element['column'] = array(
      '#type' => 'hidden',
      '#value' => NULL,
      '#prefix' => '<div id="' . $column_wrapper . '">',
      '#suffix' => '</div>',
    );
  }

  return $element;
}

/**
 * AJAX callback for showing/hiding the table and/or column fields as needed.
 */
function schema_reference_widget_ajax_callback($form, $form_state) {
  $parents = $form_state['triggering_element']['#array_parents'];

  // The type element triggered this, so remove it from parents.
  $element = array_pop($parents);
  switch ($element) {
    case 'connection':
      // If the connection field was changed, update the table field.
      $parents[] = 'table';
      break;
    case 'table':
      // If the table field was changed, update the column field.
      $parents[] = 'column';
      break;
  }

  return drupal_array_get_nested_value($form, $parents);
}

/**
 * Implements hook_field_widget_error().
 */
function schema_reference_field_widget_error($element, $error, $form, &$form_state) {
  $error_element = $element;
  if (!empty($error['element'])) {
    $error_element = drupal_array_get_nested_value($element, $error['element']);
  }
  form_error($error_element, $error['message']);
}

/**
 * Implements hook_field_formatter_view().
 */
function schema_reference_field_formatter_info() {
  $info['schema_reference_default'] = array(
    'label' => t('Default'),
    'field types' => array('schema_reference'),
  );

  return $info;
}

/**
 * Implements hook_field_formatter_view().
 */
function schema_reference_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  $settings = $display['settings'];

  foreach ($items as $delta => $item) {
    $list = array();
    $list[] = t('Connection: @value', array('@value' => $item['connection']));
    if (!empty($field['settings']['select_table']) && !empty($item['table'])) {
      $list[] = t('Table: @value', array('@value' => $item['table']));
    }
    if (!empty($field['settings']['select_column']) && !empty($item['column'])) {
      $list[] = t('Column: @value', array('@value' => $item['column']));
    }
    $element[$delta] = array(
      '#theme' => 'item_list',
      '#items' => $list,
    );
  }

  return $element;
}
