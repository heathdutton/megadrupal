<?php

/**
 * @file
 * Implements core hooks for wet4_panel_layouts.
 */

/**
 * Implements hook_panels_layouts().
 */
function wet4_panel_layouts_mccrae_panels_layouts() {

  $items['mccrae'] = array(
    'title' => t('McCrae'),
    'category' => t('Web Experience Toolkit'),
    'icon' => 'mccrae.png',
    'admin css' => 'mccrae.admin.css',
    'css' => 'mcrae.css',
    'theme' => 'mccrae',
    'theme arguments' => array('id', 'content'),
    'regions' => array(
      'sidebar' => t('Sidebar'),
      'contentheader' => t('Content Header'),
      'contentcolumn1' => t('Content Column 1'),
      'contentcolumn2' => t('Content Column 2'),
    ),
    'bodyid' => 'wb-body',
    'type' => 'one',
    'options' => array(),
    'styles' => array(),
  );

  return $items;
}

/**
 * Preprocess variables for mccrae.tpl.php.
 */
function template_preprocess_mccrae(&$vars) {
  $vars['panel_prefix'] = '';
  $vars['panel_suffix'] = '';
}
