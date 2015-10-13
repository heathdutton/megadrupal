<?php

/**
 * @file
 * Drush integration for WYSIWYG editors.
 */

/**
 * Implements drush_COMMANDFILE_COMMANDNAME_validate().
 */
function drush_drush_editor_editor_download_validate() {
  // Validate the user specified destination directory.
  $destination = drush_get_option('destination');

  if (!empty($destination)) {
    $destination = rtrim($destination, DIRECTORY_SEPARATOR);

    if (!is_dir($destination)) {
      drush_print(dt("The directory !destination does not exist.", array('!destination' => $destination)));

      if (!drush_get_context('DRUSH_SIMULATE')) {
        if (drush_confirm(dt('Would you like to create it?'))) {
          drush_mkdir($destination, TRUE);
        }

        if (!is_dir($destination)) {
          return drush_set_error('DRUSH_EDITOR_NO_DESTINATION', dt('Unable to create destination directory !destination.', array('!destination' => $destination)));
        }
      }
    }

    if (!is_writable($destination)) {
      return drush_set_error('DRUSH_EDITOR_NO_DESTINATION', dt('Destination directory !destination is not writable.', array('!destination' => $destination)));
    }

    // Ignore --use-site-dir, if given.
    if (drush_get_option('use-site-dir', FALSE)) {
      drush_set_option('use-site-dir', FALSE);
    }
  }

  $select = drush_get_option('select', FALSE);
  $build  = drush_get_option('build', FALSE);

  if ($select && $build) {
    drush_set_option('select', FALSE);
  }
}

/**
 * Implements drush_COMMANDFILE_COMMANDNAME().
 */
function drush_drush_editor_editor_download() {
  if (!$requests = drush_editor_parse_arguments(func_get_args(), FALSE)) {
    $requests = array('ckeditor');
  }

  // Parse out project name and version.
  $requests = drush_editor_parse_version($requests);
  $manager  = new DrushEditorManager();
  $error    = FALSE;
  $editors  = array();

  // Validate editor name.
  foreach ($requests as $key => $request) {
    if ($editor = $manager->create($request['name'], $request['major_version'], $request['version'], $request['preset'])) {
      if ($editor->validateRequest()) {
        $requests[$key]['editor'] = $editor;
      }
      else {
        $error = TRUE;
      }
    }
    else {
      $error = TRUE;
      drush_set_error('DRUSH_EDITOR_NOT_FOUND', dt('Editor !editor is not found.', array('!editor' => $request['name'])));
    }
  }

  if ($error) {
    return FALSE;
  }

  $select = drush_get_option('select', FALSE);

  foreach ($requests as $key => $request) {
    $editor = $request['editor'];

    if ($select) {
      $versions = $editor->discover();

      if (empty($versions)) {
        drush_set_error('DRUSH_EDITOR_DISCOVER_NO_VERSION', dt('Editor !editor has no version to download.', array('!editor' => $editor->getName())));
      }
      else {
        $options = array();

        if (empty($request['major_version'])) {
          foreach ($versions as $major_version => $minor_versions) {
            $options[key($minor_versions)] = reset($minor_versions);
          }
        }
        else {
          $options = $versions[$requests[$key]['major_version']];
        }

        $choice = drush_choice($options, dt('Choose one of the available releases for !editor:', array('!editor' => $editor->getName())));

        if (!$choice) {
          return drush_user_abort();
        }

        $editor->select($choice);
      }
    }

    $editor_path = drush_editor_package_handler_download_editor($request);

    if (!$editor_path) {
      drush_log(dt('Error downloading !name', array('!name' => $editor->getName()), 'error'));
      continue;
    }

    // Determine the install location for the project.  User provided
    // --destination has preference.
    $destination = drush_get_option('destination');

    if (!empty($destination)) {
      if (!file_exists($destination)) {
        drush_mkdir($destination);
      }

      $install_location = realpath($destination);
    }
    else {
      $install_location = _drush_editor_download_destination();
    }

    $install_location .= DIRECTORY_SEPARATOR . $editor->getLibraryName();

    // Check if install location already exists.
    if (is_dir($install_location)) {
      if (drush_confirm(dt('Install location !location already exists. Do you want to overwrite it?', array('!location' => $install_location)))) {
        drush_delete_dir($install_location, TRUE);
      }
      else {
        drush_log(dt("Skip installation of !editor to !dest.", array('!editor' => $editor->getName(), '!dest' => $install_location)), 'warning');
        continue;
      }
    }

    // Copy the project to the install location.
    if (drush_op('_drush_recursive_copy', $editor_path, $install_location)) {
      drush_log(
        dt(
          "Editor !editor (!version) downloaded to !dest.",
          array(
            '!editor'  => $editor->getName(),
            '!version' => $editor->getRequestedVersion(),
            '!dest'    => $install_location,
          )
        ),
        'success'
      );
    }
    else {
      // We don't `return` here in order to proceed with downloading additional
      // projects.
      drush_set_error('DRUSH_drush_editor_download_FAILED',
        dt(
          "Editor !editor (!version) could not be downloaded to !dest.",
          array(
            '!editor'  => $editor->getName(),
            '!version' => $editor->getRequestedVersion(),
            '!dest'    => $install_location,
          )
        )
      );
    }
  }
}

