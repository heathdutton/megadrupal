<?php

/**
 * @file
 * Drush extension for working with the Sampler API.
 *
 * Requires Drush version 4 or later.
 *
 * Provides basic interaction with Sampler API, including listing metrics,
 * processing metrics, updating storage plugins, and deleting metric data.
 *
 * More information about Drush can be found at http://drupal.org/project/drush
 *
 * Once Drush is installed, and Sampler API is installed on a site, the plugin
 * will be automatically picked up by Drush. Typing 'drush' at the command
 * line will show a list of the Sampler API Drush commands, and typing 'drush
 * help [command name]' will provide help for the specified command.
 */

/**
 * Implements hook_drush_command().
 */
function sampler_drush_command() {
  $items = array();
  $items['sampler-admin'] = array(
    'description' => 'A simple multi-step user interface to allow easy retrieval/manipulation of metrics/plugins.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );
  $items['sampler-list-plugins'] = array(
    'description' => 'Lists all known Sampler API plugin types/plugins for a site.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );
  $items['sampler-list-metrics'] = array(
    'description' => 'Lists all known metrics for a site sorted by the module providing the metric.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'arguments' => array(
      'module' => "Optional. The module providing the metric.",
      'metric' => "Optional. The machine name of the metric.",
    ),
    'examples' => array(
      'drush sampler-list-metrics' => "List all metrics provided by all modules.",
      'drush sampler-list-metrics my_module' => "List all metrics provided by the 'my_module' module.",
      'drush sampler-list-metrics my_module my_metric' => "List the 'my_metric' metric provided by the 'my_module' module.",
    ),
  );
  $items['sampler-sample'] = array(
    'description' => "Samples and optionally stores metrics for a site. Use no arguments for all metrics, the module argument for all metrics a module provides, and the module and metric arguments for a particular metric.

Any option that a plugin takes to control the sample can be passed at the command line as a full option with a value. For example to pass the 'startstamp' option to the method plugin: --startstamp=\"2010-10-23\"",
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'arguments' => array(
      'module' => "Optional. The module providing the metric.",
      'metric' => "Optional. The machine name of the metric (as listed in sampler-list-metrics).",
    ),
    'examples' => array(
      'drush sampler-sample' => "Sample all metrics provided by all modules.",
      'drush sampler-sample --save my_module' => "Sample all metrics provided by the 'my_module' module, and save the results.",
      'drush sampler-sample --save my_module my_metric' => "Sample the 'my_metric' metric provided by the 'my_module' module, and save the results.",
    ),
  );

  // Option handling changed in Drush 5.x.
  if (DRUSH_VERSION <= 4) {
    $items['sampler-sample']['options'] = array(
      '--save' => "If set, save the sample results using the default storage plugin (as listed in sampler-list-metrics). Overridden by --object_batch_size.",
      '--object_batch_size' => "If set, the metric will be processed in batch mode, first computing then saving [object_batch_size] objects at a time for each sample.  Overrides --save.",
      '--lock_interval' => "Set to customize the lock interval for a sampling run. See LOCK_DEFAULT_INTERVAL in Sampler.php for more information",
    );
  }
  else {
    $items['sampler-sample']['options'] = array(
      'save' => "If set, save the sample results using the default storage plugin (as listed in sampler-list-metrics). Overridden by --object_batch_size.",
      'object_batch_size' => "If set, the metric will be processed in batch mode, first computing then saving [object_batch_size] objects at a time for each sample.  Overrides --save.",
      'lock_interval' => "Set to customize the lock interval for a sampling run. See LOCK_DEFAULT_INTERVAL in Sampler.php for more information",
    );
    $items['sampler-sample']['allow-additional-options'] = TRUE;
  }

  $items['sampler-update-storage-plugin'] = array(
    'description' => 'Permanently updates the storage plugin for a metric.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'arguments' => array(
      'module' => "The module providing the metric.",
      'metric' => "The machine name of the metric (as listed in sampler-list-metrics).",
      'storage plugin' => "The storage plugin to update the metric to (as listed in sampler-list-plugins).",
    ),
    'examples' => array(
      'drush sampler-update-storage-plugin my_module my_metric drupal_database_table' => "Update the 'my_metric' metric provided by the 'my_module' module to use the 'drupal_database_table' storage plugin.",
    ),
  );
  $items['sampler-update-method-plugin'] = array(
    'description' => 'Permanently updates the method plugin for a metric.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'arguments' => array(
      'module' => "The module providing the metric.",
      'metric' => "The machine name of the metric (as listed in sampler-list-metrics).",
      'method plugin' => "The method plugin to update the metric to (as listed in sampler-list-plugins).",
    ),
    'examples' => array(
      'drush sampler-update-method-plugin my_module my_metric periodic' => "Update the 'my_metric' metric provided by the 'my_module' module to use the 'periodic' method plugin.",
    ),
  );
  $items['sampler-update-adjustment-plugin'] = array(
    'description' => 'Permanently updates the adjustment plugin for a metric.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'arguments' => array(
      'module' => "The module providing the metric.",
      'metric' => "The machine name of the metric (as listed in sampler-list-metrics).",
      'adjustment plugin' => "The adjustment plugin to update the metric to (as listed in sampler-list-plugins).",
    ),
    'examples' => array(
      'drush sampler-update-adjustment-plugin my_module my_metric periodic' => "Update the 'my_metric' metric provided by the 'my_module' module to use the 'periodic' adjustment plugin.",
    ),
  );
  $items['sampler-delete-data'] = array(
    'description' => 'Deletes all data for a metric, including its stored schema information -- use with caution!',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'arguments' => array(
      'module' => "The module providing the metric.",
      'metric' => "The machine name of the metric (as listed in sampler-list-metrics).",
    ),
    'examples' => array(
      'sampler-delete-data my_module my_metric' => "Deletes all data for the 'my_metric' metric provided by the 'my_module' module.",
    ),
  );

  // Option handling changed in Drush 5.x.
  if (DRUSH_VERSION <= 4) {
    $items['sampler-delete-data']['options'] = array(
      '--database' => "For external databases configured via Drupal's Database API, pass the database key, eg '--database=foo'.",
    );
  }
  else {
    $items['sampler-delete-data']['options'] = array(
      'database' => "For external databases configured via Drupal's Database API, pass the database key, eg '--database=foo'.",
    );
  }

  return $items;
}

