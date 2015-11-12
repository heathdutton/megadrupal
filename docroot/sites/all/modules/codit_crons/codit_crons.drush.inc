<?php
/**
 * @file
 * Drush commands for Codit: Crons.
 */


/**
 * Implements hook_drush_command().
 */
function codit_crons_drush_command() {
  $items = array();

  // The 'add cron' command.
  $items['codit-crons-add'] = array(
    'description' => "Creates a new cron job directory and files in codit_local/crons/ and registers the cron job",
    'arguments' => array(
      'cron-name' => 'The name of the cron job.  Make it unique, meaningful and 32 char max.',
    ),
    'options' => array(
      'enabled' => array(
        'description' => 'If the cron is enabled, it is immediately ready possible execution on the next cron, else it will create the directory with a leading _  making it disabled.',
        'example-value' => 'FALSE',
        'value' => 'optional',
      ),
    ),
    'examples' => array(
      'drush codit-crons-add monthly_email' => 'Creates a cron job named "monthly_email" and enables it.',
      'drush codit-crons-add monthly_email --enabled=TRUE' => 'Creates a cron job named "monthly_email" and enables it.',
      'drush codit-crons-add monthly_email --enabled=FALSE' => 'Creates a cron job named "monthly_email" but disables it with an _.',
    ),
    'scope' => 'site',
    'drupal dependencies' => array('codit', 'codit_local'),
    'aliases' => array('codit-cron-add', 'codit-crons-add'),
  );

  $items['codit-crons-enable'] = array(
    'description' => "Enables an existing disabled Codit Crons cron job by removing the underscore from in front of the cron job directory and flushes the cache to register it.",
    'arguments' => array(
      'cron-name' => 'The name of the cron job.',
    ),
    'examples' => array(
      'drush codit-crons-enable my_cron_name' => 'Enables a cron event named "my_block_name".',
    ),
    'scope' => 'site',
    'drupal dependencies' => array('codit', 'codit_local'),
    'aliases' => array('codit-cron-enable', 'codit-crons-enable'),
  );

  $items['codit-crons-disable'] = array(
    'description' => "Disables an existing enabled Codit Cron job by adding an underscore from in front of the cron directory and flushes the cache to remove it.",
    'arguments' => array(
      'cron-name' => 'The name of the cron job.',
    ),
    'examples' => array(
      'drush codit-crons-disable my_cron_name' => 'Disables a cron job named "my_cron_name".',
    ),
    'scope' => 'site',
    'drupal dependencies' => array('codit', 'codit_local'),
    'aliases' => array('codit-cron-disable', 'codit-crons-disable'),
  );

  $items['codit-crons-cache-clear-list'] = array(
    'description' => "Clears the existing Codit: Crons list cache to register new cron job changes.  Unneccessary if using Drush to add, enable or disable a cron job.",

    'examples' => array(
      'drush codit-crons-cache-clear-list' => 'Clears the Codit: Crons list cache to register or de-register a cron job.',
    ),
    'scope' => 'site',
    'drupal dependencies' => array('codit', 'codit_local'),
    'aliases' => array(
      'codit-crons-cache-clear-list',
      'codit-cron-cache-clear-list',
      'codit-crons-ccl',
      'codit-cron-ccl',
    ),
  );

  $items['codit-crons-list'] = array(
    'description' => "Lists all of the currently enabled cron jobs by Codit.",
    'examples' => array(
      'drush codit-crons-list' => 'Outputs a list of all enabled Codit cron jobs to terminal.',
    ),
    'scope' => 'site',
    'drupal dependencies' => array('codit', 'codit_local'),
    'aliases' => array(
      'codit-cron-list',
      'codit-crons-list',
    ),
  );

  $items['codit-crons-local-init'] = array(
    'description' => "Creates the local storage within Codit: Local if it does not already exist.",
    'examples' => array(
      'drush codit-crons-init' => 'Creates the local storage within Codit: Local if it does not already exist.',
    ),
    'scope' => 'site',
    'drupal dependencies' => array('codit', 'codit_local'),
    'aliases' => array(
      'codit-crons-init',
      'codit-cron-init',
    ),
  );

  return $items;
}


/**
 * Implements hook_drush_help().
 */
