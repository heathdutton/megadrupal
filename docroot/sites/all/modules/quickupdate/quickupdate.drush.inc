<?php

/**
 * @file
 * Drush integration for the Quick update module.
 */

/**
 * Implements hook_drush_command().
 */
function quickupdate_drush_command() {
  $items['qup-download'] = array(
    'description' => 'Download projects.',
    'arguments' => array(
      'projects' => dt('A space-separated or commas-separated list of project names.'),
    ),
    'aliases' => array('qup-dl'),
  );
  $items['qup-download-missing-dependencies'] = array(
    'description' => 'Download missing dependency projects.',
    'aliases' => array('qup-dl-md'),
  );
  $items['qup-list-missing-dependencies'] = array(
    'description' => 'List missing dependency projects.',
    'aliases' => array('qup-list-md'),
  );
  return $items;
}

/**
 * Command handler. Downloads projects and their dependency projects.
 */
function drush_quickupdate_qup_download() {
  if (func_num_args() > 0) {
    // Allows space-separated or commas-separated list of project names.
    $projects_arr = func_get_args();
    foreach ($projects_arr as $k => $v) {
      $projects_arr[$k] = trim($v, ',');
    }
    $projects_str = implode(',', $projects_arr);

    drush_print(dt('Downloading projects !self...',
      array(
        '!projects' => format_plural(count($projects_arr), 'project', 'projects'),
        '!self' => $projects_str,
      )
    ));
    drush_print(dt('-------------------------------------------------------------'));

    // Downloads projects.
    $args = array_merge(array('pm-download'), array($projects_str));
    call_user_func_array('drush_invoke', $args);

    // Downloads dependency projects.
    $args = array_merge(array('qup-dl-md'), array($projects_str));
    call_user_func_array('drush_invoke', $args);
  }
  else {
    drush_log(dt('Please enter project names. e.g. !example1 or !example2',
      array(
        '!example1' => 'drush qup-dl views zen',
        '!example2' => 'drush qup-dl views,zen',
      )), 'warning');
  }
}

/**
 * Command handler. Downloads missing dependency projects.
 */
function drush_quickupdate_qup_download_missing_dependencies() {
  $projects_arr = array();
  if (func_num_args() > 0) {
    $projects_arr = func_get_args();
    $projects_str = implode(',', $projects_arr);
    $projects_arr = explode(',', $projects_str);
  }

  $dependencies = quickupdate_load_missing_dependencies($projects_arr);
  $count = count($dependencies);

  if ($count > 0) {
    foreach ($dependencies as $short_name => $item) {
      drush_print("\n");
      drush_print(dt('Downloading dependency !projects of project !required_by...',
        array(
          '!projects' => format_plural($count, 'project', 'projects'),
          '!required_by' => $item['required_by'],
        )
      ));
      drush_print(dt('-------------------------------------------------------------'));

      $args = array_merge(array('pm-download'), array($short_name));
      call_user_func_array('drush_invoke', $args);

      // Reloads to check if there are more missing dependency projects.
      // If so, downloads them.
      $dependencies = quickupdate_load_missing_dependencies(array($short_name));
      $count = count($dependencies);

      if ($count > 0) {
        drush_quickupdate_qup_download_missing_dependencies();
      }
    }
    drush_print("\n");
  }
  else {
    drush_print("\n");
    drush_print(dt('There are no more missing dependency projects.'));
  }
}

/**
 * Command handler. Lists missing dependency projects.
 */
function drush_quickupdate_qup_list_missing_dependencies() {
  $dependencies = quickupdate_load_missing_dependencies();
  foreach ($dependencies as $short_name => $item) {
    drush_print($short_name);
  }
}
