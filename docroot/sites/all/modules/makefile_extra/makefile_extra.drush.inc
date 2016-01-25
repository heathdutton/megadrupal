<?php
/**
 * @file
 * Drush extension to check and update drush make files contrib modules.
 */

/**
 * Implements hook_drush_command().
 */
function makefile_extra_drush_command() {
  $commands = array();

  $commands['makefile-check'] = array(
    'description' => dt('Check drush makefile for module/themes updates.'),
    'arguments' => array(
      'make file' => dt('Location of a make file to check.'),
    ),
    'options' => array(
      'no-cache' => dt('Do not cache the projects update information.'),
      'exclude' => dt('Comma delimited list of projects not to update (exclude from the process)'),
    ),
    'aliases' => array('mfch'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );

  $commands['makefile-combine'] = array(
    'description' => dt('Process a makefile and it\'s includes, creating a single makefile.'),
    'arguments' => array(
      'make file in' => dt('Location of the make file to use as input'),
      'make file out' => dt('Location of the make file to write to.')
    ),
    'options' => array(
      'no-cache' => dt('Do not cache the projects update information.'),
    ),
    'aliases' => array('mfc'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );

  $commands['makefile-update'] = array(
    'description' => dt('Update drush makefile for module/themes updates.'),
    'arguments' => array(
      'make file in' => dt('Location of the make file to use as input'),
      'make file out' => dt('Location of the make file to write to.')
    ),
    'options' => array(
      'no-cache' => dt('Do not cache the projects update information.'),
      'exclude' => dt('Comma delimited list of projects not to update (exclude from the process)'),
    ),
    'aliases' => array('mfup'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );

  return $commands;
}

/**
 * Command callback to display table over project/themes that needs to be
 * updated in the make file given.
 *
 * @param string $makefile
 *  Location of the make file to analyse.
 */
function drush_makefile_extra_makefile_check($makefile) {
  // Parse make file.
  $info = make_parse_info_file($makefile);

  // Validate the information parsed.
  if ($info === FALSE || ($info = make_validate_info_file($info)) === FALSE) {
    return FALSE;
  }

  // Find project that needs to be updated.
  $projects = makefile_extra_find_updates($info, $makefile);

  // Output table with projects that needs to be updated.
  makefile_extra_print_projects($projects);

  // If this command is call from a shell script the status have to be a
  // non-zero value to be useful in if statements.
  if (count($projects) > 0) {
    return drush_set_error('MAKEFILE_UPDATES_AVAILABLE', dt('There are updates available for projects in your makefile!'));
  }
}

/**
 * Command callback to generate make file based on includes.
 *
 * @param string $makefile
 *  Location of the make file to analyse.
 * @param string $file
 *  Location of the make file to be generated based on the includes.
 * @return boolean
 *  Returns TRUE on succes else FALSE.
 */
function drush_makefile_extra_makefile_combine($makefile, $file = NULL) {
    // Parse make file.
  $info = make_parse_info_file($makefile);

  // Validate the information parsed.
  if ($info === FALSE || ($info = make_validate_info_file($info)) === FALSE) {
    return FALSE;
  }

  if (!empty($info) && drush_confirm('Should the make file be written')) {
    $contents = _drush_makefile_extra_generate_contents($info);
    if (!$file) {
      drush_print($contents);
    }
    elseif (file_put_contents($file, $contents)) {
      drush_log(dt("Wrote .make file @file", array('@file' => $file)), 'ok');
    }
    else {
      drush_make_error('FILE_ERROR', dt("Unable to write .make file !file", array('!file' => $file)));
    }
  }

  return TRUE;
}

/**
 * Command callback to generate make file based on the updates found.
 *
 * @param string $makefile
 *  Location of the make file to analyse.
 * @param string $file
 *  Location of the make file to be generated based on the updates found.
 * @return boolean
 *  Returns TRUE on succes else FALSE.
 */
function drush_makefile_extra_makefile_update($makefile, $file = NULL) {
    // Parse make file.
  $info = make_parse_info_file($makefile);

  // Validate the information parsed.
  if ($info === FALSE || ($info = make_validate_info_file($info)) === FALSE) {
    return FALSE;
  }

  $projects = makefile_extra_find_updates($info, $makefile);
  foreach ($projects as $project) {
    $info['projects'][$project['name']]['version'] = $project['recommanded'];
  }

  makefile_extra_print_projects($projects);

  if (!empty($projects) && drush_confirm('Should the make file be written')) {
    $contents = _drush_makefile_extra_generate_contents($info);
    if (!$file) {
      drush_print($contents);
    }
    elseif (file_put_contents($file, $contents)) {
      drush_log(dt("Wrote .make file @file", array('@file' => $file)), 'ok');
    }
    else {
      drush_make_error('FILE_ERROR', dt("Unable to write .make file !file", array('!file' => $file)));
    }
  }

  return TRUE;
}

/**
 * Generate the actual contents of the .make file.
 *
 * This is a modified verions of drush core make generator.
 */
function _drush_makefile_extra_generate_contents($info) {
  $header = array();
  $header[] = '; This file was auto-generated by drush make';
  $header[] = '';
  $header['core'] = $info['core'];
  $header['api'] = $info['api'];
  $header[] = '';

  $output = _drush_makefile_extra_generate_body($info['projects'], 'projects', $header);
  if (isset($info['libraries'])) {
    $output .= _drush_makefile_extra_generate_body($info['libraries'], 'libraries');
  }

  return $output;
}

/**
 * Generate the make file body for a give type (project/libraries).
 *
 * @param type $projects
 * @param type $type
 * @param type $output
 * @return string
 */
function _drush_makefile_extra_generate_body($projects, $type = 'projects', $output = array()) {
  $output[] = '; ' . ucfirst($type);

  foreach ($projects as $name => $project) {
    if (!$project && is_string($name)) {
      $output[] = $type . '[] = "' . $name . '"';
      continue;
    }
    $base = $type . '[' . $name . ']';
    foreach ($project as $key => $value) {
      if (is_array($value)) {
        foreach ($value as $subkey => $item) {
          $output[$base . '[' . $key . '][' . $subkey . ']'] = '"' . $item . '"';
        }
      }
      else {
        $output[$base . '[' . $key . ']'] = '"' . $value . '"';
      }
    }
    $output[] = '';
  }
  $string = '';
  foreach ($output as $k => $v) {
    if (!is_numeric($k)) {
      $string .= $k . ' = ' . $v;
    }
    else {
      $string .= $v;
    }
    $string .= "\n";
  }

  return $string;
}

/**
 * Prints projects as an table.
 *
 * @param array $projects
 */
function makefile_extra_print_projects($projects) {
  if (!empty($projects)) {
    // First row is the table header in the output.
    $rows = array(
      array(
        dt('Project'),
        dt('Current'),
        dt('Recommanded'),
        dt('Alternative'),
        dt('Date'),
        dt('Status')
      ),
    );

    // Loop over the projects.
    foreach ($projects as $project) {
      $rows[] = array(
        $project['name'],
        $project['version'],
        $project['recommanded'],
        $project['alternative'],
        !empty($project['date']) ? gmdate('d-M-Y', $project['date']) : '',
        implode(', ', $project['status']),
      );
    }

    // Print table.
    drush_print_table($rows, TRUE);
  }
  else {
    drush_log(dt('Your make file is up-to-date and do not need to be updated.'), 'success');
    drush_print('');
  }
}

/**
 * Find the projects that needs to be updated.
 *
 * @param type $makefile
 *  The location of the make file to parse.
 * @param string $makefile
 *  Makefile localtion.
 * @return mixed
 *  A array with associtive arrays containing the following keys: name, version,
 *  recommanded_version, do_recommanded_version, date and status.
 */
function makefile_extra_find_updates($info, $makefile) {
  // Get Drupal core version form the make file.
  $core_version = $info['core'];

  // Load release info helpers.
  drush_include_engine('release_info', 'updatexml');

  // Check the exclude option.
  $excludes = array();
  if ($exclude_str = drush_get_option('exclude')) {
    $excludes = explode(',', $exclude_str);
  }

  // Look up projects release information in cache to speed up preformance. If
  // it have not expired or the --no-cache option is given.
  $projects = array();
  $cid = 'makefile:' . md5($makefile);
  $cache = drush_cache_get($cid);
  if (!drush_get_option('no-cache') && $cache && $cache->expire > time()) {
    $projects = $cache->data;

    // Remove excluded from the cached verion.
    if (!empty($excludes)) {
      foreach ($projects as $key => $project) {
        if (in_array($project['name'], $excludes)) {
          unset($projects[$key]);
        }
      }
    }
  }
  else {
    // Loop over the projects.
    foreach ($info['projects'] as $name => $project) {
      // Check if the project is a dev version and skip it if it is.
      if (preg_match('/-dev$/', $project['version'])) {
        continue;
      }

      // Check if project is excluded and skip if it is.
      if (in_array($name, $excludes)) {
        continue;
      }

      // Get info about the project.
      $requests = pm_parse_project_version(array($name));
      $release_info = release_info_get_releases($requests);

      // No information was fetched.
      if (empty($release_info)) {
        continue;
      }

      // Get info for the module.
      $release_info = $release_info[$name];

      // Try to find the right version.
      if (!isset($project['verions'])) {
        $project_major = makefile_extra_get_version_major($project);
      }
      else {
        drush_log('Project version not found for %project', array('%project' => $name), 'error');
        continue;
      }

      // Parse recommanded version string.
      $recommanded = $release_info['recommended'];
      preg_match('/' . $core_version . '/', $recommanded, $matches);
      if (!empty($matches)) {
        $recommanded = substr($release_info['recommended'], 4);
      }

      // If the make file version differes from the recommanded.
      if ($project['version'] != $recommanded) {
        $releases = release_info_filter_releases($release_info['releases'], FALSE);

        // Get recommanded version based on project major version.
        $recommanded_version = makefile_extra_find_recommanded_version($releases, $release_info, $project_major);

        // Test if the found verions base on major release is the same.
        if (($core_version . '-' . $project['version']) != $recommanded_version) {
          $pattern = $core_version . '-';
          $release = $releases[$recommanded_version];
          $projects[] = array(
            'name' => $name,
            'version' => $project['version'],
            'recommanded' =>  str_replace($pattern, '', $recommanded_version),
            'alternative' => str_replace($pattern, '', $release_info['recommended']),
            'date' => isset($release['date']) ? $release['date'] : NULL,
            'status' => isset($release['release_status']) ? $release['release_status'] : array(),
          );
        }
      }

      drush_log(dt('Fetched project information: %p', array('%p' => $name)), 'success');
    }

    // Store collected projects in the cache.
    drush_cache_set($cid, $projects, 'default', time() + 600);
  }

  // Make some space after all the fetached messages.
  drush_print('');

  return $projects;
}

/**
 * Find a projects recommanded version based on the current project major
 * version and release date. The reason behind not using d.o's recommanded
 * version it to stay on the same release branch.
 *
 * @param array $releases
 *  Releases information from d.o about the project from
 *  release_info_filter_releases() function.
 * @param array $release_info
 *  All information about all releases.
 * @param string $project_major
 *  Project major project version (1,2 etc.)
 * @return string
 *  The version recommanded.
 */
function makefile_extra_find_recommanded_version($releases, $release_info, $project_major) {
  // Try to find alternative release (e.g 2.x vs 1.x) base on current project
  // major version that is supported.
  $versions = array();
  foreach ($releases as $version => $release) {
    if ($release['version_major'] == $project_major && in_array('Supported', $release['release_status'])) {
      $versions[$version] = $release['date'];
    }
  }

  // Based on release date find recommanded major version.
  $max = 0;
  $recommanded_version = $release_info['recommended'];
  foreach ($versions as $version => $date) {
    if ($max < $date) {
      $max = $date;
      $recommanded_version = $version;
    }
  }

  return $recommanded_version;
}

/**
 * Find project major verions.
 *
 * @param array $project
 *  Array with $project information from the makefile.
 * @return string
 *  The project major version.
 */
function makefile_extra_get_version_major($project) {
  $parts = explode('.', $project['version']);
  return $parts[0];
}
