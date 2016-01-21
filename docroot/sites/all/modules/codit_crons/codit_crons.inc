<?php
/**
 * @file
 * Helper functions to support the use of Codit: cron.
 */


/**
 * Returns a cached array of cron jobs registered by codit_cron.
 *
 * @return array
 *   The array of cron jobs is built from whatever cron directories are
 *   present in codit_local/crons/.  Cron jobs directories
 *   beginning with _ are ignored (considered disabled).
 */
function codit_crons_get_cron_jobs() {
  $a_cron_jobs_list = &drupal_static(__FUNCTION__);
  if (!isset($a_cron_jobs_list)) {
    if ($cache = cache_get('codit_crons_jobs_list')) {
      $a_cron_jobs_list = $cache->data;
    }
    else {
      // Do heavy lifting and populate $a_cron_jobs_list
      $a_cron_jobs_list = array();
      $path_to_local_crons = codit_path_to_local('crons');
      if (!empty($path_to_local_crons) && file_exists($path_to_local_crons)) {
        // Local exists, Scan codit_local/crons/ for any sub-directories.
        $dir_options = array(
          'recurse' => FALSE,
          'key' => 'name',
          'min_depth' => 0,
          'nomask' => '/.*\.(txt|inc|module|class|gif|jpg|png|cvs|svn|git|md)$/',
        );
        $mask  = '//';
        $a_cron_directories = file_scan_directory($path_to_local_crons, $mask, $dir_options);

        // Scan the crons directory for files.
        $file_options = array(
          'recurse' => TRUE,
          'key' => 'uri',
          'min_depth' => 1,
          'nomask' => '/.*\.(txt|module|class|gif|jpg|png|cvs|svn|git|md)$/',
        );
        $mask = '/.*\.(tpl.php|inc)$/';
        $mask = '//';
        $a_cron_files  = file_scan_directory($path_to_local_crons, $mask, $file_options);

        // Cycle through $a_cron_directories and build the array of crons.
        foreach (is_array($a_cron_directories) ? $a_cron_directories : array() as $cron_name => $o_cron_directory) {
          // Process this file to see if it belongs in $a_cron_jobs_list
          // Skip any beginning with '_' or '.'.  These are treated as hidden.
          if ($cron_name !== ltrim($cron_name, '._')) {
            continue;
          }

          // Check for _callback file because it is required.
          $target_callback = "{$o_cron_directory->uri}/_callback.inc";
          if (empty($a_cron_files[$target_callback])) {
            // There is no callback so do not register this cron job.
            $message = 'Codit Cron could not register the cron job: @cronname, because its _callback.inc was not found here: @pathtocrons/@cronname.';
            $variables = array(
              'cronname' => $cron_name,
              'pathtocrons' => $path_to_local_crons,
            );
            watchdog('codit_crons', $message, $variables, WATCHDOG_DEBUG);
            continue;
          }

          // It made it this far so it must be legitimate. Save the cron job.
          $a_cron_jobs_list[$cron_name] = $target_callback;
        }
      }
      cache_set('codit_crons_jobs_list', $a_cron_jobs_list, 'cache');
    }
  }

  asort($a_cron_jobs_list);
  return $a_cron_jobs_list;
}


/**
 * Function to use as a callback and argument to then call the right cron.
 *
 * @param string $cron_job_name
 *   Name of the cron function.
 *
 * @return bool
 *   TRUE if it ran, False if it didn't
 */
function codit_crons_callback_router($cron_job_name = '') {
  $run_status = FALSE;
  if (!empty($cron_job_name)) {
    // There is a $cron_job_name so lets see if it can be run.
    $a_cron_job_list = codit_crons_get_cron_jobs();
    if (isset($a_cron_job_list[$cron_job_name])) {
      // Check to see if cron components are present by checking for the dir.
      // This has been scanned once and cached so it is possible it has been
      // deleted without a cache flush, so check to be sure it is there.
      if (file_exists($a_cron_job_list[$cron_job_name])) {
        // The cron job callback file exists.
        // Create sandbox to keep included callback file from bleeding through.
        $sandbox_params = array(
          'cron_job_name' => $cron_job_name,
          'cron_job_callback_file' => $a_cron_job_list[$cron_job_name],
        );
        $sandbox = function() {
          // Extract the params.
          extract(func_get_arg(0));
          // It is possible that the callback file may be missing parts, so
          // build some fallbacks to be safe.
          // Build an empty $_callback function as a fallback.
          $_callback = function () {
            // Just a fallback, do nothing, return FALSE to say nothing ran.
            return FALSE;
          };

          // Load the cron job callback file.
          include $cron_job_callback_file;

          // Read the time pattern.
          $parameters = (!empty($_time_pattern)) ? array('PatternSubmitted' => $_time_pattern) : array();
          $time = new CronTime($cron_job_name, $parameters);

          // Is the current time far enough along to run this cron job?
          // Or is Elyia Cron in control and overriding code timing?
          if ((module_exists('elysia_cron')) || ($time->evalShouldItRun())) {
            // The cron can job can be fully executed.
            try {
              $run_status = $_callback();
              // The cron job has run so set the cron last ran to now.
              // Normalize return just in case somebody doesn't follow rules.
              $run_status = codit_sanitize_boolean($run_status, 'boolean', TRUE);
              if ($run_status) {
                // It successfully ran, so record the time.
                $time->querySetCronTime();
              }

            }
            catch (Exception $e)
            {
              watchdog('Codit: Cron', 'Exception caught !message', array('!message' => $e->getMessage()), WATCHDOG_ERROR, $link = NULL);
              $msg_vars = array('!CronJobName' => $cron_job_name);
              throw new Exception(t('Something went wrong trying to run the cron job Codit: Crons->!CronJobName .', $msg_vars), 0, $e);
            }
          }

        };
        // Get the details from sandbox.
        $run_status = $sandbox($sandbox_params);
      }
    }
  }
  return $run_status;
}
