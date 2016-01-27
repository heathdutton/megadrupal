<?php
/**
 * @file
 * The Drush remake extension for Drush.
 */

/**
 * Implements hook_drush_command().
 */
function drush_remake_drush_command() {
  $items = array();

  $items['remake'] = array(
    'description' => "Re-makes an installation profile using drush make",
    'arguments' => array(
      'installation profile' => dt('The name of the installation profile to re-make'),
    ),
    'options' => array(
      'no-working-copy' => dt('Pass this option in to do the remake without the --working-copy option being passed to drush make'),
      'makefile' => dt('A make file name to search for in profiles.'),
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_ROOT, // Bootstrap into a Drupal root.
    'allow-additional-options' => array(
      'make',
    ),
  );

  return $items;
}

/**
 * Validation hook for the drush_remake command.
 */
function drush_drush_remake_remake_validate() {
  $args = func_get_args();

  // Support a custom make file name, but default to some common ones.
  $makefiles = drush_get_option('makefile', array(
    '%profile.make',
    '%profile.yaml',
    '%profile.yml',
    'drupal-org.make',
    'drupal-org.yaml',
    'drupal-org.yml',
  ));
  if (!is_array($makefiles)) {
    $makefiles = array($makefiles);
  }
  drush_set_option('makefile', $makefiles);

  if (empty($args)) {
    // Try to offer a choice.
    $options = drush_remake_find_profiles();
    // If we can't offer any choices, return an error.
    if (count($options) == 0) {
      return drush_set_error('DRUSH_REMAKE_INCORRECT_ARGUMENTS', dt("You must specify the installation profile to rebuild."));
    }
    // If there's only once choice then just select it.
    elseif (count($options) == 1) {
      reset($options);
      $profile = key($options);
    }
    // Otherwise give the user a choice.
    else {
      $choice = drush_choice($options, dt('Choose an installation profile to remake:'));
      if (!empty($choice)) {
        $profile = $choice;
      }
      else {
        return drush_set_error('DRUSH_REMAKE_INCORRECT_ARGUMENTS', dt("You must specify the installation profile to rebuild."));
      }
    }
  }
  else {
    // Check that the first argument is actually a valid install profile with a make file.
    $profile = $args[0];
  }


  if (!file_exists(DRUPAL_ROOT . '/profiles/' . $profile)) {
    return drush_set_error('DRUSH_REMAKE_UNKNOWN_PROFILE', dt("Could not find an installation profile."));
  }

  $found = FALSE;
  foreach ($makefiles as $make) {
    $possible = DRUPAL_ROOT . '/profiles/' . $profile . '/' . str_replace('%profile', $profile, $make);
    if (file_exists($possible)) {
      $found = TRUE;
      drush_set_context('remake_file', $possible);
      break;
    }
  }
  // If we didn't find a make file to use, raise an error.
  if (!$found) {
    return drush_set_error('DRUSH_REMAKE_NO_MAKE_FILE', dt("Could not find a valid make file."));
  }

  drush_set_context('remake_profile', $profile);
}

/**
 * Get installation profiles with a make file.
 */
function drush_remake_find_profiles() {
  $options = array();
  $makefiles = drush_get_option('makefile', array());
  foreach (glob(DRUPAL_ROOT . '/profiles/*') as $profile) {
    if (is_dir($profile)) {
      $basename = basename($profile);
      foreach ($makefiles as $make) {
        if (file_exists(DRUPAL_ROOT . '/profiles/' . $basename . '/' . str_replace('%profile', $basename, $make))) {
          $options[$basename] = $basename;
        }
      }
    }
  }
  return $options;
}

/**
 * The main drush remake command callback.
 */
function drush_drush_remake_remake() {
  $profile = drush_get_context('remake_profile');
  $directory = DRUPAL_ROOT . '/profiles/' . $profile;

  drush_log(dt('Remaking @profile from @makefile.', array('@profile' => $profile, '@makefile' => drush_get_context('remake_file'))), 'ok');

  // Build the command.
  $args = array();
  $options = drush_redispatch_get_options();
  unset($options['no-working-copy']);

  $args[] = drush_get_context('remake_file');
  $args[] = '.';

  // We cd to run the command, so the contrib destination should be set to: '.'
  $options['contrib-destination'] = '.';

  // People should use drush make to make with a valid Drupal core.
  $options['no-core'] = TRUE;

  // Set the working-copy option by default, but allow people to not send it.
  if (drush_get_option('no-working-copy', FALSE)) {
    $options['working-copy'] = FALSE;
  }
  else {
    $options['working-copy'] = TRUE;
  }

  // Don't run with a Drupal root.
  $options['root'] = NULL;

  // Call the command
  $backend_options = array(
    'interactive' => TRUE,
  );

  $running_path = getcwd();
  // Convert windows paths.
  $running_path = _drush_convert_path($running_path);

  // Set to the path we want to use.
  chdir($directory);

  drush_invoke_process('@none', 'make', $args, $options, $backend_options);

  // Set back to the previous running path.
  chdir($running_path);
}