/**
 * Simple user choice interface for the various command line functions. Saves
 * a lot of typing. :)
 */
function drush_sampler_admin() {
  // Main menu selections.
  $main_menu = array(
    'drush_sampler_list_plugins' => dt('List plugins'),
    'drush_sampler_list_metrics' => dt('List metrics'),
    'drush_sampler_sample' => dt('Sample metrics'),
    'drush_sampler_update_storage_plugin' => dt('Update a storage plugin for a metric'),
    'drush_sampler_update_method_plugin' => dt('Update a method plugin for a metric'),
    'drush_sampler_update_adjustment_plugin' => dt('Update an adjustment plugin for a metric'),
    'drush_sampler_delete_data' => dt('Delete all data for a metric'),
  );
  $main_menu_choice = drush_choice($main_menu, dt("Choose an action"));

  // Take the relevant action based on the main menu item selected.
  if ($main_menu_choice) {
    $args = array();
    switch ($main_menu_choice) {
      case 'drush_sampler_list_plugins':
        break;
      case 'drush_sampler_list_metrics':
      case 'drush_sampler_sample':
        $args = _drush_sampler_choices(FALSE, FALSE);
        break;
      case 'drush_sampler_update_storage_plugin':
        $args = _drush_sampler_choices('storage');
        break;
      case 'drush_sampler_update_method_plugin':
        $args = _drush_sampler_choices('method');
        break;
      case 'drush_sampler_update_adjustment_plugin':
        $args = _drush_sampler_choices('adjustment');
        break;
      case 'drush_sampler_delete_data':
        $args = _drush_sampler_choices();
        break;
      default:
        return;
    }

    // If the choices were aborted, skip this.
    if ($args !== FALSE) {
      // Print the command to be executed, then run it.
      $sampler_command = preg_replace('/_/', '-', preg_replace('/^drush_/', 'drush ', $main_menu_choice));
      $sampler_args = implode(' ', $args);
      print "Executing '$sampler_command $sampler_args'\n";
      call_user_func_array($main_menu_choice, $args);
    }
  }
}

