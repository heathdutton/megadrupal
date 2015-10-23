<?php

/**
 * @file
 * Drush Ctools Export Bonus
 */

/**
 * Implements hook_drush_command().
 */
function drush_ctex_bonus_drush_command() {
  $items = array();

  // Uber master and rebuild master command.
  $items['cb-umaster'] = array(
    'aliases' => array('cbum'),
    'callback' => 'drush_ctex_bonus_uber_master',
    'description' => 'Uber master command that will enable the exported module but does extra checks first: a) enable i18n modules if available b) enable the export module c) run the master command d) run drush cc all.',
    'commandfile' => 'ctex',
    'arguments' => array(
      'module'  => 'Name of your module.',
    ),
    'examples' => array(
      'drush cbum export_module' => 'Will enable the module but will perform some checks first.',
    ),
    'options' => array(
      'result-file' => 'The name of the result file for database backups,',
      'ctex-log-location' => 'The place of the ctex log directory.',
    ),
  );

  $items['cbreb-master'] = array(
    'aliases' => array('cbrm'),
    'callback' => 'drush_ctex_bonus_rebuild_master',
    'description' => 'Master command to run one or all commands at once.',
    'commandfile' => 'ctex',
    'arguments' => array(
      'module'  => 'Name of your module.',
      'command'  => 'Either "all" or the name of a command.',
    ),
    'examples' => array(
      'drush cbrm export_module' => 'Will give you a selection of commands to run.',
    ),
    'options' => array(
      'result-file' => 'The name of the result file for database backups,',
      'ctex-log-location' => 'The place of the ctex log directory.',
    ),
  );

  // List of commands which have rebuild and export capabilities.
  // Note: important, if you add more commands here, make sure they are in a certain
  // order as the master commands takes these commands and fires them.
  // In some occasions, we need to make sure we don't have racing conditions e.g.
  // a field on a content type that doesn't exist yet.

  $items['cbreb-vars'] = array(
    'aliases' => array('cbrva'),
    'callback' => 'drush_ctex_bonus_perform_action',
    'description' => 'Rebuild variables.',
    'commandfile' => 'ctex',
    'arguments' => array(
      'module'  => 'Name of your module.',
    ),
    'examples' => array(
      'drush cbrva export_module' => 'Will search for a file called export_module.ctex_bonus.variables.inc and rebuild variables.',
    ),
    'options' => array(
      'result-file' => 'The name of the result file for database backups,',
      'ctex-log-location' => 'The place of the ctex log directory.',
    ),
    'ctex export' => 'variables',
    'ctex run all' => TRUE,
    'ctex delete' => TRUE,
  );

  $items['cbreb-ctypes'] = array(
    'aliases' => array('cbrct'),
    'callback' => 'drush_ctex_bonus_perform_action',
    'description' => 'Rebuild content types.',
    'commandfile' => 'ctex',
    'arguments' => array(
      'module'  => 'Name of your module.',
    ),
    'examples' => array(
      'drush cbrct export_module' => 'Will search for a file called export_module.ctex_bonus.ctypes.inc and rebuild content types.',
    ),
    'options' => array(
      'result-file' => 'The name of the result file for database backups,',
      'ctex-log-location' => 'The place of the ctex log directory.',
    ),
    'ctex export' => 'ctypes',
    'ctex run all' => TRUE,
    'ctex delete' => TRUE,
  );

  $items['cbreb-vocs'] = array(
    'aliases' => array('cbrv'),
    'callback' => 'drush_ctex_bonus_perform_action',
    'description' => 'Rebuild vocabularies.',
    'commandfile' => 'ctex',
    'arguments' => array(
      'module'  => 'Name of your module.',
    ),
    'examples' => array(
      'drush cbrv export_module' => 'Will search for a file called export_module.ctex_bonus.vocabularies.inc and rebuild vocabularies.',
    ),
    'options' => array(
      'result-file' => 'The name of the result file for database backups,',
      'ctex-log-location' => 'The place of the ctex log directory.',
    ),
    'ctex dependency' => 'taxonomy',
    'ctex export' => 'vocabularies',
    'ctex run all' => TRUE,
    'ctex delete' => TRUE,
  );

  $items['cbreb-filt'] = array(
    'aliases' => array('cbrff'),
    'callback' => 'drush_ctex_bonus_perform_action',
    'description' => 'Rebuild filter formats.',
    'commandfile' => 'ctex',
    'arguments' => array(
      'module'  => 'Name of your module.',
    ),
    'examples' => array(
      'drush cbrff export_module' => 'Will search for a file called export_module.ctex_bonus.filters.inc and rebuild filter formats.',
    ),
    'options' => array(
      'result-file' => 'The name of the result file for database backups,',
      'ctex-log-location' => 'The place of the ctex log directory.',
    ),
    'ctex export' => 'filters',
    'ctex run all' => TRUE,
  );

  $items['cbreb-wysi'] = array(
    'aliases' => array('cbrw'),
    'callback' => 'drush_ctex_bonus_perform_action',
    'description' => 'Rebuild wysiwyg profiles.',
    'commandfile' => 'ctex',
    'arguments' => array(
      'module'  => 'Name of your module.',
    ),
    'examples' => array(
      'drush cbrw export_module' => 'Will search for a file called export_module.ctex_bonus.wysiwyg.inc and rebuild wysiwyg profiles.',
    ),
    'options' => array(
      'result-file' => 'The name of the result file for database backups,',
      'ctex-log-location' => 'The place of the ctex log directory.',
    ),
    'ctex dependency' => 'wysiwyg',
    'ctex export' => 'wysiwyg',
    'ctex run all' => TRUE,
  );

  $items['cbreb-df'] = array(
    'aliases' => array('cbrdf'),
    'callback' => 'drush_ctex_bonus_perform_action',
    'description' => 'Rebuild date formats.',
    'commandfile' => 'ctex',
    'arguments' => array(
      'module'  => 'Name of your module.',
    ),
    'examples' => array(
      'drush cbrdf export_module' => 'Will search for a file called export_module.ctex_bonus.dateformats.inc and rebuild date formats.',
    ),
    'options' => array(
      'result-file' => 'The name of the result file for database backups,',
      'ctex-log-location' => 'The place of the ctex log directory.',
    ),
    'ctex export' => 'dateformats',
    'ctex run all' => TRUE,
  );

  $items['cbreb-fields'] = array(
    'aliases' => array('cbrf'),
    'callback' => 'drush_ctex_bonus_perform_action',
    'description' => 'Rebuild Field API fields and instances.',
    'commandfile' => 'ctex',
    'arguments' => array(
      'module'  => 'Name of your module.',
    ),
    'examples' => array(
      'drush cbrf export_module' => 'Will search for a file called export_module.ctex_bonus.fields.inc and rebuild fields and instances.',
    ),
    'options' => array(
      'result-file' => 'The name of the result file for database backups,',
      'ctex-log-location' => 'The place of the ctex log directory.',
    ),
    'ctex export' => 'fields',
    'ctex run all' => TRUE,
    'ctex delete' => TRUE,
  );

  $items['cbreb-alias'] = array(
    'aliases' => array('cbra'),
    'callback' => 'drush_ctex_bonus_perform_action',
    'description' => 'Rebuild aliases.',
    'commandfile' => 'ctex',
    'arguments' => array(
      'module'  => 'Name of your module.',
    ),
    'examples' => array(
      'drush cbra export_module' => 'Will search for a file called export_module.ctex_bonus.aliases.inc and rebuild aliases.',
    ),
    'options' => array(
      'result-file' => 'The name of the result file for database backups,',
      'ctex-log-location' => 'The place of the ctex log directory.',
    ),
    'ctex dependency' => 'path',
    'ctex export' => 'aliases',
    'ctex run all' => TRUE,
  );

  $items['cbreb-nodeq'] = array(
    'aliases' => array('cbrn'),
    'callback' => 'drush_ctex_bonus_perform_action',
    'description' => 'Rebuild nodequeues.',
    'commandfile' => 'ctex',
    'arguments' => array(
      'module'  => 'Name of your module.',
    ),
    'examples' => array(
      'drush cbrn export_module' => 'Will search for a file called export_module.ctex_bonus.nodequeues.inc and rebuild nodequeues.',
    ),
    'options' => array(
      'result-file' => 'The name of the result file for database backups,',
      'ctex-log-location' => 'The place of the ctex log directory.',
    ),
    'ctex dependency' => 'nodequeue',
    'ctex export' => 'nodequeues',
    'ctex run all' => TRUE,
    'ctex delete' => TRUE,
  );

  $items['cbreb-menus'] = array(
    'aliases' => array('cbrme'),
    'callback' => 'drush_ctex_bonus_perform_action',
    'description' => 'Rebuild menus.',
    'commandfile' => 'ctex',
    'arguments' => array(
      'module'  => 'Name of your module.',
    ),
    'examples' => array(
      'drush cbrme export_module' => 'Will search for a file called export_module.ctex_bonus.menus.inc and rebuild menus (and some links).',
    ),
    'options' => array(
      'result-file' => 'The name of the result file for database backups,',
      'ctex-log-location' => 'The place of the ctex log directory.',
    ),
    'ctex dependency' => 'menu',
    'ctex export' => 'menus',
    'ctex run all' => TRUE,
  );

  $items['cbreb-block'] = array(
    'aliases' => array('cbrb'),
    'callback' => 'drush_ctex_bonus_perform_action',
    'description' => 'Rebuild blocks.',
    'commandfile' => 'ctex',
    'arguments' => array(
      'module'  => 'Name of your module.',
    ),
    'examples' => array(
      'drush cbrb export_module' => 'Will search for a file called export_module.ctex_bonus.blocks.inc and rebuild blocks.',
    ),
    'options' => array(
      'result-file' => 'The name of the result file for database backups,',
      'ctex-log-location' => 'The place of the ctex log directory.',
    ),
    'ctex dependency' => 'block',
    'ctex export' => 'blocks',
    'ctex run all' => TRUE,
  );

  $items['cbreb-lang'] = array(
    'aliases' => array('cbrl'),
    'callback' => 'drush_ctex_bonus_perform_action',
    'description' => 'Rebuild languages.',
    'commandfile' => 'ctex',
    'arguments' => array(
      'module'  => 'Name of your module.',
    ),
    'examples' => array(
      'drush cbrl export_module' => 'Will search for a file called export_module.ctex_bonus.languages.inc and rebuild languages.',
    ),
    'options' => array(
      'result-file' => 'The name of the result file for database backups,',
      'ctex-log-location' => 'The place of the ctex log directory.',
    ),
    'ctex dependency' => 'locale',
    'ctex export' => 'languages',
    'ctex run all' => TRUE,
    'ctex delete' => TRUE,
  );

  $items['cbreb-wbmod'] = array(
    'aliases' => array('cbrwm'),
    'callback' => 'drush_ctex_bonus_perform_action',
    'description' => 'Rebuild workbench moderation.',
    'commandfile' => 'ctex',
    'arguments' => array(
      'module'  => 'Name of your module.',
    ),
    'examples' => array(
      'drush cbrwm export_module' => 'Will search for a file called export_module.ctex_bonus.moderation.inc and rebuild workbench moderation settings.',
    ),
    'options' => array(
      'result-file' => 'The name of the result file for database backups,',
      'ctex-log-location' => 'The place of the ctex log directory.',
    ),
    'ctex dependency' => 'workbench_moderation',
    'ctex export' => 'moderation',
    'ctex run all' => TRUE,
  );

  $items['cbreb-styles'] = array(
    'aliases' => array('cbrs'),
    'callback' => 'drush_ctex_bonus_perform_action',
    'description' => 'Revert all image styles.',
    'commandfile' => 'ctex',
    'arguments' => array(
      'module'  => 'Name of your module.',
    ),
    'examples' => array(
      'drush cbrs export_module' => 'Will look for image styles and revert them.',
    ),
    'options' => array(
      'result-file' => 'The name of the result file for database backups,',
      'ctex-log-location' => 'The place of the ctex log directory.',
    ),
    'ctex dependency' => 'image',
    'ctex write module file' => TRUE,
    'ctex no file check' => TRUE,
    'ctex export' => 'styles',
  );

  $items['cbreb-entity'] = array(
    'aliases' => array('cbrea'),
    'callback' => 'drush_ctex_bonus_perform_action',
    'description' => 'Revert all entity api exportables.',
    'commandfile' => 'ctex',
    'arguments' => array(
      'module'  => 'Name of your module.',
    ),
    'examples' => array(
      'drush cbrea export_module' => 'Will revert all entity api exportables.',
    ),
    'options' => array(
      'result-file' => 'The name of the result file for database backups,',
      'ctex-log-location' => 'The place of the ctex log directory.',
    ),
    'ctex dependency' => 'entity',
    'ctex write module file' => TRUE,
    'ctex no file check' => TRUE,
    'ctex export' => 'entity',
  );

  $items['cbreb-rep'] = array(
    'aliases' => array('cbrp'),
    'callback' => 'drush_ctex_bonus_perform_action',
    'description' => 'Rebuild roles and permissions.',
    'commandfile' => 'ctex',
    'arguments' => array(
      'module'  => 'Name of your module.',
    ),
    'examples' => array(
      'drush cbrp export_module' => 'Will search for a file called export_module.ctex_bonus.roles_and_perms.inc and rebuild roles and permissions.',
    ),
    'options' => array(
      'result-file' => 'The name of the result file for database backups,',
      'ctex-log-location' => 'The place of the ctex log directory.',
    ),
    'ctex clear cache' => TRUE,
    'ctex export' => 'roles_and_perms',
    'ctex run all' => TRUE,
  );

  // Other commands.
  $items['cbcom-deps'] = array(
    'aliases' => array('cbcd'),
    'callback' => 'drush_ctex_bonus_perform_action',
    'description' => 'Compare dependencies.',
    'commandfile' => 'ctex',
    'arguments' => array(
      'module'  => 'Name of your module.',
    ),
    'examples' => array(
      'drush cbcd export_module' => 'Will compare the dependencies of the module and enabled modules of the site printing a drush command to disable them if needed.',
    ),
    'options' => array(
      'result-file' => 'The name of the result file for database backups,',
      'ctex-log-location' => 'The place of the ctex log directory.',
    ),
    'ctex export' => 'info',
    'ctex no export' => TRUE,
    'ctex callback' => 'drush_ctex_bonus_compare_dependencies',
  );

  $items['cbcom-def'] = array(
    'aliases' => array('cbcdb'),
    'callback' => 'drush_ctex_bonus_perform_action',
    'description' => 'Search for configuration that is not in code and optionally delete them.',
    'commandfile' => 'ctex',
    'arguments' => array(
      'module'  => 'Name of your module.',
    ),
    'examples' => array(
      'drush cbcdb export_module' => 'Will check the database configuration with the CTEX bonus files. In case a database configuration is not in the code, you will have the option to remove it. This is not a diff compare.',
    ),
    'options' => array(
      'result-file' => 'The name of the result file for database backups,',
      'ctex-log-location' => 'The place of the ctex log directory.',
    ),
    'ctex interactive' => TRUE,
    'ctex callback' => 'drush_ctex_bonus_compare_defaults',
  );

  $items['cb-save-all'] = array(
    'aliases' => array('cbsa'),
    'callback' => 'drush_ctex_bonus_perform_action',
    'description' => 'Save CTools, entity exportables and image styles to the database. This will allow you to disable an exported module after everything else has been saved.',
    'commandfile' => 'ctex',
    'arguments' => array(
      'module'  => 'Name of your module.',
    ),
    'examples' => array(
      'drush cbsa export_module' => 'Save all CTools and entity exportables and image styles.',
    ),
    'options' => array(
      'result-file' => 'The name of the result file for database backups,',
      'ctex-log-location' => 'The place of the ctex log directory.',
    ),
    'ctex callback' => 'drush_ctex_bonus_save_non_ctex_objects',
  );

  return $items;
}

