<?php

/**
 * @file
 * The theme system, which controls the output of Drupal.
 *
 */

/**
 * A custom theme function for showing a remote image
 */
function theme_cjunction_fields_image_url($variables) {
  $output = theme('image', array("path" => $variables['image_url'], "alt" => '', "title" => '',  "attributes" => array("height" => "100px"), "getsize" => FALSE));
  return $output;
}

/**
 * A custom theme function for showing a buy link
 */
function theme_cjunction_fields_buy_url($variables) {
  $output = l(t('Buy It'), $variables['buy_url']);
  return $output;
}


