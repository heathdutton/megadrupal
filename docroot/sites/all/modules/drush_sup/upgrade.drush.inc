<?php

/**
 * @file
 *   Refine your Drupal major version upgrade.
 */

/**
 * Implement hook_drush_command().
 */
function upgrade_drush_command() {
  $items = array();

  $items['site-upgrade'] = array(
    'description' => 'Run a major version upgrade for Drupal (e.g. Drupal 6 to Drupal 7).  A copy of the site is made, and then upgraded; the source site is not changed.',
    'drupal dependencies' => array('update'),
    'drush dependencies' => array('sql', 'pm', 'core'),
    'core' => array(6), // Add 7 once drush supports 7 -> 8 upgrades.
    'arguments' => array(
      'target' => "The name of a sitealias, which points to the destination site. 'root' and 'uri' keys are required; db-url is recommended. See examples/aliases.drushrc.php for more information about creating a site alias.",
      'stage' => "Name (or partial name) of stage to start at",
    ),
    'examples' => array(
      'drush site-upgrade @onward' => 'Upgrade from the current site to the site specified by @onward alias.',
      'drush site-upgrade @onward --auto' => 'Skip most prompts, pausing only to allow core modifications (e.g. .htaccess and robots.txt) to be re-applied.',
      'drush site-upgrade @onward --auto --core-unmodified' => 'Skip prompts, including the check for core modifications.',
      'drush site-upgrade @onward --prompt-all' => 'Prompt about absolutely everything.',
    ),
    'options' => array(
      'create-db' => 'Create the target database, or overwrite/empty it first if it already exists',
      'prompt-all' => 'Usually, Drush site-upgrade will run all of the straightforward steps automatically, and only prompts at "important" steps.  --prompt-all forces prompts at every step.',
      'skip-optional' => 'Never prompt for optional steps; just skip them. This is the default; --skip-optional is only necessary with --prompt-all.',
      'auto' => 'Automatically select the most reasonable choice at each prompt, when one is available.',
      'source-dump' => 'Path to dump file. Medium or large sized sites should set this. Optional; default is to create a temporary file.',
      'db-su' => 'DB username to use when dropping and creating the target database. Optional.',
      'db-su-pw' => 'DB password to use when dropping and creating the target database. Optional.',
      'cache' => 'Use pm-download cache to speed up re-download from drupal.org.',
      'no-cache' => 'Transfer a fresh database from source site. Otherwise, DB dump is re-used for 24 hours. Negates --cache as well.',
      'core-only' => 'Stop after upgrading Drupal core; do not download and enable new versions of the site\'s modules.',
      'force-sites-default' => 'Forces settings.php to be written in sites/default folder, even if source settings.php is not.',
      'replace' => 'Replace target if it already exists.  Default is to prompt.',
      'reuse' => 'Reuse target if it already exists.  Default is to prompt.',
      'uninstall' => 'Comma-separated list of modules to uninstall in the target database prior to upgrade.  n.b. The source site is not affected.',
      'preferred' => 'Comma-separated list of alternative projects to prefer, when alternatives are available.',
      'postpone' => 'Comma-separated list of modules to ignore during automatic upgrade but leave database records intact for later manual upgrade.',
    ),
    'aliases' => array('sup'),
    'topics' => array('docs-aliases'),
  );
  $items['site-upgrade-prepare'] = array(
    'description' => 'Prior to running updatedb on Drupal core, disable all contrib modules and uninstall any module the user specified should be uninstalled.  Called automatically by site-upgrade.',
    'hidden' => TRUE,
    'arguments' => array(
      'modules' => 'The modules to disable.',
    ),
    'options' => array(
      'uninstall' => 'Comma-separated list of modules to uninstall prior to upgrade.',
    ),
  );
  $items['site-upgrade-progress'] = array(
    'description' => 'Show information about the upgrade status of the target site.',
    'options' => array(
      'pipe' => 'Print the full site upgrade status data structure.',
    ),
    'bootstrap' => 'DRUSH_BOOTSTRAP_DRUPAL_ROOT',
    'aliases' => array('upgrade-progress', 'supp'),
  );
  $items['docs-upgrading'] = array(
    'description' => 'Upgrading Drupal using the drush site-upgrade command.',
    'hidden' => TRUE,
    'topic' => TRUE,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'callback' => 'drush_print_file',
    'callback arguments' => array(dirname(__FILE__) . '/README.txt'),
  );
  // Add options defined by the fsm to the help text.
  $drupal_version = drush_drupal_major_version();
  $stages = _drush_upgrade_site_upgrade_stages($drupal_version ? $drupal_version + 1 : 7);
  foreach ($stages as $stage) {
    if (array_key_exists('options', $stage)) {
      foreach ($stage['options'] as $key => $option_info) {
        if (array_key_exists('description', $option_info)) {
          $items['site-upgrade']['options'][$key] = $option_info['description'];
        }
      }
    }
  }
  return $items;
}

/**
 * _drush_upgrade_site_upgrade_stages
 *
 * Load and return upgrade stages of the Finite State Machine (FSM)
 */
function _drush_upgrade_site_upgrade_stages($target_version) {
  require_once dirname(__FILE__) . '/upgrade.stages.inc';

  return _drush_upgrade_stages($target_version);
}

/**
 * Implement hook_drush_help().
 */
function upgrade_drush_help($section) {
  switch ($section) {
    case 'drush:site-upgrade':
      return dt("Execute a major version upgrade for Drupal core and enabled contrib modules. Command will download next version of Drupal and all available contrib modules that have releases. It prepares a settings.php for the target site, and copies the prior version's database to the target site. Finally, updatedb is run on core, and once for each contrib module as they are enabled. The intent is for developers to keep re-running this command until they are satisfied with the resulting site; the upgrade command may be aborted at any time and it will re-start where it left off when executed again. Run this command from within your source site (D6). Note that this command uses pm-download and sql-sync internally so most options for those commands are valid here too.");
  }
}

/**
 * Command argument complete callback.
 *
 * @return
 *  Array of available site aliases.
 */
function upgrade_site_upgrade_complete() {
  return array('values' => array_keys(_drush_sitealias_all_list()));
}

/**
 * Give a report on the progress of an upgrade
 */
function drush_upgrade_site_upgrade_progress($fsm_context = NULL) {
  if (!is_array($fsm_context)) {
    $fsm_context = _drush_upgrade_load_fsm_context(drush_get_context('DRUSH_SELECTED_DRUPAL_ROOT'));
  }
  if (!is_array($fsm_context) || (!array_key_exists('stage', $fsm_context))) {
    return drush_set_error('DRUSH_NO_UPGRADE_IN_PROGRESS', dt("There is no site upgrade in progress on the target site."));
  }
  if (drush_get_option('pipe')) {
    unset($fsm_context['extension_info']);
    unset($fsm_context['history']);
    drush_print_pipe(var_export($fsm_context, TRUE));
  }
  else {
    $source_key = array_key_exists('source_key', $fsm_context) ? $fsm_context['source_key'] : '';
    $target_key = $fsm_context['target_key'];
    drush_print(dt("Upgrade status for: drush !source site-upgrade !target\n", array('!source' => $source_key, '!target' => $target_key)));
    $result = drush_invoke_process($target_key, 'pm-list', array(), array('pipe' => TRUE, 'core' => TRUE, 'status' => 'enabled'), array('integrate' => FALSE));
    $core_modules = explode("\n", trim($result['output']));
    $result = drush_invoke_process($target_key, 'pm-list', array(), array('pipe' => TRUE, 'no-core' => TRUE, 'status' => 'enabled'), array('integrate' => FALSE));
    $upgraded_contrib_modules = explode("\n", trim($result['output']));
    drush_log(dt("The target alias is: !alias", array('!alias' => var_export($fsm_context['target_alias'], TRUE))));
    foreach ($fsm_context as $key => $value) {
      if (!is_array($value)) {
        if ($value === TRUE) {
          $value = "true";
        }
        if ($value === FALSE) {
          $value = "false";
        }
        drush_log(dt("!key: !value", array('!key' => $key, '!value' => $value)));
      }
    }
    if (array_key_exists('updatedb_messages', $fsm_context)) {
      $completed_with_no_messages = array();
      $completed_with_messages = array();
      foreach ($fsm_context['updatedb_messages'] as $module => $info) {
        if (empty($info)) {
          $completed_with_no_messages[] = $module;
        }
        else {
          $message = "";
          foreach ($info as $update_hook => $messages) {
            $message .= "$update_hook:\n\n" . implode("\n\n", $messages['status']) . "\n\n";
          }
          $completed_with_messages[$module] = rtrim($message);
        }
      }
      if (!empty($completed_with_messages)) {
        drush_print(dt("The following extensions had messages from updatedb:\n"));
        $rows = array();
        foreach ($completed_with_messages as $module => $message) {
          $rows[] = array($module, ':', $message);
        }
        drush_print_table($rows);
      }
      if (!empty($completed_with_no_messages)) {
        _drush_upgrade_site_show_message_with_data_table(dt("The following extensions were successfully upgraded without any messages from updatedb:"), $completed_with_no_messages);
      }
    }
    else {
      drush_print("Drupal core has not been upgraded yet.\n");
    }
    if (!empty($fsm_context['extensions_with_problems'])) {
      drush_print("The following contrib extesnions had problems: ");
      $rows = array();
      foreach ($fsm_context['extensions_with_problems'] as $extension => $info) {
        $message = $info['message'];
        $rows[] = array($extension, ':', $message);
      }
      drush_print_table($rows);
    }
    $rows = array();
    foreach ($fsm_context['special_projects']['module-remap'] as $old_module => $new_module) {
      if ($old_module != $new_module) {
        $rows[] = array($old_module, ':', $new_module);
      }
    }
    if (!empty($rows)) {
      drush_print(dt("The following extensions were replaced with new extensions:\n"));
      drush_print_table(array_merge(array(array('Old Module', '', 'New Module')), $rows), TRUE);
    }
    $contrib_to_upgrade = array_diff($fsm_context['contrib_to_upgrade'], array_keys($fsm_context['updatedb_messages']), array_keys($fsm_context['special_projects']['module-remap']), $core_modules);
    if (!empty($contrib_to_upgrade)) {
      _drush_upgrade_site_show_message_with_data_table(dt("The following contrib extesnions were not upgraded:"), $contrib_to_upgrade);
    }
    // Show any modules that the user may have added by hand.
    $new_upgraded = array_diff($upgraded_contrib_modules, $fsm_context['contrib_to_upgrade'], array_values($fsm_context['special_projects']['module-remap']));
    if (!empty($new_upgraded)) {
      _drush_upgrade_site_show_message_with_data_table(dt("The following extensions have been newly added to the upgrade site:"), $new_upgraded);
    }
    if ($fsm_context['stage'] != 'STOP') {
      drush_print("The current stage is " . $fsm_context['stage']);
    }
    if (array_key_exists('module_to_upgrade', $fsm_context)) {
      drush_print("The next module to upgrade is: " . $fsm_context['module_to_upgrade']);
    }
  }
  return $fsm_context;
}

