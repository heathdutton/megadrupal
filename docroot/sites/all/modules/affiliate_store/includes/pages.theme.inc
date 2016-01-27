<?php

/**
 * @file
 * Theme and template preprocess functions for pages.
 */

/**
 * Implements theme passthrough hook.
 */
function _affiliate_store_pages_theme(&$theme) {
  $path = drupal_get_path('module', 'affiliate_store');
  $theme['affiliate_store'] = array(
    'render element' => 'elements',
    'file' => 'theme.inc',
    'template' => 'affiliate-store',
    'path' => "$path/theme",
  );
}