/**
 * Helper function to run use through choices for admin functions.
 *
 * @param $plugin_type
 *   Optional. If passed, the user will be asked to select a plugin of the
 *   passed plugin type.
 * @param $require_metric
 *   If TRUE, require a module and metric to be chosen.
 *
 * @return
 *   An array of arguments to pass to the admin functions, or FALSE if the user
 *   cancels.
 */
function _drush_sampler_choices($plugin_type = FALSE, $require_metric = TRUE) {
  $args = array();
  // Choose module.
  if ($module = _drush_sampler_choose_module(!$require_metric)) {
    // Module selected.
    if ($module != dt("All")) {
      $args = array($module);
      // Choose metric.
      if ($metric = _drush_sampler_choose_metric($module, !$require_metric)) {
        // Metric selected.
        if ($metric != dt("All")) {
          $args = array_merge($args, array($metric));
          if ($plugin_type) {
            // Choose plugin.
            if ($plugin =_drush_sampler_choose_plugin($plugin_type)) {
              $args = array_merge($args, array($plugin));
            }
            else {
              return FALSE;
            }
          }
        }
      }
      else {
        return FALSE;
      }
    }
  }
  else {
    return FALSE;
  }

  return $args;
}

/**
 * Choose plugin type from a list.
 *
 * @return
 *   The plugin type chosen.
 */
function _drush_sampler_choose_plugin_type() {
  $plugin_types = array_flip(array_keys(sampler_load_plugin_types()));
  return drush_choice($plugin_types, dt("Choose a plugin type"), '!key');
}

/**
 * Choose plugin from a list.
 *
 * @param $plugin_type
 *   The plugin type used to build the list of plugins.
 *
 * @return
 *   The plugin chosen.
 */
function _drush_sampler_choose_plugin($plugin_type) {
  ctools_include('plugins');
  $plugins = ctools_get_plugins('sampler', $plugin_type);
  $plugins = array_keys($plugins);
  // Don't force a selection if only one item exists.
  if (count($plugins) == 1) {
    $plugin = $plugins[0];
    print dt("Plugin !plugin auto-selected", array('!plugin' => $plugin)) . "\n";
    return $plugins[0];
  }
  else {
    $plugins = array_flip($plugins);
    return drush_choice($plugins, dt("Choose a plugin"), '!key');
  }
}

/**
 * Choose module from a list.
 *
 * @param $include_all
 *   If TRUE, inclue an 'All' option.
 *
 * @return
 *   The module chosen.
 */
function _drush_sampler_choose_module($include_all = FALSE) {
  $modules = array_keys(sampler_load_metrics());
  // Don't force a selection if only one item exists.
  if (count($modules) == 1) {
    $module = $modules[0];
    print dt("Module !module auto-selected", array('!module' => $module)) . "\n";
    return $module;
  }
  else {
    $modules = array_flip($modules);

    if ($include_all) {
      $modules = array_merge(array(dt("All") => ''), $modules);
    }
    return drush_choice($modules, dt("Choose a module"), '!key');
  }
}

/**
 * Choose metric from a list.
 *
 * @param $include_all
 *   If TRUE, inclue an 'All' option.
 *
 * @return
 *   The metric chosen.
 */
function _drush_sampler_choose_metric($module, $include_all = FALSE) {
  $module_metrics = sampler_load_metrics($module);
  $metrics = array_keys($module_metrics);
  // Don't force a selection if only one item exists.
  if (count($metrics) == 1) {
    $metric = $metrics[0];
    print dt("Metric !metric auto-selected", array('!metric' => $metric)) . "\n";
    return $metric;
  }
  else {
    $metrics = array_flip($metrics);
    if ($include_all) {
      $metrics = array_merge(array(dt("All") => ''), $metrics);
    }
    return drush_choice($metrics, dt("Choose a metric"), '!key');
  }
}

