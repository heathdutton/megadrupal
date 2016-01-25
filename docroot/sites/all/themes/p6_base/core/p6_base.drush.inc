<?php
/**
 * @file
 * Contains functions only needed for drush integration.
 */

/**
 * Implementation of hook_drush_command().
 */
function p6_base_drush_command() {
  $items = array();

  $items['p6_base'] = array(
    'description' => 'Create a theme using Project6 Base theme.',
    'arguments' => array(
      'name'         => 'A name for your theme.',
      'machine_name' => '[optional] A machine-readable name for your theme.',
    ),
    'options' => array(
      'name'         => 'A name for your theme.',
      'machine-name' => '[a-z, 0-9] A machine-readable name for your theme.',
      'description'  => 'A description of your theme.',
    ),
    'examples' => array(
      'drush p6_base "My theme name"' => 'Create a sub-theme, using the default options.',
      'drush p6_base "My theme name" my_theme' => 'Create a sub-theme with a specific machine name.',
    ),
  );

  return $items;
}

/**
 * Create a p6_base sub-theme using the starter kit.
 */
function drush_p6_base($name = NULL, $machine_name = NULL) {
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
    '/[^a-z0-9_]/', // Remove characters not valid in function names.
    '/^[^a-z]+/',   // Functions must begin with an alpha character.
  );
  $machine_name = preg_replace($search, '', $machine_name);

  // Determine the path to the new subtheme by finding the path to p6_base.
  // Path of the p6_base theme.
  $p6_base_core_path       =  drush_locate_root() . '/' . drupal_get_path('theme', 'p6_base');
  $p6_base_core_path_array = explode('/', $p6_base_core_path);
  // Path of the P^_SUBTHEME to be cloned.
  array_pop($p6_base_core_path_array);
  $p6_base_subtheme_path = implode('/', $p6_base_core_path_array) . '/P6_SUBTHEME';
  // Path of new sub theme.
  array_pop($p6_base_core_path_array);
  $p6_base_newtheme_path = implode('/', $p6_base_core_path_array) . '/' . str_replace('_', '-', $machine_name);

  // Make a fresh copy of the original starter kit.
  drush_op('p6_base_copy', $p6_base_subtheme_path, $p6_base_newtheme_path);

  // Rename the .info file.
  $newtheme_info_file = $p6_base_newtheme_path . '/' . $machine_name . '.info';
  drush_op('rename', $p6_base_newtheme_path . '/P6_SUBTHEME.info.txt', $newtheme_info_file);
  // Rename CSS files.
  drush_op('rename', $p6_base_newtheme_path . '/css/p6_subtheme.core.css', $p6_base_newtheme_path . '/css/' . $machine_name . '.core.css');
  drush_op('rename', $p6_base_newtheme_path . '/css/p6_subtheme.custom.css', $p6_base_newtheme_path . '/css/' . $machine_name . '.custom.css');
  drush_op('rename', $p6_base_newtheme_path . '/css/p6_subtheme.layout.css', $p6_base_newtheme_path . '/css/' . $machine_name . '.layout.css');
  drush_op('rename', $p6_base_newtheme_path . '/css/p6_subtheme.superfish.css', $p6_base_newtheme_path . '/css/' . $machine_name . '.superfish.css');
  drush_op('rename', $p6_base_newtheme_path . '/css/p6_subtheme.typography.css', $p6_base_newtheme_path . '/css/' . $machine_name . '.typography.css');
  // Rename JavaScript file.
  drush_op('rename', $p6_base_newtheme_path . '/js/p6_subtheme.js', $p6_base_newtheme_path . '/js/' . $machine_name . '.js');

  // Alter the contents of the .info file based on the command options.
  $alterations = array(
    '= Project6 sub theme' => '= ' . $name,
    'P6_SUBTHEME' => $machine_name,
  );
  if ($description = drush_get_option('description')) {
    $alterations['Read the included README.txt on how to create a theme with Project6 Base theme.'] = $description;
  }
  drush_op('p6_base_file_str_replace', $newtheme_info_file, array_keys($alterations), $alterations);

  // Replace all occurrences of 'P6_SUBTHEME' with the machine name of our sub theme.
  // drush_op('p6_base_file_str_replace', $p6_base_newtheme_path . '/theme-settings.php', 'P6_SUBTHEME', $machine_name);
  drush_op('p6_base_file_str_replace', $p6_base_newtheme_path . '/template.php', 'P6_SUBTHEME', $machine_name);

  // Notify user of the newly created theme.
  drush_print(dt('Starter kit for "!name" created in: !path', array(
    '!name' => $name,
    '!path' => $p6_base_newtheme_path,
  )));
}

/**
 * Copy a directory recursively.
 */
function p6_base_copy($source_dir, $target_dir, $ignore = '/^(\.(\.)?|CVS|\.svn|\.git|\.DS_Store)$/') {
  if (!is_dir($source_dir)) {
    drush_die(dt('The directory "!directory" was not found.', array('!directory' => $source_dir)));
  }
  $dir = opendir($source_dir);
  @mkdir($target_dir);
  while($file = readdir($dir)) {
    if (!preg_match($ignore, $file)) {
      if (is_dir($source_dir . '/' . $file)) {
        p6_base_copy($source_dir . '/' . $file, $target_dir . '/' . $file, $ignore);
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
function p6_base_file_str_replace($file_path, $find, $replace) {
  $file_contents = file_get_contents($file_path);
  $file_contents = str_replace($find, $replace, $file_contents);
  file_put_contents($file_path, $file_contents);
}
