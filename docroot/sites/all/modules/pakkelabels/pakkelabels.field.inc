<?php

/**
 * @file
 * Field hooks for pakkelabels
 */

/**
 * Implements hook_field_formatter_info().
 *
 * Formatter to display.
 */
function pakkelabels_field_formatter_info() {
  return array(
    'pakkelabels_shipment_id_formatter' => array(
      'label' => t('Pakkelabels.dk Formatter'),
      'field types' => array('text'),
      'multiple values' => FIELD_BEHAVIOR_DEFAULT,
      'settings'  => array(),
    ),
  );
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function pakkelabels_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $element = array();
  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function pakkelabels_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $summary = '';
  return $summary;
}

/**
 * Implements hook_field_formatter_view().
 */
function pakkelabels_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $settings = $display['settings'];
  $element = array();

  foreach ($items as $delta => $item) {
    // Getting the actual value.
    $pakkelabels_shipment_id = $item['value'];

    if (empty($pakkelabels_shipment_id)) {
      $element[$delta]['#markup'] = t('Pending');
    }
    else {
      $options = array(
        // This tells Drupal that we're sending HTML, not plain text, otherwise it would encode it.
        'html' => TRUE,
        'attributes'  => array(
          'title' => t('Shipment'),
        ),
      );

      $link = l(t('Shipment'), pakkelabels_get_shipment_link($pakkelabels_shipment_id), $options);
      // Assign it to the #markup of the element.
      $element[$delta]['#markup'] = $link;
    }
  }
  return $element;
}