/**
 * Drush callback; List all metrics the API is aware of.
 *
 * Also shows the metric description, machine name, default storage plugin,
 * and data types stored.
 *
 * @param $module
 *   Optional. If passed, only lists metrics provided by that particular
 *   module.
 * @param $metric_only
 *   Optional. List only this metric for the module.
 *
 * @return
 *   An output string showing the summary.
 */
function drush_sampler_list_metrics($module = NULL, $metric_only = NULL) {
  if (module_exists('sampler')) {
    $output = '';

    sampler_load_sampler();

    // Allow for single metrics, per module metrics, fall back to all.
    $all_metrics = sampler_load_metrics();
    if (isset($module) && isset($metric_only)) {
      if (isset($all_metrics[$module][$metric_only])) {
        $single_metric = $all_metrics[$module][$metric_only];
        $metrics_to_process = array($module => array($metric_only => $single_metric));
      }
      else {
        drush_set_error('INIT_ERROR', dt("No metric '!metric' found for module '!module'.", array('!module' => $module, '!metric' => $metric_only)));
        $metrics_to_process = array();
      }
    }
    elseif (isset($module)) {
      if (isset($all_metrics[$module])) {
        $metrics_to_process = array($module => $all_metrics[$module]);
      }
      else {
        drush_set_error('INIT_ERROR', dt("No metrics found for module '!module'.", array('!module' => $module)));
        $metrics_to_process = array();
      }
    }
    else {
      $metrics_to_process = $all_metrics;
    }

    foreach ($metrics_to_process as $module => $metric_data) {
      $output .= dt("Module") . ": $module\n";
      foreach ($metric_data as $metric => $settings) {
        $schema = sampler_load_metric_schema($module, $metric);
        // No stored schema information yet.
        if (empty($schema)) {
          $storage_plugin = $method_plugin = $adjustment_plugin = dt("[no plugin specified]");
          $data_type = array(dt("[no data type specified]"));
        }
        else {
          $storage_plugin = $schema->storage_plugin;
          $method_plugin = $schema->method_plugin;
          $adjustment_plugin = $schema->adjustment_plugin;
          $data_type = $schema->data_type;
        }
        $data_type = implode(', ', $data_type);
        $output .= "\t{$settings['title']}\n";
        $output .= "\t\t{$settings['description']}\n";
        $output .= "\t\tMachine name: $metric\n";
        $output .= "\t\tStorage plugin: $storage_plugin\n";
        $output .= "\t\tMethod plugin: $method_plugin\n";
        $output .= "\t\tAdjustment plugin: $adjustment_plugin\n";
        $output .= "\t\tSample data value types: $data_type\n";
      }
    }
    if (empty($output)) {
      $output = dt("No metrics found") . "\n";
    }
    print $output;
  }
  else {
    drush_set_error('INIT_ERROR', dt('Sampler module is not enabled.'));
  }
}

/**
 * Drush callback; List all plugins the API is aware of.
 *
 * @return
 *   A string of information describing the plugins.
 */
function drush_sampler_list_plugins() {
  if (module_exists('sampler')) {
    $output = '';
    sampler_load_sampler();
    $sampler = new Sampler();
    $plugins = $sampler->getPlugins();
    foreach ($plugins as $type => $plugin_type_data) {
      $output .= dt("Type") . ": $type\n";
      $output .= "\t" . dt("Available plugins") . ":\n";
      foreach ($plugin_type_data['plugins'] as $plugin => $plugin_data) {
        $output .= "\t$plugin ({$plugin_data['module']})\n";
      }
    }
    if (empty($output)) {
      $output = dt("No plugins found") . "\n";
    }
    print $output;
  }
  else {
    drush_set_error('INIT_ERROR', dt('Sampler module is not enabled.'));
  }
}