function _drush_upgrade_site_show_message_with_data_table($message, $data, $columns = 3) {
  sort($data);
  drush_print($message . "\n");
  $rows = _drush_upgrade_site_arrange_in_columns($data, $columns);
  drush_print_table($rows);
}

function _drush_upgrade_site_arrange_in_columns($data, $columns = 3) {
  $empty_row = array_fill(0, $columns, '');
  $rows = array();
  $max_row = (count($data) + ($columns - 1)) / $columns;

  $row = 0;
  $col = 0;
  foreach($data as $item) {
    if (!array_key_exists($row, $rows)) {
      $rows[$row] = $empty_row;
    }
    $rows[$row][$col] = $item;
    ++$row;
    if ($row >= $max_row) {
      ++$col;
      $row = 0;
    }
  }
  return $rows;
}

/**
 * Do some sanity checks to make sure that we are ready to perform an upgrade, and
 * that the command is being called with reasonable-looking parameters.
 */
function drush_upgrade_site_upgrade_check_parameters($fsm_context) {
  if (!array_key_exists('target_key', $fsm_context)) {
    drush_print();
    return drush_set_error('DRUSH_UPGRADE_NO_TARGET', dt('No target argument specified.  To perform the site upgrade, define an alias record for the target site.'));
  }
  $target_key = $fsm_context['target_key'];
  if (!$target_alias = drush_sitealias_get_record($target_key)) {
    return drush_set_error('DRUSH_UPGRADE_NO_TARGET', dt('Site alias not found: @target-key. See example.drushrc.php.', array('@target-key' => $target_key)));
  }

  if (!file_exists(dirname($target_alias['root']))) {
    return drush_set_error('DRUSH_UPGRADE_NO_TARGET', dt('Parent directory for site alias root not found: @root; this folder must exist before running site-upgrade. See example.drushrc.php.', array('@root' => dirname($target_alias['root']))));
  }

  if (realpath($target_alias['root']) == realpath(DRUPAL_ROOT)) {
    return drush_set_error('DRUSH_UPGRADE_NO_TARGET', dt('Target site alias must have a different Drupal root directory than the source site.  Both are at @root.', array('@root' => $target_alias['root'])));
  }

  return TRUE;
}

function _drush_upgrade_load_fsm_context($destination_core) {
  $fsm_context = array();
  $cache_file = $destination_core. '/upgrade-context.serialized';
  if (file_exists($cache_file)) {
    $fsm_context = (array)unserialize(file_get_contents($cache_file));
  }
  $fsm_context['fsm_context_cache_file'] = $cache_file;
  return $fsm_context;
}

function drush_upgrade_site_upgrade_validate($target_key = NULL, $initial_stage = NULL) {
  if (DRUSH_MAJOR_VERSION < 4) {
    return drush_set_error('DRUSH_SITE_UPGRADE_REQUIRED_VERSION', dt("The site-upgrade command requires Drush version 5 or later."));
  }
  return TRUE;
}

/**
 * Main command hook for site-upgrade.
 *
 * This runs bootstrapped to the SOURCE site.
 */
function drush_upgrade_site_upgrade($target_key = NULL, $initial_stage = NULL) {
  $stage = '';
  // Set 'invoke' so that the commands we invoke do not complain about unknown options.
  drush_set_option('invoke', TRUE);

  // PREPARE:  Find the target version and determine the contrib projects and enabled modules installed.

  $fsm_context['source_version'] = drush_drupal_major_version();
  $fsm_context['target_version'] = $fsm_context['source_version'] + 1;
  $fsm_context['fsm_context_cache_file'] = FALSE;
  $fsm_context['uninstall_extensions'] = drush_get_option('uninstall', '');
  $fsm_context['postpone_extensions'] = drush_get_option('postpone', '');
  $fsm_context['core-only'] = drush_get_option('core-only', FALSE);
  $source_record = drush_sitealias_get_record('@self');
  $fsm_context['source_name'] = (!empty($source_record) ? $source_record['#name'] . '-' : '') . 'd' . $fsm_context['source_version'];
  $source_key = drush_get_context("DRUSH_TARGET_SITE_ALIAS");
  if (empty($source_key)) {
    $source_key = $source_record['#name'];
  }
  $fsm_context['source_key'] = $source_key;
  if (isset($target_key)) {
    $fsm_context['target_key'] = $target_key;
    $fsm_context['target_alias'] = drush_sitealias_get_record($target_key);
    if (!empty($fsm_context['target_alias'])) {
      $fsm_context['destination_core'] = $fsm_context['target_alias']['root'];
      if (!is_writable(dirname($fsm_context['destination_core']))) {
        return drush_set_error('DRUSH_SITE_UPGRADE_NOT_WRITABLE', dt("The parent of the target site root, !dir, must be writable, and is not.", array('!dir' => dirname($fsm_context['destination_core']))));
      }
      $fsm_context['target_name'] = (!empty($fsm_context['target_alias']) ? $fsm_context['target_alias']['#name'] . '-' : '') . 'd' . $fsm_context['target_version'];
      $fsm_context['work_name'] = $fsm_context['source_name'] . '-to-' . $fsm_context['target_name'];
      $fsm_context['work_dir'] = drush_directory_cache($fsm_context['work_name']);
      $fsm_context['project_cache'] = $fsm_context['work_dir'] . '/project_cache/disabled';
      $fsm_context['backup_dir'] = $fsm_context['work_dir'] . '/progress_backups';
      drush_mkdir($fsm_context['project_cache']);
      drush_mkdir($fsm_context['backup_dir']);

      $fsm_context['destination_conf_path'] = conf_path();
      if (drush_get_option('force-sites-default')) {
        $fsm_context['destination_conf_path'] = 'sites/default';
      }

      // Load the previous fsm context, if it exists.
      $previous_fsm_context = _drush_upgrade_load_fsm_context($fsm_context['destination_core']);
      $fsm_context['fsm_context_cache_file'] = $previous_fsm_context['fsm_context_cache_file'];
      // If the user explicitly specified a stage to start at, then force
      // a 'resume'.
      if (!empty($stage)) {
        $fsm_context = $previous_fsm_context;
      }
      // If a site already exists at the destination, ask the user what to do.
      elseif (is_dir($fsm_context['destination_core'])) {
        // TODO:  validate that $previous_fsm_context is compatible with $fsm_context
        // and the upgrade has not completed.
        if (array_key_exists('stage', $previous_fsm_context) && ($previous_fsm_context['stage'] != 'STOP')) {
          $options['resume'] = dt("Resume the upgrade at the stage it left off at: !stage", array('!stage' => $previous_fsm_context['stage']));
        }
        if (is_dir($fsm_context['destination_core'])) {
          $options['reuse'] = dt("Re-use the existing code: re-import the database from the source site and update core and contrib modules again.");
        }
        $options['replace'] = dt("Delete the existing target: start over from the beginning.");
        $backup_files = drush_scan_directory($fsm_context['backup_dir'], '/.*\.serialized$/', array('.', '..', 'CVS'));
        $backups = array();
        foreach ($backup_files as $backup_file => $info) {
          $backup_name = basename($backup_file, '.serialized');
          $backup_archive = dirname($backup_file) . '/' . $backup_name . '.tar';
          if (file_exists($backup_archive)) {
            $backup_context = (array)unserialize(file_get_contents($backup_file));
            $backups[$backup_name] = array(
              'archive' => $backup_archive,
              'context' => $backup_context
            );
            $options[$backup_name] = dt("Restore backup '!message' and resume upgrade", array('!message' => $backup_context['backup_message']));
          }
        }

        $selection = drush_choice($options, dt("A Drupal site exists at the destination. What would you like to do?"));
        // Selected a backup item?
        if (is_string($selection) && array_key_exists($selection, $backups)) {
          $archive = $backups[$selection]['archive'];
          $fsm_context = $backups[$selection]['context'];
          $stage = $fsm_context['stage'];
          drush_delete_dir($fsm_context['destination_core'], TRUE);
          $values = drush_invoke_process('@none', 'archive-restore', array($archive), array('destination' => $fsm_context['destination_core'], 'overwrite' => TRUE), array('integrate' => FALSE));
        }
        else {
          switch ($selection) {
            case 'replace':
              drush_delete_dir($fsm_context['destination_core'], TRUE);
              break;
            case 'resume':
              $fsm_context = $previous_fsm_context;
              $stage = $fsm_context['stage'];
              break;
            case 'reuse':
              // Move every project from the destination site's sites/all/modules and sites/all/themes directory
              // into the project cache.  TODO:  Maybe we should put the saved files somewhere else.
              foreach (array('sites/all/modules', 'sites/all/themes') as $dir) {
                $scan_dir = $fsm_context['destination_core'] . '/' . $dir;
                $projects = drush_scan_directory($scan_dir, '/./', array('.', '..', 'CVS'), 0, 0);
                foreach ($projects as $project_path => $info) {
                  $target_dir = $fsm_context['project_cache'] . '/' . basename($project_path);
                  if (is_dir($target_dir)) {
                    drush_delete_dir($target_dir, TRUE);
                  }
                  rename($project_path, $target_dir);
                }
              }
              break;
            default:
              return drush_user_abort();
          }
        }
      }
    }
  }
  // These flags always take on the value reflected by the cli options, regardless
  // of what might have been cached in the previous fsm context file.
  $fsm_context['auto'] = drush_get_option('auto', FALSE);
  $fsm_context['straightforward'] = (!drush_get_option('prompt-all')) | $fsm_context['auto'];
  $fsm_context['skip-optional'] = drush_get_option('skip-optional', $fsm_context['straightforward']);

  // Erase the 'download_attempts' variable on each run
  $fsm_context['download_attempts'] = array();

  $fsm_context += array(
    // TODO: we could probably determine the version of the database by inspectiion (e.g. has node_update_7000 run?)
    // However, we should probably also keep track of what version we think the target database file -should- be,
    // even if we use inspection on the current db, so we know if the active db is in sync with the stage of the
    // upgrade process we think we are in.
    'target_database_version' => 0, // No database exists -- until we copy it. Then we will set this to source_version. After updatedb, we set it to target_version.
    'extensions_with_problems' => array(),
  );
  foreach(explode(',',$fsm_context['postpone_extensions']) as $extension) {
    if (!empty($extension) && !array_key_exists($extension, $fsm_context['extensions_with_problems'])) {
      $fsm_context['extensions_with_problems'][$extension]['message'] = dt("Manually postponed.");
    }
  }

  // Collect information about the system before we begin.
  drush_upgrade_gather_information($fsm_context);
  if (empty($stage)) {
    $stage = 'START';
  }

  // TODO: Look up appropriate upgrade stages for the version of Drupal being upgraded.
  $stages = _drush_upgrade_site_upgrade_stages($fsm_context['target_version']);

  // Partial feature: start at some stage in the middle.
  // Warning: does not validate whether site is in any condition
  // to resume at this stage or not. Use at your own risk!
  // This will probably be removed in favor of enhancing the
  // above fsm context restore code to allow the user to select
  // any backup as a point to resume from.
  if ($initial_stage) {
    $pattern = preg_replace('/ +/', '.*', $initial_stage);
    $pattern = preg_replace('/([0-9]+[\.a-z0-9]*)/', '$1\b', $initial_stage);
    $pattern = "/.*(" . $pattern . ").*/";
    $alternatives = array();
    foreach ($stages as $name => $control) {
      if (preg_match($pattern, $name, $matches)) {
        $alternatives[$name] = $name;
      }
    }
    if (empty($alternatives)) {
      return drush_set_error('DRUSH_SITE_UPGRADE_STAGE_NOT_FOUND', dt("Could not find !stage", array('!stage' => $initial_stage)));
    }
    elseif (count($alternatives) == 1) {
      $stage = array_shift($alternatives);
    }
    else {
      $stage = drush_choice($alternatives, dt("Which step would you like to start on?"));
      if (!$stage) {
        return drush_user_abort();
      }
    }
  }

  // Start up the FSM and run the upgrade
  _drush_upgrade_site_state_machine($fsm_context, $stages, $stage);
}