function codit_crons_drush_help($section) {
  switch ($section) {
    case 'drush:codit-crons-list':
      return dt("Lists all enabled Codit: Crons crons jobs.");

    case 'drush:codit-crons-local-init':
      return dt("Creates the local storage /crons within Codit: Local.");

    case 'drush:codit-crons-cache-clear-list':
      return dt("This clears the Codit: Crons cache, to update available cron jobs.");

    case 'drush:codit-crons-enable':
      return dt("This enables a disabled cron job by removing an _ from in front of the directory.");

    case 'drush:codit-crons-disable':
      return dt("This disables an enabled cron job by placing an _ in front of the directory.");

    case 'drush:codit-crons-add':
      return dt("This command will create the directory and callback file in codit_local/crons/");

    // The 'title' meta is used to name a group of commands in `drush help`.
    case 'meta:codit_crons:title':
      return dt("Commands for making custom cron jobs with Codit: Crons.");

    // The 'summary' meta item is displayed in `drush help --filter`.
    case 'meta:codit_crons:summary':
      return dt("Assists in making custom cron jobs for this site.");
  }
}


/**
 * Implements drush_hook_COMMAND_validate().
 */
function codit_crons_validate() {
  if (drush_is_windows()) {
    // $name = drush_get_username();
    // TODO: implement check for elevated process using w32api
    // as sudo is not available for Windows
    // http://php.net/manual/en/book.w32api.php
  }
  else {
    $name = posix_getpwuid(posix_geteuid());
    if ($name['name'] !== 'root') {
      return drush_set_error('CODIT_CRONS', dt('There was an error processing your cron due to permissions.'));
    }
  }
}


/**
 * Drush command callback to create a cron job.
 *
 * @param string $cron_name
 *   The name of the cron job to be created.
 * @param bool $enabled
 *   Option passed to the cron.
 */
function drush_codit_crons_add($cron_name = '', $enabled = TRUE) {
  module_load_include('inc', 'codit_crons', 'codit_crons');
  $o_cron_job = new stdClass();
  // Any error, messages, or success set will be output.
  $o_cron_job->error = array();
  $o_cron_job->msg = array();
  $o_cron_job->success = array();
  // All systems are go unless one or more of the checks says otherwise.
  $o_cron_job->build = TRUE;
  $o_cron_job->name = trim($cron_name);
  $o_cron_job->length = strlen($o_cron_job->name);

  // Check to see if the local environment is present.
  $codit_local_dir = drupal_get_path('module', 'codit_local') . '/crons/';
  if (!file_exists($codit_local_dir)) {
    drush_codit_crons_local_init();
  }

  // Start the case race.  First one to trigger TRUE aborts the block creation.
  switch (TRUE) {
    // Check that we have a cronname.
    case empty($o_cron_job->name):
      $o_cron_job->error[] = dt("You must include a cron name in your request, example: drush codit-crons-add my_new_cron_job");
      break;

    // Check that length is not longer than 32 chars.
    case $o_cron_job->length > 32:
      $o_cron_job->error[] = dt("The cron name '!cronname' is !length chars, but the limit is 32 chars.  Please shorten it.", array('!cronname' => $cron_name, '!length' => $o_cron_job->length));
      break;

    // Check that it contains no hyphens.
    case strpos($o_cron_job->name, '-') !== FALSE:
      $o_cron_job->error[] = dt("The cron name '!cronname' contains hyphens and can not.  Please use underscores.", array('!cronname' => $cron_name));
      break;

    // Check that it contains no spaces.
    case strpos($o_cron_job->name, ' ') !== FALSE:
      $o_cron_job->error[] = dt("The cron name '!cronname' contains spaces and can not.  Please use underscores.", array('!cronname' => $cron_name));
      break;

    // Check to see if the cron job already exists as an enabled cron job.
    case codit_crons_dir_exists($o_cron_job):
      if ($o_cron_job->enabled) {
        // The cron job exists and is enabled.
        $o_cron_job->error[] = dt("The cron job name '!cronname' already exists as an enabled cron job.  Remember, cron job names must be unique.", array('!cronname' => $o_cron_job->name));
      }
      else {
        // The cron job exists and is disabled.
        $o_cron_job->error[] = dt("The cron job name '!cronname' already exists as a disabled cron job.  Remember, cron job names must be unique.", array('!cronname' => $o_cron_job->name));
      }
      break;

    default:
      $o_cron_job->msg[] = dt("The cron job name '!cronname' has met the criteria to be a cron job and is being built.", array('!cronname' => $cron_name));
      break;

  }

  if (!empty($o_cron_job->error) || !empty($o_cron_job->errorFlag)) {
    // There was an error so set the flag.
    $o_cron_job->errorFlag = TRUE;
  }

  if (empty($o_cron_job->errorFlag)) {
    // Passed all checks, proceed to build the block.

    // Made it this far, carry on and make the new cron job directory.
    _codit_crons_create_new_cron($o_cron_job);

    // Check to see if the enable was already set by another check.
    if (!isset($o_cron_job->enable_it)) {
      // This was not previously set by the name of the block, so read options.
      $o_cron_job->enable_it = codit_sanitize_boolean(drush_get_option('enabled', TRUE), 'boolean', TRUE);
    }

    // The block is already enabled, process the disable if there is one.
    if (!$o_cron_job->enable_it) {
      // Disable the cron job.
      drush_codit_crons_disable($o_cron_job->name);
    }
    else {
      // The cron job was built and by default is enabled.
      $o_cron_job->enabled = TRUE;
    }

    if (empty($o_cron_job->errorFlag)) {
      // No errors, and it made it this far.  Assume it was built.
      if ($o_cron_job->enabled) {
        // This cron job is enabled.
        $o_cron_job->success[] = dt("The cron job name '!cronname' was built and should ready to run on the next cron.", array('!cronname' => $o_cron_job->name));
      }
      else {
        // The cron job is disabled.
        $o_cron_job->success[] = dt("The cron job name '!cronname' was built, but is currently disabled.  Use 'drush codit-crons-enable !cronname'", array('!cronname' => $o_cron_job->name));
      }
      // Clear the Codit Crons registered cron cache to pick up an changes.
      cache_clear_all('codit_crons_jobs_list', 'cache');
    }
  }
  else {
    // Cron job build failed so issue error and do nothing else.
    $o_cron_job->error[] = "This cron job was not built.";
  }
  codit_drush_process_messages($o_cron_job);
}