/**
 * Implements hook_drush_ctex_files_alter().
 */
function drush_ctex_bonus_drush_ctex_files_alter(&$data) {
  drush_set_context('DRUSH_LOG_CALLBACK', 'drush_ctex_bonus_log');

  ctools_include('export');
  $module = $data['module'];

  // Run export functions.
  $commands = drush_ctex_bonus_drush_command();
  foreach ($commands as $key => $command) {

    // Do not proceed when there's no ctex export key.
    if (empty($command['ctex export']) || !empty($command['ctex no export'])) {
      continue;
    }

    // Check dependency. Either continue to the next command
    // or add it to the info file in the dependencies function.
    if (!empty($command['ctex dependency'])) {
      if (module_exists($command['ctex dependency'])) {
        drush_ctex_bonus_dependency($command['ctex dependency']);
      }
      else {
        continue;
      }
    }

    $code = '';
    $function = 'drush_ctex_bonus_export_' . $command['ctex export'];
    $function($data, $code, $module);

    // Some functions write to the module file.
    if (empty($command['ctex write module file'])) {
      $header = _drush_ctex_bonus_write_header($module, $command['ctex export'], $command['aliases'][0]);
      $file = $module . '.ctex_bonus.' . $command['ctex export'] . '.inc';
      $data['files'][$file] = $header . $code;
    }
  }

  // Install file.
  drush_ctex_bonus_export_install($data, $module);

  // Info file.
  drush_ctex_bonus_export_info_file($data, $module);
}

/**
 * Log a message.
 *
 * We use this function instead of _drush_print_log only so we can
 * log the message to screen and to log files.
 */
function drush_ctex_bonus_log($entry) {

  $args = drush_get_arguments();
  $module = isset($args[1]) ? $args[1] : '';


  // Determine the log directory.
  $log_dir = _drush_ctex_bonus_prepare_backup_dir() . '/';
  if (empty($log_dir)) {
    $log_dir = "./";
  }

  // Send to _drush_print_log() as well.
  _drush_print_log($entry);

  // When running in backend mode, log messages are not displayed, as they will
  // be returned in the JSON encoded associative array.
  if (drush_get_context('DRUSH_BACKEND')) {
    return TRUE;
  }

  // Create logs - we send everything to the ctex.log, including errors.
  // Ignore some types though.
  if (!in_array($entry['type'], array('bootstrap', 'debug'))) {
    $date = '[' . date('d/m/Y H:i:s', time()) . ']';
    $all_cmd = 'echo ' . $date . ' [' . $entry['type'] . '] [Mod: ' . $module . '] ' . escapeshellarg($entry['message']) . ' >> ' . $log_dir . 'ctex.log';
    drush_shell_exec($all_cmd);

    // Log errors in a separate file.
    if ($entry['type'] == 'error') {
      $specific_cmd = 'echo ' . $date . ' [Mod: ' . $module . '] ' . escapeshellarg($entry['message']) . ' >> ' . $log_dir . 'ctex-error.log';
      drush_shell_exec($specific_cmd);
    }
  }
}

/**
 * Set or return dependencies.
 */
function drush_ctex_bonus_dependency($dependency = '') {
  static $dependencies = array();

  if (!empty($dependency)) {
    if (!in_array($dependency, $dependencies) && !in_array($dependency, drush_ctex_bonus_exclude_modules())) {
      $dependencies[] = $dependency;
    }
  }
  else {
    sort($dependencies);
    return $dependencies;
  }
}

/**
 * Return a list of modules to exclude in permissions and dependencies.
 */
function drush_ctex_bonus_exclude_modules() {
  $exclude = drush_get_option('drush_ctex_bonus_exclude_modules', array(
    'coder',
    'coder_review',
    'devel',
    'devel_generate',
    'devel_node_access',
    'devel_themer',
    'field_ui',
    'update',
    'views_ui',
  ));
  return !empty($exclude) ? $exclude : array('^$');
}

/**
 * Return a list of aliases to exclude in aliases and menu links.
 */
function drush_ctex_bonus_exclude_aliases() {
  $exclude = drush_get_option('drush_ctex_bonus_exclude_aliases', array(
    'node/',
    'user/',
    'taxonomy/term/',
  ));
  return !empty($exclude) ? $exclude : array('^$');
}

/**
 * Return a list of custom menus to exclude.
 */
function drush_ctex_bonus_exclude_menus() {
  $exclude = drush_get_option('drush_ctex_bonus_exclude_menus', array(
    'devel',
    'features',
    'management',
    'navigation',
    'user-menu',
  ));
  return !empty($exclude) ? $exclude : array('^$');
}

/**
 * Return a list of variables to ignore.
 */
function drush_ctex_bonus_exclude_variables() {
  $exclude = drush_get_option('drush_ctex_bonus_exclude_variables', array(
    'install_profile',
    'install_task',
    'install_time',
    'cron_last',
    'node_cron_last',
    'ctools_last_cron',
    'css_js_query_string',
    'javascript_parsed',
    'update_last_check',
    'path_alias_whitelist',
  ));
  return !empty($exclude) ? $exclude : array('^$');
}

/**
 * Return a list of admin themes we're ignoring.
 */
function drush_ctex_bonus_admin_themes() {
  $exclude = drush_get_option('drush_ctex_bonus_exclude_themes', array(
    'seven',
    'rubik',
  ));
  return !empty($exclude) ? $exclude : array('^$');
}

/**
 * Exports variables.
 */
function drush_ctex_bonus_export_variables(&$data, &$code, $module) {
  $variables = db_query("SELECT name, value FROM {variable} where name NOT IN (:exclude_variables) ORDER BY name", array(':exclude_variables' => drush_ctex_bonus_exclude_variables()));
  foreach ($variables as $variable) {
    $code .= "  \$variables['" . $variable->name . "'] = " . ctools_var_export(unserialize($variable->value), '  ') . ";\n\n";
  }
  $code .= "  return \$variables;\n}\n\n";
}

/**
 * Exports content types.
 */
function drush_ctex_bonus_export_ctypes(&$data, &$code, $module) {
  // We're not using hook_node_info(), but just an export and resave it later.
  $ctypes = node_type_get_types();
  foreach ($ctypes as $info) {
    if ($info->base == 'node_content') {
      $code .= "  \$ctypes['" . $info->type . "'] = " . ctools_var_export((array) $info, '  ') . ";\n\n";
    }
  }
  $code .= "  return \$ctypes;\n}\n";
}