/**
 * Drush callback; Samples metrics, and optionally stores them.
 *
 * @see _drush_sample_metric()
 *
 * If no arguments are passed, all metrics are sampled. If a module is
 * passed, all metrics the module provides are sampled. If a module and metric
 * are provided, only that metric is sampled.
 *
 * Any option that a plugin takes to control the sample can be passed at the
 * command line as a full option with a value. For example to pass the
 * 'startstamp' option to the method plugin: --startstamp="2010-10-23"
 *
 * By default, samples are only computed, not saved. To save, pass the --save
 * argument.
 *
 * @param $module
 *   The module providing the metric.
 * @param $metric
 *   The metric name.
 */
function drush_sampler_sample($module = NULL, $metric = NULL) {
  $type = 'compute';
  $options = drush_get_context('cli');
  if (drush_get_option('object_batch_size')) {
    $type = 'batch';
  }
  elseif (drush_get_option('save')) {
    $type = 'save';
    unset($options['save']);
  }
  // Drush puts this in the options array, but we don't want it.
  unset($options['php']);
  foreach ($options as $key => $value) {
    // These come in as strings, but we want any numbers as integers.
    if (is_numeric($value)) {
      $options[$key] = intval($value);
    }
  }

  // Batch handles object_ids internally.
  if (isset($options['object_ids']) && $type != 'batch') {
    $options['object_ids'] = array_filter(explode(',', $options['object_ids']), 'trim');
  }

  // One metric.
  if (isset($module) && isset($metric)) {
    $output = _drush_sample_metric($module, $metric, $options, $type);
    print $output;
  }
  else {
    $all_metrics = sampler_load_metrics();
    // All metrics for a module.
    if (isset($module)) {
      if (isset($all_metrics[$module])) {
        $metrics_to_process = array($module => $all_metrics[$module]);
      }
      else {
        drush_set_error('INIT_ERROR', dt("No metrics found for module '!module'.", array('!module' => $module)));
        $metrics_to_process = array();
      }
    }
    // All metrics.
    else {
      $metrics_to_process = $all_metrics;
    }
    foreach ($metrics_to_process as $module_to_process => $metric_data) {
      foreach ($metric_data as $metric_to_process => $settings) {
        $output = _drush_sample_metric($module_to_process, $metric_to_process, $options, $type);
        print $output;
      }
    }
  }
}

/**
 * Samples metrics, and optionally stores them.
 *
 * @see drush_sampler_sample()
 *
 * @param $module
 *   The module providing the metric.
 * @param $metric
 *   The metric name.
 * @param $options
 *   An array of options to pass to the sampler. Any valid option used by a
 *   plugin is allowed.
 * @param $type
 *   The type of sample operation to perform:
 *     'compute': Only compute.
 *        'save': Compute and save.
 *       'batch': Batch process.
 * @return
 *   On success, a string of information about the sample.
 */
