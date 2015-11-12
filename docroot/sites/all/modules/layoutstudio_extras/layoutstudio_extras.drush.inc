<?php
/**
 * Implementation of hook_drush_command().
 *
 * In this hook, you specify which commands your
 * drush module makes available, what it does and
 * description.
 *
 * Notice how this structure closely resembles how
 * you define menu hooks.
 *
 * See `drush topic docs-commands` for a list of recognized keys.
 *
 * @return
 *   An associative array describing your command(s).
 */
function layoutstudio_extras_drush_command() {
  $items = array();

  $items['layoutstudio'] = array(
    'description' => "Create a sub theme of Layout Studio.",
    'callback' => 'layoutstudio_extras_callback',
    'arguments' => array(
      'name' => 'The name of the sub theme.',
      'location' => 'The location of the sub theme within sites/.',
    ),
    'examples' => array(
      'drush layoutstudio my_theme' => 'Create a Layout Studio Subtheme called my_theme.',
      'drush layoutstudio my_theme mysite/themes' => 'Create a Layout Studio Subtheme called my_theme in sites/mysite/theme.',
    ),
  );

  return $items;
}

/**
 * Implementation of hook_drush_help().
 *
 * This function is called whenever a drush user calls
 * 'drush help <name-of-your-command>'. This hook is optional. If a command
 * does not implement this hook, the command's description is used instead.
 *
 * This hook is also used to look up help metadata, such as help
 * category title and summary.  See the comments below for a description.
 *
 * @param
 *   A string with the help section (prepend with 'drush:')
 *
 * @return
 *   A string with the help text for your command.
 */
function layoutstudio_extras_drush_help($section) {
  switch ($section) {
    case 'drush:layoutstudio':
      return dt("This command will create a Layout Studio subtheme.");
  }
}

/**
 * Implementation of drush_hook_COMMAND_validate().
 *
 * The validate command should exit with
 * `return drush_set_error(...)` to stop execution of
 * the command.  In practice, calling drush_set_error
 * OR returning FALSE is sufficient.  See drush.api.php
 * for more details.
 */
function drush_layoutstudio_ls_sub_theme_validate() {
  
}

/**
 * Example drush command callback. This is where the action takes place.
 *
 * The function name should be same as command name but with dashes turned to
 * underscores and 'drush_commandfile_' prepended, where 'commandfile' is
 * taken from the file 'commandfile.drush.inc', which in this case is 'sandwich'.
 * Note also that a simplification step is also done in instances where
 * the commandfile name is the same as the beginning of the command name,
 * "drush_example_example_foo" is simplified to just "drush_example_foo".
 * To also implement a hook that is called before your command, implement
 * "drush_hook_pre_example_foo".  For a list of all available hooks for a
 * given command, run drush in --debug mode.
 *
 * If for some reason you do not want your hook function to be named
 * after your command, you may define a 'callback' item in your command
 * object that specifies the exact name of the function that should be
 * called.  However, the specified callback function must still begin
 * with "drush_commandfile_" (e.g. 'callback' => "drush_example_foo_execute")
 * if you want that all hook functions are still called (e.g.
 * drush_example_pre_foo_execute, and so on).
 *
 * In this function, all of Drupal's API is (usually) available, including
 * any functions you have added in your own modules/themes.
 *
 * @see drush_invoke()
 * @see drush.api.php
 */
function layoutstudio_extras_callback($name, $location = 'sites/all/themes') {
  // we do some basic validation on the location to ensure 
  // we are at least in the sites directory.
  if ($location != 'sites/all/themes') {
    if ($location{0} == '/') {
      $location = 'sites'. $location;
    } else {
      $location = 'sites/'. $location;
    }
  }
  if (!function_exists('layoutstudio_extras_create_subtheme')) {
    include_once drupal_get_path('module', 'layoutstudio_extras') . '/layoutstudio_extras.module';
  }
  
  // we ensure the theme name is a valid function name and doesn't already exist.
  // Thanks to the zenophile guys for this.
  if ($exists = drupal_get_path('theme', $name)) {
    drush_print("\nRESULT: This name already exists. Please try a different name.\n");
    exit();
  } elseif (!preg_match('/^[abcdefghijklmnopqrstuvwxyz][abcdefghijklmnopqrstuvwxyz0-9_]*$/', $name)) {
    drush_print("\nRESULT: The name may only consist of lowercase letters and the underscore character.\n");
    exit();
  }
  
  // We now call layoutstudio_extras_create_subtheme from within layoutstudio_extras.module
  // The second parameter indicates it is drush calling the function which will allow for
  // authenticated file transfers. No need to use Drupal 7's FTP method.
  list($success, $message) = layoutstudio_extras_create_subtheme($name, true, $location);
  if ($success) {
    drush_print("\nRESULT: ". $name ." was created successfully!\n");
  } else {
    drush_print("\nRESULT: ". $name ." could not be created. The following errors were recorded:\n". $message ."\n");
  }
}