/**
 * Exports vocabularies.
 */
function drush_ctex_bonus_export_vocabularies(&$data, &$code, $module) {
  $vocabularies = taxonomy_get_vocabularies();
  foreach ($vocabularies as $vocabulary) {
    // Don't export the vid.
    unset($vocabulary->vid);
    $code .= "  \$vocabularies['" . $vocabulary->machine_name . "'] = " . ctools_var_export((array) $vocabulary, '  ') . ";\n\n";
  }
  $code .= "  return \$vocabularies;\n}\n";
}

/**
 * Exports filter formats.
 */
function drush_ctex_bonus_export_filters(&$data, &$code, $module) {
  $filter_formats = filter_formats();
  foreach ($filter_formats as $id => $format) {
    $format->filters = array();
    foreach (filter_list_format($format->format) as $filter) {
      if (!empty($filter->status)) {
        $format->filters[$filter->name]['weight'] = $filter->weight;
        $format->filters[$filter->name]['status'] = $filter->status;
        $format->filters[$filter->name]['settings'] = $filter->settings;
      }
    }
    $code .= "  \$filters['" . $id . "'] = " . ctools_var_export($format, '  ') . ";\n\n";
  }
  $code .= "  return \$filters;\n}\n";
}

/**
 * Exports wysiwyg profiles.
 */
function drush_ctex_bonus_export_wysiwyg(&$data, &$code, $module) {
  $wysiwyg_profiles = db_query("SELECT * FROM {wysiwyg}");
  foreach ($wysiwyg_profiles as $profile) {
    $code .= "  \$wysiwyg['" . $profile->format . "'] = " . ctools_var_export((array) $profile, '  ') . ";\n\n";
  }
  $code .= "  return \$wysiwyg;\n}\n";
}

/**
 * Exports date formats.
 */
function drush_ctex_bonus_export_dateformats(&$data, &$code, $module) {
  $date_formats = db_query("SELECT * FROM {date_formats} WHERE locked = 0 ORDER BY format");
  foreach ($date_formats as $date_format) {
    $code .= "  \$dateformats['" . $date_format->dfid . "'] = " . ctools_var_export((array) $date_format, '  ') . ";\n\n";
  }
  $code .= "  return \$dateformats;\n}\n\n";

  $code .= _drush_ctex_bonus_write_header($module, 'date_format_types', 'cbrdf', FALSE);
  $date_format_types = db_query("SELECT * FROM {date_format_type} WHERE locked = 0 ORDER BY type");
  foreach ($date_format_types as $date_format_type) {
    $code .= "  \$date_format_types['" . $date_format_type->type . "'] = " . ctools_var_export((array) $date_format_type, '  ') . ";\n\n";
  }
  $code .= "  return \$date_format_types;\n}\n";
}

/**
 * Exports fields.
 */
function drush_ctex_bonus_export_fields(&$data, &$code, $module) {

  // Fields.
  $fields = field_info_fields();

  // Sort the fields by key to facilitate better file diff analysis
  ksort($fields);
  foreach ($fields as $key => $field) {
    $field_module = $field['module'];
    drush_ctex_bonus_dependency($field_module);
    unset($field['columns']);
    unset($field['id']);
    unset($field['bundles']);
    if ($field['storage']['type'] == variable_get('field_storage_default', 'field_sql_storage')) {
      unset($field['storage']);
    }
    // If we still have a storage declaration here it means that a non-default
    // storage type was altered into to the field definition. And noone would
    // never need to change the 'details' key, so don't render it.
    if (isset($field['storage']['details'])) {
      unset($field['storage']['details']);
    }
    $code .= "  \$fields['" . $key . "'] = " . ctools_var_export($field, '  ') . ";\n\n";
  }
  $code .= "  return \$fields;\n}\n\n";

  // Instances.
  $code .= _drush_ctex_bonus_write_header($module, 'instances', 'cbrf', FALSE);
  $instances = field_info_instances();

  // Sort the instances and sub-fields by key to facilitate better file diff analysis
  ksort($instances);
  foreach ($instances as $entity_type => $bundles) {
    ksort($bundles);
    foreach ($bundles as $bundle_name => $fields) {
      ksort($fields);
      foreach ($fields as $field_name => $info) {

        // Remove keys we don't need.
        unset($info['id']);
        unset($info['field_id']);

        // Widgets and display modules.
        $widget_module = $info['widget']['module'];
        drush_ctex_bonus_dependency($widget_module);
        foreach ($info['display'] as $view_mode => $view_mode_info) {
          $display_module = isset($view_mode_info['module']) ? $view_mode_info['module'] : '';
          if (!empty($display_module)) {
            drush_ctex_bonus_dependency($display_module);
          }
        }

        $identifier = $entity_type . ':' . $bundle_name . ':' . $field_name;
        $code .= "  \$instances['" . $identifier . "'] = " . ctools_var_export($info, '  ') . ";\n\n";
      }
    }
  }
  $code .= "  return \$instances;\n}\n";
}

/**
 * Exports aliases.
 */
function drush_ctex_bonus_export_aliases(&$data, &$code, $module) {
  $aliases = db_query("SELECT source, alias, language FROM {url_alias} WHERE source NOT REGEXP :exclude_aliases ORDER BY alias", array(':exclude_aliases' => implode('|', drush_ctex_bonus_exclude_aliases())));
  foreach ($aliases as $alias) {
    $code .= "  \$aliases[" . $alias->source . "-" . $alias->language . "] = " . ctools_var_export((array) $alias, '  ') . ";\n\n";
  }
  $code .= "  return \$aliases;\n}\n";
}

/**
 * Exports nodequeues.
 */
function drush_ctex_bonus_export_nodequeues(&$data, &$code, $module) {
  $nodequeues = nodequeue_load_queues(nodequeue_get_all_qids());
  foreach ($nodequeues as $queue) {
    unset($queue->qid);
    $code .= "  \$nodequeues['" . $queue->name . "'] = " . ctools_var_export($queue, '  ') . ";\n\n";
  }
  $code .= "  return \$nodequeues;\n}\n";
}

/**
 * Exports menus.
 */
function drush_ctex_bonus_export_menus(&$data, &$code, $module) {
  $menus = db_query("SELECT * FROM {menu_custom} ORDER BY menu_name");
  foreach ($menus as $menu) {
    if (in_array($menu->menu_name, drush_ctex_bonus_exclude_menus())) {
      continue;
    }
    $menu->links = db_query("SELECT link_path, router_path, link_title, options, hidden, external, weight FROM {menu_links} WHERE menu_name = :menu_name AND link_path NOT REGEXP :exclude_aliases", array(':menu_name' => $menu->menu_name, ':exclude_aliases' => implode('|', drush_ctex_bonus_exclude_aliases())))->fetchAllAssoc('link_path', PDO::FETCH_ASSOC);
    $code .= "  \$menus['" . $menu->menu_name . "'] = " . ctools_var_export($menu, '  ') . ";\n\n";
  }
  $code .= "  return \$menus;\n}\n";
}

/**
 * Exports blocks.
 */
function drush_ctex_bonus_export_blocks(&$data, &$code, $module) {
  $blocks = db_query("SELECT * FROM {block} WHERE theme NOT IN (:themes) AND module != 'block' ORDER BY module,delta,theme", array(':themes' => drush_ctex_bonus_admin_themes()));
  foreach ($blocks as $block) {
    $identifier = $block->module . '-' . $block->delta . '-' . $block->theme;
    $block->block_node_type = db_query("SELECT * FROM {block_node_type} WHERE module = :module AND delta = :delta ORDER BY module,delta", array(':module' => $block->module, ':delta' => $block->delta))->fetchAll();
    $block->block_role = db_query("SELECT * FROM {block_role} WHERE module = :module AND delta = :delta ORDER BY module,delta", array(':module' => $block->module, ':delta' => $block->delta))->fetchAll();
    unset($block->bid);
    $code .= "  \$blocks['" . $identifier . "'] = " . ctools_var_export($block, '  ') . ";\n\n";
  }
  $code .= "  return \$blocks;\n}\n";
}

/**
 * Exports languages.
 */
function drush_ctex_bonus_export_languages(&$data, &$code, $module) {
  $languages = language_list();
  $locale_languages = locale_language_list('native');

  foreach ($locale_languages as $key => $language) {
    $code .= "  \$languages['" . $key . "'] = " . ctools_var_export((array) $languages[$key], '  ') . ";\n\n";
  }
  $code .= "  return \$languages;\n}\n";
}

/**
 * Exports workbench moderation.
 */
function drush_ctex_bonus_export_moderation(&$data, &$code, $module) {
  // States.
  $states = db_query('SELECT name from {workbench_moderation_states}');
  foreach ($states as $state) {
    $code .= "  \$moderation['" . $state->name . "'] = " . ctools_var_export(workbench_moderation_state_load($state->name), '  ') . ";\n\n";
  }
  $code .= "  return \$moderation;\n}\n\n";

  // Transitions.
  $code .= _drush_ctex_bonus_write_header($module, 'transitions', 'cbrwm', FALSE);
  foreach (workbench_moderation_transitions() as $transition) {
    $code .= "  \$transitions['" . $transition->from_name . ":" . $transition->to_name . "'] = " . ctools_var_export($transition, '  ') . ";\n\n";
  }
  $code .= "  return \$transitions;\n}\n\n";
}

/**
 * Exports image styles.
 */
function drush_ctex_bonus_export_styles(&$data, &$code, $module) {
  $code = "\n";
  $code .= "/**\n";
  $code .= " * Implements hook_image_default_styles().\n";
  $code .= " */\n";
  $code .= "function " . $module . "_image_default_styles() {\n";
  $code .= "  \$styles = array();\n";
  $code .= "\n";
  $image_styles = image_styles();
  $exported_styles = array();
  foreach ($image_styles as $style_name => $style) {
    _drush_ctext_bonus_image_style_sanitize($style);
    $style_export = ctools_var_export($style, '  ');
    $exported_styles[] = "  // Exported image style: {$style_name}.";
    $exported_styles[] = "  \$styles['{$style_name}'] = {$style_export};";
    $exported_styles[] = "";
  }
  $code .= implode("\n", $exported_styles);
  $code .= "\n  return \$styles;\n}\n";

  $module_file = $module . '.module';
  $data['files'][$module_file] .= $code;
}

/**
 * Exports entity api exportables.
 */
function drush_ctex_bonus_export_entity(&$data, &$code, $module) {
  // The entity exportables are based on features, so we need to trigger
  // all these hooks ourselves - oh the irony.
  module_load_include('inc', 'entity', 'entity.features');
  $items = entity_features_api();

  $export = $code = array();
  foreach ($items as $entity_type => $info) {
    $options = entity_features_export_options($entity_type);
    $code_array[$info['default_hook']] = entity_features_export_render($module, array_keys($options), $export, $entity_type);
  }

  if (!empty($code_array)) {
    $code = '';
    foreach ($code_array as $hook => $items) {
      $hook_code = $items[$hook];

      $code = "\n";
      $code .= "/**\n";
      $code .= " * Implements hook_" . $hook . "().\n";
      $code .= " */\n";
      $code .= "function " . $module . "_" . $hook . "() {\n";
      $code .= $hook_code;
      $code .= "\n}\n";

      $module_file = $module . '.module';
      $data['files'][$module_file] .= $code;
    }
  }
}

