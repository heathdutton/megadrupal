<?php

/**
 * @file
 * Preprocess functions for Views RSS: Media Elements module.
 */

/**
 * Preprocess function for item <media:content> element.
 */
function views_rss_media_preprocess_item_content(&$variables) {
  // No value = no preprocessing.
  if (empty($variables['elements'][0]['value'])) {
    return;
  }
  if (!empty($variables['raw'])) {
    $value = array();
    foreach ($variables['raw'] as $delta => $element) {
      if (!empty($element['rendered']['#rss_element'])) {
        $value[] = $element['rendered']['#rss_element'];
      }
    }
    if (count($variables['raw']) > 1 && isset($variables['raw'][0]['rendered']['#settings']['group_multiple_values']) && $variables['raw'][0]['rendered']['#settings']['group_multiple_values']) {
      $value = array(array(
        'key' => 'media:group',
        'value' => $value,
      ));
    }
    $variables['elements'] = $value;
  }
}

/**
 * Preprocess function for item <media:title> and <media:description> elements.
 */
function views_rss_media_preprocess_item_text(&$variables) {
  // No value = no preprocessing.
  if (empty($variables['elements'][0]['value'])) {
    return;
  }
  // Check whether original value format is text or HTML.
  $type = 'plain';
  $value_decoded = htmlspecialchars_decode($variables['elements'][0]['value'], ENT_QUOTES);
  if ($value_decoded != strip_tags($value_decoded)) {
    $type = 'html';
  }
  // Add "type" argument to an element with its format.
  $variables['elements'][0]['attributes']['type'] = $type;
}

/**
 * Preprocess function for item <media:category> element.
 */
function views_rss_media_preprocess_item_category(&$variables) {
  // No value = no preprocessing.
  if (empty($variables['elements'][0]['value'])) {
    return;
  }
  if (!empty($variables['raw'])) {
    $value = array();
    foreach ($variables['raw'] as $delta => $element) {
      if (!empty($element['rendered']['#rss_element'])) {
        $value[] = $element['rendered']['#rss_element'];
      }
    }
    $variables['elements'] = $value;
  }
}
