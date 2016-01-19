<?php
/**
 * @file
 * Drush commands.
 */

/**
 * Implements hook_drush_command().
 */
function sharedcontent_drush_command() {
  $items = array();

  $items['sharedcontent-features'] = array(
    'description' => 'Create SC features with given namespace/',
    'aliases' => array('sc-features', 'scf'),
    'examples' => array(
      'drush sc-features my_module site_dir/features' => dt('Will create SC features with namespace "my_module" into directory "site_dir/features".'),
    ),
    'options' => array(
      'sc_path' => dt('Path of the sharedcontent module.'),
    ),
    'arguments' => array(
      'namespace' => dt('The namespace used as prefix for the copied modules.'),
      'path' => dt('Path to the location where the features should be copied to.'),
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );

  $items['sharedcontent-features-diff'] = array(
    'description' => 'Create SC features diff with given namespace/',
    'aliases' => array('sc-features-diff', 'scfd'),
    'examples' => array(
      'drush scfd foo 7.x-1.0 7.x-2.0' => dt('Will create a patch with the changes between 1.0 and 2.0 using the module namespace "foo" and will apply to "sites/all/modules".'),
      'drush scfd bar 7.x-0.1 7.x-1.x sites/default/modules/features' => dt('Will create a patch with the changes between 0.1 and latest dev using the module namespace foo and will apply to sites/default/modules/features.'),
    ),
    'arguments' => array(
      'namespace' => dt('The namespace used as prefix for features.'),
      'version_source' => dt('The git source version.'),
      'version_target' => dt('The git target version.'),
      'path' => dt('Path to the location where the features are placed.'),
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );

  return $items;
}

/**
 * @todo doc
 *
 * @todo Use a config file instead of a never ending parameter list.
 * @todo Allow to define separate content types for local queue
 *       (existing) and indexing (missing).
 */
function drush_sharedcontent_features($namespace, $output_path = NULL) {

  $sc_path = drush_get_option('sc_path');

  if (empty($sc_path) && function_exists('drupal_get_path')) {
    $sc_path = drupal_get_path('module', 'sharedcontent');
  }

  if (empty($sc_path)) {
    return drush_set_error('ERROR', 'Provide path to the sharedcontent module as the sc_path option.');
  }

  if (empty($output_path)) {
    $output_path = $sc_path . '/' . $namespace;
  }

  if (!file_exists($output_path)) {
    mkdir($output_path, 0777, TRUE);
  }

  drush_print(strtr('Output directory: @path', array('@path' => $output_path)));

  _sharedcontent_drush_process_features_file('client_rules', $namespace, $output_path, $sc_path);
  _sharedcontent_drush_process_features_file('server_rules', $namespace, $output_path, $sc_path);
  _sharedcontent_drush_process_features_file('ui', $namespace, $output_path, $sc_path);
}

/**
 * @todo doc
 */
function _sharedcontent_drush_process_features_file($component, $namespace, $output_path, $sc_path) {
  $output_path = $output_path . '/' . $namespace . '_sharedcontent_' . $component;
  mkdir($output_path);
  $source_path = $sc_path . '/defaults/sharedcontent_template_' . $component;
  $handle = opendir($source_path);

  while ($entry = readdir($handle)) {

    if (is_dir($source_path . '/' . $entry)) {
      continue;
    }

    $source_code = file_get_contents($source_path . '/' . $entry);

    $source_replacements = array('sharedcontent_template' => $namespace . '_sharedcontent');
    // In case we are dealing with info file, adjust module name as well.
    if (strpos($entry, '.info') !== FALSE) {
      $source_replacements['Template'] = ucfirst($namespace);
      $source_replacements['<br />You may want to make a copy of this feature in order to customize. For this purpose use <code>drush sc-features</code>.'] = '';
    }
    $source_code = strtr($source_code, $source_replacements);

    $output_file = strtr($entry, array('sharedcontent_template' => $namespace . '_sharedcontent'));
    $result = file_put_contents($output_path . '/' . $output_file, $source_code);

    if ($result) {
      drush_print(strtr('Output file @path processed.', array('@path' => $output_path . '/' . $output_file)));
    }
    else {
      drush_set_error('ERROR', strtr('Error processing file @path', array('@path' => $output_path . '/' . $output_file)));
    }
  }
}


/**
 * Create feature update patch
 *
 * Use a git diff to build an update diff for a instantiated feature.
 *
 * @param string $namespace
 *  The namespace of the instantiated features.
 * @param string $version_source
 *  The source git version.
 * @param string $version_target
 *  The target git version.
 * @param string $path
 *  The path to the features relative to the git root.
 */
function drush_sharedcontent_features_diff($namespace, $version_source, $version_target, $path = 'sites/all/modules') {
  // Remember the current directory.
  $old_path = getcwd();
  // Make sure we are inside of the git repository.
  // Assuming the module was installed using git.
  chdir(__DIR__);

  // Remove possible existing disturbing characters.
  $path = rtrim($path, '/ ');

  // Create diff
  $diff = `git diff --relative=defaults $version_source $version_target defaults`;

  // Set up necessary replacements.
  $replacements = array();
  $replacements['a/sharedcontent_template'] = "a/$path/{$namespace}_sharedcontent";
  $replacements['b/sharedcontent_template'] = "b/$path/{$namespace}_sharedcontent";
  $replacements['sharedcontent_template'] = "{$namespace}_sharedcontent";

  // Search and replace.
  $diff = strtr($diff, $replacements);

  // Print resulting diff.
  echo "$diff\n";

  chdir($old_path);
}