/**
 * Exports roles and permissions.
 */
function drush_ctex_bonus_export_roles_and_perms(&$data, &$code, $module) {
  $roles = _drush_ctex_bonus_user_roles();
  foreach ($roles as $rid => $role) {
    $code .= "  \$roles_and_perms['" . $role->name . "'] = " . ctools_var_export($role, '  ') . ";\n\n";
  }
  $code .= "  return \$roles_and_perms;\n}\n";
}

/**
 * Create an install file.
 */
function drush_ctex_bonus_export_install(&$data, $module) {
  $code = "<?php\n\n";
  $code .= "/**\n";
  $code .= " * @file\n";
  $code .= " * Install file generated by Drush Ctools Export Bonus.\n";
  $code .= " */\n\n";
  $code .= "/**\n";
  $code .= " * Implements hook_install().\n";
  $code .= " */\n";
  $code .= "function " . $module . "_install() {\n";
  $code .= "  db_update('system')\n";
  $code .= "    ->fields(array('weight' => 200))\n";
  $code .= "    ->condition('name', '$module')\n";
  $code .= "    ->execute();\n";
  $code .= "}\n";

  $install_file = $module . '.install';
  $data['files'][$install_file] = $code;
}

/**
 * Exports data to info file.
 */
function drush_ctex_bonus_export_info_file(&$data, $module) {

  // Get the dependencies which were set during this request.
  $known_dependencies = drush_ctex_bonus_dependency();

  // Get known dependencies from an already written file if available.
  $dcbd_info_file = drupal_get_path('module', $module) . '/' . $module . '.ctex_bonus.info.inc';
  if (file_exists($dcbd_info_file)) {
    include_once $dcbd_info_file;
    $function = $module . '_drush_ctex_bonus_info_dependencies';
    if (function_exists($function)) {
      $info_dependencies = $function();
      foreach ($info_dependencies as $dependency) {
        drush_ctex_bonus_dependency($dependency);
      }
    }
  }

  // Get the dependencies added by CTools.
  $info_file = $module . '.info';
  $info_file_array = drupal_parse_info_format($data['files'][$info_file]);
  if (!empty($info_file_array['dependencies'])) {
    foreach ($info_file_array['dependencies'] as $dependency) {
      drush_ctex_bonus_dependency($dependency);
    }
  }

  $known_dependencies = drush_ctex_bonus_dependency();
  _drush_ctex_bonus_message(dt("Following modules are going to be ignored:\n@exclude", array('@exclude' => implode(', ', drush_ctex_bonus_exclude_modules()))));
  _drush_ctex_bonus_message(dt("Following dependencies are going to be added:\n@dependencies", array('@dependencies' => implode(', ', $known_dependencies))));

  // Get list of modules that are enabled, but not yet in the dependencies list.
  // Add the current module as a fake info so we can calculcate it's dependencies.
  $unknown_dependencies = array();
  $module_list = system_list('module_enabled');
  $fake_module = new stdClass;
  $fake_module->name = $module;
  $fake_module->info['dependencies'] = $known_dependencies;
  $module_list[$module] = $fake_module;
  $module_list = _module_build_dependencies($module_list);
  $this_modules_requires = $module_list[$module]->requires;
  foreach ($module_list as $site_module => $info) {
    if (!in_array($site_module, $known_dependencies) && $site_module != $module && strpos($info->filename, '.profile') === FALSE && !in_array($site_module, drush_ctex_bonus_exclude_modules())) {
      if (!isset($this_modules_requires[$site_module])) {
        $unknown_dependencies[] = $site_module;
      }
    }
  }
  if (!empty($unknown_dependencies)) {
    _drush_ctex_bonus_message(dt("Following modules are enabled, but not added to the dependencies list:\n@unknown", array('@unknown' => implode(', ', $unknown_dependencies))));
  }
  drush_log(dt('You can add more separated by comma and will be remembered for the next time you export. Leave empty to continue.'), 'success');

  // Ask for which dependencies to add to the export.
  $valid = FALSE;
  while ($valid == FALSE) {
    $ask_dependencies = drush_prompt(dt('Dependencies'), NULL, FALSE);
    $ask_dependencies = trim($ask_dependencies);
    if (!empty($ask_dependencies)) {
      $error = FALSE;
      $ask_dependencies = explode(',', $ask_dependencies);
      foreach ($ask_dependencies as $added) {
        $added = trim($added);
        if (!module_exists($added)) {
          drush_log(dt('"@module" not found or does not exist.', array('@module' => $added)), 'error');
          $error = TRUE;
          break;
        }
        else {
          drush_ctex_bonus_dependency($added);
        }
      }
      if (!$error) {
        $valid = TRUE;
      }
    }
    else {
      $ask_dependencies = array();
      $valid = TRUE;
    }
  }

  $dependencies = drush_ctex_bonus_dependency();

  // Add dependencies to info file.
  if (!empty($dependencies)) {
    $dependencies_string = '';
    $dependencies_array = array();
    foreach ($dependencies as $dependency) {
      // Don't add it twice to the info file.
      if (isset($info_file_array['dependencies']) && !in_array($dependency, $info_file_array['dependencies'])) {
        $dependencies_string .= "dependencies[] = " . $dependency . "\n";
      }
      $dependencies_array[] = "'" . $dependency . "'";
    }
    $data['files'][$info_file] .= $dependencies_string;
  }

  // Write dependencies.
  $code = "<?php\n\n";
  $code .= "/**\n";
  $code .= " * @file\n";
  $code .= " * Info file generated by Drush Ctools Export Bonus.\n * \n";
  $code .= " * Only edit this file by hand in case you know what you're doing.\n";
  $code .= " * Manual changes will be kept after a new export, except were mentioned.\n";
  $code .= " */\n\n";
  $code .= "/**\n";
  $code .= " * Returns all dependencies.\n";
  $code .= " */\n";
  $code .= "function " . $module . "_drush_ctex_bonus_info_dependencies() {\n";
  $code .= "  return array(\n    ";
  $code .= implode(",\n    ", $dependencies_array);
  $code .= "\n  );\n}\n\n";

  # ------------------------------------------------------------------------- #
  // Write a function that holds all the files of this module. The key is the
  // name of the file and the value indicates whether to remove the file
  // from the files array so no code can be overwritten at all by accident.

  $files_array = array();
  foreach (array_keys($data['files']) as $file_name) {
    $files_array['' . $file_name . ''] = array('overwrite' => TRUE);
  }

  // Check if we have a previous version.
  if (file_exists($dcbd_info_file)) {
    include_once $dcbd_info_file;
    $ignored_files = array();
    $function = $module . '_drush_ctex_bonus_info_files';
    if (function_exists($function)) {
      $info_files = $function();
      foreach ($info_files as $file_name => $info) {
        if (!$info['overwrite']) {
          // Set key to false and remove it from the files array.
          $files_array[$file_name]['overwrite'] = FALSE;
          unset($data['files'][$file_name]);
          $ignored_files[$file_name] = ' - ' . $file_name;
        }
      }
    }
    if (!empty($ignored_files)) {
      _drush_ctex_bonus_message(dt("Following files have been ignored to be written:\n@files", array('@files' => implode("\n", $ignored_files))), TRUE, FALSE);
    }
  }

  $code .= "/**\n";
  $code .= " * Returns files info.\n";
  $code .= " */\n";
  $code .= "function " . $module . "_drush_ctex_bonus_info_files() {\n";
  $code .= "  return " . ctools_var_export($files_array, '  ');
  $code .= ";\n}\n";

  $dcbd_info_file = $module . '.ctex_bonus.info.inc';
  $data['files'][$dcbd_info_file] = $code;
  _drush_ctex_bonus_message(dt("The info file will been written to '@file'.\nYou can manually edit it to add to add/remove dependencies or mark which files you do not want to overwrite the next time.", array('@file' => $dcbd_info_file)), TRUE);
}

/**
 * Drush callback: uber master command.
 */
function drush_ctex_bonus_uber_master($module = '') {
  drush_set_context('DRUSH_LOG_CALLBACK', 'drush_ctex_bonus_log');

  if (module_exists($module)) {
    drush_log(dt('This "@module" module is already enabled. You probably want to run "drush cbrm @module"', array('@module' => $module)), 'error');
    return;
  }

  if (!_drush_ctex_bonus_check_module($module, 'info', FALSE)) {
    return;
  }

  // Check i18n related modules.
  $i18n_modules = array();
  drush_log(dt('Checking whether to enable i18n related modules first or not.'), 'success');
  $function = $module . '_drush_ctex_bonus_info_dependencies';
  if (function_exists($function)) {
    $info_dependencies = $function();
    foreach ($info_dependencies as $dependency) {
      if (strpos($dependency, 'i18n') !== FALSE) {
        $i18n_modules[] = $dependency;
      }
    }
  }

  if (!empty($i18n_modules)) {
    drush_log(dt('Enabling i18n modules first'), 'success');
    drush_invoke_process('@self', 'en', $i18n_modules);
  }

  // Invoke this module.
  drush_log(dt('Enabling the "@module" module', array('@module' => $module)), 'success');
  drush_invoke_process('@self', 'en', array($module));

  // Invoke master command.
  drush_log(dt('Invoking all commands of Drush ctex Bonus'), 'success');
  drush_invoke_process('@self', 'cbrm', array($module, 'all'));

  // Clear all cache.
  drush_log(dt('Clearing cache'), 'success');
  drush_invoke_process('@self', 'cc', array('all'));
}

/**
 * Drush callback: master command to present all commands and run them at once.
 */
