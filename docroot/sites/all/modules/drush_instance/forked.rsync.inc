<?php

/**
 * @file forked.rsync.inc
 */

/**
 * Fork of drush_core_rsync(), removing the y/n prompt
 */
function _instance_drush_core_rsync($source, $destination, $additional_options = array()) {
  // Preflight source in case it defines aliases used by the destination
  _drush_sitealias_preflight_path($source);
  // After preflight, evaluate file paths.  We evaluate destination paths first, because
  // there is a first-one-wins policy with --exclude-paths, and we want --target-command-specific
  // to take precidence over --source-command-specific.
  $destination_settings = drush_sitealias_evaluate_path($destination, $additional_options, FALSE, "rsync", 'target-');
  $source_settings = drush_sitealias_evaluate_path($source, $additional_options, FALSE, "rsync", 'source-');
  $source_path = $source_settings['evaluated-path'];
  $destination_path = $destination_settings['evaluated-path'];

  if (!isset($source_settings)) {
    return drush_set_error('DRUSH_BAD_PATH', dt('Could not evaluate source path !path.', array('!path' => $source)));
  }
  if (!isset($destination_settings)) {
    return drush_set_error('DRUSH_BAD_PATH', dt('Could not evaluate destination path !path.', array('!path' => $destination)));
  }

  // Check to see if this is an rsync multiple command (multiple sources and multiple destinations)
  $is_multiple = drush_do_multiple_command('rsync', $source_settings, $destination_settings, TRUE);

  if ($is_multiple === FALSE) {
    // If the user path is the same for the source and the destination, then
    // always add a slash to the end of the source.  If the user path is not
    // the same in the source and the destination, then you need to know how
    // rsync paths work, and put on the trailing '/' if you want it.
    if ($source_settings['user-path'] == $destination_settings['user-path']) {
      $source_path .= '/';
    }
    /**
     * FORK
     */
    /* // Prompt for confirmation. This is destructive.
    if (!drush_get_context('DRUSH_SIMULATE')) {
      drush_print(dt("You will destroy data from !target and replace with data from !source", array('!source' => $source_path, '!target' => $destination_path)));
      if (!drush_confirm(dt('Do you really want to continue?'))) {
        // was: return drush_set_error('CORE_SYNC_ABORT', 'Aborting.');
        return drush_user_abort();
      }
    } */

    // Exclude settings is the default only when both the source and
    // the destination are aliases or site names.  Therefore, include
    // settings will be the default whenever either the source or the
    // destination contains a : or a /.
    $include_settings_is_default = (strpos($source . $destination, ':') !== FALSE) || (strpos($source . $destination, '/') !== FALSE);

    $options = _drush_build_rsync_options($additional_options, $include_settings_is_default);

    // Get all of the args and options that appear after the command name.
    $original_args = drush_get_original_cli_args_and_options();
    foreach ($original_args as $original_option) {
      if ($original_option{0} == '-') {
        $options .= ' ' . $original_option;
      }
    }

    // Go ahead and call rsync with the paths we determined
    return drush_core_exec_rsync($source_path, $destination_path, $options);
  }
}
