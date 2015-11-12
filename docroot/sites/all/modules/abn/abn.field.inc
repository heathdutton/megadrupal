<?php
/**
 * @file
 * Field definition for ABN fields.
 */

/**
 * Implements hook_field_info().
 */
function abn_field_info() {
  return array(
    'abn' => array(
      'label' => t('Australian Business Number'),
      'description' => t('Store an ABN (Australian Business Number) in the database.'),
      'default_widget' => 'abn_text',
      'default_formatter' => 'abn_spaced',
      'settings' => array(
        'sync' => ABN_SYNC_DEFAULT,
        'expires' => ABN_EXPIRES_DEFAULT,
      ),
    ),
  );
}

/**
 * Implements hook_field_settings_form().
 */
function abn_field_settings_form($field, $instance, $has_data) {
  $form = array();
  $form['sync'] = array(
    '#type' => 'checkbox',
    '#title' => t('Request public data associated with each ABN in this field from the ABR (Recommended).'),
    '#description' => t('Without sending requests to the ABR this field can only display each ABN in plain text format, ie. no company trading or historical information.'),
    '#default_value' => isset($field['settings']['sync']) ? $field['settings']['sync'] : ABN_SYNC_DEFAULT,
  );

  $form['expires'] = array(
    '#type' => 'select',
    '#title' => t('Minimum lifetime of ABR data'),
    '#description' => t('Smaller values improve freshness of ABR data, at the expense of scalability as expired fields are updated every time cron runs. <br />This setting does not prevent data for this field being updated as content is saved.'),
    '#options' => array(
      ABN_EXPIRES_HOUR => '1 hour',
      ABN_EXPIRES_DAY => '1 day',
      ABN_EXPIRES_WEEK => '1 week',
      ABN_EXPIRES_FORTNIGHT => '2 weeks',
      ABN_EXPIRES_MONTH => '1 month (default)',
      ABN_EXPIRES_QUARTER => '3 months',
      ABN_EXPIRES_SEMESTER => '6 months',
      ABN_EXPIRES_YEAR => '1 year',
      ABN_EXPIRES_NEVER => 'Never expire data',
    ),
    '#default_value' => isset($field['settings']['expires']) ? $field['settings']['expires'] : ABN_EXPIRES_DEFAULT,
  );

  return $form;
}

/**
 * Implements hook_field_validate().
 */
function abn_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {
  foreach ($items as $delta => $item) {
    if (!empty($item['abn'])) {
      // Don't consider whitespace in validation.
      $abn = preg_replace('@\s+@', '', $item['abn']);

      if (!ctype_digit($abn)) {
        $errors[$field['field_name']][$langcode][$delta][] = array(
          'error' => 'abn_invalid',
          'message' => t('An ABN must contain only numbers.'),
        );
      }

      // An ABN is 11 digits.
      if (drupal_strlen($abn) !== 11) {
        $errors[$field['field_name']][$langcode][$delta][] = array(
          'error' => 'abn_invalid',
          'message' => t('An ABN must be exactly 11 digits long.'),
        );
      }
    }
  }
}

/**
 * Implements hook_field_presave().
 */
function abn_field_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
  // Do nothing if this field does not have syncing enabled.
  if (!$field['settings']['sync']) {
    return;
  }

  // Don't perform expensive syncing operations if the field values match the
  // "original" entity. No changes have been made to this item since last save.
  $original_items = isset($entity->original->{$field['field_name']}[$langcode]) ? $entity->original->{$field['field_name']}[$langcode] : NULL;
  foreach ($items as $key => $item) {
    if (empty($original_items[$key]) || $original_items[$key] != $item) {
      abn_sync_field_item($items[$key], $field, FALSE);
    }
  }
}

/**
 * Implements hook_field_is_empty().
 */
function abn_field_is_empty($item, $field) {
  return empty($item['abn']);
}

/**
 * Implements hook_field_formatter_info().
 */
function abn_field_formatter_info() {
  $formatters = array(
    'abn' => array(
      'label' => t('ABN: plain text'),
    ),
    // Spaces follow the convention on the ABR website.
    'abn_spaced' => array(
      'label' => t('ABN: spaced (default)'),
    ),
    'abn_normalized' => array(
      'label' => t('ABN: without spaces'),
    ),
    'abn_status' => array(
      'label' => t('ABN: ABN status'),
    ),
    'abn_name' => array(
      'label' => t('Business: name'),
    ),
    'abn_gst_start' => array(
      'label' => t('Business: GST start date'),
    ),
    'abn_type' => array(
      'label' => t('Business: type'),
    ),
    'abn_type_code' => array(
      'label' => t('Business: type code'),
    ),
    'abn_address_state' => array(
      'label' => t('Business address: state'),
    ),
    'abn_address_postcode' => array(
      'label' => t('Business address: postcode'),
    ),
    'updated' => array(
      'label' => t('ABR data: last updated timestamp'),
    ),
  );

  foreach ($formatters as $key => $formatter) {
    $formatters[$key]['field types'] = array('abn');
  }
  return $formatters;
}

/**
 * Implements hook_field_formatter_view().
 */
function abn_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  foreach ($items as $delta => $item) {
    $element[$delta] = array(
      '#type' => 'html_tag',
      '#tag' => 'span',
      '#attributes' => array(
        'class' => str_replace('_', '-', $display['type']),
      ),
    );

    $formatted_value = '';
    switch ($display['type']) {
      case 'abn_spaced':
        // Convert to an array so we can splice in spaces.
        $formatted_value = str_split(abn_normalize_abn($item['abn']));
        // Need spaces after first 2 digits and every third digit after.
        array_splice($formatted_value, 8, 0, ' ');
        array_splice($formatted_value, 5, 0, ' ');
        array_splice($formatted_value, 2, 0, ' ');
        $formatted_value = implode($formatted_value);
        break;

      case 'abn_normalized':
        $formatted_value = abn_normalize_abn($item['abn']);
        break;

      // If the formatter has the same name as the field we can just sanitize
      // and return it by default.
      default:
        $formatted_value = isset($item[$display['type']]) ? check_plain($item[$display['type']]) : '';
        break;

    }
    $element[$delta]['#value'] = $formatted_value;
  }

  return $element;
}

/**
 * Implements hook_field_widget_info().
 */
function abn_field_widget_info() {
  return array(
    'abn_text' => array(
      'label' => t('ABN'),
      'field types' => array('abn'),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function abn_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {

  $base_widget = $element;
  $base_widget['#delta'] = $delta;

  $abn_widget = $base_widget + array(
    '#type' => 'textfield',
    '#maxlength' => 16,
    '#size' => 14,
    '#default_value' => isset($items[$delta]['abn']) ? $items[$delta]['abn'] : '',
    // Display the name registered to the ABN if we know it.
    '#field_suffix' => isset($items[$delta]['abn_name']) ? $items[$delta]['abn_name'] : '',
  );
  $element['abn'] = $abn_widget;

  // All data pulled from the ABR is stored in hidden fields that the user
  // cannot access directly.
  $hidden_fields = array(
    'abn_name',
    'abn_status',
    'abn_gst_start',
    'abn_type',
    'abn_type_code',
    'abn_address_state',
    'abn_address_postcode',
    'updated',
  );

  $hidden_widget = $base_widget + array(
    '#type' => 'hidden',
    '#access' => FALSE,
  );

  foreach ($hidden_fields as $hidden_field) {
    $element[$hidden_field] = $hidden_widget + array(
      '#default_value' => isset($items[$delta][$hidden_field]) ? $items[$delta][$hidden_field] : '',
    );
  }

  return $element;
}
