<?php

/**
 * @file
 * Private functions used by the Project Source: GitHub module. These functions
 * should not be used by any other module; no API compatibility is guaranteed.
 */


/**
 * Formats a release info array given a GitHub API git commit ref array
 * structure, the project info array, and a file URL.
 *
 * @param array $branch
 *   A GitHub API git commit ref array structure (e.g. commit/branch)
 *
 * @param array $info
 *   A project info array as provided by hook_project_src_info()
 *
 * @return array
 *   A release array suitable for use in the primary release theme function.
 */
function _project_src_github_format_release_info($commitref, $info) {
  // Root of the project on GitHub.
  $github = variable_get('project_src_github_domain', 'https://github.com');
  $root = $github . '/' . $info['api']['path_with_namespace'];

  // Filter $commitref['name'] here, since it's used very often below.
  $commitref['name'] = check_plain($commitref['name']);

  // Save relevant values to a release array.
  $release['name'] = $info['short_name'] . ' ' . $commitref['name'];
  $release['version'] = $release['tag'] = $commitref['name'];
  $release['date'] = strtotime($commitref['commit']['committed_date']);
  $release['release_link'] = check_url($root) . '/releases/' . $release['version'];
  if (preg_match_all("/^\\d+\\.x-(\\d+)\\.([x\\d+])-?(.*)?/is", $commitref['name'], $version)) {
    $release['version_major'] = (int) $version[1][0];
    $release['version_patch'] = is_numeric($version[2][0]) ? (int) $version[2][0] : NULL;
    $release['version_extra'] = isset($version[3][0]) ? $version[3][0] : NULL;
    if (isset($version[3][0]) && $version[3][0] == 'dev') {
      $release['release_link'] = check_url($root . '/tree/' . substr($release['tag'], 0, -4));
    }
  }
  $release['status'] = 'published';

  // Details related to the download file.
  $release['archive_type'] = 'tar.gz';
  $release['download_link'] = $commitref['tarball_url'];
  $release['mdhash'] = $commitref['mdhash'];
  $release['filesize'] = $commitref['filesize'];

  return $release;
}


/**
 * Returns the contents of a file as a string given a file URL.
 *
 * @param string $file
 *   The URL of the file.
 * @param bool $private
 *   Whether or not the repository is private (and should therefore be accessed
 *   using GitHub API credentials).
 *
 * @return string
 *   The contents of the specified file or FALSE on error.
 */
function _project_src_github_file_as_string($file, $private = FALSE) {
  $ch = curl_init($file);

  // Set CURL options.
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
  curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 10);
  curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
  curl_setopt($ch, CURLOPT_FOLLOWLOCATION, true);

  // If this repository is private, add HTTP basic auth credentials.
  if ($private) {
    $un = variable_get('github_api_username', '');
    $pw = variable_get('github_api_password', '');
    curl_setopt($ch, CURLOPT_USERPWD, $un . ':' . $pw);
  }

  // If a UA string is set, set the UA option. Mostly useful for testing.
  if ($ua = variable_get('project_src_github_ua_string', FALSE)) {
    curl_setopt($ch, CURLOPT_USERAGENT, $ua);
  }

  // Execute the request and return the response.
  $contents = curl_exec($ch);
  curl_close($ch);

  return $contents;
}


/**
 * Given a file URI, it loads the Drupal file object from the database.
 *
 * @param string $uri
 *   The file URI of the file (e.g. public://my-file.jpg).
 *
 * @return stdClass
 *   The Drupal file object associated with the URI, or FALSE if none exists.
 */
function _project_src_github_load_file_by_uri($uri) {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'file')
    ->propertyCondition('uri', $uri)
    ->range(0, 1);
  $result = $query->execute();

  if (isset($result['file'])) {
    return file_load(key($result['file']));
  }
  else {
    return FALSE;
  }
}


/**
 * Replacement for file_unmanaged_delete_recursive() that takes into account
 * symlinks.
 *
 * @see file_unmanaged_delete_recursive()
 */
function _project_src_github_delete_recursive($path) {
  if (is_dir($path)) {
    foreach(array_diff(scandir($path), array('.', '..')) as $file) {
      _project_src_github_delete_recursive(drupal_realpath($path) . '/' . $file);
    }
    return drupal_rmdir($path);
  }
  return drupal_unlink($path);
}
