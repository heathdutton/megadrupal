<?php

/**
 * @file Drush sql-dump extension
 */


/**
 * Implements hook_drush_help_alter().
 *
 * Adds --hold option to drush's core sql-dump command
 */
function sql_dump_hold_drush_help_alter(&$command) {
  if ($command['command'] == 'sql-dump') {
    $command['options']['hold'] = 'Hold the number of dumps, before purging oldest, if combined with result-file option.';
  }
}

/**
 * Implements drush_hook_post_COMMAND().
 *
 * Hold the number of dumps, before purging oldest,
 * if hold and result-file option were provided
 */
function drush_sql_dump_hold_post_sql_dump_execute($db_spec = NULL) {
  $file = drush_get_option('result-file', FALSE);
  $hold = drush_get_option('hold', FALSE);

  // do nothing without specified result-file or hold option
  if (!$file || !$hold) {
    return;
  }

  // no drush_hook_COMMAND_validate() here, 
  // we want a proper execution of sql-dump and skip only purging/drush_hook_post_COMMAND()
  if (!is_numeric($hold)) {
    return drush_log(dt('Hold option is not numeric.'), 'error');  
  }


  if ($file && $hold) {
    $path = explode('/', $file);
    unset($path[count($path) - 1]);
    $path = implode('/', $path);
    (count($path)) ? $path = $path : $path = realpath(getcwd());
    if($dh = opendir($path)) {
      while ($filename = readdir($dh)) {
        $file = $path . '/' .  $filename;
        $path_parts = pathinfo($file);
        if (is_file($file) && isset($path_parts['extension'])) {
          if ($path_parts['extension'] == 'sql' || $path_parts['extension'] == 'gz') {
            // use created time for order...
            $ctime = filectime($file); 
            $files[$ctime] = $file;
          }
        }
      }
      // sort and reverse order
      ksort($files);
      $files = array_reverse($files);

      // slice array at $hold to get items to purge
      $purge = array();
      $purge = array_slice($files, $hold);

      if (count($purge)) {        
        drush_print(dt("The following dumps will be purged:\n!dumps", array("!dumps" => implode("\n", $purge))));
        if (!drush_confirm(dt('Do you really want to continue?'))) {          
          return drush_user_abort();
        }
        foreach ($purge as $file) {
          unlink($file);
        }	
        return drush_log(dt("Purged dumps:\n!dumps", array('!dumps' => implode("\n", $purge))), 'success');
      }
      else {
        return drush_log(dt('No files to purge'), 'notice');
      }
    }
  }
}
