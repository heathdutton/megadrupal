<?php

/**
 * Implements hook_field_info().
 */
function gittip_giving_field_info() {
  $info = array();

  $info['gittip_giving'] = array(
    'label' => t('Gittip giving'),
    'settings' => array(
      'username' => '',
      'api_key' => '',
      'maximum_amount' => '',
    ),
    'default_widget' => 'gittip_giving_default',
    'default_formatter' => 'hidden',
  );

  return $info;
}

/**
 * Implements hook_field_is_empty().
 */
function gittip_giving_field_is_empty($item, $field) {
  if (empty($item['username']) && (float) $item['amount'] <= 0) {
    return TRUE;
  }
}

/**
 * Implements hook_field_validate().
 */
function gittip_giving_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {
  foreach ($items as $delta => $item) {
    if (!gittip_giving_field_is_empty($item, $field)) {
      if (!gittip_giving_is_username_valid($item['username'], $item['platform'])) {
        $errors[$field['field_name']][$langcode][$delta][] = array(
          'error' => 'gittip_giving_username_invalid',
          'message' => t('The @platform username %username is invalid.', array('@platform' => $item['platform'], '%username' => $item['username'])),
          'element' => array('username'),
        );
      }
    }
  }

  if (!empty($field['settings']['maximum_amount'])) {
    $total_amount = gittip_giving_calculate_total($items);
    if ($total_amount > $field['settings']['maximum_amount']) {
      $errors[$field['field_name']][$langcode][0][] = array(
        'error' => 'gittip_giving_maximum_exceeded',
        'message' => t('The total amount of Gittips ($@amount) exceeds the allowed maximum ($@max).', array('@amount' => $total_amount, '@max' => $field['settings']['maximum_amount'])),
      );
    }
  }
}

/**
 * Implements hook_field_update_forbid().
 */
function gittip_giving_field_update_forbid($field, $prior_field, $has_data) {
  // Check that the API key being used for each field is unique.
  if ($field['type'] == 'gittip_giving') {
    $fields = field_read_fields(array('type' => 'gittip_giving'));
    foreach ($fields as $existing_field) {
      if ($field['field_name'] != $existing_field['field_name'] && $field['settings']['api_key'] == $existing_field['settings']['api_key']) {
        throw new FieldUpdateForbiddenException("Cannot re-use the same Gittip API key between different fields.");
      }
    }
  }
}

/**
 * Implements hook_field_settings_form().
 */
function gittip_giving_field_settings_form($field, $instance, $has_data) {
  $settings = $field['settings'];
  $form['username'] = array(
    '#type' => 'textfield',
    '#title' => t('Gittip username to distribute gittips'),
    '#default_value' => $settings['username'],
    '#required' => TRUE,
    '#disabled' => $has_data,
    '#size' => 15,
  );
  $form['api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Gittip user API key'),
    '#default_value' => $settings['api_key'],
    '#required' => TRUE,
    '#disabled' => $has_data,
  );
  $form['maximum_amount'] = array(
    '#type' => 'numberfield',
    '#title' => t('Maximum amount for each field value'),
    '#description' => t('Leave empty for no limit.'),
    '#default_value' => $settings['maximum_amount'],
    '#required' => FALSE,
    '#min' => 0,
    '#step' => 0.01,
    '#field_prefix' => '$',
    '#size' => 5,
  );
  return $form;
}

/**
 * Implements hook_field_widget_info().
 */
function gittip_giving_field_widget_info() {
  $info = array();

  $info['gittip_giving_default'] = array(
    'label' => t('Default'),
    'weight' => -100,
    'field types' => array('gittip_giving'),
  );

  return $info;
}

function gittip_giving_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $item = isset($items[$delta]) ? $items[$delta] : array();

  if ($field['cardinality'] == 1) {
    $element['#type'] = 'fieldset';
  }

  $element['username'] = array(
    '#prefix' => '<div class="container-inline">',
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#default_value' => isset($item['username']) ? $item['username'] : '',
    '#required' => $element['#required'],
    '#size' => 15,
  );
  $element['platform'] = array(
    '#type' => 'select',
    '#title' => t('On'),
    '#options' => array(
      'gittip' => t('Gittip'),
      'twitter' => t('Twitter'),
    ),
    '#default_value' => isset($item['platform']) ? $item['platform'] : 'gittip',
    '#access' => FALSE,
  );
  $element['amount'] = array(
    '#type' => 'numberfield',
    '#title' => t('Amount'),
    '#min' => 0,
    '#step' => 0.01,
    '#field_prefix' => '$',
    '#default_value' => gittip_giving_number_format(isset($item['amount']) ? $item['amount'] : 0),
    '#required' => $element['#required'],
    '#size' => 5,
  );
  if (!empty($field['settings']['maximum_amount'])) {
    $element['amount']['#max'] = (float) $field['settings']['maximum_amount'];
  }
  $element['reason'] = array(
    '#type' => 'textfield',
    '#title' => t('Reason'),
    '#default_value' => isset($item['reason']) ? $item['reason'] : '',
    //'#description' => t('Why give this person a gittip?'),
    '#suffix' => '</div>',
    '#placeholder' => t('Why give this person a gittip?'),
  );

  return $element;
}

/**
 * Implements hook_field_widget_error().
 */
function gittip_giving_field_widget_error($element, $error, $form, &$form_state) {
  $error_element = $element;
  if (!empty($error['element'])) {
    $error_element = drupal_array_get_nested_value($element, $error['element']);
  }
  if ($error_element) {
    form_error($error_element, $error['message']);
  }
}

/**
 * Implements hook_field_formatter_info().
 */
function gittip_giving_field_formatter_info() {
  $info = array();

  $info['gittip_giving_total'] = array(
    'label' => t('Giving total'),
    'field types' => array('gittip_giving'),
  );

  return $info;
}

/**
 * Implements hook_field_formatter_view().
 */
function gittip_giving_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  $settings = $display['settings'];

  switch ($display['type']) {
    case 'gittip_giving_total':
      $total = gittip_giving_calculate_total($items);
      $element[0]['#markup'] = t('Giving @amount', array('@amount' => gittip_giving_number_format($total, '$')));
      break;
  }

  return $element;
}
