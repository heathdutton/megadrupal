<?php

/**
 * Implementation hook_drush_command().
 */
function dt_cron_drush_command() {
  $items['dt-cron'] = array(
    'callback' => 'drush_dt_cron',
    'description' => 'Run all of your cron tasks and show how long they take to execute.',
    'options' => array(
      'omit' => 'Omit the specified cron hooks from the cron run.',
    ),
    'aliases' => array('dtc'),
  );
  $items['dt-cron-item'] = array(
    'callback' => 'drush_dt_cron_item',
    'description' => 'Run a single cron function.',
    'arguments' => array(
      'function' => 'The function callback to run',
    ),
    'aliases' => array('dtci'),
  );
  return $items;
}

/**
 * Implements hook_drush_help().
 */
function dt_cron_drush_help($section) {
  switch ($section) {
    // This is necessary to tie drush_debug_tools commands from
    // different commandfiles together in the output of drush help.
    case 'meta:drush_debug_tools:title':
      return dt("Drush debug tools commands");
  }
}

function drush_dt_cron() {
  $omit = drupal_map_assoc(explode(',', drush_get_option(array('omit', 'o'))));
  $failed = 0;
  foreach (module_implements("cron") as $module) {
    $function = $module . "_cron";
    if (isset($omit[$function])) {
      drush_print($function . ': omitted');
    }
    else {
      $result = drush_dt_cron_item($module);
      if (empty($result)) {
        drush_log(dt('The cron task for "!module" failed.', array('!module' => $module)), 'error');
        $failed ++;
      }
    }
  }
  if (!$failed) {
    variable_set('cron_last', time());
  }
}

function drush_dt_cron_item($module) {
  global $timers;
  $function = $module . '_cron';
  if (function_exists($function)) {
    timer_start($function);
    call_user_func($function);
    timer_stop($function);
    drush_log($function . ": " . $timers[$function]["time"] . "ms", 'ok');
    return $timers[$function]["time"];
  }
  else {
    drush_log(dt('The module "!module" does not have a cron hook.', array('!module' => $module)), 'error');
  }
}

