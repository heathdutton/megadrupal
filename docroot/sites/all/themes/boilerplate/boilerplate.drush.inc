<?php
/**
 * @file
 * Contains functions only needed for drush integration.
 */

/**
 * Implements hook_drush_command().
 */
function boilerplate_drush_command() {
  $items = array();

  $items['boilerplate'] = array(
    'description' => 'Generate a theme based on Boilerplate with your customizations.',
    'arguments' => array(
      'name'         => 'A name for your new theme.',
      'machine_name' => '[optional] A machine-readable name for your theme.',
    ),
    'options' => array(
      'name'         => 'A name for your new theme.',
      'machine-name' => '[a-z, 0-9] A machine-readable name for your theme.',
      'description'  => 'A description of your new theme.',
    ),
    'examples' => array(
      'drush boilerplate "My theme name"' => 'Create a new theme based on Boilerplate.',
      'drush boilerplate "My theme name" my_theme' => 'Create a new theme with a specific machine name.',
    ),
    'aliases' => array('bp'),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function boilerplate_drush_help($section) {
  switch ($section) {
    case 'drush:boilerplate':
      return dt("Generate a theme from Boilerplate with your customizations.");
    case 'meta:boilerplate:summary':
      return dt("Automates creating a Boilerplate based theme.");
  }
}

/*
* Create a customized Boilerplate instance.
*/
function drush_boilerplate($name = NULL, $machine_name = NULL) {
  // Determine the theme name.
  if (!isset($name)) {
    $name = drush_get_option('name');
  }

  // Determine the machine name.
  if (!isset($machine_name)) {
    $machine_name = drush_get_option('machine-name');
  }
  if (!$machine_name) {
    $machine_name = $name;
  }
  $machine_name = str_replace(' ', '_', strtolower($machine_name));
  $search = array(
    // Remove characters not valid in function names.
    '/[^a-z0-9_]/',
    // Functions must begin with an alpha character.
    '/^[^a-z]+/',
  );
  $machine_name = preg_replace($search, '', $machine_name);

  // Determine the path to the new subtheme by finding the path to boilerplate.
  system_rebuild_theme_data();
  $drupal_root = drush_get_context('DRUSH_SELECTED_DRUPAL_ROOT') . '/';
  $boilerplate_path =  drupal_get_path('theme', 'boilerplate');
  $newtheme_path = $drupal_root . 'sites/all/themes/custom/';
  //check to see if sites/all/themes/custom is writeable,
  //and if so, put the theme there.
  if (file_prepare_directory($newtheme_path)) {
    $newtheme_path = $drupal_root . 'sites/all/themes/custom/' .
      str_replace('_', '-', $machine_name);
  }
  //and if not, just use sites/all/themes.
  else {
    $newtheme_path = $drupal_root . 'sites/all/themes/' .
      str_replace('_', '-', $machine_name);
  }

  // Make a copy with our changes.
  drush_op('boilerplate_copy', $boilerplate_path . '/', $newtheme_path . '/');

  // Rename the .info file.
  $newtheme_info_file = $newtheme_path . '/' . $machine_name . '.info';
  drush_op('rename', $newtheme_path . '/boilerplate.info', $newtheme_info_file);

  // Alter the contents of the .info file based on the command options.
  $alterations = array(
    '= Boilerplate' => '= ' . $name,
    'version = "1.0"' => '',
    'project = "Boilerplate"' => '',
  );

  if (drush_confirm('Will you be using SASS?')) {
    $alterations['css/style.css'] = 'css/compiled/style.css';
    $cssbase = $newtheme_path . '/css/';
    unlink($cssbase . 'style.css');
    unlink($cssbase . 'tabs.css');
  }

  if ($description = drush_get_option('description')) {
    $alterations['Metaltoad HTML5/SASS base theme for Drupal 7'] = $description;
  }
  drush_op('boilerplate_file_str_replace', $newtheme_info_file, array_keys($alterations), $alterations);

  // Remove the packaging script signature from the end of the info file.
  drush_op('boilerplate_file_preg_replace', $newtheme_info_file, '/\n; Information added by drupal.*\Z/s', '');

  // Replace all occurrences of 'Boilerplate' with the machine name of our new theme.
  drush_op('boilerplate_file_str_replace', $newtheme_path . '/theme-settings.php', 'boilerplate', $machine_name);
  drush_op('boilerplate_file_str_replace', $newtheme_path . '/template.php', 'boilerplate', $machine_name);

  // Notify user of the newly created theme.
  drush_print(dt('New theme, "!name" created in: !path', array(
    '!name' => $name,
    '!path' => $newtheme_path,
  )));
}

/**
 * Copy a directory recursively.
 */
function boilerplate_copy($source_dir, $target_dir, $ignore = '/^(\.(\.)?|boilerplate.drush.inc|CVS|\.svn|\.git|\.DS_Store|\.sass-cache)$/') {
  if (!is_dir($source_dir)) {
    drush_die(dt('The directory "!directory" was not found.', array('!directory' => $source_dir)));
  }
  $dir = opendir($source_dir);
  @mkdir($target_dir);
  while ($file = readdir($dir)) {
    if (!preg_match($ignore, $file)) {
      if (is_dir($source_dir . '/' . $file)) {
        boilerplate_copy($source_dir . '/' . $file, $target_dir . '/' . $file, $ignore);
      }
      else {
        copy($source_dir . '/' . $file, $target_dir . '/' . $file);
      }
    }
  }
  closedir($dir);
}

/**
 * Replace strings in a file.
 */
function boilerplate_file_str_replace($file_path, $find, $replace) {
  $file_contents = file_get_contents($file_path);
  $file_contents = str_replace($find, $replace, $file_contents);
  file_put_contents($file_path, $file_contents);
}
/**
 * Replace strings in a file (with feeling).
 */
function boilerplate_file_preg_replace($file_path, $pattern, $replace) {
  $file_contents = file_get_contents($file_path);
  $file_contents = preg_replace($pattern, $replace, $file_contents);
  file_put_contents($file_path, $file_contents);
}
