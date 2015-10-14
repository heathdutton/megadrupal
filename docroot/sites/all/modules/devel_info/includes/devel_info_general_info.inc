<?php
// $Id: devel_info_general_info.inc,v 1.1.2.1 2010/07/14 11:51:51 flanker Exp $

/**
 * @file
 *   general_info block functions for the devel_info module.
 */


/**
 * Block help text.
 */
function devel_info_general_info_block_help() {

  $output = '<p>'. t('This block displays general information.') .'</p>';

  return $output;
}


/**
 * Block description.
 */
function devel_info_general_info_block_info() {

  $block = array(
    'info' => t('DI: General Info'),
    'cache' => DRUPAL_NO_CACHE,
  );

  return $block;
}


/**
 * Block configuration form.
 */
function devel_info_general_info_block_configure() {

  $config = devel_info_general_info_get_variable();

  $form = array();
  $form['general_info_block_behavior'] = array(
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
  // User
  $form['block_content']['general_info_user'] = array(
    '#type' => 'checkboxes',
    '#title' => t('User'),
    '#description' => t('Display user related information.'),
    '#default_value' => array(
      $config['user_uid'],
      $config['user_name'],
    ),
    '#options' => array(
      'user_uid' => t('Display the current user id. %variable', array('%variable' => '$user->uid')),
      'user_name' => t('Display the current user name. %variable', array('%variable' => '$user->name')),
    ),
  );
  // System
  $form['block_content']['general_info_system'] = array(
    '#type' => 'checkboxes',
    '#title' => t('System'),
    '#description' => t('Display system related information.'),
    '#default_value' => array(
      $config['version'],
      $config['install_profile'],
      $config['install_time'],
      $config['base_url'],
      $config['base_path'],
      $config['conf_path'],
      $config['cron_last'],
      $config['language'],
    ),
    '#options' => array(
      'version' => t('Display Drupal version. %value', array('%value' => 'VERSION')),
      'install_profile' => t('Display Drupal installation profile. %value', array('%value' => "variable_get('install_profile')")),
      'install_time' => t('Display Drupal installation time. %value', array('%value' => "variable_get('install_time')")),
      'base_url' => t('Display the base URL of the drupal installation. %variable', array('%variable' => '$base_url')),
      'base_path' => t('Display the base URL path of the Drupal installation. %function', array('%function' => 'base_path()')),
      'conf_path' => t('Display the appropriate configuration directory. %function', array('%function' => 'conf_path()')),
      'cron_last' => t('Display last run of cron. %value', array('%value' => "variable_get('cron_last')")),
      'language' => t('Display the active interface language. %variable', array('%variable' => '$language->language')),
    ),
  );
  // Theme
  $form['block_content']['general_info_theme'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Theme'),
    '#description' => t('Display theme related information.'),
    '#default_value' => array(
      $config['theme_key'],
      $config['theme_path'],
    ),
    '#options' => array(
      'theme_key' => t('Display the name of the active theme. %variable', array('%variable' => '$theme_key')),
      'theme_path' => t('Display the path to the active theme. %variable', array('%variable' => '$theme_path')),
    ),
  );

  return $form;
}


/**
 * Block configuration saving.
 */
function devel_info_general_info_block_save($edit) {

  $var = array();
  $var['block_content'] = $edit['general_info_block_behavior']['block_content'];
  $var['block_footer'] = $edit['general_info_block_behavior']['block_footer'];
  // User
  $var['user_uid'] = $edit['general_info_user']['user_uid'];
  $var['user_name'] = $edit['general_info_user']['user_name'];
  // System
  $var['version'] = $edit['general_info_system']['version'];
  $var['install_profile'] = $edit['general_info_system']['install_profile'];
  $var['install_time'] = $edit['general_info_system']['install_time'];
  $var['base_url'] = $edit['general_info_system']['base_url'];
  $var['base_path'] = $edit['general_info_system']['base_path'];
  $var['conf_path'] = $edit['general_info_system']['conf_path'];
  $var['cron_last'] = $edit['general_info_system']['cron_last'];
  $var['language'] = $edit['general_info_system']['language'];
  // Theme
  $var['theme_key'] = $edit['general_info_theme']['theme_key'];
  $var['theme_path'] = $edit['general_info_theme']['theme_path'];

  variable_set('devel_info_general_info', serialize($var));
}


/**
 * Block content.
 */
function devel_info_general_info_block_view() {

  $config = devel_info_general_info_get_variable();

  if ($config['block_content']) {

    $item = array();

    // User
    if ($config['user_uid']) {
      global $user;
      $item[] = t('User ID: %uid', array('%uid' => $user->uid));
    }
    if ($config['user_name']) {
      global $user;
      $name = isset($user->name) ? $user->name : variable_get('anonymous', t('Anonymous'));
      $item[] = t('User name: %name', array('%name' => $name));
    }

    // System
    if ($config['version']) {
      $item[] = t('Version: %value', array('%value' => VERSION));
    }
    if ($config['install_profile']) {
      $item[] = t('Install profile: %value', array('%value' => variable_get('install_profile', 'default')));
    }
    if ($config['install_time']) {
      $item[] = t('Install time: %value', array('%value' => gmdate('r', variable_get('install_time', 0))));
    }
    if ($config['base_url']) {
      global $base_url;
      $item[] = t('Base URL: %url', array('%url' => $base_url));
    }
    if ($config['base_path']) {
      $item[] = t('Base path: %path', array('%path' => base_path()));
    }
    if ($config['conf_path']) {
      $item[] = t('Conf path: %path', array('%path' => conf_path()));
    }
    if ($config['cron_last']) {
      if (!$cron_last = variable_get('cron_last', 0)) {
        $cron_last = variable_get('install_time', 0);
      }
      $item[] = t('Last cron: %value', array('%value' => format_interval(time() - $cron_last)));
    }
    if ($config['language']) {
      global $language;
      $item[] = t('Language: %language', array('%language' => $language->language));
    }

    // Theme
    if ($config['theme_key']) {
      global $theme_key;
      $item[] = t('Theme key: %theme', array('%theme' => $theme_key));
    }
    if ($config['theme_path']) {
      global $theme_path;
      $item[] = t('Theme path: %path', array('%path' => $theme_path));
    }
  }

  $block = array();
  $block['subject'] = t('DI: General Info');
  $block['content'] = !empty($item) ? theme('item_list', array('items' => $item)) : '';
  $block['content'] .= devel_info_get_block_link('general_info', $config);

  return $block;
}


/**
 * Block behavior.
 */
function devel_info_general_info_block_behavior($action, $mode) {

  if ($action == 'block_content') {

    $var = devel_info_general_info_get_variable();
    $var['block_content'] = $mode == 'show' ? 'block_content' : 0;

    variable_set('devel_info_general_info', serialize($var));
  }
}


/**
 * Block uninstall.
 */
function devel_info_general_info_block_uninstall() {

  variable_del('devel_info_general_info');
}


/**
 * Helper function; block configuration options.
 */
function devel_info_general_info_get_variable() {

  $default = array();
  $default['block_content'] = 'block_content';
  $default['block_footer'] = 'block_footer';
  // User
  $default['user_uid'] = 'user_uid';
  $default['user_name'] = 'user_name';
  // System
  $default['version'] = 'version';
  $default['install_profile'] = 0;
  $default['install_time'] = 0;
  $default['base_url'] = 0;
  $default['base_path'] = 0;
  $default['conf_path'] = 'conf_path';
  $default['cron_last'] = 'cron_last';
  $default['language'] = 'language';
  // Theme
  $default['theme_key'] = 'theme_key';
  $default['theme_path'] = 0;

  $custom = variable_get('devel_info_general_info', array());

  return !empty($custom) ? array_merge($default, unserialize($custom)) : $default;
}
