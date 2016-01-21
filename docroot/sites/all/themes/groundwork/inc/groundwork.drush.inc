<?php
/**
 * @file
 * Contains functions only needed for drush integration.
 */

/**
 * Implements hook_drush_command().
 */
function groundwork_drush_command() {
  $items = array();

  $items['groundwork'] = array(
    'description' => 'Create a theme using the Groundwork Theme Framework.',
    'arguments' => array(
      'name'         => 'A name for your theme.',
      'machine_name' => '[optional] A machine-readable name for your theme.',
    ),
    'options' => array(
      'name'                 => 'A name for your theme.',
      'machine-name'         => '[a-z, 0-9] A machine-readable name for your theme.',
      'description'          => 'A description of your theme.',
      'grid-sidebar-first'  => '@gridSidebarFirst - How many units does the sidebar-first occupy? Defaults to 8',
      'grid-sidebar-second' => '@gridSidebarSecond - How many units does the sidebar-second occupy? Defaults to 4.',
      'grid-branding'       => '@gridBranding - How many units does the branding (logo, site name and slogan) occupy? Defaults to 16',
    ),
    'examples' => array(
      'drush groundwork \'Philippine Sea\'' => 'Create a custom theme called Philippine Sea, using the default options.',
      'drush groundwork \'Philippine Sea\' philsea' => 'Create a custom theme called Philippine Sea with a specific machine name philsea.',
      'drush groundwork \'Philippine Sea\' --grid-sidebar-first=6 --grid-sidebar-second=6' => 'Create a custom theme called Philippine Sea, with sidebar widths of 6 units each.',
    ),
  );

  return $items;
}

/**
 * Create a Groundwork sub-theme using the CustomWork kit.
 */
function drush_groundwork($name = NULL, $machine_name = NULL) {
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

  // Determine the path to the new subtheme by finding the path to Groundwork.
  $groundwork_path = drupal_get_path('theme', 'groundwork');
  $subtheme_path = explode('/', $groundwork_path);
  array_pop($subtheme_path);
  $subtheme_path = implode('/', $subtheme_path) . '/' . str_replace('_', '-', $machine_name);

  // Make a fresh copy of the original CustomWork.
  drush_op('groundwork_copy', $groundwork_path . '/CustomWork', $subtheme_path);

  // Rename the .info file.
  $subtheme_info_file = $subtheme_path . '/' . $machine_name . '.info';
  drush_op('rename', $subtheme_path . '/CustomWork.info.txt', $subtheme_info_file);

  // Alter the contents of the .info file based on the command options.
  $info_alterations = array(
    '= CustomWork for Groundwork' => '= ' . $name,
  );
  if ($description = drush_get_option('description')) {
    $info_alterations['Custom theme for Groundwork Front-end Framework. To learn more about sub-theming in Groundwork, please visit the <a href="http://groundwork.noceda.me/docs">Groundwork Docs</a> or the included README.txt file.'] = $description;
  }
  drush_op('groundwork_file_str_replace', $subtheme_info_file, array_keys($info_alterations), $info_alterations);

  // Alter the contents of app-settings.css.less file based on the command options.
  $settings_alterations = array(
    ': sites/all/themes/customwork' => ': ' . $subtheme_path,
  );
  if ($grid_sidebar_first = drush_get_option('grid-sidebar-first')) {
    $settings_alterations[': 8'] = ': ' . $grid_sidebar_first;
  }
  if ($grid_sidebar_second = drush_get_option('grid-sidebar-second')) {
    $settings_alterations[': 4'] = ': ' . $grid_sidebar_second;
  }
  if ($grid_branding = drush_get_option('grid-branding')) {
    $settings_alterations[': 16'] = ': ' . $grid_branding;
  }
  drush_op('groundwork_file_str_replace', $subtheme_path . '/less/custom/app-settings.css.less', array_keys($settings_alterations), $settings_alterations);

  // Replace all occurrences of 'CustomWork' with the machine name of our sub theme.
  drush_op('groundwork_file_str_replace', $subtheme_path . '/theme-settings.php', 'CustomWork', $machine_name);
  drush_op('groundwork_file_str_replace', $subtheme_path . '/template.php', 'CustomWork', $machine_name);

  // Notify user of the newly created theme.
  drush_print(dt('Your new theme "!name" with machine name "!machine" created in: !path', array(
    '!name' => $name,
    '!machine' => $machine_name,
    '!path' => $subtheme_path,
  )));
}

/**
 * Copy a directory recursively.
 */
function groundwork_copy($source_dir, $target_dir, $ignore = '/^(\.(\.)?|CVS|\.svn|\.git|\.DS_Store)$/') {
  if (!is_dir($source_dir)) {
    drush_die(dt('The directory "!directory" was not found.', array('!directory' => $source_dir)));
  }
  $dir = opendir($source_dir);
  @mkdir($target_dir);
  while ($file = readdir($dir)) {
    if (!preg_match($ignore, $file)) {
      if (is_dir($source_dir . '/' . $file)) {
        groundwork_copy($source_dir . '/' . $file, $target_dir . '/' . $file, $ignore);
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
function groundwork_file_str_replace($file_path, $find, $replace) {
  $file_contents = file_get_contents($file_path);
  $file_contents = str_replace($find, $replace, $file_contents);
  file_put_contents($file_path, $file_contents);
}