function drush_ctex_bonus_rebuild_master($module = '', $command_line_selection = '') {
  drush_set_context('DRUSH_LOG_CALLBACK', 'drush_ctex_bonus_log');

  if (!_drush_ctex_bonus_check_module($module)) {
    return;
  }

  $path = drupal_get_path('module', $module);

  // Commands.
  $options = array('all' => dt('All'));
  $commands = drush_ctex_bonus_drush_command();
  foreach ($commands as $key => $command) {

    // Some commands can not be run again.
    if (in_array($command['callback'], array('drush_ctex_bonus_rebuild_master', 'drush_ctex_bonus_uber_master'))) {
      unset($commands[$key]);
      continue;
    }

    // Check if the file exists.
    if (!empty($command['ctex export']) && empty($command['ctex write module file'])) {
      $file = $path . '/' . $module . '.ctex_bonus.' . $command['ctex export'] . '.inc';
      if (!file_exists($file)) {
        continue;
      }
    }

    $all = empty($command['ctex run all']) ? ' (*)' : '';
    $options[$key] = $command['description'] . $all;
  }

  // Add ctools commands.
  $options['cter'] = dt('Revert all CTools exportables. (*)');

  // Selection.
  if (empty($command_line_selection)) {
    $selection = drush_choice($options, dt('Select an action - actions marked with (*) are not executed when selecting "all":'));
    if (!$selection) {
      return drush_user_abort(dt('No action selected.'));
    }
  }
  else {
    $selection = $command_line_selection;
  }

  // Make sure there are no spaces (shouldn't be but you never know).
  $selection = trim($selection);

  // Log location.
  $log_dir = drush_get_option('ctex-log-location');
  $log_dir = empty($log_dir) ? array() : array('--ctex-log-location=' . drush_get_option('ctex-log-location'));

  // CTools revert all exportables.
  if ($selection == 'cter') {
    _drush_ctex_bonus_cache_check(array('ctex run all' => FALSE));
    drush_invoke_process('@self', 'cter', array(), array('--all'));
  }
  // All selection.
  elseif ($selection == 'all') {

    // Create a backup first.
    _drush_ctex_bonus_backup(FALSE, drush_get_option('result-file', FALSE));

    // Add ctools command.
    $commands['cter'] = array('ctex run all' => FALSE);

    // Dependencies check.
    _drush_ctex_bonus_dependencies_check($module);

    drush_log(dt('Peforming all actions.'), 'success');
    foreach ($options as $key => $title) {
      if (!empty($commands[$key]['ctex run all'])) {
        _drush_ctex_bonus_cache_check($commands[$key]);
        drush_invoke_process('@self', 'cbrm', array($module, $key), $log_dir);
      }
    }
  }
  // Command line selection.
  elseif (!empty($command_line_selection) && isset($commands[$command_line_selection])) {
    drush_log(dt('Peforming action @action', array('@action' => $commands[$command_line_selection]['description'])), 'success');
    $commands[$command_line_selection]['callback']($module, FALSE, $command_line_selection);
  }
  // Single selection.
  elseif (!empty($selection) && isset($commands[$selection])) {

    // Some commands need to run interactive.
    if (!empty($commands[$selection]['ctex interactive'])) {
      drush_log(dt('You can only run this command directly: drush @alias @module', array('@alias' => $commands[$selection]['aliases'][0], '@module' => $module)), 'error');
      return;
    }

    _drush_ctex_bonus_backup(TRUE, drush_get_option('result-file', FALSE));
    _drush_ctex_bonus_cache_check($commands[$selection]);
    _drush_ctex_bonus_dependencies_check($module);
    drush_invoke_process('@self', 'cbrm', array($module, $selection), $log_dir);
  }
}

/**
 * Drush callback: perform an action.
 *
 * @param $module
 *   The name of the module.
 * @param $ask_and_check
 *   Whether we need to ask for backup and check dependencies and clear cache.
 * @param $command_key
 *   The drush command key. This is passed on when the all command is used.
 */
function drush_ctex_bonus_perform_action($module = '', $ask_and_check = TRUE, $command_key = '') {
  drush_set_context('DRUSH_LOG_CALLBACK', 'drush_ctex_bonus_log');

  // The module must be enabled.
  if (!_drush_ctex_bonus_check_module($module)) {
    return;
  }

  // Get command.
  if (empty($command_key)) {
    $command = drush_get_command();
  }
  else {
    $commands = drush_ctex_bonus_drush_command();
    $command = $commands[$command_key];
  }

  // Check dependency.
  if (!empty($command['ctex dependency']) && !module_exists($command['ctex dependency'])) {
    drush_log(dt('Module "@module" does not exists.', array('@module' => $command['ctex dependency'])), 'error');
    return;
  }

  // Get an include file if wanted.
  if (empty($command['ctex no file check']) && !_drush_ctex_bonus_check_module($module, $command['ctex export'])) {
    return;
  }

  // Ask for backup and check dependencies and clear cache.
  if ($ask_and_check) {
    _drush_ctex_bonus_backup(TRUE, drush_get_option('result-file', FALSE));
    _drush_ctex_bonus_cache_check($command);
    _drush_ctex_bonus_dependencies_check($module);
  }

  // Run function.
  $function = !empty($command['ctex callback']) ? $command['ctex callback'] : 'drush_ctex_bonus_rebuild_' . $command['ctex export'];
  $function($module);
}

/**
 * Rebuild all variables.
 */
function drush_ctex_bonus_rebuild_variables($module) {
  // Variables.
  $variables_function = $module . "_drush_ctex_bonus_variables";
  $variables = $variables_function();

  foreach ($variables as $name => $value) {
    try {
      variable_set($name, $value);
      drush_log(dt('Saved variable "@var"', array('@var' => $name)), 'success');
    }
    catch (Exception $e) {
      $message = dt("Error saving variable \"@var\"\n" . $e->getMessage(), array('@var' => $name));
      _drush_ctex_bonus_message($message, TRUE, TRUE, TRUE);
    }
  }
}

/**
 * Rebuild ctypes.
 */
function drush_ctex_bonus_rebuild_ctypes($module) {
  // Get defined content types.
  $ctypes_function = $module . "_drush_ctex_bonus_ctypes";
  $ctypes = $ctypes_function();

  foreach ($ctypes as $ctype) {
    try {
      node_type_save((object) $ctype);
      drush_log(dt('Saved content type @content_type', array('@content_type' => $ctype['name'])), 'success');
    }
    catch (Exception $e) {
      $message = dt("Error saving content type \"@content_type\"\n" . $e->getMessage(), array('@content_type' => $ctype['name']));
      _drush_ctex_bonus_message($message, TRUE, TRUE, TRUE);
    }
  }
}

/**
 * Rebuild vocabularies.
 */
function drush_ctex_bonus_rebuild_vocabularies($module) {
  // Existing vocabularies.
  $existing_vocabularies = taxonomy_get_vocabularies();

  // Get defined content types.
  $vocabularies_function = $module . "_drush_ctex_bonus_vocabularies";
  $vocabularies = $vocabularies_function();

  foreach ($vocabularies as $vocabulary) {
    $vocabulary = (object) $vocabulary;
    foreach ($existing_vocabularies as $existing_vocabulary) {
      if ($existing_vocabulary->machine_name === $vocabulary->machine_name) {
        $vocabulary->vid = $existing_vocabulary->vid;
      }
    }
    try {
      taxonomy_vocabulary_save($vocabulary);
      drush_log(dt('Saved vocabulary @vocabulary', array('@vocabulary' => $vocabulary->name)), 'success');
    }
    catch (Exception $e) {
      $message = dt("Error saving vocabulary \"@vocabulary\"\n" . $e->getMessage(), array('@vocabulary' => $vocabulary->name));
      _drush_ctex_bonus_message($message, TRUE, TRUE, TRUE);
    }
  }
}

/**
 * Rebuild filter formats.
 */
function drush_ctex_bonus_rebuild_filters($module) {
  // Get filter formats.
  $filters_function = $module . "_drush_ctex_bonus_filters";
  $filter_formats = $filters_function();

  foreach ($filter_formats as $format) {
    try {
      filter_format_save($format);
      drush_log(dt('Saved filter format @format', array('@format' => $format->name)), 'success');
    }
    catch (Exception $e) {
      $message = dt("Error saving filter format \"@format\"\n" . $e->getMessage(), array('@format' => $format->name));
      _drush_ctex_bonus_message($message, TRUE, TRUE, TRUE);
    }
  }
}

/**
 * Rebuild wysiwyg profiles.
 */
function drush_ctex_bonus_rebuild_wysiwyg($module) {
  // Clear table
  db_truncate('wysiwyg')->execute();

  // Get wysiwyg profiles.
  $wysiwyg_function = $module . "_drush_ctex_bonus_wysiwyg";
  $wysiwyg_profiles = $wysiwyg_function();

  foreach ($wysiwyg_profiles as $profile) {
    try {
      drupal_write_record('wysiwyg', $profile);
      drush_log(dt('Saved wysiwyg profile for @format', array('@format' => $profile['format'])), 'success');
    }
    catch (Exception $e) {
      $message = dt("Error saving wysiwyg profile for \"@format\"\n" . $e->getMessage(), array('@format' => $profile['format']));
      _drush_ctex_bonus_message($message, TRUE, TRUE, TRUE);
    }
  }

  // Clear wysiwyg cache.
  wysiwyg_profile_cache_clear();
}

/**
 * Rebuild date formats.
 */
function drush_ctex_bonus_rebuild_dateformats($module) {
  // Get date formats and date format types.
  $dateformats_function = $module . "_drush_ctex_bonus_dateformats";
  $date_formats = $dateformats_function();
  $date_format_types_function = $module . "_drush_ctex_bonus_date_format_types";
  $date_format_types = $date_format_types_function();

  foreach ($date_formats as $format) {
    db_merge('date_formats')
      ->key(array(
        'dfid' => $format['dfid']
      ))
      ->fields(array(
        'format' => $format['format'],
        'type' => $format['type'],
        'locked' => $format['locked']
      ))
      ->execute();
    drush_log(dt('Saving date format @df', array('@df' => $format['format'])), 'success');
  }

  foreach ($date_format_types as $type) {
    db_merge('date_format_type')
      ->key(array(
        'type' => $type['type']
      ))
      ->fields(array(
        'title' => $type['title'],
        'locked' => $type['locked']
      ))
      ->execute();
    drush_log(dt('Saving date format type @dft', array('@dft' => $type['type'])), 'success');
  }
}

/**
 * Rebuild fields.
 */
function drush_ctex_bonus_rebuild_fields($module) {
  // Get fields and instances.
  $fields_function = $module . "_drush_ctex_bonus_fields";
  $fields = $fields_function();
  $instance_function = $module . "_drush_ctex_bonus_instances";
  $instances = $instance_function();

  foreach ($fields as $field) {
    // Create or update field.
    if ($existing_field = field_info_field($field['field_name'])) {
      try {
        field_update_field($field);
        drush_log(dt('Updated field "@field_name"', array('@field_name' => $field['field_name'])), 'success');
      }
      catch (Exception $e) {
        $message = dt("Error updating field \"@field_name\"\n" . $e->getMessage(), array('@field_name' => $field['field_name']));
        _drush_ctex_bonus_message($message, TRUE, TRUE, TRUE);
      }
    }
    else {
      try {
        field_create_field($field);
        drush_log(dt('Created field "@field_name"', array('@field_name' => $field['field_name'])), 'success');
      }
      catch (Exception $e) {
        $message = dt("Error creating field \"@field_name\"\n" . $e->getMessage(), array('@field_name' => $field['field_name']));
        _drush_ctex_bonus_message($message, TRUE, TRUE, TRUE);
      }
    }
  }

  foreach ($instances as $instance) {
    // Create or update field instance.
    $existing_instance = field_info_instance($instance['entity_type'], $instance['field_name'], $instance['bundle']);
    if ($existing_instance) {
      try {
        field_update_instance($instance);
        drush_log(dt('Updated instance "@instance_name" for @entity_type / @bundle', array('@instance_name' => $instance['field_name'], '@entity_type' => $instance['entity_type'], '@bundle' => $instance['bundle'])), 'success');
      }
      catch (Exception $e) {
        $message = dt("Error updating instance \"@instance_name\" for @entity_type / @bundle\n" . $e->getMessage(), array('@instance_name' => $instance['field_name'], '@entity_type' => $instance['entity_type'], '@bundle' => $instance['bundle']));
        _drush_ctex_bonus_message($message, TRUE);
      }
    }
    else {
      try {
        field_create_instance($instance);
        drush_log(dt('Created instance @instance_name for @entity_type / @bundle ', array('@instance_name' => $instance['field_name'], '@entity_type' => $instance['entity_type'], '@bundle' => $instance['bundle'])), 'success');
      }
      catch (Exception $e) {
        $message = dt("Error creating instance @instance_name for @entity_type / @bundle\n" . $e->getMessage(), array('@instance_name' => $instance['field_name'], '@entity_type' => $instance['entity_type'], '@bundle' => $instance['bundle']));
        _drush_ctex_bonus_message($message, TRUE);
      }
    }
  }

  // Trigger a menu rebuild.
  variable_set('menu_rebuild_needed', TRUE);
}

