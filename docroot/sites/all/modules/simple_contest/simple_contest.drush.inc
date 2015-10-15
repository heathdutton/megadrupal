<?php

/**
 * @file
 *   simple_contest module drush integration.
 */

/**
 * Implements hook_drush_command().
 * 
 * @See drush_parse_command() for a list of recognized keys.
 *
 * @return
 *   An associative array describing your command(s).
 */
function simple_contest_drush_command() {
  $items = array();
  $items['simple_contest'] = array(
    'description' => "Manage the contest.",
    'arguments' => array(
      'nid' => 'The .',
    ),
    'options' => array(
      '--action' => "The action to be performed on the contest.
      
        activate = To activae the contest.
        pause    = To inactivate the contest.
        finalize = To finalize the contest.
        cancel   = To cancel the contest.
        
      ",
    ),
    'drupal dependencies' => array('simple_contest'),
    'aliases' => array('sc'),
  );
  $items['sc-types'] = array(
    'description' => "List all the available contest types.",
    'drupal dependencies' => array('simple_contest'),
    'aliases' => array('sct'),
  );
  $items['sc-list'] = array(
    'description' => "List all the available contests.",
    'drupal dependencies' => array('simple_contest'),
    'aliases' => array('scl'),
    'arguments' => array(
      'feature' => 'Feature name to export or a single component.',
      'components' => '(optional) List of components to include, like source:component [source2:component2]...'
    ),
  );
  return $items;
}

/**
 * Implements hook_drush_help().
 */
function simple_contest_drush_help($section) {
  switch ($section) {
    case 'drush:simple_contest':
      return dt("List all the available simple_contest for your site.");
    case 'drush:simple_contest-export':
      return dt("Export a feature from your site into a module. If called with no arguments, display a list of available components. If called with a single argument, attempt to create a feature including the given component with the same name. The option '--destination=foo' may be used to specify the path (from Drupal root) where the feature should be created. The default destination is 'sites/all/modules'.");
    case 'drush:simple_contest-update':
      return dt("Update a feature module on your site.");
    case 'drush:simple_contest-update-all':
      return dt("Update all feature modules on your site.");
    case 'drush:simple_contest-revert':
      return dt("Revert a feature module on your site.");
    case 'drush:simple_contest-revert-all':
      return dt("Revert all enabled feature module on your site.");
    case 'drush:simple_contest-revert':
      return dt("Revert a feature module on your site.");
    case 'drush:simple_contest-diff':
      return dt("Show a diff of a feature module.");
  }
}

/**
 * Get a list of all feature modules.
 */
function drush_simple_contest_sc_types() {
  
  $rows = array(array(dt('Type'), dt('Name'), dt('Winner type'), dt('Selection')));
  foreach (simple_contest_get_types() as $type => $info) {
    $settings = simple_contest_get_settings($type);
    $rows[] = array(
      $type,
      $info->name,
      $settings['winner_type'],
      $settings['winner_selection_type'],
    );
  }
  drush_print_table($rows, TRUE);
}

/**
 * Display details about a particular contest.
 */
