<?php

/**
 * Implementation of hook_hap_info().
 */
function hap_core_hap_info() {
  return array(
    'name' => 'Core',
    'description' => 'Core utilities for highly attractive people.',
    'hooks' => array('menu'),
    'always enabled' => TRUE,
  );
}

/**
 * Implements hook_menu().
 */
function hap_core_menu() {
  // TOC for all tool's configurations
  $items['admin/config/hap'] = array(
    'title' => '★ Highly Attractive People',
    'description' => '"Tools for Highly Attractive People" related configuration.',
    'weight' => -100,
  ) + hap_menu_base();

  return $items;
}


/******************************************************************************
 * The bizniss.
 *****************************************************************************/

/**
 * Array structure that helps build a table of content for groups of menu
 * items. Typically added using array addition to a basic array definition
 * defined by YOU! Observe:
 *
 * @code
 *  hap_include();
 *  function myModule_menu() {
 *    $items['admin/config/myModule] = array(
 *      'title' => 'myModule Settings Table of Contents',
 *      'description' => 'Oh hells yes. You know how I do my thang..',
 *    ) + hap_menu_toc();
 *  }
 * @endcode
 */
function hap_menu_base() {
  return array(
    'description' => t('Table of Contents page.'),
    'page callback' => 'system_admin_menu_block_page',
    'page arguments' => array(),
    'position' => 'left',
    'weight' => -20,
    'access arguments' => array('access administration pages'),
    'file' => 'system.admin.inc',
    'file path' => drupal_get_path('module', 'system'),
  );
}