/**
 * Rebuild aliases.
 */
function drush_ctex_bonus_rebuild_aliases($module) {
  // Get aliases.
  $aliases_function = $module . "_drush_ctex_bonus_aliases";
  $aliases = $aliases_function();

  // We remove the aliases. Possibly we could add more functionality,
  // but let's see after some testing.
  foreach ($aliases as $alias) {
    try {
      $condition = array('source' => $alias['source']);
      if (path_load($condition)) {
        path_delete($condition);
      }
      path_save($alias);
      drush_log(dt('Saved alias @alias for @source', array('@alias' => $alias['alias'], '@source' => $alias['source'])), 'success');
    }
    catch (Exception $e) {
      $message = dt("Error saving alias \"@alias\" for @source\n" . $e->getMessage(), array('@alias' => $alias['alias'], '@source' => $alias['source']));
      _drush_ctex_bonus_message($message, TRUE, TRUE, TRUE);
    }
  }
}

/**
 * Rebuild nodequeues.
 */
function drush_ctex_bonus_rebuild_nodequeues($module) {
  // Get existing nodequeues.
  $existing_nodequeues_names = array();
  $existing_nodequeues = nodequeue_load_queues(nodequeue_get_all_qids());
  foreach ($existing_nodequeues as $queue) {
    $existing_nodequeues_names[$queue->name] = $queue->qid;
  }

  // Get nodequeues.
  $nodequeues_function = $module . "_drush_ctex_bonus_nodequeues";
  $nodequeues = $nodequeues_function();

  foreach ($nodequeues as $name => $queue) {

    try {
      // Check on existing queue.
      if (isset($existing_nodequeues_names[$queue->name])) {
        $queue->qid = $existing_nodequeues_names[$queue->name];
      }
      else {
        $queue->new = TRUE;
      }

      // Send to owner submit function.
      $form_state = array('values' => (array) $queue);
      $function = $queue->owner . "_nodequeue_form_submit";
      if (function_exists($function)) {
        $function($queue, $form_state);
      }

      // Save the actual queue.
      nodequeue_save($queue);

      // Send to owner submit finish function.
      $function = $queue->owner . "_nodequeue_form_submit_finish";
      if (function_exists($function)) {
        $function($queue, $form_state);
      }

      // Check sizes.
      nodequeue_check_subqueue_sizes($queue);

      // Message.
      if (!empty($queue->new)) {
        drush_log(dt('Created nodequeue @name', array('@name' => $queue->title)), 'success');
      }
      else {
        drush_log(dt('Updated nodequeue @name', array('@name' => $queue->title)), 'success');
      }
    }
    catch (Exception $e) {
      $message = dt("Error saving nodequeue \"@name\"\n" . $e->getMessage(), array('@name' => $queue->title));
      _drush_ctex_bonus_message($message, TRUE, TRUE, TRUE);
    }
  }
}

/**
 * Rebuild menus.
 */
function drush_ctex_bonus_rebuild_menus($module) {
  // Get menus.
  $menus_function = $module . "_drush_ctex_bonus_menus";
  $menus = $menus_function();

  foreach ($menus as $key => $menu) {
    try {
      menu_save((array) $menu);
      drush_log(dt('Saved custom menu "@title"', array('@title' => $menu->title)), 'success');
    }
    catch (Exception $e) {
      $message = dt("Error saving custom menu \"@title\"\n" . $e->getMessage(), array('@title' => $menu->title));
      _drush_ctex_bonus_message($message, TRUE, TRUE, TRUE);
    }
    if (!empty($menu->links)) {
      foreach ($menu->links as $item) {
        $item['menu_name'] = $key;
        $item['options'] = unserialize($item['options']);
        $existing_item = _drush_ctex_bonus_get_existing_menu_item($item);
        if (!empty($existing_item)) {
          $item['mlid'] = $existing_item->mlid;
          $item['plid'] = $existing_item->plid;
        }
        try {
          menu_link_save($item, (array) $existing_item);
          drush_log(dt('Saved menu link "@title"', array('@title' => $item['link_title'])), 'success');
        }
        catch (Exception $e) {
          $message = dt("Error saving menu link \"@title\"\n" . $e->getMessage(), array('@title' => $item['link_title']));
          _drush_ctex_bonus_message($message, TRUE, TRUE, TRUE);
        }
      }
    }
  }
}

/**
 * Rebuild blocks.
 */
function drush_ctex_bonus_rebuild_blocks($module) {
  // Rebuild the blocks.
  _block_rehash();

  // Get existing blocks.
  $existing_blocks = array();
  $existing_blocks_result = db_query("SELECT bid, module, delta, theme FROM {block} WHERE theme NOT IN (:themes) AND module != 'block'", array(':themes' => drush_ctex_bonus_admin_themes()));
  foreach ($existing_blocks_result as $block) {
    $identifier = $block->module . '-' . $block->delta . '-' . $block->theme;
    $existing_blocks[$identifier] = $block->bid;
  }

  // Truncate extra configuration.
  db_truncate('block_node_type')->execute();
  db_truncate('block_role')->execute();

  // Get blocks.
  $blocks_function = $module . "_drush_ctex_bonus_blocks";
  $blocks = $blocks_function();

  $block_role_node_types = array();
  foreach ($blocks as $identifier => $block) {
    $update = array();
    if (isset($existing_blocks[$identifier])) {
      $update = array('module', 'delta', 'theme');
    }
    try {
      drupal_write_record('block', $block, $update);
      drush_log(dt('Saved block for \"@module / @delta\"', array('@module' => $block->module, '@delta' => $block->delta)), 'success');
    }
    catch (Exception $e) {
      $message = dt("Error saving block for \"@module / @delta\"\n" . $e->getMessage(), array('@module' => $block->module, '@delta' => $block->delta));
      _drush_ctex_bonus_message($message, TRUE, TRUE, TRUE);
    }
    foreach (array('block_node_type', 'block_role') as $table) {
      if (!empty($block->{$table})) {
        foreach ($block->{$table} as $record) {
          $identifier = implode('-', array_values( (array) $record));
          if (!isset($block_role_node_types[$identifier])) {
            $block_role_node_types[$identifier] = TRUE;
            drupal_write_record($table, $record);
          }
        }
      }
    }
  }
}

/**
 * Rebuild languages.
 */
function drush_ctex_bonus_rebuild_languages($module) {
  $existing_languages = language_list();

  // Get languages.
  $languages_function = $module . "_drush_ctex_bonus_languages";
  $languages = $languages_function();

  include_once DRUPAL_ROOT . '/includes/locale.inc';

  foreach ($languages as $key => $language) {
    try {
      // Either update or create a language.
      if (!isset($existing_languages[$key])) {
        locale_add_language($key);
        drush_log(dt('Created language @language', array('@language' => $language['name'])), 'success');
      }
      else {
        db_update('languages')
          ->fields(array(
            'name' => $language['name'],
            'native' => $language['native'],
            'domain' => $language['domain'],
            'prefix' => $language['prefix'],
            'direction' => $language['direction'],
          ))
          ->condition('language',  $key)
          ->execute();
        drush_log(dt('Updated language @language', array('@language' => $language['name'])), 'success');
      }
    }
    catch (Exception $e) {
      $message = dt("Error saving language \"@language\"\n" . $e->getMessage(), array('@language' => $language['module']));
      _drush_ctex_bonus_message($message, TRUE, TRUE, TRUE);
    }
  }
}

/**
 * Rebuild workbench moderation profiles.
 */
function drush_ctex_bonus_rebuild_moderation($module) {
  // Get states.
  $states_function = $module . "_drush_ctex_bonus_moderation";
  $states = $states_function();
  foreach ($states as $state) {
    workbench_moderation_state_save($state);
    drush_log(dt('Saved workbench state "@state"', array('@state' => $state->label)), 'success');
  }

  // Get transitions.
  $transitions_function = $module . "_drush_ctex_bonus_transitions";
  $transitions = $transitions_function();
  foreach ($transitions as $transition) {
    workbench_moderation_transition_save($transition);
    drush_log(dt('Saved workbench transition "@from" => "@to"', array('@from' => $transition->from_name, '@to' => $transition->to_name)), 'success');
  }
}

/**
 * Revert all image styles.
 */
function drush_ctex_bonus_rebuild_styles($module) {
  $image_styles = image_styles();

  $styles_function = $module . '_image_default_styles';
  if (function_exists($styles_function)) {
    $default_styles = $styles_function();
    foreach ($default_styles as $style_name => $info) {
      if (isset($image_styles[$style_name]) && $image_styles[$style_name]['storage'] == IMAGE_STORAGE_OVERRIDE) {
        image_default_style_revert($image_styles[$style_name]);
        drush_log(dt('Reverted style "@style."', array('@style' => $style_name)), 'success');
      }
    }
  }
}

/**
 * Revert all entity api exportables.
 */
function drush_ctex_bonus_rebuild_entity($module) {
  // Load the hooks.
  module_load_include('inc', 'entity', 'entity.features');
  $items = entity_features_api();

  foreach ($items as $entity_type => $info) {
    $hook = $module . '_' . $info['default_hook'];
    if (function_exists($hook)) {
      $items = $hook();
      foreach ($items as $name => $data) {
        entity_delete($entity_type, $name);
        drush_log(dt('Reverted "@name" for @entity_type.', array('@name' => $name, '@entity_type' => $info['name'])), 'success');
      }
    }
  }
}

/**
 * Rebuild roles and permissions.
 */
function drush_ctex_bonus_rebuild_roles_and_perms($module) {
  // Get roles and permissions.
  $roles_and_perms_function = $module . "_drush_ctex_bonus_roles_and_perms";
  $roles_and_perms = $roles_and_perms_function();

  foreach ($roles_and_perms as $name => $role) {

    try {
      // Check if there's an existing one
      if ($existing = user_role_load_by_name($name)) {
        $role->rid = $existing->rid;
      }
      // Remove all permissions for this rid.
      if ($role->rid) {
        db_delete('role_permission')
          ->condition('rid', $role->rid)
          ->execute();
      }

      // Save role.
      user_role_save($role);

      // Save permissions.
      user_role_change_permissions($role->rid, $role->permissions);

      drush_log(dt('Saved role and permissions for "@role"', array('@role' => $role->name)), 'success');
    }
    catch (Exception $e) {
      $message = dt("Error saving role and permissions for \"@role\"\n" . $e->getMessage(), array('@role' => $role->name));
      _drush_ctex_bonus_message($message, TRUE, TRUE, TRUE);
    }
  }
}