/**
 * Returns the best destination for a particular download type we can find.
 *
 * It is based on the project type and drupal and site contexts.
 *
 * @return string
 *   Best installation path.
 */
function _drush_editor_download_destination() {
  $drupal_root    = drush_get_context('DRUSH_DRUPAL_ROOT');
  $site_root      = drush_get_context('DRUSH_DRUPAL_SITE_ROOT', FALSE);
  $full_site_root = $drupal_root . '/' . $site_root;
  $sites_all      = $drupal_root . '/sites/all';

  $in_site_directory = FALSE;

  // Check if we are running within the site directory.
  if (strpos(realpath(drush_cwd()), realpath($full_site_root)) !== FALSE || (drush_get_option('use-site-dir', FALSE))) {
    $in_site_directory = TRUE;
  }

  $destination = '';

  // Attempt 1: If we are in a specific site directory, and the destination
  // directory already exists, then we use that.
  if (empty($destination) && $site_root && $in_site_directory) {
    $create_dir  = drush_get_option('use-site-dir', FALSE);
    $destination = _drush_editor_download_destination_lookup($drupal_root, $full_site_root, $create_dir);
  }
  // Attempt 2: If the destination directory already exists for sites/all,
  // then we use that.
  if (empty($destination) && $drupal_root) {
    $destination = _drush_editor_download_destination_lookup($drupal_root, $sites_all);
  }
  // Attempt 3: If a specific (non default) site directory exists and
  // sites/all does not exist, then we create destination in the site
  // specific directory.
  if (empty($destination) && $site_root && $site_root !== 'sites/default' && is_dir($full_site_root) && !is_dir($sites_all)) {
    $destination = _drush_editor_download_destination_lookup($drupal_root, $full_site_root, TRUE);
  }
  // Attempt 4: If sites/all exists, then we create destination in the
  // sites/all directory.
  if (empty($destination) && is_dir($sites_all)) {
    $destination = _drush_editor_download_destination_lookup($drupal_root, $sites_all, TRUE);
  }
  // Attempt 5: If site directory exists (even default), then we create
  // destination in the this directory.
  if (empty($destination) && $site_root && is_dir($full_site_root)) {
    $destination = _drush_editor_download_destination_lookup($drupal_root, $full_site_root, TRUE);
  }

  // Attempt 6: If we didn't find a valid directory yet (or we somehow found
  // one that doesn't exist) we always fall back to the current directory.
  if (empty($destination) || !is_dir($destination)) {
    $destination = drush_cwd();
  }

  return $destination;
}

/**
 * Determine a candidate destination directory for a particular site path.
 *
 * Return it if it exists, optionally attempting to create the directory.
 *
 * @param string $drupal_root
 *   Path to drupal root.
 *
 * @param string $sitepath
 *   Current site path.
 *
 * @param bool $create
 *   Determine to create new directory.
 *
 * @return mixed
 *   Path to directory or FALSE.
 */
function _drush_editor_download_destination_lookup($drupal_root, $sitepath, $create = FALSE) {
  $destination = $sitepath . '/libraries';

  if ($create) {
    drush_log(dt('Attempting to create destination directory at !dir', array('!dir' => $destination)));
    drush_mkdir($destination, TRUE);
  }

  if (is_dir($destination)) {
    drush_log(dt('Using destination directory !dir', array('!dir' => $destination)));
    return $destination;
  }

  drush_log(dt('Could not find destination directory at !dir', array('!dir' => $destination)));

  return FALSE;
}
