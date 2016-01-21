<?php

/**
 * @file Code required during release node packaging.
 *
 * In addition to some shared helper functions, this file defines the
 * interface that all CTools release_packager plugins must implement.
 *
 */

interface ProjectReleasePackagerInterface {
  /**
   * Constructor for a packager plugin class.
   *
   * @param stdClass $release_node
   *   The fully-loaded release node to create package(s) for.
   * @param string $temp_directory
   *   The full path to a temporary directory to use during packaging.
   */
  public function __construct($release_node, $temp_directory);

  /**
   * Create package(s) for the given release.
   *
   * @param array $files
   *   Reference to an associative array of files created for this release.
   *   The keys are the filename (relative to the root of the Drupal
   *   installation) and the values are the {project_release_files}.weight for
   *   each file.
   *
   * @return string
   *   The results of the packaging attempt. Must be one of:
   *   - 'success': Everything worked and new package(s) were created
   *   - 'rebuilt': Everything worked and package(s) were rebuilt
   *   - 'error': There was an error generating package(s)
   *   - 'no-op': There was nothing to do to package the given release
   */
  public function createPackage(&$files);
}

/**
 * Find the approriate packager CTools plugin for the given release.
 *
 * First, we see if $release_node->project_release['packager_plugin'] is
 * defined, and if so, we use that. Otherwise, we load the project the
 * release is attached to, and check in the project node to see if
 * $project_node->project_release['packager_plugin'] is defined. Then, we see
 * if there's a project-type-specific site-wide packager plugin variable
 * defined. Finally, we look for a global site-wide default plugin variable.
 *
 * Once we find the appropriate plugin name to use, we ask CTools to give us
 * the right class to use for that plugin, and try to instantiate an object.
 *
 * @param $release_node
 *   The fully-loaded release node to find the packaging plugin for.
 * @param string $temp_directory
 *   The full path to a temporary directory to use during packaging.
 *
 * @return
 *   An instantiated packager plugin for the given release node, or FALSE if
 *   we couldn't find or instantiate the right plugin.
 */
function project_release_get_packager_plugin($release_node, $temp_directory) {
  $packager_plugin = '';
  // If this particular release has its own plugin defined, use it.
  if (!empty($release_node->project_release['packager_plugin'])) {
    $packager_plugin = $release_node->project_release['packager_plugin'];
  }
  else {
    $project_node = node_load(project_release_get_release_project_nid($release_node));
    // If this particular project has its own plugin defined, use it.
    if (!empty($project_node->project_release['packager_plugin'])) {
      $packager_plugin = $project_node->project_release['packager_plugin'];
    }
    // Otherwise, look for a project-type-specific pluging variable.
    else {
      $packager_plugin = variable_get('project_release_packager_plugin_' . $project_node->type, '');
    }
    // If we still don't know the plugin to use, try the site-wide default.
    if (empty($packager_plugin)) {
      $packager_plugin = variable_get('project_release_packager_plugin', '');
    }
  }
  if (!empty($packager_plugin)) {
    ctools_include('plugins');
    if ($class = ctools_plugin_load_class('project_release', 'release_packager', $packager_plugin, 'packager')) {
      // Make sure the plugin implements the right interface.
      if (in_array('ProjectReleasePackagerInterface', class_implements($class, TRUE))) {
        return new $class($release_node, $temp_directory);
      }
    }
  }
  return FALSE;
}

/**
 * Update the DB with the new file info for a given release node.
 *
 * @param $release_node
 *   The fully loaded release node to update.
 * @param $release_files
 *   Array of files to add to the release node. The keys are filenames, and
 *   the values are the weight for the {project_release_file}.weight column.
 */
function project_release_packager_update_node($release_node, $release_files = array()) {
  // If the site is using DB replication, force this node_load() to use the
  // primary database to avoid node_load() failures.
  db_ignore_slave();

  if (!empty($release_files)) {
    // PHP will cache the results of stat() and give us stale answers
    // here, unless we manually tell it otherwise!
    clearstatcache();

    // Load existing files.
    $existing_files = array();
    if (!empty($release_node->field_release_files)) {
      $existing_files = array_map('field_collection_field_get_entity', $release_node->field_release_files[$release_node->language]);
    }
    // Clear out files for repopulation.
    $release_node->field_release_files[$release_node->language] = array();

    // Sort by weight and use array_keys() to get 0, 1, 2 .. ordering.
    asort($release_files);
    foreach (array_keys($release_files) as $delta => $uri) {
      $existing = FALSE;
      foreach ($existing_files as $file) {
        if ($file->field_release_file[$release_node->language][0]['uri'] === $uri) {
          // Update existing file.
          $existing = TRUE;

          // Update the timestamp and any other bookkeeping.
          file_save(file_load($file->field_release_file[$release_node->language][0]['fid']));
          // Re-populate the release node.
          $release_node->field_release_files[$release_node->language][$delta]['entity'] = $file;
        }
      }

      if (!$existing) {
        // Add new file.
        $file = new stdClass();
        $file->fid = NULL;
        $file->uri = $uri;
        $file->filename = drupal_basename($uri);
        $file->filemime = file_get_mimetype($file->uri);
        $file->uid = $release_node->uid;
        $file->status = FILE_STATUS_PERMANENT;
        file_save($file);

        $release_node->field_release_files[$release_node->language][$delta]['entity'] = entity_create('field_collection_item', array('field_name' => 'field_release_files'));
        $release_node->field_release_files[$release_node->language][$delta]['entity']->field_release_file[$release_node->language][0] = array(
          'fid' => $file->fid,
          'display' => 1,
          'description' => '',
        );
      }
    }
  }

  // Don't auto-publish security updates.
  if (project_release_is_security_update($release_node)) {
    watchdog('package_security', 'Not auto-publishing security update release.', array(), WATCHDOG_NOTICE, l(t('view'), 'node/' . $release_node->nid));
  }
  // Finally publish the node if it is currently unpublished.
  elseif ($release_node->status == NODE_NOT_PUBLISHED) {
    $release_node->status = NODE_PUBLISHED;
  }

  node_save($release_node);
}
