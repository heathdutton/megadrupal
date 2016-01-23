<?php
// $Id: devel_info_theme_list.inc,v 1.1.2.1 2010/07/14 11:51:51 flanker Exp $

/**
 * @file
 *   theme_list block functions for the devel_info module.
 */


/**
 * Block help text.
 */
function devel_info_theme_list_block_help() {

  $output = '<p>'. t('This block displays list of enabled themes.') .'</p>';

  return $output;
}


/**
 * Block description.
 */
function devel_info_theme_list_block_info() {

  $block = array(
    'info' => t('DI: Theme List'),
    'cache' => DRUPAL_NO_CACHE,
  );

  return $block;
}


/**
 * Block configuration form.
 */
function devel_info_theme_list_block_configure() {

  $config = devel_info_theme_list_get_variable();

  $form = array();
  $form['theme_list_block_behavior'] = array(
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
  // Theme
  $form['block_content']['theme_list_name'] = array(
    '#type' => 'radios',
    '#title' => t('Theme'),
    '#default_value' => $config['name'],
    '#options' => array(
      'info' => t('Display theme name as info name.'),
      'system' => t('Display theme name as system name.'),
    ),
  );
  $form['block_content']['theme_list_version'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display theme version.'),
    '#default_value' => $config['version'],
  );
  $form['block_content']['theme_list_datestamp'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display theme datestamp.'),
    '#default_value' => $config['datestamp'],
  );
  $form['block_content']['theme_list_filename'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display theme filename.'),
    '#default_value' => $config['filename'],
  );
  $form['block_content']['theme_list_base_theme'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display base theme.'),
    '#default_value' => $config['base_theme'],
  );

  return $form;
}


/**
 * Block configuration saving.
 */
function devel_info_theme_list_block_save($edit) {

  $var = array();
  $var['block_content'] = $edit['theme_list_block_behavior']['block_content'];
  $var['block_footer'] = $edit['theme_list_block_behavior']['block_footer'];
  // Theme
  $var['name'] = $edit['theme_list_name'];
  $var['version'] = $edit['theme_list_version'];
  $var['datestamp'] = $edit['theme_list_datestamp'];
  $var['filename'] = $edit['theme_list_filename'];
  $var['base_theme'] = $edit['theme_list_base_theme'];

  variable_set('devel_info_theme_list', serialize($var));
}


/**
 * Block content.
 */
function devel_info_theme_list_block_view() {

  $config = devel_info_theme_list_get_variable();

  if ($config['block_content']) {

    $item = array();

    foreach (list_themes() as $theme) {

      if ($theme->status) {

        $name = $config['name'] == 'info' ? $theme->info['name'] : $theme->name;
        $version = $config['version'] ? $theme->info['version'] : '';
        $datestamp = $config['datestamp'] && isset($theme->info['datestamp']) ? gmdate('r', $theme->info['datestamp']) : '';
        $filename = $config['filename'] ? $theme->filename : '';
        $base_theme = $config['base_theme'] ? (isset($theme->base_theme) ? $theme->base_theme : '') : '';

        $item[] = t('@name @version %datestamp %filename %base_theme', array('@name' => $name, '@version' => $version, '%datestamp' => $datestamp, '%filename' => $filename, '%base_theme' => $base_theme));
      }
    }
  }

  $block = array();
  $block['subject'] = t('DI: Theme List');
  $block['content'] = !empty($item) ? theme('item_list', array('items' => $item)) : '';
  $block['content'] .= devel_info_get_block_link('theme_list', $config);

  return $block;
}


/**
 * Block behavior.
 */
function devel_info_theme_list_block_behavior($action, $mode) {

  if ($action == 'block_content') {

    $var = devel_info_theme_list_get_variable();
    $var['block_content'] = $mode == 'show' ? 'block_content' : 0;

    variable_set('devel_info_theme_list', serialize($var));
  }
}


/**
 * Block uninstall.
 */
function devel_info_theme_list_block_uninstall() {

  variable_del('devel_info_theme_list');
}


/**
 * Helper function; block configuration options.
 */
function devel_info_theme_list_get_variable() {

  $default = array();
  $default['block_content'] = 'block_content';
  $default['block_footer'] = 'block_footer';
  // Theme
  $default['name'] = 'system';
  $default['version'] = 1;
  $default['datestamp'] = 0;
  $default['filename'] = 0;
  $default['base_theme'] = 0;

  $custom = variable_get('devel_info_theme_list', array());

  return !empty($custom) ? array_merge($default, unserialize($custom)) : $default;
}