function _drush_upgrade_rewrap_msg($msg) {
  $msg = preg_replace("/^[ \t]*/m", "", $msg);
  $msg = preg_replace("/\n\n+/s", "<br>", $msg);
  $msg = preg_replace("/[ \t\n]+/s", " ", $msg);
  $msg = preg_replace("/<br>/s", "\n\n", $msg);
  $msg = wordwrap($msg, drush_get_context('DRUSH_COLUMNS', 80) - 5);
  if (preg_match("/^ *([0-9-]+\. +)/", $msg, $matches)) {
    $msg = preg_replace("/^/m", str_pad('', strlen($matches[1])), $msg);
  }
  return trim($msg);
}

/**
 * Makes a replacements array for use with dt();
 * includes a !key'ed replacement for every item
 * in the fsm context that has a string value.
 */
function _drush_site_upgrade_replacements($fsm_context) {
  $replacements = array();
  foreach ($fsm_context as $key => $value) {
    if (is_string($value)) {
      $replacements['!' . $key] = $value;
    }
  }
  return $replacements;
}

function _drush_upgrade_site_state_machine(&$fsm_context, $stages, $initial_stage) {
  $stage = $initial_stage;
  $fsm_context += array(
    'auto' => FALSE,
    'straightforward' => FALSE,
    'skip-optional' => FALSE,
  );
  while ($stage != 'STOP') {
    $fsm_context['stage'] = $stage;
    $control = $stages[$stage];
    $control += array(
      'omit-stage-header' => FALSE,
      'no-op' => FALSE,
      'description' => '',
      'required' => '',
      'optional' => '',
      'not-always-needed' => '',
      'unnecessary' => '',
      'note' => '',
      'straightforward' => FALSE,
      'continue' => FALSE,
      'callback' => NULL,
      'backup' => FALSE,
      'drush-command' => FALSE,
      'verify' => NULL,
      'prepare' => NULL,
      'display' => NULL,
      'verify-message' => FALSE,
      'options' => array(),
      'prompt' => 'What would you like to do?',
      'selection-options' => array(),
      'next' => array(),
    );
    // TODO: call an 'alter' callback on the control structure (YNGNI?)
    if ($control['prepare']) {
      $control['prepare']($fsm_context, $control);
    }
    if (!is_array($control['next'])) {
      $control['next'] = array('ok' => $control['next']);
    }
    $control['next'] += array('error' => 'STOP');
    if (!array_key_exists('ok', $control['next'])) {
      // Find the next stage in the fsm
      $all_stages = array_keys($stages);
      $position = array_search($stage, $all_stages);
      if (isset($all_stages[$position + 1])) {
        $control['next']['ok'] = $all_stages[$position + 1];
      }
      else {
        $control['next']['ok'] = 'STOP';
      }
    }
    if (!$control['omit-stage-header']) {
      drush_print("\n" . str_pad("", drush_get_context('DRUSH_COLUMNS', 80) - 2, ':'). "\n" . $stage);
    }
    if (array_key_exists('module_to_upgrade', $fsm_context)) {
      drush_print(dt("Upgrading module: !module_to_upgrade", _drush_site_upgrade_replacements($fsm_context)));
    }
    foreach (array('description', 'display', 'optional', 'not-always-needed', 'unnecessary', 'note') as $message_key) {
      if (!empty($control[$message_key])) {
        if ($message_key == 'display') {
          $control[$message_key]($fsm_context, $control);
        }
        else {
          drush_print("\n" . _drush_upgrade_rewrap_msg($control[$message_key]));
        }
      }
    }
    $control['selection'] = '_run';
    $stage_result = TRUE;
    $prompt_user = TRUE;
    // If the 'verify' function returns TRUE, that indicates that the
    // step has already been completed.  In that instance, the stage
    // is skipped without prompting the user.
    if (($control['verify']) && ($control['verify']($fsm_context, $control) === TRUE)) {
      $control['selection'] = '#verified';
      $prompt_user = FALSE;
      $stage_result = TRUE;
      if ($control['verify-message']) {
        drush_print("\n" . $control['verify-message']);
      }
    }
    elseif ($control['continue']) {
      $control['selection'] = 'continue';
      $prompt_user = FALSE;
      $stage_result = TRUE;
    }
    if ($control['drush-command']) {
      $control['callback'] = '_drush_upgrade_site_drush_command';
    }
    $has_selection_options = !empty($control['selection-options']);
    if (($control['callback'] || $control['backup']) && $control['straightforward'] && $fsm_context['straightforward']) {
      $prompt_user = FALSE;
      drush_print();
      drush_print(dt("Drush will automatically do this step now."));
    }
    if (($control['optional'] || $control['unnecessary']) && $fsm_context['skip-optional']) {
      $control['selection'] = '#skip';
      $prompt_user = FALSE;
      drush_print();
      drush_print(dt("Skipping optional step."));
    }
    if ($prompt_user) {
      $auto = $fsm_context['auto'];
      if (!$control['callback']) {
        if (empty($control['unnecessary']) && !$control['no-op']) {
          drush_print(dt("\nDrush cannot do this step; you must perform it manually."));
          $control['selection-options'] += array(
            '#manual' => dt("I will do this step manually."),
          );
          $auto = FALSE;
        }
        elseif (!$has_selection_options) {
          $control['selection-options'] += array(
            '#skip' => dt("Continue."),
          );
        }
      }
      elseif (!empty($control['required'])) {
        $control['selection-options']['_run'] = $control['required'];
      }
      elseif (!$has_selection_options) {
        $control['selection-options'] += array(
          '_run' => dt("Do it for me."),
          '#manual' => dt("I will do this step manually."),
        );
      }
      if (!empty($control['optional']) || (!empty($control['not-always-needed'])) || !empty($control['unnecessary'])) {
        $control['selection-options'] += array(
          '#skip' => dt("Skip this step."),
        );
      }

      // In auto mode, we will always select the first option for the user.
      if (!empty($control['selection-options'])) {
        $user_selection = FALSE;
        foreach ($control['options'] as $option => $option_info) {
          if ($option_info === TRUE) {
            $option_info = array('list' => TRUE);
          }
          elseif (!is_array($option_info)) {
            $option_info = array('next' => $option_info);
          }
          if (drush_get_option($option, FALSE)) {
            if (array_key_exists('indirect', $option_info)) {
              $user_selection = drush_get_option($option);
            }
            elseif (array_key_exists('list', $option_info)) {
              $list = drush_get_option($option);
              if (!is_array($list)) {
                $list = explode(',', $list);
              }
              $list_item = array_shift($list);
              if (array_key_exists($list_item, $control['selection-options'])) {
                $user_selection = $list_item;
                drush_set_option($option, $list);
              }
              else {
                drush_log(dt('The item !item in !option is not an option; flag ignored', array('!item' => $list_item, '!option' => $option)), 'warning');
                drush_unset_option($option);
              }
            }
            elseif (array_key_exists('next', $option_info)) {
              $user_selection = $option_info['next'];
            }
          }
        }
        if (is_string($user_selection) && !array_key_exists($user_selection, $control['selection-options'])) {
          $user_selection = FALSE;
        }
        if ($auto || $user_selection) {
          if (is_string($user_selection) && array_key_exists($user_selection, $control['selection-options'])) {
            $control['selection'] = $user_selection;
          }
          else {
            $control['selection'] = key($control['selection-options']);
          }
          drush_print(dt("Drush will now do the step '!stage'.", array('!stage' => $control['selection-options'][$control['selection']])));
        }
        else {
          drush_print();
          $control['selection'] = drush_choice($control['selection-options'], $control['prompt']);
          if (!$control['selection']) {
            // If a stage is aborted, it is not recorded in the history.
            return drush_user_abort();
          }
        }
      }
    }
    // TODO: call a 'post selection' callback on the control structure (YNGNI?)
    // If the user's selection has the same key as one of the 'next' steps,
    // this overrides the callback item, and advances the FSM directly to
    // the specified stage.
    if (array_key_exists($control['selection'], $control['next'])) {
      $stage_result = $control['selection'];
    }
    elseif (($control['selection']{0} != '#') && $control['callback']) {
      $stage_result = $control['callback']($fsm_context, $control);
    }
    // Convert TRUE / FALSE results into 'ok' / 'error'.
    if ($stage_result === TRUE) {
      $stage_result = 'ok';
    }
    elseif ($stage_result === FALSE) {
      $stage_result = 'error';
    }
    // If a 'required' step reported success, and if there is
    // a verify function, then we will check again to make sure
    // that it is now passing.
    if (($stage_result == 'ok') && $control['verify'] && !empty($control['required'])) {
      if ($control['verify']($fsm_context, $control) === FALSE) {
        $control['post-verify-failure'] = TRUE;
      }
    }
    // Save a history of everything done in each stage.
    $control['stage_result'] = $stage_result;
    $fsm_context['history'][] = $control;
    // Exit if there was a verify failure
    if (array_key_exists('post-verify-failure', $control)) {
      drush_print();
      return drush_set_error('DRUSH_UPGRADE_REQUIREMENT_FAILURE', dt("Verify function did not pass for a required step; aborting."));
    }
    // Go on to the next stage of the FSM.
    if (array_key_exists($stage_result, $control['next'])) {
      $stage = $control['next'][$stage_result];
    }
    else {
      return drush_set_error('DRUSH_UPGRADE_STATE_MACHINE_ERROR', dt("Cannot continue; no next stage !stage.", array('!stage' => $stage_result)));
    }
    // Check to see if we should back up the target site after this stage.
    $backup_file = FALSE;
    if (($stage_result == 'ok') && $control['backup']) {
      $backup_message = dt($control['backup_message'], _drush_site_upgrade_replacements($fsm_context));
      $backup_file = $fsm_context['backup_dir'] . '/' . $control['backup'] . '.tar';
      drush_print(dt("Making backup: '!msg' in !file", array('!msg' => $backup_message, '!file' => $backup_file)));
      $fsm_context['backups'][$control['backup']] = array(
        'message' => $backup_message,
        'file' => $backup_file,
      );
      $fsm_context['backup_message'] = $backup_message;
      $saved_context_file = $fsm_context['backup_dir'] . '/' . $control['backup'] . '.serialized';
      file_put_contents($saved_context_file, serialize($fsm_context));
    }
    // Cache the $fsm_context so that we can restart the upgrade operation again later.
    // We want to make sure this is written before the backup is made, but after the
    // information about the backup is cached in the context.  We should also cache the
    // state even in stages where a backup is not made.  We do, however, avoid saving
    // the fsm state until the destination directory exists.
    if ($fsm_context['fsm_context_cache_file'] && is_dir(dirname($fsm_context['fsm_context_cache_file']))) {
      $fsm_context['stage'] = $stage;
      file_put_contents($fsm_context['fsm_context_cache_file'], serialize($fsm_context));
    }
    // If a backup was requested, make it now.
    if ($backup_file) {
      $values = drush_invoke_process($fsm_context['target_key'], 'archive-dump', array(), array('destination' => $backup_file, 'overwrite' => TRUE), array('integrate' => FALSE));
    }
  }
}