function _drush_sample_metric($module, $metric, $options, $type = 'compute') {
  $output = '';
  $sampler = _drush_sampler_object($module, $metric, $options);
  if (!empty($sampler)) {
    // Set custom lock interval if provided.
    if (isset($options['lock_interval'])) {
      $sampler->lockInterval = intval($options['lock_interval']);
      unset($options['lock_interval']);
    }
    if ($type == 'batch') {
      print "Batch started...\n";
      $process_result = $sampler->process();
      if ($process_result) {
        $output .= dt("Batch sample for metric '!metric' from module '!module' completed successfully, using storage plugin '!storage_plugin'.", array('!module' => $module, '!metric' => $metric, '!storage_plugin' => $sampler->storagePlugin));
        $output .= "\n";
      }
      else {
        if ($sampler->metricLocked) {
          _sampler_lock_error($module, $metric);
        }
        else {
          drush_set_error('SAMPLER_ERROR', dt("Batch sample for metric '!metric' from module '!module' failed, using storage plugin '!storage_plugin'.", array('!module' => $module, '!metric' => $metric, '!storage_plugin' => $sampler->storagePlugin)));
        }
      }
    }
    else {
      // We're calling lower-level sampling functions which don't have lock
      // management, so provide our own.
      if (lock_acquire($sampler->lockName, $sampler->lockInterval)) {
        $samples = $sampler->computeSamples();
        // Samples computed.
        if (is_array($samples)) {
          // Display some useful information about the computation.
          $output .= dt("Sampled metric '!metric' from module '!module', Samples computed: !samples_computed.", array('!module' => $module, '!metric' => $metric, '!samples_computed' => $sampler->samplesComputed));
          if (!empty($sampler->pluginOutput)) {
            $output .= '  ' . dt('Description: ') . implode(' ', $sampler->pluginOutput);
          }
          $output .= "\n";
          if ($type == 'save') {
            // Samples saved.
            if ($sampler->saveSamples($samples)) {
              // Display some useful information about the storage operation.
              $samples_saved = isset($sampler->samplesSaved) ? $sampler->samplesSaved : 0;
              $objects_saved = isset($sampler->objectsSaved) ? $sampler->objectsSaved : 0;
              $output .= dt("Samples saved using storage plugin '!storage_plugin', Samples saved: !samples_saved, Objects saved: !objects_saved\n", array('!storage_plugin' => $sampler->storagePlugin, '!samples_saved' => $samples_saved, '!objects_saved' => $objects_saved));
            }
            else {
              drush_set_error('SAMPLER_ERROR', dt("Could not save samples for metric '!metric' from module '!module', using storage plugin '!storage_plugin'.", array('!module' => $module, '!metric' => $metric, '!storage_plugin' => $sampler->storagePlugin)));
            }
          }
        }
        else {
          drush_set_error('SAMPLER_ERROR', dt("Could not build samples for metric '!metric' from module '!module'.", array('!module' => $module, '!metric' => $metric)));
        }
        lock_release($sampler->lockName);
      }
      else {
        _sampler_lock_error($module, $metric);
      }
    }
  }
  else {
    _sampler_load_error($module, $metric);
  }
  return $output;
}

/**
 * Updates the storage plugin for a metric.
 *
 * @see _drush_sampler_update_plugin()
 *
 * @param $module
 *   The module providing the metric.
 * @param $metric
 *   The metric name.
 * @param $storage
 *   The new storage plugin to use.
 */
function drush_sampler_update_storage_plugin($module, $metric, $storage) {
  _drush_sampler_update_plugin($module, $metric, 'storage', $storage);
}

/**
 * Updates the method plugin for a metric.
 *
 * @see _drush_sampler_update_plugin()
 *
 * @param $module
 *   The module providing the metric.
 * @param $metric
 *   The metric name.
 * @param $method
 *   The new method plugin to use.
 */
function drush_sampler_update_method_plugin($module, $metric, $method) {
  _drush_sampler_update_plugin($module, $metric, 'method', $method);
}

/**
 * Updates the adjustment plugin for a metric.
 *
 * @see _drush_sampler_update_plugin()
 *
 * @param $module
 *   The module providing the metric.
 * @param $metric
 *   The metric name.
 * @param $adjustment
 *   The new adjustment plugin to use.
 */
function drush_sampler_update_adjustment_plugin($module, $metric, $adjustment) {
  _drush_sampler_update_plugin($module, $metric, 'adjustment', $adjustment);
}

/**
 * Updates a plugin of a specific plugin type for a metric.
 *
 * @param $module
 *   The module providing the metric.
 * @param $metric
 *   The metric name.
 * @param $plugin_type
 *   The plugin type, as shown in sampler-list-plugins.
 * @param $plugin
 *   The name of the plugin to update for the type.
 *
 * @return
 *   A string of output describing the update on success.
 */
