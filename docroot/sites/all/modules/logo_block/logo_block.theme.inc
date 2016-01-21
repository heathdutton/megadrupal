<?php

/**
 * @file
 * Theme callback functions for Logo Block
 */

/**
 * Theme wrapper for logo block.
 */
function theme_logo_block_wrapper($variables) {
  $site_name = variable_get('site_name', '');
  $link = variable_get('logo_block_link', '');
  if ($link) {
    return l($variables['element']['#children'], $link, array(
      'absolute' => TRUE,
      'attributes' => array('title' => $site_name),
      'html' => TRUE));
  }
  return $variables['element']['#children'];
}