function _drush_upgrade_site_drush_command(&$fsm_context, $control) {
  $control += array(
    'drush-site' => '@self',
    'drush-arguments' => array(),
    'drush-options' => array(),
    'drush-backend-options' => array(),
  );
  if ($control['drush-site'] == '@upgrade-target') {
    $control['drush-site'] = $fsm_context['target_key'];
  }
  //$control['drush-options'] += array('debug' => TRUE);
  $result = drush_invoke_process($control['drush-site'], $control['drush-command'], $control['drush-arguments'], $control['drush-options'], $control['drush-backend-options']);
  return ($result['error_status'] == 0);
}

function _drush_upgrade_site_already_offline(&$fsm_context, $control) {
  $result = drush_invoke_process('@self', 'variable-get', array('maintenance_mode'), array(), array('integrate' => FALSE));
  $site_offline = is_array($result['object']) ? array_values($result['object']) : array();
  return (count($site_offline) > 0) && ($site_offline[0] == 1);
}

function _drush_upgrade_site_already_online(&$fsm_context, $control) {
  $result = drush_invoke_process($fsm_context['target_key'], 'variable-get', array('maintenance_mode'), array(), array('integrate' => FALSE));
  $site_offline = is_array($result['object']) ? array_values($result['object']) : array();
  return (count($site_offline) == 0) || ($site_offline[0] == 0);
}

function _drush_upgrade_update_free_access_already_false(&$fsm_context, $control) {
  $result = drush_invoke_process($fsm_context['target_key'], 'php-eval', array('var_export($GLOBALS["update_free_access"]);'), array(), array('integrate' => FALSE));
  return $result['output'] == 'false';
}

function _drush_upgrade_site_already_latest_version_drupal(&$fsm_context, $control) {
  $result = drush_invoke_process('@self', 'pm-releases', array('drupal'), array(), array('integrate' => FALSE));
  $installed = $result['object']['drupal']['installed'];
  $latest = array_shift(array_keys($result['object']['drupal']['releases']));
  drush_print();
  if ($installed == $latest) {
    drush_print(dt("Drupal core is up-to-date; version !installed installed.", array('!installed' => $installed)));
  }
  else {
    drush_print(dt("Drupal core is out of date; version !installed installed, latest verison is !latest.", array('!installed' => $installed, '!latest' => $latest)));
  }
  // TODO:  We should call `pm-updatecode --no` and see if it returns any out-of-date modules.
  return $installed == $latest;
}

function _drush_site_upgrade_reset_module_to_upgrade(&$fsm_context, &$control) {
  // Nothing selected yet; unset the module to upgrade from the previous iteration.
  unset($fsm_context['module_to_upgrade']);
}

function _drush_site_upgrade_select_contrib_module_to_upgrade(&$fsm_context, &$control) {
  // We will get a list of all of the enabled modules from the target site fresh every
  // time we get here, just in case something changed since last time.
  // TODO: maybe we could cache this info, and refresh it only if the user
  // stops and then resumes the site upgrade?
  $values = drush_invoke_process($fsm_context['target_key'], 'pm-list', array(), array('status' => 'enabled'), array('integrate' => FALSE));
  $target_enabled_modules = $values['object'];

  // We would like to download and enable the modules identified at
  // the beginning of the upgrade process.  However, we need to do
  // them in order, handling the ones that do not depend on any other
  // modules that have not been upgraded yet.  The issue is that
  // we can't tell what a module's dependencies are until we downloaded it.
  // Okay, let's go ahead and download everything upfront...
  $projects_to_download = array();
  $contrib_to_upgrade = array();
  $project_map = array();
  $label_map = array();
  foreach ($fsm_context['contrib_to_upgrade'] as $extension) {
    if (!array_key_exists($extension, $fsm_context['unavailable_extensions']) && !array_key_exists($extension, $contrib_to_upgrade)) {
      // There -must- be an entry for the extension in the extension_info
      // data, because $extension is the pre-upgrade name of the extension,
      // and the extension_info was built from the same source.
      if (array_key_exists($extension, $fsm_context['extension_info'])) {
        $project = $fsm_context['extension_info'][$extension]->info['project'];
      }
      else {
        $project = $extension;
        drush_log(dt("No project can be found for !extension", array('!extension' => $extension)), 'error');
      }
      // Remap the extension if its name changed
      if (array_key_exists($extension, $fsm_context['special_projects']['module-remap'])) {
        $orig_name = $extension;
        $extension = $fsm_context['special_projects']['module-remap'][$extension];
        $label = dt("!origname (now renamed to !newname)", array('!origname' => $orig_name, '!newname' => $extension));
        $label_map[$extension] = $label;
      }
      // Relocate the extension to another project if it moved
      if (array_key_exists($extension, $fsm_context['special_projects']['module-project-relocation'])) {
        $project = $fsm_context['special_projects']['module-project-relocation'][$extension];
      }
      // Remap the project if its name changed
      elseif (array_key_exists($extension, $fsm_context['special_projects']['project-remap'])) {
        $project = $fsm_context['special_projects']['project-remap'][$project];
      }

      $contrib_to_upgrade[] = $extension;
      $project_map[$extension] = $project;
    }
  }
  $fsm_context['project_map'] = $project_map;
  foreach ($contrib_to_upgrade as $extension) {
    $project = $project_map[$extension];
    if (!is_dir($fsm_context['project_cache'] . '/' . $project) && ($project != 'drupal')) {
      // In 'auto' mode, don't bother to attempt to download projects more than once.
      if ((!$fsm_context['auto']) || (!in_array($project, $fsm_context['download_attempts']))) {
        $projects_to_download[$project] = $project;
        $fsm_context['download_attempts'][] = $project;
      }
    }
  }
  // If we haven't already downloaded everything on a previous iteration, grab them all now.
  // It is a little lame to keep showing the user the warning messages about all of the projects
  // that STILL are not available for download, but if the upgrade process is protracted, and
  // additional modules become available, we want to catch them at some point.
  // TODO: Maybe we should at least suppress the warning message to eliminate noise.
  if (!empty($projects_to_download)) {
    $values = drush_invoke_process('@none', 'pm-download',
      array_keys($projects_to_download),
      array('destination' => $fsm_context['project_cache']),
      array('integrate' => FALSE));
  }
  // Make a list of extensions that still need to be upgraded
  $extensions_to_be_upgraded = array();
  $dependencies_map = array();
  foreach ($contrib_to_upgrade as $extension) {
    $project = $project_map[$extension];
    // Only consider extensions that are not already enabled
    if (!array_key_exists($extension, $target_enabled_modules)) {
      // Only include extensions that have code available (in the project cache)
      if (is_dir($fsm_context['project_cache'] . '/' . $project)) {
        // We could call drupal_parse_info_file directly, save for the fact that we are bootstrapped to the old version of Drupal, and would need to bootstrap the new site to use this. We will therefore use Drush's copy instead.
        $info = drush_site_upgrade_parse_info_file($fsm_context['project_cache'] . '/' . $project, $extension);
        if (!empty($info)) {
          $dependencies_map[$extension] = array_key_exists('dependencies', $info) ? $info['dependencies'] : array();
          $extensions_to_be_upgraded[$extension] = $extension;
        }
        else {
          drush_log(dt("No .info file could be found for !extension", array('!extension' => $extension)), 'warning');
        }
      }
    }
  }
  // If a given extension has any dependencies that are in the "to be upgraded" list,
  // then we will move it to the "not ready for upgrade" list.
  $extensions_ready_for_upgrade = array();
  $extensions_not_ready_for_upgrade = array();
  foreach ($extensions_to_be_upgraded as $extension) {
    $readyForUpgrade = TRUE;
    $hasDependencies = FALSE;
    $dependencies = $dependencies_map[$extension];
    foreach ($dependencies as $dependency) {
      $hasDependencies = TRUE;
      if (in_array($dependency, $extensions_to_be_upgraded)) {
        $readyForUpgrade = FALSE;
      }
    }
    if ($readyForUpgrade) {
      // If $hasDependencies is false, then put this on the head of the list; otherwise, put it at the end.
      // This insures that we will order extensions without dependencies ahead of extensions with dependencies)
      $key = $extension;
      $label = $extension;
      if (array_key_exists($extension, $label_map)) {
        $label = $label_map[$extension];
      }
      if ($hasDependencies) {
        $extensions_ready_for_upgrade[$key] = $label . " (Deps: " . implode(',', $dependencies) . ")";
      }
      else {
        $extensions_ready_for_upgrade = array_merge(array($key => $label), $extensions_ready_for_upgrade);
      }
    }
    else {
      $extensions_not_ready_for_upgrade[] = $extension . " (Deps: " . implode(',', $dependencies) . ")";
    }
  }
  // Filter out extensions that had problems on a previous pass
  $extensions_without_problems = array();
  $extensions_with_problems = array();
  foreach ($extensions_ready_for_upgrade as $extension => $label) {
    if (!array_key_exists($extension, $fsm_context['extensions_with_problems'])) {
      $extensions_without_problems[$extension] = $label;
    }
    else {
      $extensions_with_problems[$extension] = $label . ' [[' . $fsm_context['extensions_with_problems'][$extension]['message'] . ']]';
    }
  }
  // If there are no extensions without problems remaining (and ready for upgrade),
  // but there still are extensions with problems, then un-set 'auto' mode
  // so that the user will be prompted about what to do next.
  if (empty($extensions_without_problems) && !empty($extensions_with_problems)) {
    $fsm_context['auto'] = FALSE;
  }
  // We will allow the user to select from modules that have not failed AND also retry
  // modules that have failed, if desired, but we will put the problem extensions on the end.
  $extensions_ready_for_upgrade = array_merge($extensions_without_problems, $extensions_with_problems);
  // Add a 'note' about the extensions that need to be upgraded, but
  // are not yet ready.
  if (!empty($extensions_not_ready_for_upgrade)) {
    drush_print(dt("Note: in addition to the modules below, the following still need to be upgraded, but cannot be started until their dependencies below are upgraded: ") . implode(',', $extensions_not_ready_for_upgrade));
  }
  $control['selection-options'] = $extensions_ready_for_upgrade;
  if (empty($extensions_to_be_upgraded)) {
    $control['prompt'] = dt("There are no more modules to be upgraded.");
    $control['selection-options']['#done'] = dt("All done; finish upgrade");
  }
  else {
    $control['prompt'] = dt("Please select a module to upgrade, or some other action below:");
    if (!$fsm_context['auto']) {
      if (count($extensions_to_be_upgraded) > 1) {
        $control['selection-options']['_auto'] = dt("Automatically enable and upgrade all modules remaining in list");
      }
      if (array_key_exists('updatedb_messages', $fsm_context)) {
        $control['selection-options']['#progress'] = dt("Show upgrade progress so far");
      }
    }
    $control['selection-options']['#done'] = dt("Finish upgrade, leaving remaining modules un-upgraded");
  }
}

