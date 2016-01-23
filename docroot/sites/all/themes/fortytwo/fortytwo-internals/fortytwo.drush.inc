<?php

/**
 * @file
 * Contains functions only needed for drush integration.
 */

/**
 * Implements hook_drush_command().
 */
function fortytwo_drush_command() {
  $items = array();

  $items['fortytwo'] = array(
    'description' => 'Create a sub theme using Fortytwo.',
    'arguments' => array(
      'name'         => 'A name for your theme.',
      'machine-name' => '[optional] A machine-readable name for your theme.',
      'description'  => '[optional] A description of your theme.',
    ),
    'options' => array(
      'name'         => 'A name for your theme.',
      'machine-name' => '[a-z, 0-9] A machine-readable name for your theme.',
      'description'  => 'A description of your theme.',
    ),
    'examples' => array(
      'drush fortytwo "My theme name"' => 'Create a sub-theme, using the default options.',
      'drush fortytwo "My theme name" my_theme' => 'Create a sub-theme with a specific machine and human name.',
      'drush fortytwo --name="Example theme" --machine-name=example --description "This is a description of my sub theme"' => 'Create a sub-theme with a specific description.',
    ),
  );

  return $items;
}

/**
 * Create a Fortytwo sub-theme using the starter kit.
 */
function drush_fortytwo($name = NULL, $machine_name = NULL) {
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

  // Determine the path to the new subtheme by finding the path to Fortytwo.
  $fortytwo_path = drush_locate_root() . '/' . drupal_get_path('theme', 'fortytwo');
  $subtheme_path = explode('/', $fortytwo_path);
  array_pop($subtheme_path);
  $subtheme_path = implode('/', $subtheme_path) . '/' . str_replace('_', '-', $machine_name);

  // Make a fresh copy of the original starter kit.
  drush_op('fortytwo_copy', $fortytwo_path . '/STARTERKIT', $subtheme_path);

  // Rename the .info file.
  $subtheme_info_file = $subtheme_path . '/' . $machine_name . '.info';
  drush_op('rename', $subtheme_path . '/STARTERKIT.info.txt', $subtheme_info_file);

  // Alter the contents of the .info file based on the command options.
  $alterations = array(
    '= Forty Two Sub-theme Starter Kit' => '= ' . $name,
  );
  $description = drush_get_option('description');
  if ($description) {
    $alterations['Forty Two Sub theme made from the starterkit'] = $description;
  }
  drush_op('fortytwo_file_str_replace', $subtheme_info_file, array_keys($alterations), $alterations);

  // Replace all occurrences of 'STARTERKIT' with the machine name of sub theme.
  drush_op('fortytwo_file_str_replace', $subtheme_path . '/template.php', 'STARTERKIT', $machine_name);

  // Notify user of the newly created theme.
  drush_print(dt('Starter kit for "!name" created in: !path', array(
    '!name' => $name,
    '!path' => $subtheme_path,
  )));
}

/**
 * Copy a directory recursively.
 */
function fortytwo_copy($source_dir, $target_dir, $ignore = '/^(\.(\.)?|CVS|\.svn|\.git|\.DS_Store)$/') {
  if (!is_dir($source_dir)) {
    drush_die(dt('The directory "!directory" was not found.', array('!directory' => $source_dir)));
  }
  $dir = opendir($source_dir);
  mkdir($target_dir);
  while ($file = readdir($dir)) {
    if (!preg_match($ignore, $file)) {
      if (is_dir($source_dir . '/' . $file)) {
        fortytwo_copy($source_dir . '/' . $file, $target_dir . '/' . $file, $ignore);
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
function fortytwo_file_str_replace($file_path, $find, $replace) {
  $file_contents = file_get_contents($file_path);
  $file_contents = str_replace($find, $replace, $file_contents);
  file_put_contents($file_path, $file_contents);
}
