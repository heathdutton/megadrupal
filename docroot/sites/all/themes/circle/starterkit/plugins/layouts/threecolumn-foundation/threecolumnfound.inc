<?php

/**
 * @file
 * Implementation of hook_panels_layouts().
 */

$plugin = array(
  'title' => t('(3) Three Columns'),
  'category' => t('Foundation Columns'),
  'icon' => 'threecolumnfound.png',
  'theme' => 'panels_threecolumnfound',
  'css' => 'threecolumnfound.css',
  'regions' => array(
    'one' => t('Left'),
    'two' => t('Center'),
    'three' => t('Right'),
  ),
);
