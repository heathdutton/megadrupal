<?php
// $Id: devel_info_project_list.inc,v 1.1.2.1 2010/07/14 11:51:51 flanker Exp $

/**
 * @file
 *   project_list block functions for the devel_info module.
 */


/**
 * Block help text.
 */
function devel_info_project_list_block_help() {

  $output = '<p>'. t('This block displays list of enabled projects.') .'</p>';

  return $output;
}


/**
 * Block description.
 */
function devel_info_project_list_block_info() {

  $block = array(
    'info' => t('DI: Project List'),
    'cache' => DRUPAL_NO_CACHE,
  );

  return $block;
}


/**
 * Block configuration form.
 */
function devel_info_project_list_block_configure() {

  $config = devel_info_project_list_get_variable();

  $form = array();
  $form['project_list_block_behavior'] = array(
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
  // Project
  $form['block_content']['project_list_name'] = array(
    '#type' => 'radios',
    '#title' => t('Project'),
    '#default_value' => $config['name'],
    '#options' => array(
      'project' => t('Display project name as project title if update information is available otherwise system name.'),
      'system' => t('Display project name as system name.'),
    ),
  );
  $form['block_content']['project_list_name_link'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display project name as link if update information is available.'),
    '#default_value' => $config['name_link'],
  );
  $form['block_content']['project_list_version'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display project version.'),
    '#default_value' => $config['version'],
  );
  $form['block_content']['project_list_datestamp'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display project datestamp.'),
    '#default_value' => $config['datestamp'],
  );

  return $form;
}


/**
 * Block configuration saving.
 */
function devel_info_project_list_block_save($edit) {

  $var = array();
  $var['block_content'] = $edit['project_list_block_behavior']['block_content'];
  $var['block_footer'] = $edit['project_list_block_behavior']['block_footer'];
  // Project
  $var['name'] = $edit['project_list_name'];
  $var['name_link'] = $edit['project_list_name_link'];
  $var['version'] = $edit['project_list_version'];
  $var['datestamp'] = $edit['project_list_datestamp'];

  variable_set('devel_info_project_list', serialize($var));
}


/**
 * Block content.
 */
function devel_info_project_list_block_view() {

  $config = devel_info_project_list_get_variable();

  if ($config['block_content']) {

    $available = array();
    $projects = array();

    if (module_exists('update')) {

      module_load_include('module', 'update');
      module_load_include('inc', 'update', 'update.compare');

      if ($available = update_get_available()) {
        $projects = update_calculate_project_data($available);
      }
    }

    if (!$available) {

      foreach (module_list() as $module) {

        $result = db_query('SELECT name, info, filename, weight FROM {system} WHERE type = :type AND name = :name', array(':type' => 'module', ':name' => $module));
        $file = $result->fetchObject();
        $file->info = unserialize($file->info);

        if (isset($file->info['project'])) {
          $projects[$file->info['project']] = array('name' => $file->info['project'], 'info' => $file->info);
        }
      }
    }

    $item = array();

    foreach ($projects as $project) {

      $name = $config['name'] == 'project' && $available ? $project['title'] : $project['name'];
      $name_link = $config['name_link'] && $available ? l($name, $project['link']) : check_plain($name);
      $version = $config['version'] ? $project['info']['version'] : '';
      $datestamp = $config['datestamp'] && isset($project['info']['datestamp']) ? gmdate('r', $project['info']['datestamp']) : '';

      $item[$name] = t('!name_link @version %datestamp', array('!name_link' => $name_link, '@version' => $version, '%datestamp' => $datestamp));
    }

    ksort($item);
  }

  $block = array();
  $block['subject'] = t('DI: Project List');
  $block['content'] = !empty($item) ? theme('item_list', array('items' => $item)) : '';
  $block['content'] .= devel_info_get_block_link('project_list', $config);

  return $block;
}


/**
 * Block behavior.
 */
function devel_info_project_list_block_behavior($action, $mode) {

  if ($action == 'block_content') {

    $var = devel_info_project_list_get_variable();
    $var['block_content'] = $mode == 'show' ? 'block_content' : 0;

    variable_set('devel_info_project_list', serialize($var));
  }
}


/**
 * Block uninstall.
 */
function devel_info_project_list_block_uninstall() {

  variable_del('devel_info_project_list');
}


/**
 * Helper function; block configuration options.
 */
function devel_info_project_list_get_variable() {

  $default = array();
  $default['block_content'] = 'block_content';
  $default['block_footer'] = 'block_footer';
  // Project
  $default['name'] = 'project';
  $default['name_link'] = 1;
  $default['version'] = 1;
  $default['datestamp'] = 0;

  $custom = variable_get('devel_info_project_list', array());

  return !empty($custom) ? array_merge($default, unserialize($custom)) : $default;
}
