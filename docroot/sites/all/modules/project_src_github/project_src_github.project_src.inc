<?php

/**
 * @file
 * Hooks invoked by the Project Source API.
 */


/**
 * Implements hook_project_src_info().
 */
function project_src_github_project_src_info() {
  // This can be an expensive operation, so we cache our results.
  $cid = 'project_src_github_projects';
  if (!$projects = cache_get($cid)) {
    $github_client = github_api_client();
    $org = variable_get('project_src_github_org', '');
    try {
      $organizationApi = $github_client->api('organization');
      $paginator = project_src_github_get_paginator($github_client);
      $response = $paginator->fetchAll($organizationApi, 'repositories', array($org));
    }
    catch (Exception $e) {
      watchdog('project src github', 'An error occurred when attempting to load repositories for %org from GitHub. Message returned:<br /><br />@msg', array(
        '%org' => $org,
        '@msg' => $e->getMessage(),
      ), WATCHDOG_ERROR);
      $response = array();
    }

    // Build out project definitions as expected by this hook's invoker.
    foreach ($response as $project_info) {
      // Initialize this project.
      $project = array();
      $project['short_name'] = check_plain($project_info['name']);
      $project['private'] = $project_info['private'];

      // Project-wide variables.
      $project['title'] = check_plain($project_info['name']);
      $project['creator'] = check_plain($project_info['owner']['login']);
      $project['project_status'] = 'published';
      $project['link'] = check_url($project_info['html_url']);
      if (preg_match_all("/^(\\d+).x-(\\d+).x$/is", $project_info['default_branch'], $version)) {
        $project['default_major'] = check_plain($version[2][0]);
      }

      // Variables required for GitHub API handling.
      $project['api'] = array(
        'id' => $project_info['id'],
        'path_with_namespace' => $project_info['full_name'],
        'branches' => array(),
      );

      // Get all branches on this project.
      try {
        $branches = $github_client->api('repo')->branches($project['creator'], $project['short_name']);
      }
      catch (Exception $e) {
        watchdog('project src github', 'An error occurred when attempting to load branches for %project (by %creator) from GitHub. Message returned:<br /><br />@msg', array(
          '%project' => $project['creator'],
          '%creator' => $project['short_name'],
          '@msg' => $e->getMessage(),
        ), WATCHDOG_ERROR);
        $branches = array();
      }
      foreach ($branches as $branch) {
        if (preg_match_all("/^(\\d+).x-(\\d+).x$/is", $branch['name'], $versions)) {
          // Attempt to load and populate the commit date.
          try {
            $commit = $github_client->api('repo')->commits()->show($org, $project['short_name'], $branch['commit']['sha']);
            $branch['commit']['committed_date'] = $commit['commit']['author']['date'];
          }
          catch (Exception $e) {
            watchdog('project src github', 'An error occurred when attempting to load commit details for the @branch branch of %project from GitHub. Message returned:<br /><br />@msg', array(
              '@branch' => $branch['name'],
              '%project' => $project['short_name'],
              '@msg' => $e->getMessage(),
            ), WATCHDOG_ERROR);
          }

          $api_version = check_plain($versions[1][0] . '.x');
          $project['api_version'] = $api_version;
          $project['api']['branches'][$api_version][$branch['name']] = $branch;
          // @todo This isn't actually working...
          $project['supported_majors'] = implode(',', $versions[2]);
          $project['recommended_major'] = max($versions[2]);

          // Save off this project to the proper place in return array.
          $projects[$api_version][$project['short_name']] = $project;
        }
      }
    }

    // If projects were returned, cache them. Depending on the backend used to
    // generate the project, this process could be expensive.
    if (!empty($projects)) {
      cache_set($cid, $projects, 'cache');
    }
  }
  else {
    $projects = $projects->data;
  }

  return $projects;
}


/**
 * Implements hook_project_src_releases().
 */