function drush_simple_contest_simple_contest($nid) {
  
  $node = node_load(intval($nid));
  
  if (!$node) {
    drush_die(dt('The contest !nid could not be found.', array('!nid' => intval($nid))));
  }
  
  if (!isset($node->simple_contest)) {
    drush_die(dt('The node !nid is not a contest.', array('!nid' => intval($nid))));
  }
  
  $field = $node->{$settings['date_field']};
  $sdate = $edate = '';
  
  if ($field['und'][0]['value']) {
    $sdate = $field['und'][0]['value'];
    $edate = $field['und'][0]['value2'];
  }
  
  drush_print(dt('Node ID     : !nid', array('!nid' => intval($nid))));
  drush_print(dt('Title       : !title', array('!title' => $node->title)));
  drush_print(dt('Start date  : !sd', array('!sd' => $sdate)));
  drush_print(dt('End date    : !ed', array('!ed' => $edate)));
  
  
  $action = drush_get_option(array('action'), NULL);
  if ($action) {
    if ($action == 'finalize') {
      simple_contest_finalize_results_node($node);
      drush_print(dt('Action: Contest finalized.'));
    }
    if ($action == 'cancel') {
      $node->simple_contest['status'] = SIMPLE_CONTEST_STATUS_CANCELED;
      node_save($node);
      drush_print(dt('Action: Contest canceled.'));
    }
    if ($action == 'pause') {
      $node->simple_contest['status'] = SIMPLE_CONTEST_STATUS_INACTIVE;
      node_save($node);
      drush_print(dt('Action: Contest paused.'));
    }
    if ($action == 'activate') {
      $node->simple_contest['status'] = SIMPLE_CONTEST_STATUS_ACTIVE;
      node_save($node);
      drush_print(dt('Action: Contest activated.'));
    }
  }
  
  $status = $node->simple_contest['status'];
  $status_name = simple_contest_status_name($status);
  
  drush_print(dt('Status      : !status', array('!status' => $status_name)));
  
  // print winners if completed
  if ($status == SIMPLE_CONTEST_STATUS_COMPLETED) {
    
    drush_print();
    drush_print(dt('Winners'));
    drush_print(dt('========================================'));
    
    $settings = simple_contest_get_settings($node->type);
    
    $results = simple_contest_get_node_result($node);
    $headers = array(
      'weight' => dt('Rank'), 
      'uid' => dt('UID'), 
      'name' => dt('Name'),
    );
    
    if ($settings['winner_type'] == 'node') {
      $headers['wnid'] = dt('Win node');
    }
    
    $rows = array(array());
    
    foreach($headers as $key => $name) {
      $rows[0][] = $name;
    }
    
    foreach($results as $result) {
      $row = array();
      foreach($headers as $key => $name) {
        $row[] = $result[$name];
      }
      $rows[] = $row;
    }
    drush_print_table($rows, TRUE);
  }
  
}

/**
 * Create a feature module based on a list of components.
 */
function drush_simple_contest_export() {
  $args = func_get_args();

  if (count($args) == 1) {
    // Assume that the user intends to create a module with the same name as the
    // "value" of the component.
    list($source, $component) = explode(':', $args[0]);
    $stub = array($source => array($component));
    $sources = simple_contest_get_components();
    if (empty($component) || empty($sources[$source])) {
      _simple_contest_drush_set_error($args[0], 'simple_contest_COMPONENT_NOT_FOUND');
      exit;
    }
    else {
      _drush_simple_contest_export($stub, $component);
    }
  }
  elseif (count($args) > 1) {
    // Assume that the user intends to create a new module based on a list of 
    // components. First argument is assumed to be the name.
    $name = array_shift($args);
    $stub = array();
    foreach ($args as $v) {
      list($source, $component) = explode(':', $v);
      $stub[$source][] = $component;
    }
    _drush_simple_contest_export($stub, array(), $name);
  }
  else {
    $rows = array(array(dt('Available sources')));
    foreach (simple_contest_get_components(TRUE) as $component => $info) {
      if ($options = simple_contest_invoke($component, 'simple_contest_export_options')) {
        foreach ($options as $key => $value) {
          $rows[] = array($component .':'. $key);
        }
      }
    }
    drush_print_table($rows, TRUE);
  }
}

/**
 * Update an existing feature module.
 */
function drush_simple_contest_update() {
  if ($args = func_get_args()) {
    foreach ($args as $module) {
      if (($feature = feature_load($module, TRUE)) && module_exists($module)) {
        _drush_simple_contest_export($feature->info['simple_contest'], $feature->info['dependencies'], $feature->name, dirname($feature->filename));
      }
      else if ($feature) {
        _simple_contest_drush_set_error($module, 'simple_contest_FEATURE_NOT_ENABLED');
      }
      else {
        _simple_contest_drush_set_error($module);
      }
    }
  }
  else {
    // By default just show contexts that are available.
    $rows = array(array(dt('Available simple_contest')));
    foreach (simple_contest_get_simple_contest(NULL, TRUE) as $name => $info) {
      $rows[] = array($name);
    }
    drush_print_table($rows, TRUE);
  }
}

/**
 * Update all enabled simple_contest. Optionally pass in a list of simple_contest to
 * exclude from being updated.
 */
