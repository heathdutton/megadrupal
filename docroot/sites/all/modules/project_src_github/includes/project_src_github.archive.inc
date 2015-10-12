<?php

/**
 * @file
 * Project Source: GitHub callback handler for tarball repackaging.
 */


/**
 * Menu callback that returns the archive from GitHub in a way that drush and
 * other associated package handling systems can digest.
 *
 * @param $namespace
 *   The namespace (or group) under which the project exists.
 * @param $project_name
 *   The name of the project.
 * @param $version
 *   The version of the project.
 *
 * @return
 *   The archive as a string, suitable for streaming.
 */
function project_src_github_get_archive($namespace, $project_name, $version) {
  // Determine whether or not the provided repository is private.
  $api_version = substr($version, 0, 3);
  $projects = project_src_get_projects($api_version, 'project_src_github');
  $private = $projects[$project_name]['private'];

  // Provide a shortcut for -dev releases (which should never be saved to the
  // local file system).
  if (substr($version, -1) == 'x') {
    drupal_add_http_header('Content-type', 'application/x-gzip');
    drupal_add_http_header('Content-transfer-encoding', 'binary');
    drupal_add_http_header('Content-disposition', 'attachment; filename="' . $project_name . '-' . $version . '.tar.gz"');

    return _project_src_github_repackage_tarball($namespace, $project_name, $version, '', $private);
  }

  // The directory in which we'll ultimately save the re-packaged file.
  $destination = format_string('public://project-src-github/!ns/!pn/!v', array(
    '!ns' => $namespace,
    '!pn' => $project_name,
    '!v' => $version,
  ));

  // Don't start repackaging a tarball if the package already exists or if
  // repackaging is in progress in another thread.
  $lock_name = __FUNCTION__ . '-' . $project_name . '-' . $version;
  if (!file_exists($destination . '/download.tar.gz')) {
    $lock_acquired = lock_acquire($lock_name);
    if (!$lock_acquired) {
      // Tell the client to retry again in 10 seconds.
      drupal_add_http_header('Status', '503 Service Unavailable');
      drupal_add_http_header('Retry-After', 10);
      print t('Tarball packaging in progress. Try again shortly.');
      drupal_exit();
    }
  }

  // Try to repackage the tarball, unless another thread just did it while we
  // were acquiring the lock.
  $success = file_exists($destination . '/download.tar.gz') || _project_src_github_repackage_tarball($namespace, $project_name, $version, $destination, $private);

  if (!empty($lock_acquired)) {
    lock_release($lock_name);
  }

  if ($success) {
    drupal_add_http_header('Content-type', 'application/x-gzip');
    drupal_add_http_header('Content-transfer-encoding', 'binary');
    drupal_add_http_header('Content-disposition', 'attachment; filename="' . $project_name . '-' . $version . '.tar.gz"');

    if ($success === TRUE) {
      return file_get_contents($destination . '/download.tar.gz');
    }
    else {
      return $success;
    }
  }
  else {
    watchdog('project src github', 'An error occurred while attempting to repackage the following tarball: %tar', array(
      '%tar' => $destination,
    ), WATCHDOG_ERROR);
    drupal_add_http_header('Status', '500 Internal Server Error');
    print t('Error repackaging tarball.');
    drupal_exit();
  }
}


/**
 * Gets and repackages a GitHub packaged tarball in a form more familiar to
 * drush and other Drupal package management tools.
 *
 * @param string $namespace
 *   The namespace of the project in question.
 * @param string $project
 *   The name of the project in question.
 * @param string $version
 *   The version of the project in question.
 * @param string $managed_destination
 *   The destination directory of the repackaged tarball.
 * @param bool $private
 *   Whether or not the archive we're loading corresponds to a private repo.
 *
 * @return string
 *   The contents of a tarball suitable for use with Drush, etc.
 */