/**
 * Find the info file and parse it
 */
function drush_site_upgrade_parse_info_file($project_dir, $extension) {
  $files = drush_scan_directory($project_dir, '/^' . $extension . '\.info$/', array('.', '..', 'CVS'));
  if (empty($files)) {
    return array();
  }
  $info_file = array_shift(array_keys($files));
  // We could call drupal_parse_info_file directly, save for the fact that we are bootstrapped to the old version of Drupal, and would need to bootstrap the new site to use this. We will therefore use Drush's copy instead.
  $info = drush_drupal_parse_info_file($info_file);
  return $info;
}

function _drush_site_upgrade_prepare_module_upgrade(&$fsm_context, &$control) {
  if ($control['selection'] == '_auto') {
    $fsm_context['auto'] = TRUE;
    $fsm_context['auto_for_contrib_step_only'] = TRUE;
    $fsm_context['module_to_upgrade'] = array_shift(array_keys($control['selection-options']));
  }
  else {
    $fsm_context['module_to_upgrade'] = $control['selection'];
  }
  return TRUE;
}

function _drush_site_upgrade_show_module_upgrade_txt(&$fsm_context, &$control) {
  // TODO: the UPGRADE.txt for views is D7UPGRADE.txt
  $extension = $fsm_context['module_to_upgrade'];
  $project = $fsm_context['project_map'][$extension];
  $upgrade_txt = $fsm_context['project_cache'] . '/' . $project . '/UPGRADE.txt';
  if (is_file($upgrade_txt)) {
    $control['note'] = file_get_contents($upgrade_txt);
  }
  else {
    $control['note'] = dt("There is no UPGRADE.txt file in !extension", array("!extension" => $extension));
  }
  $control['prompt'] = dt("Please review the information above:");
  $control['selection-options']['_run'] = dt("Begin upgrade of module !module", array('!module' => $fsm_context['module_to_upgrade']));
  $control['selection-options']['#postpone'] = dt("Do not upgrade !module yet; select another one to do first", array('!module' => $fsm_context['module_to_upgrade']));
  return FALSE;
}

function _drush_site_upgrade_show_module_location(&$fsm_context, &$control) {
  $destination_core = $fsm_context['destination_core'];
  $extension = $fsm_context['module_to_upgrade'];
  $project = $fsm_context['project_map'][$extension];
  // TODO: Reconcile with code below
  $src = $fsm_context['project_cache'] . '/' . $project;
  $dest = $destination_core . '/sites/all/modules/' . $project;
  if (is_dir($src)) {
    $control['note'] .= "\n" . dt("Drush has a cached version of !module at !path.", array('!module' => $project, '!path' => $src));
  }
  $control['note'] .= "\n" . dt("The destination directory is !path", array('!path' => $dest));
}

function _drush_site_upgrade_verify_module_code_exists(&$fsm_context, &$control) {
  $destination_core = $fsm_context['destination_core'];
  $extension = $fsm_context['module_to_upgrade'];
  $project = $fsm_context['project_map'][$extension];
  $dest = $destination_core . '/sites/all/modules/' . $project;
  // TODO: We should validate that the directory at $dest is actually the right
  // version of the desired module (at least insure that the Drupal major verison matches).
  // For now, we will assume that the code in place is correct if it is there.
  // If the wrong thing is there, we should prompt the user and ask if it is
  // okay to delete it.
  return is_dir($dest);
}

function _drush_site_upgrade_get_module_code(&$fsm_context, &$control) {
  // TODO: if we haven't fetched this project already, then fetch it and mark it as already fetched.
  $destination_core = $fsm_context['destination_core'];
  $extension = $fsm_context['module_to_upgrade'];
  $project = $fsm_context['project_map'][$extension];
  // TODO: Kind of a bummer that we're going to need to reproduce logic in pm-download to
  // put modules and themes in the right location. Maybe there's a better way to do this?
  $src = $fsm_context['project_cache'] . '/' . $project;
  $dest = $destination_core . '/sites/all/modules/' . $project;
  // n.b. _drush_site_upgrade_verify_module_code_exists should insure that $dest is empty
  return drush_copy_dir($src, $dest, FALSE);
}

function _drush_site_upgrade_enable_module(&$fsm_context, &$control) {
  return _drush_site_upgrade_enable_specified_module($fsm_context, $control, $fsm_context['module_to_upgrade']);
}

function _drush_site_upgrade_enable_content_migrate(&$fsm_context, &$control) {
  return _drush_site_upgrade_enable_specified_module($fsm_context, $control, 'content_migrate');
}

function _drush_site_upgrade_enable_specified_module(&$fsm_context, &$control, $specified_module) {
  $result = drush_invoke_process($fsm_context['target_key'], 'pm-enable', array($specified_module), array(), array('integrate' => TRUE));
  $resultcode = TRUE;
  $module_was_enabled = is_array($result['object']) && array_key_exists($specified_module, $result['object']);
  // If we could successfully enable the module, clear it from the 'extensions with problems'
  // list, if it was included there.  If the module could not be enabled, then take it out of the list.
  // TODO: If the module was already enabled, that isn't really a problem. Double-check for that before signalling a failure
  if ($module_was_enabled) {
    unset($fsm_context['extensions_with_problems'][$specified_module]);
  }
  else {
    $fsm_context['extensions_with_problems'][$specified_module] = array('message' => dt("Error: failed to enable."));
    return FALSE;
  }
  return TRUE;
}

/**
 * TO DO: Remove once http://drupal.org/node/1698088 is committed.
 */
function _drush_site_upgrade_content_migrate_strip_html(&$item, $key) {
  $item = trim(drush_html_to_text($item));
}

/**
 * TO DO: Remove once http://drupal.org/node/1698088 is committed.
 */
function drush_site_upgrade_content_migrate_status() {
  module_load_include('inc', 'content_migrate', 'includes/content_migrate.admin');
  $fields_info = content_migrate_get_options();

  $rows = array();
  $rows[] = array(t('Status'), t('Field'), t('Field type'), t('Content type(s)'), t('Other information'));
  foreach ($fields_info as $status => $fields) {
    foreach ($fields as $field_name => $field) {
      array_walk($field, '_drush_site_upgrade_content_migrate_strip_html');
      array_unshift($field, t(ucfirst($status)));
      $rows[] = $field;
    }
  }
  drush_print_table($rows, TRUE);
}

function _drush_site_upgrade_show_migrations(&$fsm_context, &$control) {
  // TO DO: Go back to 'content-migrate-status' once http://drupal.org/node/1698088 is committed.
  $result = drush_invoke_process($fsm_context['target_key'], 'php-eval', array('drush_site_upgrade_content_migrate_status();'), array(), array('integrate' => TRUE));
  // $result = drush_invoke_process($fsm_context['target_key'], 'content-migrate-status', array(), array(), array('integrate' => TRUE));
  return TRUE;
}

function _drush_site_upgrade_migrate_fields(&$fsm_context, &$control) {
  $result = drush_invoke_process($fsm_context['target_key'], 'content-migrate-fields', array(), array(), array('integrate' => TRUE));
  return TRUE;
}

function _drush_site_upgrade_finish_up_contrib(&$fsm_context, $control) {
  if (array_key_exists('auto_for_contrib_step_only', $fsm_context)) {
    $fsm_context['auto'] = FALSE;
    unset($fsm_context['auto_for_contrib_step_only']);
  }
  return TRUE;
}

function _drush_site_upgrade_fix_permissions(&$fsm_context, $control) {
  // Make the 'files' directory writable to clear up one more potential problem
  // with the status report.
  $result = drush_invoke_process($fsm_context['target_key'], "drupal-directory", array("%files"), array(), array('integrate' => FALSE));
  $files_dir = $result['object'];
  drush_op('chmod', dirname($files_dir), 0777);
  drush_op('chmod', $files_dir, 0777);
  // We will also create the temp directory and make it writable
  $result = drush_invoke_process($fsm_context['target_key'], "drupal-directory", array("%temp"), array(), array('integrate' => FALSE));
  $temp_dir = $result['object'];
  drush_op('chmod', dirname($temp_dir), 0777);
  drush_op('drush_mkdir', $temp_dir);
  drush_op('chmod', $temp_dir, 0777);
  return TRUE;
}

function _drush_site_upgrade_run_cron(&$fsm_context, $control) {
  $result = (drush_invoke_process($fsm_context['target_key'], 'core-cron', array(), array(), array('interactive' => TRUE)) == 0);
  return $result;
}

function _drush_upgrade_site_download_next_version_drupal(&$fsm_context, $control) {
  $source_version = $fsm_context['source_version'];
  $target_version = $fsm_context['target_version'];
  $target_key = $fsm_context['target_key'];
  // Should be impossible to get here without a destination core
  if (array_key_exists('destination_core', $fsm_context)) {
    $destination_core = $fsm_context['destination_core'];
    $destination_conf_path = $fsm_context['destination_conf_path'];

    // Skip the download if there are already files at the destination core directory
    if (file_exists($destination_core . '/includes/bootstrap.inc')) {
      drush_print("Re-using existing Drupal core.");
    }
    else {
      drush_upgrade_download_drupal($target_version, $destination_core);
      if (drush_get_error()) return FALSE; // Early exit if we see an error.
    }
  }
  return TRUE;
}

