<?php

/**
 * @file
 * Implementation of hook_panels_layouts().
 */

$circle_path = drupal_get_path('theme', 'circle');
include_once './' . $circle_path . '/plugins/layouts/bootstrap-settings.inc';

$plugin = array(
  'title' => t('(3) Three Columns'),
  'category' => t('Bootstrap Columns'),
  'icon' => 'threecolumn.png',
  'theme' => 'panels_threecolumn',
  'css' => 'threecolumn.css',
  'settings' => array('use_container' => NULL, 'panel_class' => NULL),
  'settings form' => '_circle_bootstrap_layout_settings_form',
  'regions' => array(
    'one' => t('Left'),
    'two' => t('Center'),
    'three' => t('Right'),
  ),
);
