<?php

/**
 * @file
 * Implementation of hook_panels_layouts().
 */

$circle_path = drupal_get_path('theme', 'circle');
include_once './' . $circle_path . '/plugins/layouts/bootstrap-settings.inc';

$plugin = array(
  'title' => t('(5) Five Columns'),
  'category' => t('Bootstrap Columns'),
  'icon' => 'fivecolumn.png',
  'theme' => 'panels_fivecolumn',
  'css' => 'fivecolumn.css',
  'settings' => array('use_container' => NULL, 'panel_class' => NULL),
  'settings form' => '_circle_bootstrap_layout_settings_form',
  'regions' => array(
    'one' => t('Column One'),
    'two' => t('Column Two'),
    'three' => t('Column Three'),
    'four' => t('Column Four'),
    'five' => t('Column Five'),
  ),
);
