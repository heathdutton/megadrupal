<?php

/**
 * Implements hook_field_formatter_info().
 */
function ad_field_formatter_info() {
  // A simple formatter which links the image to the destination URLs, if any.
  $formatters = array(
    'ad_image' => array(
      'label' => t('Ad Image'),
      'field types' => array('image'),
    ),
  );

  return $formatters;
}

/**
 * Implements hook_field_formatter_view().
 */
function ad_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  $destination = ad_get_destination($entity);
  if (!empty($destination)) {
    // Also add placeholders for impression id.
    $uri = 'ad/click/' . $entity->nid . '/' . AD_IMPRESSION_ID_PLACEHOLDER;
  }
  else {
    $uri = '';
  }

  foreach ($items as $delta => $item) {
    $element[$delta] = array(
      // Reuse theme_image_formatter().
      '#theme' => 'image_formatter',
      '#item' => $item,
      '#image_style' => '',
      '#path' => isset($uri) ? array('path' => $uri) : '',
    );
  }

  return $element;
}