function project_src_github_project_src_releases($short_name, $api_version, $info) {
  $public_stream_wrapper = new DrupalPublicStreamWrapper();

  // Load all of our declared projects for this API version.
  $projects = project_src_get_projects($api_version, 'project_src_github');

  // Loop through all relevant branches from GitHub and create dev releases,
  // regardless of whether or not there are any tags for a release.
  $dev_cid = 'project_src_github_releases_' . $short_name . '_' . $api_version . '_dev';
  if (!$dev_releases = cache_get($dev_cid)) {
    $dev_releases = array();
    foreach ($info['api']['branches'][$api_version] as $branch) {
      $original = $branch['name'];
      $branch['name'] = $original . '-dev';
      $public_files = $public_stream_wrapper->getDirectoryPath();
      $tarball_root = $public_files . '/project-src-github/';
      $tarball_path = $info['api']['path_with_namespace'] . '/' . $original;
      $branch['tarball_url'] = url($tarball_root . $tarball_path . '/download.tar.gz', array(
        'absolute' => TRUE,
      ));

      // Note, we're not checking locally because dev releases should never be
      // saved to the local filesystem.
      if ($file_contents = _project_src_github_file_as_string($branch['tarball_url'], $info['private'])) {
        $branch['mdhash'] = md5($file_contents);
        $branch['filesize'] = strlen($file_contents);
      }

      $dev_releases[$original] = _project_src_github_format_release_info($branch, $info);
      $dev_releases[$original]['download_link'] = str_replace($branch['name'], $original, $dev_releases[$original]['download_link']);
    }
    // Cache these using CACHE_TEMPORARY to ensure dev releases stay up-to-date.
    cache_set($dev_cid, $dev_releases, 'cache', CACHE_TEMPORARY);
  }
  else {
    $dev_releases = $dev_releases->data;
  }

  // React if the incoming project is one of ours.
  if (isset($projects[$short_name])) {
    // Get this project.
    $cid = 'project_src_github_releases_' . $short_name . '_' . $api_version;
    if (!$tags = cache_get($cid)) {
      // Attempt to load all tags for the given release and API version.
      $github_client = github_api_client();
      $org = variable_get('project_src_github_org', '');
      try {
        $tags = $github_client->api('repo')->tags($org, $short_name);
      }
      catch (Exception $e) {
        watchdog('project src github', 'An error occurred when attempting to load tags for %project (by %creator) from GitHub. Message returned:<br /><br />@msg', array(
          '%project' => $short_name,
          '%creator' => $org,
          '@msg' => $e->getMessage(),
        ), WATCHDOG_ERROR);
        $tags = array();
      }

      $releases = array();
      // Loop through all release tags from GitLab and create releases.
      foreach ($tags as $tag) {
        // Match on tags of the proper form.
        if (preg_match_all("/^(\\d+).x-(\\d+).\\d+/is", $tag['name'], $version)) {
          // Attempt to load and populate the commit date.
          try {
            $commit = $github_client->api('repo')->commits()->show($org, $short_name, $tag['commit']['sha']);
            $tag['commit']['committed_date'] = $commit['commit']['author']['date'];
          }
          catch (Exception $e) {
            watchdog('project src github', 'An error occurred when attempting to load commit details for the @tag tag of %project from GitHub. Message returned:<br /><br />@msg', array(
              '@tag' => $tag['name'],
              '%project' => $project['short_name'],
              '@msg' => $e->getMessage(),
            ), WATCHDOG_ERROR);
          }

          $api = $version[1][0] . '.x';
          // Only return releases for the current API version.
          if ($api == $api_version) {
            // Custom URL for download.
            $public_files = $public_stream_wrapper->getDirectoryPath();
            $tarball_root = $public_files . '/project-src-github/';
            $tarball_path = $info['api']['path_with_namespace'] . '/' . $tag['name'];
            $tag['tarball_url'] = url($tarball_root . $tarball_path . '/download.tar.gz', array(
              'absolute' => TRUE,
            ));

            // Attempt to load a file size and an md5 hash from the local file.
            $uri = 'public://project-src-github/' . $tarball_path . '/download.tar.gz';
            if (file_exists($uri) && $file_contents = file_get_contents($uri)) {
              // @todo As a managed file, we should be loading the file object
              //   and getting properties, not loading the file into memory.
              $tag['mdhash'] = md5($file_contents);
              $tag['filesize'] = strlen($file_contents);
            }
            else {
              if ($file_contents = _project_src_github_file_as_string($tag['tarball_url'], $info['private'])) {
                $tag['mdhash'] = md5($file_contents);
                $tag['filesize'] = strlen($file_contents);
              }
            }

            // Add the release to the master releases array.
            $releases[$tag['name']] = _project_src_github_format_release_info($tag, $info);
          }
        }
      }

      cache_set($cid, $releases, 'cache');
    }
    else {
      $releases = $tags->data;
    }
  }

  return $releases + $dev_releases;
}


/**
 * Implements hook_project_src_settings_alter().
 */
function project_src_github_project_src_settings_alter(&$settings, $context, $setting) {
  // Provide sane options for supported major versions (i.e. based on actual
  // branches, not just a default list that doesn't correspond to anything).
  if ($setting == 'supported_majors' || $setting == 'recommended_major') {
    $real_options = array();

    // Parse the actual, available branches.
    $api_version = $context['project']['api_version'];
    if (isset($context['project']['api']['branches'][$api_version])) {
      $branches = $context['project']['api']['branches'][$api_version];
      foreach ($branches as $branch => $info) {
        if (preg_match_all("/^(\\d+).x-(\\d+).x$/is", $branch, $versions)) {
          $real_options[$versions[2][0]] = $branch;
        }
      }
    }

    // Set our real options.
    $settings['element']['#options'] = $real_options;
  }
}
