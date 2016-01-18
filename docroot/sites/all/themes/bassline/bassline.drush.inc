<?php
/**
 * @file
 * Contains drush integration for bassline.
 *
 * Based on the drush implementation from zen
 * @see http://drupalcode.org/project/zen.git/blob/refs/heads/7.x-5.x:/zen-internals/zen.drush.inc
 */

/**
 * Implements hook_drush_command().
 */
function bassline_drush_command() {
  $items = array();
  $items['bassline'] = array(
    'description' => 'Create a Baseline child theme.',
    'arguments' => array(
      'name' => 'A name for your theme.',
      'machine_name' => '[optional] A machine-readable name for your theme.',
    ),
    'options' => array(
      'name' => 'A name for your theme.',
      'machine-name' => '[a-z, 0-9] A machine-readable name for your theme.',
      'path' => 'The path where your theme will be created. Defaults to: sites/all/themes',
      'description' => 'A description of your theme.',
    ),
    'examples' => array(
      'drush bassline "My theme name"' => 'Create a child theme, using the default options.',
      'drush bassline "My theme name" my_theme' => 'Create a child theme with a specific machine name.',
      'drush bassline "My theme" --path=sites/default/themes' => 'Create a child theme in the specified directory.',
    ),
  );
  return $items;
}

/**
 * Create a copy of CHILDTHEME renamed to specified machine name.
 */
function drush_bassline($name = NULL, $machine_name = NULL) {
  if (!isset($name)) {
    $name = drush_get_option('name');
  }

  // Figure out the machine name of the subtheme, either use the one passed by
  // the user or generate one from the name of the theme.
  if (!isset($machine_name)) {
    $machine_name = drush_get_option('machine-name');
  }
  if (!$machine_name) {
    $machine_name = $name;
  }
  $machine_name = drupal_clean_css_identifier(
    strtolower($machine_name),
    array(' ' => '_', '_' => '_', '/' => '', '[' => '', ']' => '')
  );

  $baseline_path = drush_locate_root() . '/' . drupal_get_path('theme', 'bassline');
  $newtheme_path = dirname($baseline_path) . '/bassline/CHILDTHEME';

  // Figure out the path to the child theme, if a user didn't specify a sitename
  // directory use the same as baseline itself.
  if ($site_name = drush_get_option('site-name')) {
    $subtheme_path = drush_locate_root() . '/sites/' . $site_name . '/themes/' . $machine_name;
    $subtheme_dir = dirname($subtheme_path);
    // Make sure the themes folder exist within the sitename directory.
    if (!is_dir($subtheme_dir)) {
      return drush_set_error('DRUSH_bassline_COPY_DIR', dt('Destination directory !path does not exist. You have to create it.', array(
        '!path' => $subtheme_dir,
      )));
    }
  }
  // The user didn't provide a sitename so we extract it from where baseline
  // itself is installed.
  else {
    $path = explode('/', $baseline_path);
    $path = array_slice($path, 0, -2);
    $subtheme_path = implode('/', $path) . '/themes/' . $machine_name;
  }

  // Create a renamed copy of baselines example subtheme.
  if (!drush_copy_dir($newtheme_path, $subtheme_path)) {
    return drush_set_error('DRUSH_bassline_COPY_DIR', dt('Subtheme generation failed, exiting.'));
  }

  $subtheme_info_file = $subtheme_path . '/' . $machine_name . '.info';
  // Activate the info file and rename it to the new machine name.
  drush_op('rename', $subtheme_path . '/CHILDTHEME.info.example', $subtheme_info_file);
  // Rename all references to CHILDTHEME and change the theme description if the
  // user provided an alternative.
  $alterations = array(
    'CHILDTHEME' => $name,
  );
  if ($description = drush_get_option('description')) {
    $alterations += array("= Hi im $name based on bassline - you should really rename me :)" => "= $description");
  }
  drush_op('bassline_file_str_replace', $subtheme_info_file, array_keys($alterations), $alterations);
  drush_op('bassline_file_str_replace', $subtheme_path . '/template.php', 'CHILDTHEME', $machine_name);

  drush_print(dt('bassline subtheme "!name" created in: !path', array(
    '!name' => $name,
    '!path' => $subtheme_path,
  )));
  drush_print(dt('Visit your themes settings page and configure it to your liking: !path', array(
    '!path' => url('admin/appearance/settings/' . $machine_name, array('absolute' => TRUE)),
  )));
}

/**
 * Do string replacement within a specified file.
 */
function bassline_file_str_replace($file_path, $find, $replace) {
  $file_contents = file_get_contents($file_path);
  $file_contents = str_replace($find, $replace, $file_contents);
  file_put_contents($file_path, $file_contents);
}