function _drush_upgrade_site_make_settings_writable(&$fsm_context, $control) {
  if (array_key_exists('destination_core', $fsm_context) && array_key_exists('target_alias', $fsm_context)) {
    $destination_core = $fsm_context['destination_core'];
    $destination_conf_path = $fsm_context['destination_conf_path'];
    @chmod($destination_core . '/' . $destination_conf_path, 0777);
    $fsm_context['settings_destination'] = $destination_core . '/' . $destination_conf_path . '/settings.php';
    if (file_exists($fsm_context['settings_destination'])) {
      @chmod($fsm_context['settings_destination'], 0777);
    }
    return drush_upgrade_copy_settings($fsm_context['target_alias'], $fsm_context['settings_destination']);
  }
  return TRUE;
}

function _drush_upgrade_site_copy_database_and_disable_contrib(&$fsm_context, $control) {
  $target_key = $fsm_context['target_key'];
  $target_alias = $fsm_context['target_alias'];
  $destination_core = $fsm_context['destination_core'];
  $destination_conf_path = $fsm_context['destination_conf_path'];
  $fsm_context['settings_destination'] = $destination_core . '/' . $destination_conf_path . '/settings.php';
  // Copy source database to target database. The source DB is not changed.
  // Always set 'common' at minimum. Sites that want other can create other key in drushrc.php.
  if (!drush_get_option('structure-tables-key')) {
    drush_set_option('structure-tables-key', 'common');
  }
  // Empty or create the target database as needed.
  drush_sql_empty_db(drush_sitealias_get_db_spec($target_alias));
  // There are some options that would be destructive to pass through, so
  // we need to be selective here
  $command_options = array('yes' => TRUE);
  foreach (array('create-db', 'db-su', 'db-su-pw') as $option_name) {
    $option_value = drush_get_option($option_name, NULL);
    if (isset($option_value)) {
      $command_options[$option_name] = $option_value;
    }
  }
  drush_invoke_process('@self', 'sql-sync', array('@self', $target_key), $command_options, array('integrate' => TRUE));
  if (drush_get_error()) return FALSE; // Early exit if we see an error.
  // When we first copy the database, its version will be the same as the source site.
  // We will advance its version by one after we run updatedb.
  $fsm_context['target_database_version'] = $fsm_context['source_version'];

  if (!empty($fsm_context['contrib_extensions'])) {
    if (!_drush_upgrade_site_create_transition_site($fsm_context, $control)) {
      return FALSE;
    }
    $result = (drush_invoke_process($fsm_context['modify_site'], 'site-upgrade-prepare', $fsm_context['contrib_extensions'], array('uninstall' => implode(',', $fsm_context['uninstall_extensions']), 'yes' => TRUE), array('interactive' => TRUE)) == 0);
    if ($result === FALSE) return FALSE;
  }

  return TRUE;
}

function _drush_upgrade_site_verify_default_settings_removed(&$fsm_context, $control) {
  return !file_exists($fsm_context['destination_core'] . '/' . $fsm_context['destination_conf_path'] . '/default.settings.php');
}

function _drush_upgrade_site_remove_default_settings(&$fsm_context, $control) {
  drush_op('unlink', $fsm_context['destination_core'] . '/' . $fsm_context['destination_conf_path'] . '/default.settings.php');
  return TRUE;
}

function _drush_site_upgrade_remove_broken_module(&$fsm_context, $control) {
  $extension = $fsm_context['module_to_upgrade'];
  $fsm_context['contrib_to_upgrade'] = array_diff($fsm_context['contrib_to_upgrade'], array($extension));
  $fsm_context['removed_from_upgrade'][] = $extension;
  $fsm_context['removed_from_upgrade'] = array_unique($fsm_context['removed_from_upgrade']);
  return TRUE;
}

function _drush_site_upgrade_copy_files(&$fsm_context, $control) {
  $source_site = '@self';
  $target_site = $fsm_context['target_key'];
  $result = drush_invoke_process('@self', 'core-rsync', array($source_site . ':%files', $target_site . ':%files'), array('yes' => TRUE), array('integrate' => FALSE));
  return ($result['error_status'] == 0);
}

/**
 * The 'transition site' is a new multi-site entry created in the sites
 * folder of the *source* site that points at the database for the
 * *destination* site.  After the source database is copied to the target
 * database, it will still be in the format of the source site's version
 * of Drupal.  Drush will bootstrap the transition site in order to use
 * the source site's Drupal code to pm-disable and pm-uninstall modules
 * in the destination database without affecting those modules in the
 * source site's database.
 *
 * After updatedb is run for the first time on the target site's database,
 * the transition site will not be usable any longer, because the database
 * will have been converted to the format of the target site's version of
 * Drupal.  Once this happens, it won't be possible to bootstrap the target
 * database with the source site's Drupal code.  Therefore, it will not
 * be possible to pm-uninstall any modules after updatedb is ran.  If you
 * decide to do this, you'll need to go back, copy the source database
 * again, pm-uninstall modules as desired, and then re-run updatedb again.
 */
function _drush_upgrade_site_create_transition_site(&$fsm_context, $control) {
  $target_alias_databases = sitealias_get_databases_from_record($fsm_context['target_alias']);
  $fsm_context['modify_site_conf_path'] = NULL;

  // Make an alias record that uses the CODE from @self and the DATABASE from $target.
  // Since we just did an sql-sync from @self to @target, we can use this hybrid specification
  // to do manipulations on the target database before runing updatedb.  In brief, we are going
  // to disable all contrib modules to prevent problems with updatedb.
  $fsm_context['modify_site'] = array (
    'root' => DRUPAL_ROOT,
    'uri' => $target_alias_databases['default']['default']['database'],
  );

  if (!drush_get_context('DRUSH_SIMULATE')) {
    // In theory, if the sql-sync worked, this should never be empty.
    if (empty($fsm_context['modify_site']['uri'])) {
      return drush_set_error('DRUSH_UPGRADE_DATABASE_SPEC_UNKNOWN', dt('Failed to look up database spec for @target', array('@target' => $target_key)));
    }
    $fsm_context['modify_site_conf_path'] = dirname(conf_path()) . '/' . $fsm_context['modify_site']['uri'];
    $modify_site_settings = $fsm_context['modify_site_conf_path'] . '/settings.php';
    drush_log('Set up a transient site by copying ' . $fsm_context['settings_destination'] . ' to ' . $modify_site_settings, 'debug');
    if ((drush_mkdir($fsm_context['modify_site_conf_path'], TRUE) === FALSE) || drush_op('copy', $fsm_context['settings_destination'], $modify_site_settings) !== TRUE) {
      return drush_set_error('DRUSH_UPGRADE_COULD_NOT_DISABLE', dt("Could not create a temporary multisite "));
    }
  }
  // Debugging:
  // $result = (drush_invoke_process($fsm_context['modify_site'], 'sql-conf', array('integrate' => TRUE)) == 0);
  return TRUE;
}

/**
 * After updatedb is ran on the database, you might as well get rid of
 * the transition site.  @see _drush_upgrade_site_create_transition_site
 */
function _drush_upgrade_site_delete_transition_site(&$fsm_context, $control) {
  // Delete the temporary site now that we're done with it.
  if (isset($fsm_context['modify_site_conf_path'])) {
    drush_delete_dir($fsm_context['modify_site_conf_path']);
    unset($fsm_context['modify_site_conf_path']);
  }
}

function _drush_upgrade_site_updatedb(&$fsm_context, $control) {
  $module_to_upgrade = array_key_exists('module_to_upgrade', $fsm_context) ? $fsm_context['module_to_upgrade'] : 'core';
  $values = drush_invoke_process($fsm_context['target_key'], 'updatedb', array(), array('yes' => TRUE), array('integrate' => TRUE));
  // Find all of the updatedb status messages, and key them by the update
  // hook that produced them.
  $updatedb_status = array();
  $update_hook = '';
  foreach ($values['log'] as $logid => $record) {
    if (($record['type'] == 'notice') && preg_match("/Executing (.*)/", $record['message'], $matches)) {
      $update_hook = $matches[1];
    }
    if (!empty($update_hook) && (($record['type'] == 'status') || ($record['type'] == 'error'))) {
      $updatedb_status[$update_hook][$record['type']][$logid] = $record['message'];
    }
  }
  $fsm_context['updatedb_messages'][$module_to_upgrade] = $updatedb_status;
  // Return an error on failure
  if ($values['error_status']) {
    return drush_set_error('DRUSH_UPGRADE_UPDATEDB_FAILED', dt("updatedb failed for !module_to_upgrade", array('!module_to_upgrade' => $module_to_upgrade)));
  }
  // After updatedb runs successfuly, set the target database version
  $fsm_context['target_database_version'] = $fsm_context['target_version'];
  return TRUE;
}

function _drush_upgrade_site_backup_after_updatedb(&$fsm_context, $control) {
  return TRUE;
}

/**
 * Refresh information about enabled modules on every run, in case user changed
 * something.
 */
function drush_upgrade_gather_information(&$fsm_context) {
  $source_version = $fsm_context['source_version'];
  $target_version = $fsm_context['target_version'];

  drush_print();
  drush_print(dt("Gathering information about core and contrib modules..."));

  // Refresh the extension info every time until the database is
  // sync'ed to the target site. After that point, the extension info
  // is frozen, as changes to the source site will no longer affect the
  // target site.
  if (!array_key_exists('extension_info', $fsm_context) || ($fsm_context['target_database_version'] == 0)) {
    $fsm_context['extension_info'] = drush_get_extensions();

    // Get a list of enabled contrib extensions in source site.
    $values = drush_invoke_process('@self', 'pm-list', array(), array('status'=>'enabled','no-core'=>TRUE), array('integrate' => FALSE, 'override-simulated' => TRUE));
    if ($values['error_status'] != 0) return FALSE;
    $fsm_context['contrib_extensions'] = array_keys($values['object']);

    // Get a list of enabled core extensions in source site.
    $values = drush_invoke_process('@self', 'pm-list', array(), array('status'=>'enabled','core'=>TRUE), array('integrate' => FALSE, 'override-simulated' => TRUE));
    if ($values['error_status'] != 0) return FALSE;
    $fsm_context['core_extensions'] = array_keys($values['object']);
  }

  // Make a list of modules that are not available to be enabled in the target
  // version of Drupal, either because they have not been ported yet, or because
  // they have been rolled into core.
  $fsm_context['unavailable_extensions'] = array();

  // Get the list of modules the user would like to uninstall (if any).
  if ($fsm_context['uninstall_extensions'] == "all") {
    $fsm_context['uninstall_extensions'] = $fsm_context['contrib_extensions'];
  }
  elseif (!is_array($fsm_context['uninstall_extensions'])) {
    $fsm_context['uninstall_extensions'] = explode(',', $fsm_context['uninstall_extensions']);
  }
}

