<?php
/**
 * @file
 * Layout plugin definition.
 */

/**
 * Implements hook_panels_layouts().
 */
// Plugin definition.
$plugin = array(
  'title' => t('One column with injector'),
  'category' => t('Injector regions'),
  'icon' => 'one_col_with_injector.png',
  'theme' => 'one_col_with_injector',
  'theme arguments' => array('id', 'content'),
  'css' => 'one_col_with_injector.css',
  'regions' => array(
    'banner_area' => t('Banner Area (injected)'),
    'content' => t('Content'),
  ),
);

