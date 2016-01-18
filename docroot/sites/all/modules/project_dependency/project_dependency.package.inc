<?php

/**
 * @file
 * Provides storage functions for use during release packaging.
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 */

/**
 * Clear the list of components and dependencies for a release.
 *
 * @param $release_nid
 *   The project release ID.
 */
function project_dependency_info_package_clear($release_nid) {
  $component_ids = db_query(
    "SELECT component_id FROM {project_dependency_component}
      WHERE release_nid = :release_nid",
    array(':release_nid' => $release_nid)
  )->fetchCol();

  if (!empty($component_ids)) {
    db_delete('project_dependency_dependency')
      ->condition('component_id', $component_ids, 'IN')
      ->execute();
  }
  db_delete('project_dependency_component')
    ->condition('release_nid', $release_nid)
    ->execute();
}

/**
 * Store the list of components for a release.
 *
 * @param $release_nid
 *   The project release ID.
 * @param $info
 *   Associative array of information for component keyed by the component name
 *   and providing the following keys: name, title, description.
 *
 * @return
 *   Associative array of stored records keyed by the component name with all
 *   the previous keys and the generated component_id.
 */
function project_dependency_info_package_list_store($release_nid, array $info) {
  $records = array();
  foreach ($info as $component => $component_info) {
    // These will always be insertions, because we've already deleted the old
    // ones.
    $project_dependency_component_info = array(
      'release_nid' => $release_nid,
      'name' => $component_info['name'],
      'title' => $component_info['title'],
      'description' => $component_info['description'],
    );
    $result = drupal_write_record('project_dependency_component', $project_dependency_component_info);
    $records[$component] = $project_dependency_component_info;
  }
  return $records;
}


/**
 * Store a dependency row into the project_dependency_dependency table.
 *
 * @param $release_nid
 *   The nid for the release node which contains the component that depends
 *   on the dependencies.
 * @param $component_id
 *   The component_id for the component which depends on the dependencies.
 * @param array $dependencies
 *   Array of component_name => array('external' => TRUE|FALSE')
 * @param $dependency_type
 *   The type of the dependency (PROJECT_DEPENDENCY_DEPENDENCY_*)
 */
function project_dependency_info_package_dependencies_store($release_nid, $component_id, array $dependencies, $dependency_type = PROJECT_DEPENDENCY_DEPENDENCY_REQUIRED) {
  $release = node_load($release_nid);
  $api_term = project_dependency_info_core_api($release);

  foreach ($dependencies as $dependency => $attributes) {
    $record = array(
      'component_id' => $component_id,
      'dependency' => $dependency,
      'core_api' => $api_term->name,
      'external' => !empty($attributes['external']),
      'dependency_type' => $dependency_type,
    );
    drupal_write_record('project_dependency_dependency', $record);
  }
}