/**
 * Checks to see if the cron aciton exists and reads/saves enabled property.
 *
 * @param object $o_cron_job
 *   The cron job object that is being built.
 *
 * @return bool
 *   TRUE if the cron job exists, FALSE otherwise.
 */
function codit_crons_dir_exists(&$o_cron_job) {
  // Adjust name in case requested disabled cron.
  $trimmed = ltrim($o_cron_job->name, '_');
  if ($o_cron_job->name != $trimmed) {
    // Had an initial underscore so make it disabled.
    $o_cron_job->name = $trimmed;
    $o_cron_job->enable_it = FALSE;
  }

  // Is this cron job present?

  // Verify cron job storage in codit_local.
  if (codit_path_to_local('crons')) {
    // Codit: Local crons storage is available.
    $o_cron_job->location = codit_path_to_local('crons') . '/' . $o_cron_job->name;
    $o_cron_job->location_disabled = codit_path_to_local('crons') . '/_' . $o_cron_job->name;

    if (file_exists($o_cron_job->location)) {
      // Yes the cron job exists as enabled.
      $o_cron_job->exists = TRUE;
      $o_cron_job->enabled = TRUE;
    }
    elseif (file_exists($o_cron_job->location_disabled)) {
      // The cron job exists but is disabled.
      $o_cron_job->exists = TRUE;
      $o_cron_job->enabled = FALSE;
    }
    else {
      // The cron job does not exist at all.
      $o_cron_job->exists = FALSE;
      $o_cron_job->enabled = FALSE;
    }

    return $o_cron_job->exists;
  }
  else {
    // Storage is not available.  Throw an error.
    $o_cron_job->error[] = dt('Storage in Codit: Local does not seem to be available.  Is the module Codit: Local enabled?');
    return FALSE;
  }
}

/**
 * Creates an enabled cron job from boilerplate files.
 *
 * @param object $o_cron_job
 *   The cron job object with block details.
 */