function _project_src_github_repackage_tarball($namespace, $project, $version, $managed_destination, $private = FALSE) {
  $github = variable_get('project_src_github_domain', 'https://github.com');
  $extension = $namespace . '/' . $project;
  $get = $github . '/' . $extension . '/archive/' . $version . '.tar.gz';
  $tarball = _project_src_github_file_as_string($get, $private);

  $file = '';

  // Save the tarball to the temp directory.
  $tar_name = md5($get . microtime()) . '.tar.gz';
  $tar_file = file_unmanaged_save_data($tarball, 'temporary://' . $tar_name);

  // Get an archiver and extract the GitHub tarball into its own directory.
  $tar_extracted = 'temporary://' . $tar_name . '.extract';
  file_prepare_directory($tar_extracted, FILE_CREATE_DIRECTORY);
  $tar = archiver_get_archiver($tar_file);
  $tar->extract(drupal_realpath($tar_extracted));

  $source = $tar_extracted . '/' . $project . '-' . $version;
  $destination_dir = 'temporary://' . md5($project . '-' . $version . microtime());
  $destination = $destination_dir . '/' . $project;

  // Drush expects a folder without version appended to it.
  if (mkdir($destination_dir) && rename($source, $destination)) {
    // Append package details to the info file.
    $info_file = $destination . '/' . $project . '.info';
    if (file_exists($info_file)) {
      $info = _project_src_github_get_info($project, $version);
      file_put_contents($info_file, implode("\n", $info) . "\n", FILE_APPEND);
    }

    // Create a new tar extract and add the relevant package contents; in doing
    // so, ensure that the contents exist in the new package at the root.
    $final_extract = 'temporary://' . $tar_name . '.final';
    $new_tar = new Archive_Tar($final_extract);
    $new_tar->addModify(drupal_realpath($destination), '', drupal_realpath($destination_dir));
    $file = file_get_contents($final_extract);

    // Save the file as a managed file entry.
    if (!empty($managed_destination)) {
      if (file_prepare_directory($managed_destination, FILE_CREATE_DIRECTORY)) {
        file_save_data($file, $managed_destination . '/download.tar.gz', FILE_EXISTS_REPLACE);
      }
    }

    // Clean up our new archive.
    _project_src_github_delete_recursive($destination_dir);
    file_unmanaged_delete($final_extract);
  }

  // Clean up all remaining temporary files.
  file_unmanaged_delete($tar_file);
  _project_src_github_delete_recursive($tar_extracted);

  return $file;
}


/**
 * Returns data to be appended to a Drupal project's .info file.
 *
 * @param string $project_name
 *   The short name of the project in question.
 * @param string $version
 *   The version of the project (e.g. 7.x-1.0 or 7.x-1.x).
 *
 * @return array
 *   Returns an array of .info strings to be appended to an existing project
 *   .info file.
 */
function _project_src_github_get_info($project_name, $version) {
  // Prepare the defaults.
  $info = array(
    'newline' => "\n",
    'comment' => '; Project Source: GitHub packaging details.',
    'project status url' => 'project status url = ' . url('drupal/release-history', array('absolute' => TRUE)),
    'project' => 'project = ' . $project_name,
    'version' => 'version = ' . $version,
  );

  // Prepare to make use of the GitHub API client to get package date.
  $github_client = github_api_client();
  $org = variable_get('project_src_github_org', '');

  // If the version ends in an "x," we mean to get a dev release from a branch.
  if (substr($version, -1) == 'x') {
    try {
      $releases = $github_client->api('repo')->branches($org, $project_name);
      $info['version'] .= '-dev';
    }
    catch (Exception $e) {
      watchdog('project src github', 'An error occurred when attempting to load branches for %project (by %creator) from GitHub. Message returned:<br /><br />@msg', array(
        '%project' => $pname,
        '%creator' => $org,
        '@msg' => $e->getMessage(),
      ), WATCHDOG_ERROR);
      $releases = array();
    }
  }
  // Otherwise, we're getting info for a tagged release.
  else {
    try {
      $releases = $github_client->api('repo')->tags($org, $project_name);
    }
    catch (Exception $e) {
      watchdog('project src github', 'An error occurred when attempting to load tags for %project (by %creator) from GitHub. Message returned:<br /><br />@msg', array(
        '%project' => $pname,
        '%creator' => $org,
        '@msg' => $e->getMessage(),
      ), WATCHDOG_ERROR);
      $releases = array();
    }
  }

  // Iterate through all releases returned.
  foreach ($releases as $release) {
    // If the release matches our version, check the timestamp.
    if ($release['name'] == $version) {
      $commit = $github_client->api('repo')->commits()->show($org, $project_name, $release['commit']['sha']);
      $info['datestamp'] = 'datestamp = ' . strtotime($commit['commit']['author']['date']);
      break;
    }
  }

  return $info;
}
