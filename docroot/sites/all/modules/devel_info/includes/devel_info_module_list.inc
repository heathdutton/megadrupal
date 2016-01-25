<?php
// $Id: devel_info_module_list.inc,v 1.1.2.2 2010/07/14 13:56:02 flanker Exp $

/**
 * @file
 *   module_list block functions for the devel_info module.
 */


/**
 * Block help text.
 */
function devel_info_module_list_block_help() {

  $output = '<p>'. t("This block displays list of enabled modules sorted by module's call-up.") .'</p>';

  return $output;
}


/**
 * Block description.
 */
function devel_info_module_list_block_info() {

  $block = array(
    'info' => t('DI: Module List'),
    'cache' => DRUPAL_NO_CACHE,
  );

  return $block;
}


/**
 * Block configuration form.
 */
function devel_info_module_list_block_configure() {

  $config = devel_info_module_list_get_variable();

  $form = array();
  $form['module_list_block_behavior'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Block behavior'),
    '#description' => t('Configure block behavior.'),
    '#default_value' => array(
      $config['block_content'],
      $config['block_footer'],
    ),
    '#options' => array(
      'block_content' => t('Show block content.'),
      'block_footer' => t('Show block footer links.'),
    ),
  );
  $form['block_content'] = array(
    '#type' => 'fieldset',
    '#title' => t('Block content'),
  );
  // Module
  $form['block_content']['module_list_name'] = array(
    '#type' => 'radios',
    '#title' => t('Module'),
    '#default_value' => $config['name'],
    '#options' => array(
      'info' => t('Display module name as info name.'),
      'system' => t('Display module name as system name.'),
    ),
  );
  $form['block_content']['module_list_version'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display module version.'),
    '#default_value' => $config['version'],
  );
  $form['block_content']['module_list_datestamp'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display module datestamp.'),
    '#default_value' => $config['datestamp'],
  );
  $form['block_content']['module_list_filename'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display module filename.'),
    '#default_value' => $config['filename'],
  );
  $form['block_content']['module_list_weight'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display module weight.'),
    '#default_value' => $config['weight'],
  );

  return $form;
}


/**
 * Block configuration saving.
 */
function devel_info_module_list_block_save($edit) {

  $var = array();
  $var['block_content'] = $edit['module_list_block_behavior']['block_content'];
  $var['block_footer'] = $edit['module_list_block_behavior']['block_footer'];
  // Module
  $var['name'] = $edit['module_list_name'];
  $var['version'] = $edit['module_list_version'];
  $var['datestamp'] = $edit['module_list_datestamp'];
  $var['filename'] = $edit['module_list_filename'];
  $var['weight'] = $edit['module_list_weight'];

  variable_set('devel_info_module_list', serialize($var));
}


/**
 * Block content.
 */
function devel_info_module_list_block_view() {

  $config = devel_info_module_list_get_variable();

  if ($config['block_content']) {

    $item = array();

    foreach (module_list() as $module) {

      $result = db_query('SELECT name, info, filename, weight FROM {system} WHERE type = :type AND name = :name', array(':type' => 'module', ':name' => $module));
      $file = $result->fetchObject();
      $file->info = unserialize($file->info);

      $name = $config['name'] == 'info' ? $file->info['name'] : $file->name;
      $version = $config['version'] ? $file->info['version'] : '';
      $datestamp = $config['datestamp'] && isset($file->info['datestamp']) ? gmdate('r', $file->info['datestamp']) : '';
      $filename = $config['filename'] ? $file->filename : '';
      $weight = $config['weight'] ? $file->weight : '';

      $item[] = t('@name @version %datestamp %filename %weight', array('@name' => $name, '@version' => $version, '%datestamp' => $datestamp, '%filename' => $filename, '%weight' => $weight));
    }
  }

  $block = array();
  $block['subject'] = t('DI: Module List');
  $block['content'] = !empty($item) ? theme('item_list', array('items' => $item)) : '';
  $block['content'] .= devel_info_get_block_link('module_list', $config);

  return $block;
}


/**
 * Block behavior.
 */
function devel_info_module_list_block_behavior($action, $mode) {

  if ($action == 'block_content') {

    $var = devel_info_module_list_get_variable();
    $var['block_content'] = $mode == 'show' ? 'block_content' : 0;

    variable_set('devel_info_module_list', serialize($var));
  }
}


/**
 * Block uninstall.
 */
function devel_info_module_list_block_uninstall() {

  variable_del('devel_info_module_list');
}


/**
 * Helper function; block configuration options.
 */
function devel_info_module_list_get_variable() {

  $default = array();
  $default['block_content'] = 'block_content';
  $default['block_footer'] = 'block_footer';
  // Module
  $default['name'] = 'system';
  $default['version'] = 1;
  $default['datestamp'] = 0;
  $default['filename'] = 0;
  $default['weight'] = 0;

  $custom = variable_get('devel_info_module_list', array());

  return !empty($custom) ? array_merge($default, unserialize($custom)) : $default;
}