function drush_upgrade_generate_information_messages(&$fsm_context) {
  $fsm_context['project_download_list'] = array();
  $fsm_context['project_info_map'] = array();
  $fsm_context['contrib_to_upgrade'] = array();
  $fsm_context['special_warning'] = array();
  if (!empty($fsm_context['contrib_extensions']) && !$fsm_context['core-only']) {
    drupal_site_upgrade_build_special_project_list($fsm_context);

    // Make a list of all of the extensions to download.  We will
    // download everything in the contrib extension list, but we
    // will skip projects that already exist.
    $in_core = array();
    foreach ($fsm_context['contrib_extensions'] as $extension_name) {
      // Only check extensions that are NOT in our uninstall list.
      if (!in_array($extension_name, $fsm_context['uninstall_extensions']) && array_key_exists($extension_name, $fsm_context['extension_info'])) {
        if (empty($fsm_context['extension_info'][$extension_name]->info['project'])) {
          drush_log(dt('No project information could be found for the module !module. Enable git_deploy project if you are using Git clones as projects.', array('!module' => $extension_name)), 'warning');
        }
        else {
          $project = $fsm_context['extension_info'][$extension_name]->info['project'];
          $fsm_context['project_info_map'][$project]['name'] = $fsm_context['extension_info'][$extension_name]->info['name'];
          $fsm_context['project_info_map'][$project]['version'] = $fsm_context['extension_info'][$extension_name]->info['version'];
          // If the module has been rolled into core in the next major release of
          // Drupal, then we do not need to download it.  Add it to an array for
          // reporting purposes.
          if ((in_array($project, $fsm_context['special_projects']['projects-in-core'])) || (in_array($extension_name, $fsm_context['special_projects']['modules-in-core']))) {
            $in_core[$extension_name] = $extension_name;
            // Might some of these need to be enabled? No, I think we just skip the 'disable' step for this module, since it is now core
            $fsm_context['unavailable_extensions'][] = $extension_name;
          }
          else {
            $fsm_context['contrib_to_upgrade'][] = $extension_name;
            if ($fsm_context['extension_info'][$extension_name]->type == 'module') {
              $fsm_context['project_download_list'][$project][] = $extension_name;
            }
            // If there is a special warning about a project, then add it
            // to the warning list for later reporting.
            if (array_key_exists($project . ':' . $extension_name, $fsm_context['special_projects']['warning'])) {
              $fsm_context['special_warning'][$project][$extension_name] = $fsm_context['special_projects']['warning'][$project . ':' . $extension_name];
            }
            elseif (array_key_exists($project, $fsm_context['special_projects']['warning'])) {
              $fsm_context['special_warning'][$project][0] = $fsm_context['special_projects']['warning'][$project];
            }
          }
        }
      }
    }
  }
  // Add the projects from 'selected alternatives' to the end of the download list
  foreach ($fsm_context['selected_alternatives'] as $module => $info) {
    $alternate_project = $info['alternate-project'];
    $fsm_context['project_download_list'][$alternate_project][] = $module;
    // Look up the title of the project we're adding to the download list
    $v = drush_invoke_process("@none", "pm-releases", array($alternate_project), array(), array('integrate' => FALSE));
    $fsm_context['project_info_map'][$alternate_project]['name'] = $v['object'][$alternate_project]['title'];
    $fsm_context['project_info_map'][$alternate_project]['alternate-of'][] = $module;
  }
  return TRUE;
}

function drupal_site_upgrade_build_special_project_list(&$fsm_context) {
  // Make a list of all of the extensions to download.  We will
  // download everything in the contrib extension list, but we
  // will skip projects that already exist.
  $fsm_context['special_projects'] = drush_upgrade_project_map($fsm_context['target_version']);

  // Take all of the project alternatives and put them into another
  // map keyed by the original module name.
  $modules_with_alternatives = array();
  foreach ($fsm_context['special_projects']['project-alternatives'] as $alternative_project => $info) {
    foreach ($info['module-remap'] as $original_module => $alternate_module) {
      if (array_key_exists($original_module, $fsm_context['extension_info'])) {
        $modules_with_alternatives[$original_module][$alternative_project] = $info;
      }
    }
  }

  // Make a list of all modules in every project in a temporary array; we will remove items as modules are remapped
  $remaining_modules_in_original_project = array();
  foreach ($fsm_context['contrib_extensions'] as $original_module) {
    if (array_key_exists('project',$fsm_context['extension_info'][$original_module]->info )) {
      $original_project = $fsm_context['extension_info'][$original_module]->info['project'];
      $remaining_modules_in_original_project[$original_project][$original_module] = $original_module;
    }
  }
  // Get a list of preferred alternative projects from the user.
  $preferred = drush_get_option_list('preferred');

  // Reduce the modules with alternatives to a secondary list containing
  // only the selected alternative.  We assume that the first listed is
  // best if there is no explicitly-selected option.
  $selected_alternatives = array();
  foreach ($modules_with_alternatives as $original_module => $alternative_projects) {
    $best_alternate = FALSE;
    $best_weight = 0;
    foreach ($alternative_projects as $alternative_project => $info) {
      $weight = 1;
      if (in_array($alternative_project, $preferred)) {
        $weight += 10;
      }
      // TODO: maybe bump the weight of the project exists in the site, or exists in the cache?
      if ($weight > $best_weight) {
        $best_weight = $weight;
        $best_alternate = $alternative_project;
      }
    }
    // Store the selected alternative into the selected alternatives array
    // keyed by original module, and containing the following fields:
    //   'alternate-project'
    //   'description'
    //   'alternate-module'
    //   'other-alternates'
    $selected_alternatives[$original_module]['alternate-project'] = $best_alternate;
    $selected_alternatives[$original_module]['description'] = $alternative_projects[$best_alternate]['description'];
    $selected_alternatives[$original_module]['alternate-module'] = $alternative_projects[$best_alternate]['module-remap'][$original_module];
    $selected_alternatives[$original_module]['other-alternatives'] = array_diff(array_keys($alternative_projects), array($best_alternate));

    // Remove remapped modules, so that we can determine when all modules from a given project have replacements
    $original_project = $fsm_context['extension_info'][$original_module]->info['project'];
    unset($remaining_modules_in_original_project[$original_project][$original_module]);
  }
  $fsm_context['selected_alternatives'] = $selected_alternatives;

  // Fill in the module remap, project remap and module project relocation based on
  // the selected alternative modules.
  foreach ($selected_alternatives as $original_module => $info) {
    $original_project = $fsm_context['extension_info'][$original_module]->info['project'];
    $fsm_context['special_projects']['module-remap'][$original_module] = $info['alternate-module'];
    $fsm_context['special_projects']['module-project-relocation'][$info['alternate-module']] = $info['alternate-project'];
    // project-remap should only be used if all modules in the project move. If just one is relocated, we should not remap the whole project.
    if (empty($remaining_modules_in_original_project[$original_project])) {
      $fsm_context['special_projects']['project-remap'][$original_project] = $info['alternate-project'];
    }
    if (!empty($info['other-alternatives'])) {
      $alternate_name = $info['alternate-project'];
      if ($info['alternate-project'] != $info['alternate-module']) {
        $alternate_name .= ':' . $info['alternate-module'];
      }
      $fsm_context['special_warning'][$original_project][$original_module] = dt("The module !module in this project has no D!version version available, but there are multiple alternative projects that may be used instead. Drush picked !alternate, but !other also available.  Run again with --preferred to select a different preference.",
        array('!project' => $original_project, '!module' => $original_module, '!version' => $fsm_context['target_version'], '!alternate' => $alternate_name, '!other' => implode(',', $info['other-alternatives'])));
    }
  }
}

function drush_upgrade_check_modules_and_themes(&$fsm_context, $control) {
  // Presume we are ready to go
  $ready_to_upgrade = TRUE;
  $source_version = $fsm_context['source_version'];
  $target_version = $fsm_context['target_version'];
  if (array_key_exists('destination_core', $fsm_context)) {
    $destination_core = $fsm_context['destination_core'];
    $destination_conf_path = $fsm_context['destination_conf_path'];
  }
  if (empty($fsm_context['contrib_extensions'])) {
    drush_print();
    drush_print(dt("There are no enabled contrib extensions; Drush will upgrade Drupal core."));
  }
  elseif ($fsm_context['core-only']) {
    drush_print();
    drush_print(dt("--core-only option specified; Drush will upgrade Drupal core only."));
  }
  else {
    drush_upgrade_generate_information_messages($fsm_context);
    $rows[] = array(dt('Project'), dt('Title'), dt('Installed'), dt('Available'), dt('Status'));
    // Consider each project from the contrib extensions and check with PM to see if there is
    // a recommended release available to download.  If there is NO release available, then
    // we are not ready to upgrade (but still can, without that project); if there is no RECOMMENDED
    // release, then we might not be ready to upgrade (but still can, with a non-recommended release).
    if (!empty($fsm_context['project_download_list'])) {
      $remapped_project_download_list = array();
      foreach ($fsm_context['project_download_list'] as $project => $extension_list) {
        $project_to_download = $project;
        // Check our lookup table to see if a project has been renamed.
        if (array_key_exists($project, $fsm_context['special_projects']['project-remap'])) {
          $project_to_download = $fsm_context['special_projects']['project-remap'][$project];
        }
        $remapped_project_download_list[] = $project_to_download;
      }
      $result = drush_invoke_process('@none', 'pm-releases', $remapped_project_download_list, array('default-major' => $target_version, 'strict' => 0), array('integrate' => FALSE, 'override-simulated' => TRUE, 'log' => FALSE));
      $project_releases = $result['object'];
      if (!is_array($project_releases)) {
        return drush_set_error('DRUSH_SITE_UPGRADE_NO_RELEASE_INFO', dt("Could not get release information via drush pm-releases."));
      }
      // n.b. $extension_list contains the _original_ module names; for example, in a d6-to-d7 upgrade, we might find 'redirect' => array('globalredirect', 'path_redirect'), since both globalredirect and path_redirect remap to the D7 'redirect' module.
      $project_upgrade_information = array();
      foreach ($fsm_context['project_download_list'] as $project => $extension_list) {
        $project_to_download = $project;
        $alternate_message = '';
        // Check our lookup table to see if a project has been renamed.
        if (array_key_exists($project, $fsm_context['special_projects']['project-remap'])) {
          $alternate_message = '; using ' . $fsm_context['special_projects']['project-remap'][$project];
          $fsm_context['special_warning'][$project][0] = dt("!project: this project has been replaced by the project !newproject.  The new project will be downloaded automatically.",
            array('!project' => $project, '!newproject' => $fsm_context['special_projects']['project-remap'][$project]));
        }

        if (!array_key_exists($project_to_download, $project_releases) || empty($project_releases[$project_to_download]['releases'])) {
          $fsm_context['unavailable_extensions'] = array_merge($fsm_context['unavailable_extensions'], $extension_list);
          $ready_to_upgrade = FALSE;
          $available_version = '-';
          $status = dt("NO !major_version RELEASES", array('!major_version' => $target_version . ".x"));
        }
        else {
          $all_versions = array_keys($project_releases[$project_to_download]['releases']);
          $available_version = $all_versions[0];
          if (empty($project_releases[$project_to_download]['recommended'])) {
            $ready_to_upgrade = 'maybe';
            $status = implode(',', $project_releases[$project_to_download]['releases'][$available_version]['release_status']);
          }
          else {
            $status = dt('Recommended');
          }
        }
        $title = $fsm_context['project_info_map'][$project]['name'];
        $installed_version = array_key_exists('version', $fsm_context['project_info_map'][$project]) ? $fsm_context['project_info_map'][$project]['version'] : '(' . implode(',', $fsm_context['project_info_map'][$project]['alternate-of']) . ')';
        $project_upgrade_information[$project] = array('title' => $title, 'installed' => $installed_version, 'available' => $available_version, 'status' => $status . $alternate_message);
      }
      foreach ($project_upgrade_information as $project => $info) {
        $rows[] = array($project, $info['title'], $info['installed'], $info['available'], $info['status']);
      }
    }
    drush_print_table($rows);

    if (!empty($in_core) || !empty($fsm_context['special_warning'])) {
      drush_print(dt("Special notes about some of the projects you are using:\n"));
    }
    // Print out some messages about projects that migrated  to core, or modules that will require special processing.
    if (!empty($in_core)) {
      drush_print(dt("The following contrib modules were enabled in your Drupal site, but are now standard in core:\n\n    !in_core.\n\nThese modules may need to be reconfigured after the upgrade is complete.\n", array('!in_core' => implode(', ', $in_core))), 'ok');
    }
    $rows = array();
    foreach ($fsm_context['special_warning'] as $project => $warnings) {
      foreach ($warnings as $module => $warning) {
        if ($warning === TRUE) {
          $warning = '!special  Please see !project_page and !source for more information on how to do this.';
        }
        if ($warning === FALSE) {
          $warning = '!special  So far there is no indication of when a migration path will be provided.  Check !project_page for updates.';
          $ready_to_upgrade = 'maybe';
        }
        $special = dt("This project requires data migration or other special processing.");
        $rows[] = array($project, ':', dt("$warning", array('!special' => $special, '!project' => $project, '!project_page' => 'http://drupal.org/project/' . $project, '!source' => $fsm_context['special_projects']['source'])));
      }
    }
    if (!empty($rows)) {
      drush_print_table($rows);
    }
  }
  if (drush_upgrade_site_upgrade_check_parameters($fsm_context) === FALSE) {
    return FALSE;
  }

  // Summarize whether or not there is a good chance that the site can be upgraded.
  if ($ready_to_upgrade !== TRUE) {
    drush_print();
    drush_log(dt("Based on the contrib modules enabled in this site, it is possible that the site-upgrade command might fail.  See warnings above."), (($ready_to_upgrade === FALSE) ? 'warning' : 'notice'));
  }

  return TRUE;
}

