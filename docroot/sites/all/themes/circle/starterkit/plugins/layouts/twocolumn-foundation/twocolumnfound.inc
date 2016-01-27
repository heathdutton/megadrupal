<?php

/**
 * @file
 * Implementation of hook_panels_layouts().
 */

$plugin = array(
  'title' => t('(2) Two Columns'),
  'category' => t('Foundation Columns'),
  'icon' => 'twocolumnfound.png',
  'theme' => 'panels_twocolumnfound',
  'css' => 'twocolumnfound.css',
  'regions' => array(
    'one' => t('Left'),
    'two' => t('Right'),
  ),
);
