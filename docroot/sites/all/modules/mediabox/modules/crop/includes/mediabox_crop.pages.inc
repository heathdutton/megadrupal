<?php

/**
 * @file
 * Mediabox crop pages.
 */

function mediabox_crop_styles($mediabox) {
  drupal_add_library('mediabox_crop', 'Jcrop');

  $path = drupal_get_path('module', 'mediabox_crop');
  drupal_add_css($path . '/css/mediabox_crop.css');
  drupal_add_js($path . '/js/mediabox_crop.js');

  module_load_include('inc', 'mediabox_crop', 'includes/mediabox_crop.forms');
  return drupal_get_form('mediabox_crop_form_styles', $mediabox);
}
