<?php

/**
 * @file
 * Implementation of hook_panels_layouts().
 */

$plugin = array(
  'title' => t('(5) Five Columns'),
  'category' => t('Foundation Columns'),
  'icon' => 'fivecolumnfound.png',
  'theme' => 'panels_fivecolumnfound',
  'css' => 'fivecolumnfound.css',
  'regions' => array(
    'one' => t('Column One'),
    'two' => t('Column Two'),
    'three' => t('Column Three'),
    'four' => t('Column Four'),
    'five' => t('Column Five'),
  ),
);
