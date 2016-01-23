<?php

/**
 * Implements hook_drush_command().
 */
function standalone_coder_review_drush_command() {
  $items = array();
   
  $items['standalone-coder-review'] = array(
    'description' => "Command description",
    'arguments' => array(
    ),
    'options' => array(
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH, // No bootstrap at all.
  );
   
  return $items;
}

/**
 * Validation callback for the standalone coder review command.
 */
function drush_standalone_coder_review_validate() {
  // Make sure we have a valid output format.
  if ((drush_get_option('format', 'checkstyle') != 'checkstyle') && (drush_get_option('format', 'checkstyle') != 'xml')) {
    return drush_set_error('STANDALONE_CODER_REVIEW_INVALID_FORMAT', 'You must specify a valid format: checkstyle or xml.');
  }
  
  // Check that some files were specified.
  $files = drush_get_option('files');
  if (!file_exists($files)) {
    return drush_set_error('STANDALONE_CODER_REVIEW_NO_FILES', 'You must specify a some files to review.');
  }
  
  // Check that the severity is valid.
  if (!in_array(drush_get_option('severity', 'minor'), array('major', 'minor', 'critical'))) {
    return drush_set_error('STANDALONE_CODER_REVIEW_SEVERITY_INVALID', 'You must specify a valid severity level.');
  }
  
  // Check that an output filename was given.
  $filename = drush_get_option('output-filename');
  if (empty($filename)) {
    return drush_set_error('STANDALONE_CODER_REVIEW_NO_OUTPUT_FILENAME', 'You must specify an output filename.');
  }
  if (substr($filename, 0, 1) != '/') {
    $filename = drush_cwd() . '/' . $filename;
    drush_set_option('output-filename', $filename);
  }
  
  // Now try to download and bootstrap a Drupal.
  $base = drush_find_tmp();
  $base .= '/coder-review/' . standalong_coder_review_command_version();
  $core = 'drupal';
  if (!drush_valid_drupal_root($base . '/' . $core)) {
    drush_set_option('destination', $base);
    // Download a new drupal.
    
    drush_set_option('drupal-project-rename', $core);
    if (drush_invoke_args('pm-download', array($core)) === FALSE) {
      return drush_set_error('QUICK_DRUPAL_CORE_DOWNLOAD_FAIL', 'Drupal core download/extract failed.');
    }
    drush_set_option('root', $base . '/' . $core);
    
    if (!drush_get_option('db-url', FALSE)) {
      drush_set_option('db-url', 'sqlite:' . $base . '/coder-review.sqlite');
    }
    
    if (!drush_bootstrap_to_phase(DRUSH_BOOTSTRAP_DRUPAL_ROOT)) {
      return drush_set_error('QUICK_DRUPAL_ROOT_LOCATE_FAIL', 'Unable to locate Drupal root directory.');
    }  
    drush_invoke('site-install', drush_get_option('profile', 'minimal'));
  }
  drush_set_option('root', $base . '/' . $core);
  
  // Ensure the site has the coder module.
  drush_bootstrap_to_phase(DRUSH_BOOTSTRAP_DRUPAL_FULL);
  if (!module_exists('coder_review')) {
    drush_unset_option('destination');
    if (drush_invoke('pm-download', 'coder') === FALSE) {
      return drush_set_error('QUICK_DRUPAL_PROJECT_DOWNLOAD_FAIL', 'Project download/extract failed.');
    }
    drush_invoke('pm-enable', 'coder_review');
  }
}

/**
 * Standalone coder review command.
 */
function drush_standalone_coder_review() {
  
  // Work out what reviews we're going to do
  $selected_reviews = explode(',', drush_get_option('reviews', 'style'));
  $reviews = array();
  $avail_reviews = _coder_review_reviews();
  foreach ($selected_reviews as $name) {
    if (isset($avail_reviews[$name])) {
      $reviews[$name] = $avail_reviews[$name];
    }
  }
  
  // Now work out which files to scan.
  $php_extensions = variable_get('coder_review_php_ext', array('inc', 'php', 'install', 'test'));
  $include_extensions = _coder_review_get_reviews_extensions($php_extensions, $reviews);
  $includes = array_merge($php_extensions, $include_extensions);
  foreach ($includes as $k => $inc) {
    $includes[$k] = preg_quote($inc, '/');
  }
  
  $files = drush_get_option('files');
  if (!file_exists($files) && file_exists(drush_cwd() . '/' . $files)) {
    $files = drush_cwd() . '/' . $files;
  }
  
  // Despite what coder claims, files needs to be a file, not a bunch of directories.
  if (is_dir($files)) {
    $files = drush_scan_directory($files, '/.*(' . implode('|', $includes) .')$/');
  }
  else {
    $files = array($files => $files);
  }
  
  $args = array();
  $args[] = drush_get_option('format', 'checkstyle');
  $args = array_merge($args, array_keys($reviews));
  $args[] = drush_get_option('severity', 'minor');
  $args = array_merge($args, array_keys($files));
  
  drush_log(dt('Running the coder review'), 'ok');
  $output = drush_backend_invoke('coder-review', $args, 'GET', FALSE);
  
  if (empty($output['error_status'])) {
    $result = file_put_contents(drush_get_option('output-filename'), $output['output']);
    if ($result !== FALSE) {
      drush_log(dt('Results written to @filename', array('@filename' => drush_get_option('output-filename'))), 'ok');
    }
  }
  else {
    return drush_set_error('TEST', 'Call to coder failed.');
  }
}

function standalong_coder_review_command_version() {
  return 'version-1';
}

