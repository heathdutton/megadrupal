<?php

/**
 * Implements hook_field_formatter_info().
 *
 */
function brilliant_gallery_field_formatter_info() {
  return array(
    'brilliant_gallery_render' => array(
      'label' => t('Brilliant Gallery: local path, BT Sync secret (default 33 bytes only), Picasa RSS link or Google+ album URL'),
      'field types' => array('text'),
    ),
    'brilliant_gallery_single' => array(
      'label' => t('A random image from Brilliant Gallery: local path, BT Sync secret (default 33 bytes only), Picasa RSS link or Google+ album URL'),
      'field types' => array('text'),
    ),
  );
}

/**
 * Implements hook_field_formatter_view().
 *
 */
function brilliant_gallery_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();

  switch ($display['type']) {

    case 'brilliant_gallery_render':
      foreach ($items as $delta => $item) {
        $pathurl = trim($item['value']);
        // If this is a BT Sync folder (secret), change $pathurl
        if (function_exists('replace_brilliant_gallery_tags') AND $pathurl <> ''){
          $result = '';
          $bgcode = '[bg|'.$pathurl.']';
          $gallery = replace_brilliant_gallery_tags($bgcode);
          if (strip_tags($gallery) <> ''){ // Useful?
            $result .= $gallery; // <br clear=all><br clear=all>
          }
          $element[$delta] = array(
            '#markup' => $result,
          );
        }
      }
      break;

    case 'brilliant_gallery_single':
      foreach ($items as $delta => $item) {
        $pathurl = trim($item['value']);
        // If this is a BT Sync folder (secret), change $pathurl
        if (function_exists('replace_brilliant_gallery_tags') AND $pathurl <> ''){
          $result = '';
          // [bg|path/to/your/gallery/folder/without/wrapping/slashes |columncountoverride|widthoverride|sortorrandomoverride|maximumnumbertoshow|colouroverride|beginfromoverride|caption-yes-no-text]
          // maximumnumbertoshow = 1
          // random
          // columnoverride = 0 ... because it does not matter, we show just 1 image
          // widthoverride ... taken from settings
          $bgcode = '[bg|'.$pathurl.'|0|'.variable_get('brilliant_gallery_maximagewidth', 150).'|random|1]';
          $gallery = replace_brilliant_gallery_tags($bgcode);
          if (strip_tags($gallery) <> ''){ // Useful?
            $result .= $gallery;
          }
          $element[$delta] = array(
            '#markup' => $result,
          );
        }
      }
      break;

  }

  return $element;
}
