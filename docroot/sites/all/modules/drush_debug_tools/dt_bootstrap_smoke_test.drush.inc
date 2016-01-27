<?php

/**
 * Implements hook_drush_command().
 */
function dt_bootstrap_smoke_test_drush_command() {
  $items = array();

  $items['dt-bootstrap-smoke-test'] = array(
    'description' => 'Measure time and memory consumption within a full Drupal bootstrap.',
    'callback' => 'drush_dt_bootstrap_smoke_test',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH, // No bootstrap.
    'options' => array(
      'no-table' => 'Output rows of pipe-separated results (e.g. for easy sorting).',
    ),
    'examples' => array(
      'drush dtbst --no-table' => 'Measure time and memory consumption within a full Drupal bootstrap, output rows of pipe-separated results.',
    ),
    'aliases' => array('dtbst'),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function dt_bootstrap_smoke_test_drush_help($section) {
  switch ($section) {
    case 'drush:dt-bootstrap-smoke-test':
      return dt("Measure time and memory consumption within a full Drupal bootstrap.");
    // this is necessary to tie drush_debug_tools commands from
    // different commandfiles together in the output of drush help
    case 'meta:drush_debug_tools:title':
      return dt("Drush debug tools commands");
  }
}


function drush_dt_bootstrap_smoke_test() {
  global $_dtbst_output;
  $_dtbst_output[] = array('operation', 'time', 'time-delta', 'memory', 'memory-delta');

  if (!drush_bootstrap_to_phase(DRUSH_BOOTSTRAP_DRUPAL_DATABASE) || !defined('DRUPAL_ROOT')) {
    return drush_set_error('DRUPAL_SITE_NOT_FOUND', dt('You need to specify an alias or run this command within a drupal site.'));
  }

  require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
  require_once DRUPAL_ROOT . '/includes/common.inc';

  if ((defined('DRUPAL_CORE_COMPATIBILITY') && DRUPAL_CORE_COMPATIBILITY == '6.x') || function_exists('drupal_init_language')) {
    drupal_bootstrap(DRUPAL_BOOTSTRAP_PATH);
    bst_bootstrap_full_d6();
  }
  else {
    // assume D7 for now
    drupal_bootstrap(DRUPAL_BOOTSTRAP_LANGUAGE);
    bst_bootstrap_full_d7();
  }

  // todo: add options to sort by memory or time-delta within drush
  if (drush_get_option('no-table')) {
    foreach ($_dtbst_output as $output_line) {
      $line = implode('|', $output_line);
      drush_print($line);
    }
  }
  else {
    drush_print_table($_dtbst_output, TRUE);
  }
}

/**
 * based on https://api.drupal.org/api/drupal/includes%21common.inc/function/_drupal_bootstrap_full/6
 */
function bst_bootstrap_full_d6() {

  timer_start('bst');

  bst_debug('starting bootstrap_full');

  static $called;

  if ($called) {
    return;
  }
  $called = 1;
  require_once './includes/theme.inc';
  require_once './includes/pager.inc';
  require_once './includes/menu.inc';
  require_once './includes/tablesort.inc';
  require_once './includes/file.inc';
  require_once './includes/unicode.inc';
  require_once './includes/image.inc';
  require_once './includes/form.inc';
  require_once './includes/mail.inc';
  require_once './includes/actions.inc';

  bst_debug('finished includes');

  // Set the Drupal custom error handler.
  set_error_handler('drupal_error_handler');

  // Emit the correct charset HTTP header.
  drupal_set_header('Content-Type: text/html; charset=utf-8');

  // Detect string handling method
  unicode_check();

  // Undo magic quotes
  fix_gpc_magic();

  bst_debug('loading enabled modules');

  // Load all enabled modules
  bst_module_load_all_d6();

  bst_debug('modules loaded');

  // out-of-date copies of D6 might not have this function
  //  it's probably not necessary for what we're trying to do anyway
  if (function_exists('drupal_random_bytes')) {
    // Ensure mt_rand is reseeded, to prevent random values from one page load
    // being exploited to predict random values in subsequent page loads.
    $seed = unpack("L", drupal_random_bytes(4));
    mt_srand($seed[1]);
  }

  // Let all modules take action before menu system handles the request
  // We do not want this while running update.php.
  if (!defined('MAINTENANCE_MODE') || MAINTENANCE_MODE != 'update') {
    bst_debug('starting to invoke hook_init');
    bst_module_invoke_all_d6('init');
    bst_debug('finished hook_init');
  }

  bst_debug('end of bootstrap_full');
}

/**
 * based on https://api.drupal.org/api/drupal/includes%21common.inc/function/_drupal_bootstrap_full/7
 */
function bst_bootstrap_full_d7() {

  timer_start('bst');

  bst_debug('starting bootstrap_full');

  static $called = FALSE;

  if ($called) {
    return;
  }
  $called = TRUE;
  require_once DRUPAL_ROOT . '/' . variable_get('path_inc', 'includes/path.inc');
  require_once DRUPAL_ROOT . '/includes/theme.inc';
  require_once DRUPAL_ROOT . '/includes/pager.inc';
  require_once DRUPAL_ROOT . '/' . variable_get('menu_inc', 'includes/menu.inc');
  require_once DRUPAL_ROOT . '/includes/tablesort.inc';
  require_once DRUPAL_ROOT . '/includes/file.inc';
  require_once DRUPAL_ROOT . '/includes/unicode.inc';
  require_once DRUPAL_ROOT . '/includes/image.inc';
  require_once DRUPAL_ROOT . '/includes/form.inc';
  require_once DRUPAL_ROOT . '/includes/mail.inc';
  require_once DRUPAL_ROOT . '/includes/actions.inc';
  require_once DRUPAL_ROOT . '/includes/ajax.inc';
  require_once DRUPAL_ROOT . '/includes/token.inc';
  require_once DRUPAL_ROOT . '/includes/errors.inc';

  bst_debug('finished includes');

  // Detect string handling method
  unicode_check();

  // Undo magic quotes
  fix_gpc_magic();

  bst_debug('loading enabled modules');

  // Load all enabled modules
  bst_module_load_all_d7();

  bst_debug('modules loaded');

  bst_debug('getting stream_wrappers');

  // Make sure all stream wrappers are registered.
  file_get_stream_wrappers();

  bst_debug('got stream_wrappers');

  $test_info = &$GLOBALS['drupal_test_info'];
  if (!empty($test_info['in_child_site'])) {
    // Running inside the simpletest child site, log fatal errors to test
    // specific file directory.
    ini_set('log_errors', 1);
    ini_set('error_log', 'public://error.log');
  }

  bst_debug('initialising path');

  // Initialize $_GET['q'] prior to invoking hook_init().
  drupal_path_initialize();

  bst_debug('initialised path');

  // Let all modules take action before the menu system handles the request.
  // We do not want this while running update.php.
  if (!defined('MAINTENANCE_MODE') || MAINTENANCE_MODE != 'update') {
    // Prior to invoking hook_init(), initialize the theme (potentially a custom
    // one for this page), so that:
    // - Modules with hook_init() implementations that call theme() or
    //   theme_get_registry() don't initialize the incorrect theme.
    // - The theme can have hook_*_alter() implementations affect page building
    //   (e.g., hook_form_alter(), hook_node_view_alter(), hook_page_alter()),
    //   ahead of when rendering starts.
    bst_debug('setting custom theme');
    menu_set_custom_theme();
    bst_debug('set custom theme');
    bst_debug('initialising theme');
    drupal_theme_initialize();
    bst_debug('initialised theme');
    bst_debug('starting to invoke hook_init');
    bst_module_invoke_all_d7('init');
    bst_debug('finished hook_init');
  }
  bst_debug('end of bootstrap_full');
}

/**
 * based on https://api.drupal.org/api/drupal/includes!module.inc/function/module_load_all/7
 */
function bst_module_load_all_d7($bootstrap = FALSE) {
  static $has_run = FALSE;

  if (isset($bootstrap)) {
    foreach (module_list(TRUE, $bootstrap) as $module) {
      bst_debug("loading module $module");
      drupal_load('module', $module);
      bst_debug("loaded module $module");
    }
    // $has_run will be TRUE if $bootstrap is FALSE.
    $has_run = !$bootstrap;
  }

  /**
   * This will slow things down a little, but without it any calls to theme() within hook_init
   *   implementations will throw an exception (in D7), as it will appear that modules have not
   *   been loaded. The static within drupal_load should mean this returns fairly fast without
   *   really doing very much work. We'll track this step for transparency though.
   */
  bst_debug("calling the real module_load_all");
  module_load_all();
  bst_debug("called the real module_load_all");

  return $has_run;
}

/**
 * based on https://api.drupal.org/api/drupal/includes!module.inc/function/module_load_all/6
 */
function bst_module_load_all_d6() {
  foreach (module_list(TRUE, FALSE) as $module) {
    bst_debug("loading module $module");
    drupal_load('module', $module);
    bst_debug("loaded module $module");
  }
}

/**
 * based on https://api.drupal.org/api/drupal/includes%21module.inc/function/module_invoke_all/7
 */
function bst_module_invoke_all_d7($hook) {
  $args = func_get_args();
  // Remove $hook from the arguments.
  unset($args[0]);
  $return = array();
  foreach (module_implements($hook) as $module) {
    bst_debug("start hook_$hook in $module");
    $function = $module . '_' . $hook;
    if (function_exists($function)) {
      $result = call_user_func_array($function, $args);
      if (isset($result) && is_array($result)) {
        $return = array_merge_recursive($return, $result);
      }
      elseif (isset($result)) {
        $return[] = $result;
      }
    }
    bst_debug("end hook_$hook in $module");
  }

  return $return;
}

/**
 * based on https://api.drupal.org/api/drupal/includes%21module.inc/function/module_invoke_all/6
 */
function bst_module_invoke_all_d6() {
  $args = func_get_args();
  $hook = $args[0];
  unset($args[0]);
  $return = array();
  foreach (module_implements($hook) as $module) {
    bst_debug("start hook_$hook in $module");
    $function = $module . '_' . $hook;
    $result = call_user_func_array($function, $args);
    if (isset($result) && is_array($result)) {
      $return = array_merge_recursive($return, $result);
    }
    elseif (isset($result)) {
      $return[] = $result;
    }
    bst_debug("end hook_$hook in $module");
  }

  return $return;
}

function bst_debug($mark) {
  global $_dtbst_output;
  static $old_time, $old_memory;
  $time = round(timer_read('bst')); // sometimes returns floats without this round
  $time_delta = $time - $old_time;
  $old_time = $time;
  $memory = round(memory_get_peak_usage(TRUE) / 1024 / 1024, 2) . 'MB';
  $memory_delta = $memory - $old_memory;
  $old_memory = $memory;
  $_dtbst_output[] = array(
    'mark' => $mark,
    't' => $time,
    't^' => $time_delta,
    'm' => $memory,
    'm^' => $memory_delta,
    );
}