function drush_simple_contest_update_all() {
  $simple_contest_to_update = array();
  $simple_contest_to_exclude = func_get_args();
  foreach (simple_contest_get_simple_contest() as $module) {
    if ($module->status && !in_array($module->name, $simple_contest_to_exclude)) {
      $simple_contest_to_update[] = $module->name;
    }
  }
  drush_print(dt('The following modules will be updated: !modules', array('!modules' => implode(', ', $simple_contest_to_update))));
  if (drush_confirm(dt('Do you really want to continue?'))) {
    foreach ($simple_contest_to_update as $module_name) {
      drush_backend_invoke('simple_contest-update '. $module_name);
    }
  }
  else {
    drush_die('Aborting.');
  }
}

/**
 * Write a module to the site dir.
 *
 * @param $requests
 *   An array of the context requested in this export.
 * @param $module_name
 *  Optional. The name for the exported module.
 */
function _drush_simple_contest_export($stub, $dependencies, $module_name = NULL, $directory = NULL) {
  $root = drush_get_option(array('r', 'root'), drush_locate_root());
  if ($root) {
    $destination = drush_get_option(array('destination'), 'sites/all/modules');
    $directory = isset($directory) ? $directory : $destination . '/' . $module_name;
    if (is_dir($directory)) {
      drush_print(dt('Module appears to already exist in !dir', array('!dir' => $directory)));
      if (!drush_confirm(dt('Do you really want to continue?'))) {
        drush_die('Aborting.');
      }
    }
    else {
      drush_op('mkdir', $directory);
    }
    if (is_dir($directory)) {
      drupal_flush_all_caches();
      module_load_include('inc', 'simple_contest', 'simple_contest.export');
      $export = simple_contest_populate($stub, $dependencies, $module_name);
      if (!feature_load($module_name)) {
        $export['name'] = $module_name;
      }
      $files = simple_contest_export_render($export, $module_name, TRUE);
      foreach ($files as $extension => $file_contents) {
        if (!in_array($extension, array('module', 'info'))) {
          $extension .= '.inc';
        }
        drush_op('file_put_contents', "{$directory}/{$module_name}.$extension", $file_contents);
      }
      drush_log(dt("Created module: !module in !directory", array('!module' => $module_name, '!directory' => $directory)), 'ok');
    }
    else {
      drush_die(dt('Couldn\'t create directory !directory', array('!directory' => $directory)));
    }
  }
  else {
    drush_die(dt('Couldn\'t locate site root'));
  }
}

/**
 * Revert a feature to it's code definition.
 */
function drush_simple_contest_revert() {
  if ($args = func_get_args()) {
    module_load_include('inc', 'simple_contest', 'simple_contest.export');
    simple_contest_include();

    // Determine if revert should be forced.
    $force = drush_get_option('force');
    foreach ($args as $module) {
      if (($feature = feature_load($module, TRUE)) && module_exists($module)) {

        $components = array();
        // Forcefully revert all components of a feature.
        if ($force) {
          foreach (array_keys($feature->info['simple_contest']) as $component) {
            if (simple_contest_hook($component, 'simple_contest_revert')) {
              $components[] = $component;
            }
          }
        }
        // Only revert components that are detected to be Overridden/Needs review.
        else {
          $states = simple_contest_get_component_states(array($feature->name), FALSE);
          foreach ($states[$feature->name] as $component => $state) {
            if (in_array($state, array(simple_contest_OVERRIDDEN, simple_contest_NEEDS_REVIEW)) && simple_contest_hook($component, 'simple_contest_revert')) {
              $components[] = $component;
            }
          }
        }

        if (empty($components)) {
          drush_log(dt('Current state already matches defaults, aborting.'), 'ok');
        }
        else {
          foreach ($components as $component) {
            if (drush_confirm(dt('Do you really want to revert !component?', array('!component' => $component)))) {
              simple_contest_revert(array($module => array($component)));
              drush_log(dt('Reverted !component.', array('!component' => $component)), 'ok');
            }
            else {
              drush_log(dt('Skipping !component.', array('!component' => $component)), 'ok');
            }
          }
        }
      }
      else if ($feature) {
        _simple_contest_drush_set_error($module, 'simple_contest_FEATURE_NOT_ENABLED');
      }
      else {
        _simple_contest_drush_set_error($module);
      }
    }
  }
  else {
    drush_simple_contest_list();
    return;
  }
}

/**
 * Revert all enabled simple_contest to their definitions in code. Optionally pass in
 * a list of simple_contest to exclude from being reverted.
 */
