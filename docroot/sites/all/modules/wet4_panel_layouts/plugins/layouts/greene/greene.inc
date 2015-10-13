<?php

/**
 * @file
 * Implements core hooks for wet4_panel_layouts.
 */

/**
 * Implements hook_panels_layouts().
 */
function wet4_panel_layouts_greene_panels_layouts() {
  $items['greene'] = array(
    'title' => t('Greene'),
    'category' => t('Web Experience Toolkit'),
    'icon' => 'greene.png',
    'theme' => 'greene',
    'admin css' => 'greene.admin.css',
    'css' => 'greene.css',
    'theme arguments' => array('id', 'content'),
    'regions' => array(
      'greene_top'    => t('Top (conditional)'),
      'greene_first'  => t('Left'),
      'greene_second' => t('Right'),
      'greene_bottom' => t('Bottom (conditional)'),
    ),
    'bodyid' => 'wb-body',
    'type' => 'two',
    'options' => array(),
    'styles' => array(),
  );

  return $items;
}

/**
 * Preprocess variables for greene.tpl.php.
 */
function template_preprocess_greene(&$vars) {
  $vars['panel_prefix'] = '';
  $vars['panel_suffix'] = '';
}
