<?php

/**
 * @file
 * Implementation of hook_panels_layouts().
 */

$plugin = array(
  'title' => t('(4) Four Columns'),
  'category' => t('Foundation Columns'),
  'icon' => 'fourcolumnfound.png',
  'theme' => 'panels_fourcolumnfound',
  'css' => 'fourcolumnfound.css',
  'regions' => array(
    'one' => t('Column One'),
    'two' => t('Column Two'),
    'three' => t('Column Three'),
    'four' => t('Column Four'),
  ),
);
