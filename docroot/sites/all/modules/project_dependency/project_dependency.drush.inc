<?php
/**
 * @file
 * Provide Drush integration for release building and dependency building.
 */

/**
 * Implements hook_drush_command().
 */
function project_dependency_drush_command() {
  $items = array();

  $items['project-dependency-process-project'] = array(
    'description' => 'Build dependencies for a given project shortname.',
    'callback' => 'drush_project_dependency_process_project',
    'arguments' => array(
      'shortname' => 'Project shortname/uri',
    ),
    'aliases' => array('pdpp'),
  );
  $items['project-dependency-process-version'] = array(
    'description' => 'Build dependencies for a given project version (primarily for debugging).',
    'callback' => 'drush_project_dependency_process_version',
    'arguments' => array(
      'shortname' => 'Project shortname/uri',
      'version' => 'Version (like 7.x-3.x-dev)',
    ),
    'aliases' => array('pdpv'),
  );
  $items['project-dependency-show-dependencies'] = array(
    'description' => 'Show the dependencies of the given release node',
    'callback' => 'drush_project_show_dependencies',
    'arguments' => array(
      'shortname' => 'Shortname of the project (uri)',
      'version' => 'Version of the project (like "7.x-1.x-dev"',
    ),
    'aliases' => array('pdsd'),
  );

  return $items;
}



/**
 * Update and save all dependencies for the project named.
 *
 * @param $shortname
 *   Any number of project shortnames to be processed
 */
function drush_project_dependency_process_project() {
  module_load_include('inc', 'project_dependency', 'project_dependency.drupal');

  $args = func_get_args();

  foreach ($args as $shortname) {
    drush_print("Processing dependencies for $shortname");

    // Get all versions for a project and build dependencies for them.
    $pid = project_get_nid_from_machinename($shortname);

    // Find all releases for a project.
    $query = new EntityFieldQuery();
    $releases = $query->entityCondition('entity_type', 'node')
      ->entityCondition('bundle', 'project_release')
      ->fieldCondition('field_release_project', 'target_id', $pid)
      ->propertyOrderBy('nid', 'DESC')
      ->execute();

    $release_nodes = array();
    if (!empty($releases['node'])) {
      $release_nodes = node_load_multiple(array_keys($releases['node']));
    }
    foreach ($release_nodes as $node) {
      $dependencies = project_dependency_process_release($shortname, $node);

      $wrapper = entity_metadata_wrapper('node', $node);
      $version = $wrapper->field_release_vcs_label->value();
      if ($dependencies === FALSE) {
        drush_print("Skipped (or git failed) $shortname: {$version}");
        continue;
      }
      drush_print_r("Processed $shortname: {$version}");
    }
  }
}


/**
 * Process a single release node (version)
 * @param $shortname
 *   The project shortname (uri)
 * @param $version
 *   The version string (like 7.x-3.x-dev)
 */
function drush_project_dependency_process_version($shortname, $version) {
  module_load_include('inc', 'project_dependency', 'project_dependency.drupal');
  drush_print("Processing dependencies for $shortname: $version");

  // Load the project.
  $pid = project_get_nid_from_machinename($shortname);
  $query = new EntityFieldQuery();
  $releases = $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'project_release')
    ->fieldCondition('field_release_project', 'target_id', $pid)
    ->fieldCondition('field_release_version', 'value', $version)
    ->propertyOrderBy('nid', 'DESC')
    ->execute();

  $node = node_load(current($releases['node'])->nid);

  $dependencies = project_dependency_process_release($shortname, $node);
  if ($dependencies === FALSE) {
    drush_print("Skipped $shortname: $version");
    return;
  }
  drush_print_r($dependencies);
}

/**
 * Show dependencies for a given project + version
 *
 * @param $shortname
 *   Project shortname (uri) like 'views'
 * @param unknown_type $version
 *   Project version, like '7.x-1.1'
 */
function drush_project_show_dependencies($shortname, $version) {
  module_load_include('inc', 'project_dependency', 'project_dependency.drupal');

  // Find the release nid for the given shortname + version
  $pid = project_get_nid_from_machinename($shortname);
  $query = new EntityFieldQuery();
  $releases = $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'project_release')
    ->fieldCondition('field_release_project', 'target_id', $pid)
    ->fieldCondition('field_release_version', 'value', $version)
    ->propertyOrderBy('nid', 'DESC')
    ->range(0, 1)
    ->execute();

  if (empty($releases['node'])) {
    drush_print_r("Failed to load a release for $shortname:$version");
    return;
  }
  $nid = current($releases['node'])->nid;
  $dependent_releases = project_dependency_get_external_release_dependencies($nid);

  drush_print_r($dependent_releases);
}