/**
 * Compare dependencies.
 */
function drush_ctex_bonus_compare_dependencies($module) {
  $module_dependencies = array();
  $function = $module . '_drush_ctex_bonus_info_dependencies';
  if (function_exists($function)) {
    $module_dependencies = $function();
  }

  $compare = $checked = array();
  if (!empty($module_dependencies)) {
    $site_module_list = system_list('module_enabled');
    $site_module_list = _module_build_dependencies($site_module_list);
    $this_modules_requires = $site_module_list[$module]->requires;
    foreach ($site_module_list as $site_module => $info) {
      if (!in_array($site_module, $module_dependencies) && $site_module != $module && strpos($info->filename, '.profile') === FALSE) {
        if (!isset($this_modules_requires[$site_module])) {
          $compare[] = $site_module;
        }
      }
    }
  }

  if (empty($compare)) {
    drush_log(dt('No differences found.'), 'success');
  }
  else {
    drush_log(dt('Following modules are enabled which are not listed as a dependency (with drush disable command):'), 'success');
    drush_log(dt('drush dis @modules', array('@modules' => implode(' ', $compare))), 'success');
  }
}

/**
 * Compare defaults and database configuration.
 */
function drush_ctex_bonus_compare_defaults($module) {

  $commands = drush_ctex_bonus_drush_command();
  // Move variables to the very end.
  $variables_command = $commands['cbreb-vars'];
  unset($commands['cbreb-vars']);
  $commands[] = $variables_command;
  foreach ($commands as $key => $command) {
    if (!empty($command['ctex delete'])) {
      $export = $command['ctex export'];
      if (_drush_ctex_bonus_check_module($module, $export)) {
        drush_log(dt('Checking defaults with database in the @file file.', array('@file' => $export)), 'success');
        $function = 'drush_ctex_bonus_compare_' . $export;
        $function($module);
        _drush_ctex_bonus_message('', FALSE, TRUE);
      }
    }
  }
}

/**
 * Compare variables.
 */
function drush_ctex_bonus_compare_variables($module) {

  // Default variables.
  $variables_function = $module . "_drush_ctex_bonus_variables";
  $default_variables = $variables_function();

  // Database variables.
  $may_delete = array();
  $variables = db_query("SELECT name FROM {variable} where name NOT IN (:exclude_variables)", array(':exclude_variables' => drush_ctex_bonus_exclude_variables()));
  foreach ($variables as $variable) {
    if (!isset($default_variables[$variable->name])) {
      $may_delete[] = $variable->name;
    }
  }

  if (!empty($may_delete)) {
    $confirm = drush_confirm(dt("@count variables found to delete, do you want to see them and optionally delete them one by one ?", array('@count' => count($may_delete))));
    if ($confirm) {
      foreach ($may_delete as $name) {
        $confirm = drush_confirm(dt('Do you want to delete variables "@variable"', array('@variable' => $name)));
        if ($confirm) {
          variable_del($name);
        }
      }
    }
  }
}

/**
 * Compare content types.
 */
function drush_ctex_bonus_compare_ctypes($module) {

  // Default ctypes.
  $ctypes_function = $module . "_drush_ctex_bonus_ctypes";
  $default_ctypes = $ctypes_function();

  // Database node types.
  $may_delete = array();
  $node_types = node_type_get_types();
  foreach ($node_types as $node_type) {
    if ($node_type->base == 'node_content') {
      if (!isset($default_ctypes[$node_type->type])) {
        $may_delete[$node_type->type] = $node_type->name;
      }
    }
  }

  if (!empty($may_delete)) {
    $confirm = drush_confirm(dt("@count content types found to delete, do you want to see them and optionally delete them one by one ?", array('@count' => count($may_delete))));
    if ($confirm) {
      foreach ($may_delete as $ctype => $title) {
        $confirm = drush_confirm(dt('Do you want to delete content type "@ctype"', array('@ctype' => $title)));
        if ($confirm) {
          node_type_delete($ctype);
        }
      }
    }
  }
}

/**
 * Compare vocabularies.
 */
function drush_ctex_bonus_compare_vocabularies($module) {

  // Default ctypes.
  $vocabularies_function = $module . "_drush_ctex_bonus_vocabularies";
  $default_vocabularies = $vocabularies_function();

  // Database vocabularies.
  $may_delete = array();
  $vocabularies = taxonomy_get_vocabularies();
  foreach ($vocabularies as $vocabulary) {
    if (!isset($default_vocabularies[$vocabulary->machine_name])) {
      $may_delete[$vocabulary->vid] = $vocabulary->name;
    }
  }

  if (!empty($may_delete)) {
    $confirm = drush_confirm(dt("@count vocabularies found to delete, do you want to see them and optionally delete them one by one ?", array('@count' => count($may_delete))));
    if ($confirm) {
      foreach ($may_delete as $vid => $vocabulary) {
        $confirm = drush_confirm(dt('Do you want to delete vocabulary "@vocabulary"', array('@vocabulary' => $vocabulary)));
        if ($confirm) {
          taxonomy_vocabulary_delete($vid);
        }
      }
    }
  }
}

/**
 * Compare fields.
 */
function drush_ctex_bonus_compare_fields($module) {

  // The clue is to look at instances and call field_delete_instance() which
  // in turn will mark fields to be deleted in case all instances are gone.
  // We currently do not care about fields and instances of disabled modules.

  // Default instances.
  $instances_function = $module . "_drush_ctex_bonus_instances";
  $default_instances = $instances_function();

  // Site instances.
  $instances = field_info_instances();
  foreach ($instances as $entity_type => $bundles) {
    foreach ($bundles as $bundle_name => $fields) {
      foreach ($fields as $field_name => $info) {
        $identifier = $entity_type . ':' . $bundle_name . ':' . $field_name;
        if (!isset($default_instances[$identifier])) {
          $may_delete[] = array(
            'title' => $field_name .' ' . dt('on') . ' ' . $entity_type . ' / ' . $bundle_name,
            'instance' => $info,
          );
        }
      }
    }
  }

  if (!empty($may_delete)) {
    $confirm = drush_confirm(dt("@count instances found to delete, do you want to see them and optionally delete them one by one ?", array('@count' => count($may_delete))));
    if ($confirm) {
      foreach ($may_delete as $key => $info) {
        $confirm = drush_confirm(dt('Do you want to delete instance "@instance"', array('@instance' => $info['title'])));
        if ($confirm) {
          field_delete_instance($info['instance']);
        }
      }
    }
  }
}

/**
 * Compare nodequeues.
 */
function drush_ctex_bonus_compare_nodequeues($module) {

  // Default nodequeues.
  $nodequeues_function = $module . "_drush_ctex_bonus_nodequeues";
  $default_queues = $nodequeues_function();

  // Database nodequeues.
  $may_delete = array();
  $nodequeues = nodequeue_load_queues(nodequeue_get_all_qids());
  foreach ($nodequeues as $queue) {
    if (!isset($default_queues[$queue->name])) {
      $may_delete[$queue->qid] = $queue->title;
    }
  }

  if (!empty($may_delete)) {
    $confirm = drush_confirm(dt("@count nodequeues found to delete, do you want to see them and optionally delete them one by one ?", array('@count' => count($may_delete))));
    if ($confirm) {
      foreach ($may_delete as $qid => $title) {
        $confirm = drush_confirm(dt('Do you want to delete nodequeue "@nodequeue"', array('@nodequeue' => $title)));
        if ($confirm) {
          nodequeue_delete($qid);
        }
      }
    }
  }
}

/**
 * Compare languages.
 */
function drush_ctex_bonus_compare_languages($module) {

  // Default languages.
  $languages_function = $module . "_drush_ctex_bonus_languages";
  $default_languages = $languages_function();

  // Database nodequeues.
  $may_delete = array();
  $languages = language_list('native');
  foreach ($languages as $language) {
    if (!isset($default_languages[$language->language])) {
      $may_delete[$language->language] = $language->name;
    }
  }

  if (!empty($may_delete)) {
    $confirm = drush_confirm(dt("@count languages found to delete, do you want to see them and optionally delete them one by one ?", array('@count' => count($may_delete))));
    if ($confirm) {
      foreach ($may_delete as $language => $title) {
        $confirm = drush_confirm(dt('Do you want to delete language "@language"', array('@language' => $title)));
        if ($confirm) {
          $form = $form_state = array();
          module_load_include('inc', 'locale', 'locale.admin');
          $form_state['values']['langcode'] = $language;
          locale_languages_delete_form_submit($form, $form_state);
        }
      }
    }
  }
}

/**
 * Save all CTools, entity exportables an image styles to database.
 */
function drush_ctex_bonus_save_non_ctex_objects($module = '') {

  // Entity exportables.
  if (module_exists('entity')) {
    _drush_ctex_bonus_message(dt('Starting to save entity exportables.'), 'NEXT ACTION', FALSE);
    // Load the hooks.
    module_load_include('inc', 'entity', 'entity.features');
    $items = entity_features_api();

    foreach ($items as $entity_type => $info) {
      $hook = $module . '_' . $info['default_hook'];
      if (function_exists($hook)) {
        $items = $hook();
        $entity_info = entity_get_all_property_info($entity_type);
        foreach ($items as $name => $$entity) {
          $existing = entity_load($entity_type, array($name));
          if ($existing) {
            $entity->{$entity_info['status']['schema field']} = ENTITY_OVERRIDDEN;
            $entity->{$entity_info['id']['schema field']} = key($existing);
          }
          entity_save($entity_type, $entity);
          drush_log(dt('Saved "@name" for @entity_type.', array('@name' => $name, '@entity_type' => $info['name'])), 'success');
        }
      }
    }
  }

  // Image styles.
  if (module_exists('image')) {
    _drush_ctex_bonus_message(dt('Starting to save image styles exportables.'), 'NEXT ACTION', FALSE);

    $styles_function = $module . '_image_default_styles';

    if (function_exists($styles_function)) {
      // Get current and default image styles.
      $image_styles = image_styles();
      $default_styles = $styles_function();
      foreach ($default_styles as $style_name => $info) {
        if (isset($image_styles[$style_name])) {

          // Revert first to be absolute sure.
          image_default_style_revert($image_styles[$style_name]);

          // Now save again.
          $style = image_style_save($info);
          foreach ($info['effects'] as $effect) {
            $effect['isid'] = $style['isid'];
            image_effect_save($effect);
          }
          drush_log(dt('Saved style "@style."', array('@style' => $style_name)), 'success');
        }
      }
    }
  }

  // CTools variables.
  _drush_ctex_bonus_message(dt('Starting to save CTools exportables.'), 'NEXT ACTION');
  ctools_include('export');
  $schemas = ctools_export_get_schemas(TRUE);
  foreach ($schemas as $table_name => $schema) {
    $ctools_defaults = _ctools_export_get_defaults($table_name, $schema['export']);
    foreach ($ctools_defaults as $object_name => $object) {
      if (_ctools_drush_object_is_enabled($object) && isset($object->export_module) && $object->export_module == $module) {

        // Load the original to see if we have an existing object.
        // Remove first if necessary.
        $original_object = ctools_export_crud_load($table_name, $object_name);
        $primary_key = $schema['export']['primary key'];
        if (!empty($original_object->{$primary_key})) {
          ctools_export_crud_delete($table_name, $original_object);
        }
        ctools_export_crud_save($table_name, $object);
        drush_log(dt('Saved object "@object_name" of table "@table" to the database.', array('@object_name' => $object_name, '@table' => $table_name)), 'success');
      }
    }
  }

  // Clearing cache.
  _drush_ctex_bonus_message(dt('Clearing cache.'), 'NEXT ACTION');
  drush_invoke_process('@self', 'cc', array('all'));
}