function _codit_crons_create_new_cron(&$o_cron_job) {
  // Check for required properties to build the cron job.
  // Must not exist and have no errors set.
  if ((empty($o_cron_job->exists)) && (empty($o_cron_job->error))) {
    // Criteria met, so create from boilerplate.
    $boilerplate_dir = drupal_get_path('module', 'codit_crons') . '/boilerplate/crons/_a_sample_cron_job';
    // Make the directory in the enabled state.
    $o_cron_job->location = codit_path_to_local('crons') . $o_cron_job->name;
    // Copy and rename the entire boilerplate.
    $o_cron_job->built = drush_copy_dir($boilerplate_dir, $o_cron_job->location);

    if (empty($o_cron_job->built)) {
      // The cron job was not built.
      $o_cron_job->error[] = dt("The cron job name '!cronname' could not be created because they already exist at !location.", array('!cronname' => $o_cron_job->name, '!location' => $o_cron_job->location));
    }
    else {
      // The cron job was built.
      $o_cron_job->msg[] = dt("The cron job files for '!cronname' were created at !location.", array('!cronname' => $o_cron_job->name, '!location' => $o_cron_job->location));

    }
  }
}

/**
 * Enables an existing disabled Codit:cron job.
 *
 * @param string $cron_name
 *   The cron job name to enable.
 *
 * @return bool
 *   Return True if the cron job is enabled.
 */
function drush_codit_crons_enable($cron_name = '') {
  // Try to find the cron.
  if (!empty($cron_name)) {
    $o_cron_job = new stdClass();
    $o_cron_job->name = $cron_name;
    if (codit_crons_dir_exists($o_cron_job)) {
      // The cron job exists, so check its state.
      if ($o_cron_job->enabled) {
        // The cron job is already enabled.
        $o_cron_job->msg[] = dt("The cron job '!cronname' is enabled.", array('!cronname' => $o_cron_job->name));
        $return = TRUE;
      }
      else {
        // The cron job exists but is disabled, so enable it by changing the
        // directory name to not start with an underscore.
        $enabled = drush_move_dir($o_cron_job->location_disabled, $o_cron_job->location);
        if ($enabled) {
          // Means the enable was successful.
          $return = TRUE;
          $o_cron_job->enabled = TRUE;
          $o_cron_job->success[] = dt("The cron job '!cronname' has been enabled.", array('!cronname' => $o_cron_job->name));
          // Clear the Codit Crons registered cron cache to pick up changes.
          cache_clear_all('codit_crons_jobs_list', 'cache');
        }
        else {
          // For some reason the move failed.
          $return = FALSE;
          $o_cron_job->enabled = FALSE;
          $o_cron_job->err[] = dt("The cron job '!cronname' failed to enable. Perhaps a file permission issue?", array('!cronname' => $o_cron_job->name));
        }
      }
    }
    else {
      // This cron job does not exist.
      $return = FALSE;
      $o_cron_job->err[] = dt("The cron job '!cronname' Does not exist.", array('!cronname' => $o_cron_job->name));
      $o_cron_job->msg[] = dt("Suggest using 'drush codit-crons-add !cronname' to create this cron job.", array('!cronname' => $o_cron_job->name));
    }
  }
  else {
    // No cron job name specified.
    $return = TRUE;
    $o_cron_job->err[] = dt("You must specify a cron job name to enable.  Example: 'drush codit-crons-enable my_cron_job'");
  }

  codit_drush_process_messages($o_cron_job);

  return $return;
}


/**
 * Disables an existing Codit:cron job.
 *
 * @param string $cron_name
 *   The cron job name to disable.
 *
 * @return bool
 *   Return True if the cron job is disabled.
 */
