<?php

/**
 * Theme function for entity properties.
 *
 * Simple wrapper around theme_field that sets default values and ensures
 * properties render consistently with fields.
 */
function theme_entityspice_entity_property_field($variables) {
  return theme('field', $variables);
}

/**
 * Implements hook_formatter_info().
 *
 * Provided field display formatters:
 *   - entityspice_nl2br : converts newlines to HTML <br> tag.
 */
function entityspice_field_formatter_info() {
  $formatter['entityspice_nl2br'] = array(
    'description' => t('A regular text formatter where it converts newlines to HTML BR tag.'),
    'label' => t('EntitySpice nl2br'),
    'field types' => array('text_long'),
  );

  return $formatter;
}

/**
 * Implements hook_field_formatter_view().
 *
 * @see entityspice_field_formatter_info().
 */
function entityspice_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();

  switch ($display['type']) {
    case 'entityspice_nl2br':
      foreach ($items as $delta => $item) {
        $element[$delta] = array('#markup' => nl2br(strip_tags($item['value'])));
      }
      break;
  }

  return $element;
}

