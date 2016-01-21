<?php

/**
 * @file
 * Contains admin UI callbacks.
 */

/**
 * Page callback for the bundle admin base page.
 *
 * This shows a list of links to the various bundles' admin pages. This is
 * analogous to admin/structure/types, and hence modelled on
 * node_overview_types().
 *
 * @param $entity_type
 *  The entity type.
 */
function entity_bundle_admin_types_list_page($entity_type) {
  $entity_info = entity_get_info($entity_type);
  $base_path = $entity_info['bundle admin ui']['path'] . '/manage';

  module_load_include('inc', 'node', 'content_types');

  $header = array(t('Name'), array(
    'data' => t('Operations'),
    'colspan' => 3,
  ));
  $rows = array();

  foreach ($entity_info['bundles'] as $bundle_name => $bundle_info) {
    // We have to hack the bundle info into an object for node_admin_overview(),
    // but it's handy to piggyback this.
    $bundle_object = (object) $bundle_info;
    $bundle_object->type = $bundle_name;

    $row = array(theme('node_admin_overview', array('name' => $bundle_info['label'], 'type' => $bundle_object)));

    // Set the info column.
    $row[] = array('data' => l(t('info'), "$base_path/$bundle_name"));

    // Manage fields.
    // We're just going to assume FieldUI is present: why would you use this
    // module if not???
    $row[] = array('data' => l(t('manage fields'), "$base_path/$bundle_name/fields"));

    // Display fields.
    $row[] = array('data' => l(t('manage display'), "$base_path/$bundle_name/display"));

    $rows[] = $row;
  }

  $build['bundle_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No bundles available.'),
  );

  return $build;
}

/**
 * Page callback for a generic bundle's admin page.
 *
 * @param $entity_type
 *  The entity type.
 * @param $bundle_name
 *  The bundle name.
 */
function entity_bundle_admin_types_type_page($entity_type, $bundle_name) {
  $entity_info = entity_get_info($entity_type);

  return isset($entity_info['bundles'][$bundle_name]['description'])
    ? $entity_info['bundles'][$bundle_name]['description']
    : t('Entity bundle @bundle.', array('@bundle' => $entity_info['bundles'][$bundle_name]['label']));
}
