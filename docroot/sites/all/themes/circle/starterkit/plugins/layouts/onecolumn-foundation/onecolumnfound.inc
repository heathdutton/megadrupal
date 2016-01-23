<?php

/**
 * @file
 * Implementation of hook_panels_layouts().
 */

$plugin = array(
  'title' => t('(1) One Column'),
  'category' => t('Foundation Columns'),
  'icon' => 'onecolumnfound.png',
  'theme' => 'panels_onecolumnfound',
  'css' => 'onecolumnfound.css',
  'regions' => array(
    'one' => t('Main'),
  ),
);
