<?php

/**
 * @file
 * Implements core hooks for wet4_panel_layouts.
 */

/**
 * Implements hook_panels_layouts().
 */
function wet4_panel_layouts_penfield2_panels_layouts() {
  $items['penfield2'] = array(
    'title'    => t('Penfield 2'),
    'category' => t('Web Experience Toolkit'),
    'icon'     => 'penfield2.png',
    'theme'    => 'penfield2',
    'admin css' => 'penfield2.admin.css',
    'css' => 'penfield2.css',
    'theme arguments' => array('id', 'content'),
    'regions' => array(
      'penfield2_top'    => t('Top (conditional)'),
      'penfield2_content_header' => t('Content Header'),
      'penfield2_first'  => t('Content 1'),
      'penfield2_second' => t('Content 2'),
      'penfield2_third'  => t('Sidebar'),
      'penfield2_bottom' => t('Bottom (conditional)'),
    ),
    'bodyid' => 'wb-body-sec',
    'type' => 'three',
    'options' => array(),
    'styles' => array(),
  );

  return $items;
}

/**
 * Preprocess variables for penfield2.tpl.php.
 */
function template_preprocess_penfield2(&$vars) {
  $vars['panel_prefix'] = '';
  $vars['panel_suffix'] = '';
}