/**
 * http://drupal.org/node/895314 lists projects that are now in
 * core; it also has a list of projects that require special handling.
 * Keep a record here too.
 *
 * @param $target_version
 * The version of Drupal being upgraded to.
 * @return @array
 *   - source              URL to the page where more information about this upgrade can be found.
 *   - target-version      The version of Drupal being upgraded to.
 *   - projects-in-core    An array containing projects that were once contrib that are now in core.
 *   - modules-in-core     An array containing modules that were once in contrib that are now in core.
 *                         Use 'modules-in-core' in place of 'projects-in-core' only if there is a
 *                         project where only some of its subcomponents were moved to core.
 *   - warning             An array of warning messages to display to the user related to this upgrade.
 *                         The key should be the name of the project that the warning applies to.
 *                         use 'project:module' as the key if the warning only applies when the given
 *                         module in the project is enabled; otherwise, the warning will be shown whenever
 *                         any module in the specific project is enabled.  The value can either be the
 *                         warning string to display, or TRUE to tell the user to see the project page
 *                         for information on the migration path, or FALSE to tell the user that there
 *                         currently is no migration path for the given project.
 */
function drush_upgrade_project_map($target_version) {
  require_once dirname(__FILE__) . '/upgrade.mappings_' . $target_version . '.inc';

  $result = drush_upgrade_mappings();
  $result += array(
    'source' => '',
    'target-version' => $target_version,
    'projects-in-core' => array(),
    'modules-in-core' => array(),
    'warning' => array(),
  );

  drush_command_invoke_all_ref('drush_upgrade_project_map_alter', $result);

  return $result;
}

/**
 * Prepare to upgrade; the first step is to disable all contrib modules.
 */
function drush_upgrade_site_upgrade_prepare() {
  $contrib_extensions = func_get_args();
  $uninstall_extensions = explode(',', drush_get_option('uninstall', ''));

  // Set theme back to garland per Upgrade.txt.
  variable_set('theme_default', 'garland');

  // http://drupal.org/node/724102 recommends using "seven" as your admin theme.  Don't switch it to garland if it is already seven.
  $admin_theme = variable_get('admin_theme', NULL);
  if ($admin_theme != "seven") {
    variable_set('admin_theme', 'garland');
  }
  else {
    drush_log(dt("Admin theme is already set to 'seven'."), 'ok');
  }

  // Disable all contrib modules per Upgrade.txt.
  drush_log(dt("Disabling !list", array('!list' => implode(", ", $contrib_extensions))), 'ok');
  drush_invoke('pm-disable', $contrib_extensions);
  if (drush_get_error()) return FALSE; // Early exit if we see an error.

  // Uninstall any modules specified via the --uninstall flag.
  if (!empty($uninstall_extensions)) {
    drush_log(dt("Uninstalling !list", array('!list' => implode(", ", $uninstall_extensions))), 'ok');
    drush_invoke('pm-uninstall', $uninstall_extensions);
    if (drush_get_error()) return FALSE; // Early exit if we see an error.
  }
}

/**
 * Download the upgraded version of Drupal for site-upgrade.
 */
function drush_upgrade_download_drupal($target_version, $destination_core) {
  if (drush_get_context('DRUSH_SIMULATE')) {
    drush_log(dt("Simulated download of drupal-!version", array('!version' => $target_version)));
  }
  else {
    // Fetch target core and place as per target alias root.
    $dl_options['destination'] = dirname($destination_core);
    $dl_options['drupal-project-rename'] = basename($destination_core);

    // No need for version control in this command.
    $dl_options['version-control'] = 'backup';
    $dl_options['yes'] = TRUE;
    $values = drush_invoke_process('@none', 'pm-download', array('drupal-' . $target_version), $dl_options, array('integrate' => FALSE));

    // Check and see if there is a Drupal site at the target.
    if (!file_exists($destination_core . '/includes/bootstrap.inc')) {
      return drush_set_error('DRUSH_UPGRADE_NO_DRUPAL', dt('Drupal could not be downloaded to the target directory, @root.  Move existing content out of the way first.', array('@root' => $destination_core)));
    }
  }
  return TRUE;
}

/**
 * Copy the settings.php file from the source site to the target site,
 * and fix it up so that it will have its own database settings.
 */
function drush_upgrade_copy_settings(&$target_alias, $settings_destination) {
  $settings_destination_folder = dirname($settings_destination);

  // Create sites subdirectory in target if needed.
  $settings_source = conf_path() . '/settings.php';
  if (!file_exists($settings_destination_folder)) {
    if (!drush_op('mkdir', $settings_destination_folder) && !drush_get_context('DRUSH_SIMULATE')) {
      return drush_set_error('DRUSH_UPGRADE_MKDIR_FAILED', dt('Failed to create directory @settings_destination', array('@settings_destination' => $settings_destination_folder)));
    }
  }

  // Copy settings.php to target.
  if (!drush_op('copy', $settings_source, $settings_destination) && !drush_get_context('DRUSH_SIMULATE')) {
    return drush_set_error('DRUSH_UPGRADE_COPY_FAILED', dt('Failed to copy @source to  @dest', array('@source' => $settings_source, 'dest' => $settings_destination)));
  }

  // Append new $db_url with new DB name in target's settings.php.
  return drush_upgrade_fix_db_url($target_alias, $settings_destination);
}

/**
 * Replace db_url with DB name from target. Write a $databases record to settings.php.
 */
function drush_upgrade_fix_db_url(&$target_alias, $settings_destination) {
  $old_url = $GLOBALS['db_url'];
  if (is_array($old_url)) {
    $old_url = $old_url['default'];
  }
  $old_databases = empty($GLOBALS['databases']) ? drush_sitealias_convert_db_from_db_url($old_url) : $GLOBALS['databases'];
  $target_alias_databases = sitealias_get_databases_from_record($target_alias);
  $database_name = isset($target_alias_databases) ? $target_alias_databases['default']['default']['database'] : '';
  if (empty($database_name)) {
    $database_name = preg_replace("/[^a-zA-Z0-9\s]/", "", $target_alias['#name']) . "db";
    drush_log(dt("No database name specified; defaulting to !dbname", array("!dbname" => $database_name)), 'notice');
  }

  $append = "\n# Added by drush site-upgrade.";
  if (drush_drupal_major_version() <= 6) {
    $databases = drush_sitealias_convert_db_from_db_url($old_url);
    $databases['default']['default']['database'] = $database_name;
    // Handle db prefix
    if (isset($GLOBALS['db_prefix'])) {
      $append .= "\n\$db_prefix = '" . $GLOBALS['db_prefix'] . "';";
      $databases['default']['default']['prefix'] = $GLOBALS['db_prefix'];
    }
    // Update credentials
    if (isset($target_alias_databases)) {
      $databases['default']['default']['username'] = $target_alias_databases['default']['default']['username'];
      $databases['default']['default']['password'] = $target_alias_databases['default']['default']['password'];
    }
    $new_url = drush_upgrade_database_record_to_db_url($databases['default']['default']);
    $append .= "\n# New db_url:\n\$db_url = '$new_url';";
    // The mysqli driver is no longer supported in Drupal 7 and later.  See http://drupal.org/node/475138
    if ($databases['default']['default']['driver'] == 'mysqli') {
      $databases['default']['default']['driver'] = 'mysql';
    }
  }
  else {
    $databases = $GLOBALS['databases'];
    $databases['default']['default']['database'] = $database_name;
    if (isset($$target_alias_databases)) {
      $databases['default']['default']['username'] = $target_alias_databases['default']['default']['username'];
      $databases['default']['default']['password'] = $target_alias_databases['default']['default']['password'];
    }
  }
  $append .= "\n# New databases array:\n" . '$databases = ' . var_export($databases, TRUE) . ';';
  // Caching the database record in the alias record allows sql-sync to work
  // before updatedb is called. updatedb is what converts from a db_url to a
  // DBTNG array; this conversion is required by sql-sync.
  drush_sitealias_cache_db_settings($target_alias, $databases);

  // Also append the new configuration options to the end of settings.php.
  return drush_op('file_put_contents', $settings_destination, $append, FILE_APPEND) !== FALSE;
}

function drush_upgrade_database_record_to_db_url($database_record) {
  return $database_record['driver'] .
    '://' .
    $database_record['username'] .
    ':' .
    $database_record['password'] .
    '@' .
    $database_record['host'] .
    '/' .
    $database_record['database'];
}