function _drush_sampler_update_plugin($module, $metric, $plugin_type, $plugin) {
  $sampler = _drush_sampler_object($module, $metric);
  $output = '';
  if ($sampler) {
    $plugin_map = array(
      'storage' => array(
        'property' => 'storagePlugin',
        'method' => 'StoragePlugin',
      ),
      'method' => array(
        'property' => 'methodPlugin',
        'method' => 'MethodPlugin',
      ),
      'adjustment' => array(
        'property' => 'adjustmentPlugin',
        'method' => 'AdjustmentPlugin',
      ),
    );
    $plugin_property = $plugin_map[$plugin_type]['property'];
    $sampler->$plugin_property = $plugin;
    $plugin_method = $plugin_map[$plugin_type]['method'];
    $method = "updateMetric{$plugin_method}";
    $t_args = array(
      '!plugin_type' => $plugin_type,
      '!module' => $module,
      '!metric' => $metric,
      '!plugin' => $plugin,
    );
    if ($sampler->$method()) {
      $output .= dt("!plugin_type for metric '!metric' from module '!module' updated to '!plugin'.\n", $t_args);
    }
    else {
      drush_set_error('SAMPLER_ERROR', dt("Could not update metric '!metric' from module '!module' to !plugin_type '!plugin'.", $t_args));
    }
  }
  else {
    _sampler_load_error($module, $metric);
  }
  print $output;
}

/**
 * Deletes all data for a metric, including stored values and the stored schema
 * information.
 *
 * @param $module
 *   The module providing the metric.
 * @param $metric
 *   The metric name.
 */
function drush_sampler_delete_data($module, $metric) {
  $options = drush_get_context('cli');
  $sampler_options = isset($options['database']) ? array('database' => $options['database']) : array();
  $sampler = _drush_sampler_object($module, $metric, $sampler_options);
  $output = '';
  if ($sampler) {
    // Ensure the --database flag is passed for external drupal storage plugins.
    $metric_schema = sampler_load_metric_schema($module, $metric);
    $external_drupal_storage = array(
      'drupal_database_table_external',
      'drupal_database_table_per_metric_external',
    );
    if (in_array($metric_schema->storage_plugin, $external_drupal_storage) && !drush_get_option('database')) {
      drush_set_error('SAMPLER_ERROR', dt("--database argument is required with storage plugin '!plugin'.", array('!plugin' => $metric_schema->storage_plugin)));
    }
    else {
      if (drush_confirm("WARNING: all data for this metric will be deleted!  Are you sure?")) {
        // Metric deleted.
        if ($sampler->deleteMetricFromSchema()) {
          $output .= dt("All data for '!metric' from module '!module' has been deleted using storage plugin '!storage_plugin'.\n", array('!module' => $module, '!metric' => $metric, '!storage_plugin' => $sampler->storagePlugin));
        }
        else {
          drush_set_error('SAMPLER_ERROR', dt("Could not delete metric '!metric' from module '!module' using storage plugin '!storage_plugin'.", array('!module' => $module, '!metric' => $metric, '!storage_plugin' => $sampler->storagePlugin)));
        }
      }
      else {
        $output .= dt("Aborted\n");
      }
    }
  }
  else {
    _sampler_load_error($module, $metric);
  }
  print $output;
}

/**
 * Helper function to generate an error message when a sampler object cannot be
 * loaded for a metric.
 *
 * @param $module
 *   The module providing the metric.
 * @param $metric
 *   The metric name.
 */
function _sampler_load_error($module, $metric) {
  drush_set_error('SAMPLER_ERROR', dt("Could not load sampler for metric '!metric' from module '!module'.", array('!module' => $module, '!metric' => $metric)));
}

/**
 * Helper function to generate an error message when a sampler object cannot
 * obtain a lock for a metric.
 *
 * @param $module
 *   The module providing the metric.
 * @param $metric
 *   The metric name.
 */
function _sampler_lock_error($module, $metric) {
  drush_set_error('SAMPLER_ERROR', dt("Failed to aquire lock for metric '!metric' from module '!module'.", array('!module' => $module, '!metric' => $metric)));
}

/**
 * Helper function to create a new sampler object.
 *
 * @param $module
 *   The module providing the metric.
 * @param $metric
 *   The metric name.
 * @param $options
 *   An array of options to pass to the sampler. Any valid option used by a
 *   plugin is allowed. Optional.
 * @return
 *   The sampler object if it can be created for the metric, FALSE otherwise.
 */
function _drush_sampler_object($module, $metric, $options = array()) {
  sampler_load_sampler();
  $sampler = new Sampler();
  return $sampler->newSampler($module, $metric, $options);
}