function drush_simple_contest_revert_all() {
  $simple_contest_to_revert = array();
  $simple_contest_to_exclude = func_get_args();
  $force = drush_get_option('force');
  foreach (simple_contest_get_simple_contest() as $module) {
    if ($module->status && !in_array($module->name, $simple_contest_to_exclude)) {
      $simple_contest_to_revert[] = $module->name;
    }
  }
  drush_print(dt('The following modules will be reverted: !modules', array('!modules' => implode(', ', $simple_contest_to_revert))));
  if (drush_confirm(dt('Do you really want to continue?'))) {
    foreach ($simple_contest_to_revert as $module_name) {
      drush_backend_invoke('simple_contest-revert '. $module_name, array('force' => $force));
    }
  }
  else {
    drush_die('Aborting.');
  }
}

/**
 * Show the diff of a feature module.
 */
function drush_simple_contest_diff() {
  if (!$args = func_get_args()) {
    drush_simple_contest_list();
    return;
  }
  $module = $args[0];
  $feature = feature_load($module);
  if (!module_exists($module)) {
    drush_log(dt('No such feature is enabled: ' . $module), 'error');
    return;
  }
  module_load_include('inc', 'simple_contest', 'simple_contest.export');
  $overrides = simple_contest_detect_overrides($feature);
  if (empty($overrides)) {
    drush_log(dt('Feature is in its default state. No diff needed.'), 'ok');
    return;
  }
  module_load_include('inc', 'diff', 'diff.engine');

  if (!class_exists('DiffFormatter')) {
    if (drush_confirm(dt('It seems that the Diff module is not available. Would you like to download and enable it?'))) {
      // Download it if it's not already here.
      $project_info = drush_get_projects();
      if (empty($project_info['diff']) && !drush_backend_invoke('dl diff')) {
        return drush_set_error(dt('Diff module could not be downloaded.'));
      }

      if (!drush_backend_invoke('en diff')) {
        return drush_set_error(dt('Diff module could not be enabled.'));
      }
    }
    else {
      return drush_set_error(dt('Diff module is not enabled.'));
    }
    // If we're still here, now we can include the diff.engine again.
    module_load_include('inc', 'diff', 'diff.engine');
  }

  $formatter = new DiffFormatter();
  $formatter->leading_context_lines = 2;
  $formatter->trailing_context_lines = 2;
  $formatter->show_header = FALSE;

  if (drush_get_context('DRUSH_NOCOLOR')) {
    $red = $green = "%s";
  }
  else {
    $red = "\033[31;40m\033[1m%s\033[0m";
    $green = "\033[0;32;40m\033[1m%s\033[0m";
  }

  // Print key for colors
  drush_print(dt('Legend: '));
  drush_print(sprintf($red,   dt('Code:       drush simple_contest-revert will remove the overrides.')));
  drush_print(sprintf($green, dt('Overrides:  drush simple_contest-update will update the exported feature with the displayed overrides')));
  drush_print();

  foreach ($overrides as $component => $items) {
    $diff = new Diff(explode("\n", $items['default']), explode("\n", $items['normal']));
    drush_print();
    drush_print(dt("Component: !component", array('!component' => $component)));
    $rows = explode("\n", $formatter->format($diff));
    foreach ($rows as $row) {
      if (strpos($row, '>') === 0) {
        drush_print(sprintf($green, $row));
      }
      elseif (strpos($row, '<') === 0) {
        drush_print(sprintf($red, $row));
      }
      else {
        drush_print($row);
      }
    }
  }
}

/**
 * Helper function to call drush_set_error().
 *
 * @param $feature
 *   The string name of the feature.
 * @param $error
 *   A text string identifying the type of error.
 * @return
 *   FALSE.  See drush_set_error().
 */
function _simple_contest_drush_set_error($feature, $error = '') {
  $args = array('!feature' => $feature);

  switch ($error) {
    case 'simple_contest_FEATURE_NOT_ENABLED':
      $message = 'The feature !feature is not enabled.';
      break;
    case 'simple_contest_COMPONENT_NOT_FOUND':
      $message = 'The given component !feature could not be found.';
      break;
    default:
      $error = 'simple_contest_FEATURE_NOT_FOUND';
      $message = 'The feature !feature could not be found.';
  }

  return drush_set_error($error, dt($message, $args));
}