/**
 * Helper function to log a message with separators.
 *
 * @param $message
 *   The message to print.
 * @param $important
 *   Whether to print the IMPORTANT line or not.
 * @param $footer
 *   Whether to print the footer line or not.
 * @param $error
 *  Whether to trigger an error message or not.
 */
function _drush_ctex_bonus_message($message = '', $important = FALSE, $footer = TRUE, $error = FALSE) {
  $type = $error ? 'error' : 'success';

  if ($important) {
    $header_message = is_string($important) ? $important : 'IMPORTANT';
    drush_log("------------------- " . $header_message . " -------------------", $type);
  }
  if (!empty($message)) {
    drush_log($message, $type);
  }
  if ($footer) {
    $add = '';
    $length = (!empty($header_message) ? strlen($header_message) : 0) - 9;
    if ($length > 0) {
      while ($length != 0) {
        $add .= '-';
        $length--;
      }
    }
    drush_log("-------------------------------------------------" . $add, $type);
  }
}

/**
 * Helper function to return an existing menu item link.
 */
function _drush_ctex_bonus_get_existing_menu_item($item) {
  $query = "SELECT * FROM {menu_links} WHERE link_path = :link_path AND menu_name = :menu_name";
  return db_query($query, array(':link_path' => $item['link_path'], ':menu_name' => $item['menu_name']))->fetchObject();
}

/**
 * Helper function for backups.
 *
 * @param $ask
 *   Whether to ask for confirmation first or not. In case $ask is
 *   false, a backup is take automatically.
 * @param $result_file
 *   The name of the result file.
 */
function _drush_ctex_bonus_backup($ask = TRUE, $result_file) {
  if ($ask) {
    $backup = drush_confirm(dt('Do you want to take a backup first before applying all changes ?'));
  }
  else {
    $backup = TRUE;
  }
  if ($backup) {
    drush_log(dt('Taking backup ...'), 'success');
    $result_file = empty($result_file) ? array('--result-file') : array('--result-file=' . $result_file);
    drush_invoke_process('@self', 'sql-dump', array(), $result_file);
  }
}

/**
 * Helper function to check the command and clear the cache if necessary first.
 *
 * @param $command
 *   A command defined in this module.
 */
function _drush_ctex_bonus_cache_check($command) {
  if (!empty($command['ctex clear cache'])) {
    drush_log(dt('Clearing cache before moving on to the next action.'), 'success');
    drush_invoke_process('@self', 'cc', array('all'));
  }
}

/**
 * Helper function to check dependencies before firing a command first.
 */
function _drush_ctex_bonus_dependencies_check($module) {
  $dcbd_info_file = drupal_get_path('module', $module) . '/' . $module . '.ctex_bonus.info.inc';
  if (file_exists($dcbd_info_file)) {
    include_once $dcbd_info_file;
    $function = $module . '_drush_ctex_bonus_info_dependencies';
    if (function_exists($function)) {
      drush_log(dt('Checking dependencies.'), 'success');
      $info_dependencies = $function();
      $to_enable = array();
      foreach ($info_dependencies as $dependency) {
        if (!module_exists($dependency)) {
          $to_enable[] = $dependency;
        }
      }
      if (!empty($to_enable)) {
        drush_log(dt('Enabling following modules first: @to_enable', array('@to_enable' => implode(', ', $to_enable))), 'success');
        module_enable($to_enable);
        drush_log(dt('Clearing cache'), 'success');
        drush_invoke_process('@self', 'cc', array('all'));
      }
    }
  }
}

/**
 * Helper function to write header of a file with its hook implementation.
 *
 * This is mainly used when the exports go in their separate file. It will
 * write the start of the PHP file and the beginning of the hook.
 *
 * @param $module
 *   The name of the module
 * @param $type
 *   The type of exports, used to created the hook
 * @param $drush_alias
 *   The drush alias
 * @param $start_file
 *   Whether this is the start of the file or not.
 *
 * @return $code
 *   A string of code.
 */
function _drush_ctex_bonus_write_header($module, $type, $drush_alias, $start_file = TRUE) {
  $code = '';

  if ($start_file) {
    $code = "<?php\n\n";
    $code .= "/**\n";
    $code .= " * @file\n";
    $code .= " * Bulk export of " . $type . " generated by Drush Ctools Export Bonus.\n";
    $code .= " * Use drush " . $drush_alias . " modulename to rebuild " . $type . ".\n";
    $code .= " */\n\n";
  }

  $code .= "/**\n";
  $code .= " * Implements hook_drush_ctex_bonus_" . $type . "().\n";
  $code .= " */\n";
  $code .= "function " . $module . "_drush_ctex_bonus_" . $type . "() {\n";
  $code .= "  \$$type = array();\n\n";

  return $code;
}

/**
 * Helper function to check if a module exists and include files if necessary.
 *
 * @param $module
 *   The name of the module.
 * @param $file
 *   The name of the file to include.
 * @param $check_enabled
 *   Whether to check for a not enabled module or just the info file.
 *
 * @return
 *   True if found, FALSE
 */
function _drush_ctex_bonus_check_module($module, $file = '', $check_enabled = TRUE) {
  if (empty($module)) {
    drush_log(dt('Please pass a module name.'), 'error');
    return FALSE;
  }

  if ($check_enabled) {
    if (!module_exists($module)) {
      drush_log(dt('The module "@module" does not exist or is not enabled.', array('@module' => $module)), 'error');
      return;
    }
  }
  else {
    $module_exists = drupal_get_path('module', $module) . '/' . $module . '.info';
    if (!$module_exists) {
      drush_log(dt('The module "@module" does not exist.', array('@module' => $module)), 'error');
      return;
    }
  }

  if ($file) {
    $file_to_include = drupal_get_path('module', $module) . '/' . $module . '.ctex_bonus.' . $file . '.inc';
    if (!file_exists($file_to_include)) {
      drush_log(dt('File not found: @file.', array('@file' => $file_to_include)), 'warning');
      return FALSE;
    }

    // Include the file.
    include_once $file_to_include;
  }

  return TRUE;
}

/**
 * Helper function to get the user roles untranslated and its permissions.
 */
function _drush_ctex_bonus_user_roles($builtin = TRUE) {
  $roles = array();
  $all_permissions = user_permission_get_modules();

  $result = db_query("SELECT rid, name, weight FROM {role} ORDER BY weight ASC, name ASC");
  foreach ($result as $row) {
    $permissions = user_role_permissions(array($row->rid => $row->rid));
    foreach ($permissions[$row->rid] as $permission => $value) {
      // Add this permission's module as a dependency. Make sure the module
      // is enabled as it's possible some permissions can still be left over
      // in the database.
      $module = isset($all_permissions[$permission]) ? $all_permissions[$permission] : '';
      if (!empty($module) && module_exists($module)) {
        drush_ctex_bonus_dependency($module);
      }
      else {
        unset($permissions[$row->rid][$permission]);
        continue;
      }
      // If this is a permission from a module that we don't want
      // to be enabled on a production site, remove it as well.
      if (in_array($module, drush_ctex_bonus_exclude_modules())) {
        unset($permissions[$row->rid][$permission]);
      }
    }
    $row->permissions = array_shift($permissions);
    unset($row->rid);
    $roles[$row->name] = $row;
  }

  return $roles;
}

/**
 * Helper function to remove unnecessary image style keys for export.
 */
function _drush_ctext_bonus_image_style_sanitize(&$style, $child = FALSE) {
  $omit = $child ? array('isid', 'ieid', 'storage') : array('isid', 'ieid', 'storage', 'module');
  if (is_array($style)) {
    foreach ($style as $k => $v) {
      if (in_array($k, $omit, TRUE)) {
        unset($style[$k]);
      }
      else if (is_array($v)) {
        _drush_ctext_bonus_image_style_sanitize($style[$k], TRUE);
      }
    }
  }
}

/**
 * Prepare a logs directory.
 */
function _drush_ctex_bonus_prepare_backup_dir($subdir = NULL) {
  $backup_dir = _drush_ctex_bonus_preflight_backup_dir($subdir);
  $backup_parent = dirname($backup_dir);

  $drupal_root = drush_get_context('DRUSH_DRUPAL_ROOT');
  $drupal_root .= '/';

  if ((!empty($drupal_root)) && (strpos($backup_parent, $drupal_root) === 0)) {
    return drush_set_error(dt('CTEX failed logging.'), dt('It\'s not allowed to store backups inside the Drupal root directory.'));
  }
  if (!file_exists($backup_parent)) {
    if (!drush_mkdir($backup_parent, TRUE)) {
      return drush_set_error(dt('CTEX failed logging.'), dt('Unable to create backup directory !dir.', array('!dir' => $backup_parent)));
    }
  }
  if (!is_writable($backup_parent)) {
    return drush_set_error(dt('CTEX failed logging.'), dt('Backup directory !dir is not writable.', array('!dir' => $backup_parent)));
  }

  if (!drush_mkdir($backup_dir, TRUE)) {
    return FALSE;
  }
  return $backup_dir;
}


/**
 * Decide where our logs directory is.
 *
 * @param string $subdir
 *   The name of the desired subdirectory(s) under ctex-logs.
 *   Usually a database name.
 */
function _drush_ctex_bonus_preflight_backup_dir($subdir = NULL) {
  $backup_dir = drush_get_context('DRUSH_CTEX_LOG_DIR', drush_get_option('ctex-log-location'));

  if (empty($backup_dir)) {

    // Try to use db name as subdir if none was provided.
    if (empty($subdir)) {
      $subdir = 'unknown';
      if ($creds = drush_get_context('DRUSH_DB_CREDENTIALS')) {
        $subdir = $creds['name'];
      }
    }

    $backup_dir = drush_get_option('ctex-log-location', drush_server_home() . '/' . 'drush-ctex-logs');
    $backup_dir = drush_trim_path($backup_dir) . '/' . $subdir;
    drush_set_context('DRUSH_CTEX_LOG_DIR', $backup_dir);
  }

  return $backup_dir;
}

// Add support for Drush 4.6-5.0 as the drush_trim_path does not exist in
// that codebase.
if (!function_exists('drush_trim_path')) {

/**
 * Remove the trailing DIRECTORY_SEPARATOR from a path.
 * Will actually remove either / or \ on Windows.
 */
function drush_trim_path($path, $os = NULL) {
  if (drush_is_windows($os)) {
    return rtrim($path, '/\\');
  }
  else {
    return rtrim($path, '/');
  }
}

}