function drush_codit_crons_disable($cron_name = '') {
  // Try to find the corn job.
  if (!empty($cron_name)) {
    $o_cron_job = new stdClass();
    $o_cron_job->name = $cron_name;
    if (codit_crons_dir_exists($o_cron_job)) {
      // The cron job exists, so check its state.
      if (!$o_cron_job->enabled) {
        // The cron aciton is already disabled.
        $o_cron_job->msg[] = dt("The cron job '!cronname' is disabled.", array('!cronname' => $o_cron_job->name));
        $o_cron_job->enabled = FALSE;
        $return = TRUE;
      }
      else {
        // The cron job exists but is enabled, so disable it by changing the
        // directory name to start with an underscore.
        $o_cron_job->enabled = !drush_move_dir($o_cron_job->location, $o_cron_job->location_disabled);
        if (!$o_cron_job->enabled) {
          // Means the disable was successful.
          $return = TRUE;
          $o_cron_job->success[] = dt("The cron job '!cronname' has been disabled.", array('!cronname' => $o_cron_job->name));
          // Clear the Codit Crons registered cron cache to pick up changes.
          cache_clear_all('codit_crons_jobs_list', 'cache');
        }
        else {
          // For some reason the move failed.
          $return = FALSE;
          $o_cron_job->err[] = dt("The cron job '!cronname' failed to disable. Perhaps a file permission issue?", array('!cronname' => $o_cron_job->name));
        }
      }
    }
    else {
      // This cron job does not exist.
      $return = FALSE;
      $o_cron_job->err[] = dt("The cron aciton '!cronname' Does not exist.", array('!cronname' => $o_cron_job->name));
      $o_cron_job->msg[] = dt("Suggest using 'drush codit-crons-add !cronname --enable=false' to create a disabled cron job.", array('!cronname' => $o_cron_job->name));
    }
  }
  else {
    // No cron job name specified.
    $return = TRUE;
    $o_cron_job->err[] = dt("You must specify a cron job name to enable.  Example: 'drush codit-crons-enable my_cron-job'");
  }

  codit_drush_process_messages($o_cron_job);

  return $return;
}

/**
 * Clears the Codit:Crons list cache and re-registers the enabled cron jobs.
 */
function drush_codit_crons_cache_clear_list() {
  module_load_include('inc', 'codit_crons', 'codit_crons');
  // Clear the Codit crons registered block cache to pick up any changes.
  cache_clear_all('codit_crons_jobs_list', 'cache');
  $count = count(codit_crons_get_cron_jobs());
  drush_log("Codit: Crons cache has been cleared and $count cron jobs re-registered.", 'success');
}


/**
 * Adds a cache clear option for Codit: Crons.
 */
function codit_crons_drush_cache_clear(&$types) {
  $types['codit_crons cron job list'] = 'drush_codit_crons_cache_clear_list';
}

/**
 * Lists all the cron acitons currently enabled by Codit Crons.
 */
function drush_codit_crons_list() {
  drush_codit_crons_cache_clear_list();
  // Get the list of registered blocks from cache.
  $cron_jobs = codit_crons_get_cron_jobs();
  if (!empty($cron_jobs)) {
    drush_print(dt('The following cron jobs are enabled by Codit Crons: '));
    // Load from the db all the cron timings.
    $result = db_query('SELECT c.cron_name, c.ran_date FROM {codit_crons} c');
    $jobs_ran = $result->fetchAllAssoc('cron_name', PDO::FETCH_ASSOC);
    foreach (is_array($cron_jobs) ? array_keys($cron_jobs) : array() as $cron_job) {
      // Has this run before?
      if (!empty($jobs_ran[$cron_job])) {
        // This has run so figure out when.
        $unix_time = $jobs_ran[$cron_job]['ran_date'];
        $date = dt('ran ') . date('m/d/Y j:i a', $unix_time);
      }
      else {
        $date = dt('Has not run yet.');
      }
      // Output each cron job name and when it ran.
      drush_print("-  $cron_job  -->  $date");
    }
  }
}


/**
 * Initializes the local storage if it does not already exist.
 */
function drush_codit_crons_local_init() {
  $codit_local_dir = drupal_get_path('module', 'codit_local') . '/crons/';
  $msg_vars = array(
    '@path' => $codit_local_dir,
  );
  // Check to see if the directory already exists.
  if (!file_exists($codit_local_dir)) {
    // Does not exists, so init the directory and sample by copying boilerplate.
    $boilerplate_dir = drupal_get_path('module', 'codit_crons') . '/boilerplate/crons';
    $success = drush_copy_dir($boilerplate_dir, $codit_local_dir, FILE_EXISTS_ABORT);
    // Make sure it worked.
    if ($success) {
      // It worked.
      drush_log(dt('The Codit: Crons storage was created at: @path', $msg_vars), 'success');
    }
    else {
      // It failes.
      drush_log(dt('The Codit: Crons storage Failed to create at: @path', $msg_vars), 'error');
    }
  }
  else {
    // The crons directory already exists in codit local.
    drush_log(dt('The Codit: Crons storage already exists at: @path', $msg_vars), 'error');
  }
}
