<?php

/**
 * Implements hook_menu().
 */
function _xc_index_menu() {
  $items['admin/xc/index'] = array(
    'title' => 'Indexing',
    'description' => 'Indexing settings. Alert! These are advanced settings, do not modify before consulting the manual.',
    'file' => 'system.admin.inc',
    'file path' => drupal_get_path('module', 'system'),
    'page callback' => 'system_admin_menu_block_page',
    'access arguments' => array(ADMINISTER_XC_INDEX),
    'type' => MENU_NORMAL_ITEM,
    'weight' => 3,
  );

  $items['admin/xc/index/attribute'] = array(
    'title' => 'Attribute handling',
    'description' => 'How to index metadata schema attribute?',
    'page callback' => 'xc_index_attribute_view',
    'access arguments' => array(ADMINISTER_XC_INDEX),
    'type' => MENU_NORMAL_ITEM,
    'weight' => 0,
    'file' => 'includes/xc_index_attribute.inc',
  );
  $items['admin/xc/index/field_type'] = array(
    'title' => 'Indexing fields',
    'description' => 'Mapping of metadata schema field and indexing type.',
    'page callback' => 'xc_index_field_type_view',
    'access arguments' => array(ADMINISTER_XC_INDEX),
    'type' => MENU_NORMAL_ITEM,
    'weight' => 1,
    'file' => 'xc_index_field_type.inc',
  );
  $items['admin/xc/index/facets'] = array(
    'title' => 'Manage facets',
    'description' => 'Create, modify, delete, and view predefined facets.',
    'page callback' => 'xc_index_facet_list',
    'access arguments' => array(ADMINISTER_XC_INDEX),
    'type' => MENU_NORMAL_ITEM,
    'weight' => 2,
    'file' => 'xc_index_facet.inc',
  );

  $items['admin/xc/index/sortables'] = array(
    'title' => 'Sortable fields',
    'description' => 'Select sortable fields.',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_index_sortables_form'),
    'access arguments' => array(ADMINISTER_XC_INDEX),
    'type' => MENU_NORMAL_ITEM,
    'weight' => 3,
    'file' => 'xc_index_sortables.inc',
  );

  // field_type
  $items['admin/xc/index/field_type/%xc_index_field_type_metadata'] = array(
    'title' => 'Map fields to Solr types',
    'description' => 'Map fields to Solr types',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_index_field_type_form', 4),
    'access arguments' => array(ADMINISTER_XC_INDEX),
    'type' => MENU_LOCAL_TASK,
    'file' => 'xc_index_field_type.inc',
  );

  // attribute
  $items['admin/xc/index/attribute/%xc_index_attribute_metadata'] = array(
    'title' => 'Handling fields\' attributes',
    'description' => 'Handling fields\' attributes',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_index_attribute_form', 4),
    'access arguments' => array(ADMINISTER_XC_INDEX),
    'type' => MENU_LOCAL_TASK,
    'file' => 'includes/xc_index_attribute.inc',
  );

  // Predefined facets
  $items['admin/xc/index/facets/list'] = array(
    'title' => 'List facets',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
  );
  $items['admin/xc/index/facets/listfacets'] = array(
    'title' => 'List mappings by facets',
    'description' => 'List mappings by facets',
    'page callback' => 'xc_index_facet_mapping_list_by_facets',
    'access arguments' => array(ADMINISTER_XC_INDEX),
    'type' => MENU_LOCAL_TASK,
    'weight' => 0,
    'file' => 'xc_index_facet_mapping.inc',
  );
  $items['admin/xc/index/facets/listfields'] = array(
    'title' => 'List mappings by fields',
    'description' => 'List mappings by fields',
    'page callback' => 'xc_index_facet_mapping_list_by_fields',
    'access arguments' => array(ADMINISTER_XC_INDEX),
    'type' => MENU_LOCAL_TASK,
    'weight' => 1,
    'file' => 'xc_index_facet_mapping.inc',
  );
  $items['admin/xc/index/facets/add'] = array(
    'title' => 'Add new facet',
    'description' => 'Add new facet',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_index_facet_form'),
    'access arguments' => array(ADMINISTER_XC_INDEX),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
    'file' => 'xc_index_facet.inc',
  );
  $items['admin/xc/index/facets/restore_defaults'] = array(
    'title' => 'Restore defaults',
    'description' => 'Restore default facets',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_index_facet_restore_defaults_form'),
    'access arguments' => array(ADMINISTER_XC_INDEX),
    'type' => MENU_LOCAL_TASK,
    'weight' => 3,
    'file' => 'xc_index_facet.inc',
  );
  $items['admin/xc/index/facets/%xc_index_facet/view'] = array(
    'title' => 'View facet',
    'description' => 'View facet',
    'page callback' => 'xc_index_facet_view',
    'page arguments' => array(4),
    'access arguments' => array(ADMINISTER_XC_INDEX),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'xc_index_facet.inc',
  );
  $items['admin/xc/index/facets/%xc_index_facet/edit'] = array(
    'title' => 'Edit facet',
    'description' => 'Edit facet',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_index_facet_edit_form', 4),
    'access arguments' => array(ADMINISTER_XC_INDEX),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'xc_index_facet.inc',
  );
  $items['admin/xc/index/facets/%xc_index_facet/delete'] = array(
    'title' => 'Delete facet',
    'description' => 'Delete facet',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_index_facet_edit_form', 4),
    'access arguments' => array(ADMINISTER_XC_INDEX),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'xc_index_facet.inc',
  );
  $items['admin/xc/index/facets/%xc_index_facet/enable'] = array(
    'title' => 'Enable facet',
    'description' => 'Delete facet',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_index_facet_enable_form', 4, 5),
    'access arguments' => array(ADMINISTER_XC_INDEX),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'xc_index_facet.inc',
  );
  $items['admin/xc/index/facets/%xc_index_facet/disable'] = array(
    'title' => 'Disable facet',
    'description' => 'Disable facet',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_index_facet_enable_form', 4, 5),
    'access arguments' => array(ADMINISTER_XC_INDEX),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'xc_index_facet.inc',
  );

  $items['admin/xc/index/superlocation'] = array(
    'title' => 'Super locations',
    'description' => 'List locations',
    'page callback' => 'xc_index_superlocation_list',
    'access arguments' => array(ADMINISTER_XC_INDEX),
    'type' => MENU_NORMAL_ITEM,
    'weight' => 4,
    'file' => 'xc_index.superlocation.inc',
  );
  $items['admin/xc/index/superlocation/list'] = array(
    'title' => 'List',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
    'file' => 'xc_index.superlocation.inc',
  );
  $items['admin/xc/index/superlocation/upload'] = array(
    'title' => 'Upload super location table',
    'description' => 'Upload super location table',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_index_superlocation_form'),
    'access arguments' => array(ADMINISTER_XC_INDEX),
    'type' => MENU_LOCAL_TASK,
    'file' => 'xc_index.superlocation.inc',
  );

  return $items;
}
