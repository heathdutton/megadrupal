<?php

/**
 * @file
 * Define menu items for MaPS Import module.
 */

use Drupal\maps_import\Filter\Condition\Condition;

/**
 * Generate menu entries related to the converters.
 *
 * @param string $type
 *   The type of elements (object, media, link).
 */
function maps_import_menu_converters($type, $weight = 1) {
  $items = array();

  $items['admin/maps-suite/profiles/%maps_import_profile/' . $type] = array(
    'title' => ucfirst($type),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('maps_import_converters_overview_form', 3, $type),
    'access callback' => 'maps_import_access',
    'access arguments' => array(3),
    'type' => MENU_LOCAL_TASK,
    'file' => 'admin/maps_import.converter.inc',
    'weight' => $weight,
  );

  $items['admin/maps-suite/profiles/%maps_import_profile/add/' . $type] = array(
    'title' => 'New @type converter',
    'title arguments' => array('@type' => $type),
    'page callback' => 'maps_import_converter_add',
    'page arguments' => array(3, 5),
    'access callback' => 'maps_import_access',
    'access arguments' => array(3),
    'type' => MENU_CALLBACK,
    'file' => 'admin/maps_import.converter.inc',
  );

  $items['admin/maps-suite/profiles/%maps_import_profile/' . $type . '/%maps_import_converter'] = array(
    'title callback' => 'maps_import_converter_title',
    'title arguments' => array(5),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('maps_import_converter_edit_form', 3, 5),
    'load arguments' => array(3),
    'access callback' => 'maps_import_converter_access',
    'access arguments' => array(5, 'edit'),
    'file' => 'admin/maps_import.converter.inc',
  );

  $items['admin/maps-suite/profiles/%maps_import_profile/' . $type . '/%maps_import_converter/edit'] = array(
    'title' => 'Edit',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
  );

  $items['admin/maps-suite/profiles/%maps_import_profile/' . $type . '/%maps_import_converter/delete'] = array(
    'title' => 'Delete',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('maps_import_converter_delete_confirm_form', 3, 5),
    'access callback' => 'maps_import_converter_access',
    'access arguments' => array(5, 'delete'),
    'load arguments' => array(3),
    'file' => 'admin/maps_import.converter.inc',
    'type' => MENU_LOCAL_TASK,
    'weight' => 10,
  );

  $items['admin/maps-suite/profiles/%maps_import_profile/' . $type . '/%maps_import_converter/filters'] = array(
    'title' => 'Filters',
    'page callback' => 'drupal_get_form',
    'page arguments' => array(
      'maps_import_converter_conditions_overview_form',
      3,
      5,
    ),
    'access callback' => 'maps_import_converter_access',
    'access arguments' => array(5, 'filters'),
    'load arguments' => array(3),
    'type' => MENU_LOCAL_TASK,
    'file' => 'admin/maps_import.condition.inc',
    'weight' => 3,
  );

  $items['admin/maps-suite/profiles/%maps_import_profile/' . $type . '/%maps_import_converter/filters/add'] = array(
    'title' => 'Add a condition',
    'page callback' => 'drupal_get_form',
    'page arguments' => array(
      'maps_import_converter_filter_add_condition_form',
      3,
      5,
      8,
    ),
    'access callback' => 'maps_import_converter_access',
    'access arguments' => array(5, 'filters'),
    'load arguments' => array(3),
    'type' => MENU_LOCAL_ACTION,
    'file' => 'admin/maps_import.condition.inc',
  );

  $items['admin/maps-suite/profiles/%maps_import_profile/' . $type . '/%maps_import_converter/filters/add_and'] = array(
    'title' => 'Add AND condition',
    'page callback' => 'drupal_get_form',
    'page arguments' => array(
      'maps_import_converter_filter_add_operator_form',
      3,
      5,
      Condition::CONDITION_AND,
    ),
    'access callback' => 'maps_import_converter_access',
    'access arguments' => array(5, 'filters'),
    'load arguments' => array(3),
    'type' => MENU_LOCAL_ACTION,
    'file' => 'admin/maps_import.condition.inc',
  );

  $items['admin/maps-suite/profiles/%maps_import_profile/' . $type . '/%maps_import_converter/filters/add_or'] = array(
    'title' => 'Add OR condition',
    'page callback' => 'drupal_get_form',
    'page arguments' => array(
      'maps_import_converter_filter_add_operator_form',
      3,
      5,
      Condition::CONDITION_OR,
    ),
    'access callback' => 'maps_import_converter_access',
    'access arguments' => array(5, 'filters'),
    'load arguments' => array(3),
    'type' => MENU_LOCAL_ACTION,
    'file' => 'admin/maps_import.condition.inc',
  );

  $items['admin/maps-suite/profiles/%maps_import_profile/' . $type . '/%maps_import_converter/filters/%maps_import_condition/edit'] = array(
    'title' => 'Edit condition',
    'page callback' => 'drupal_get_form',
    'page arguments' => array(
      'maps_import_converter_filter_edit_condition_form',
      3,
      5,
      7,
    ),
    'access callback' => 'maps_import_converter_access',
    'access arguments' => array(5, 'filters'),
    'load arguments' => array(3, 5),
    'type' => MENU_VISIBLE_IN_BREADCRUMB,
    'file' => 'admin/maps_import.condition.inc',
  );

  $items['admin/maps-suite/profiles/%maps_import_profile/' . $type . '/%maps_import_converter/filters/%maps_import_condition/delete'] = array(
    'title' => 'Delete condition',
    'page callback' => 'drupal_get_form',
    'page arguments' => array(
      'maps_import_converter_filter_delete_condition_form',
      3,
      5,
      7,
    ),
    'access callback' => 'maps_import_converter_access',
    'access arguments' => array(5, 'filters'),
    'load arguments' => array(3, 5),
    'type' => MENU_VISIBLE_IN_BREADCRUMB,
    'file' => 'admin/maps_import.condition.inc',
  );

  $items['admin/maps-suite/profiles/%maps_import_profile/' . $type . '/%maps_import_converter/mapping'] = array(
    'title' => 'Mapping',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('maps_import_converter_mapping_form', 3, 5),
    'access callback' => 'maps_import_converter_access',
    'access arguments' => array(5, 'mapping'),
    'load arguments' => array(3),
    'type' => MENU_LOCAL_TASK,
    'file' => 'admin/maps_import.mapping.inc',
  );

  $items['admin/maps-suite/profiles/%maps_import_profile/' . $type . '/%maps_import_converter/mapping/%maps_import_mapping_item/delete'] = array(
    'title' => 'Delete mapping',
    'page callback' => 'drupal_get_form',
    'page arguments' => array(
      'maps_import_converter_mapping_delete_form',
      3,
      5,
      7,
    ),
    'access callback' => 'maps_import_converter_access',
    'access arguments' => array(5, 'mapping'),
    'load arguments' => array(3, 5),
    'type' => MENU_VISIBLE_IN_BREADCRUMB,
    'file' => 'admin/maps_import.mapping.inc',
  );

  $items['admin/maps-suite/profiles/%maps_import_profile/' . $type . '/%maps_import_converter/mapping/%maps_import_mapping_item/options'] = array(
    'title' => 'Options',
    'page callback' => 'drupal_get_form',
    'page arguments' => array(
      'maps_import_converter_mapping_options_form',
      3,
      5,
      7,
    ),
    'access callback' => 'maps_import_item_options_acess',
    'access arguments' => array(5, 'mapping', 7),
    'load arguments' => array(3, 5),
    'type' => MENU_VISIBLE_IN_BREADCRUMB,
    'file' => 'admin/maps_import.mapping.inc',
  );

  $items['admin/maps-suite/profiles/%maps_import_profile/' . $type . '/%maps_import_converter/mapping/add_related_entity'] = array(
    'title' => 'Add related entity',
    'page callback' => 'drupal_get_form',
    'page arguments' => array(
      'maps_import_converter_mapping_add_related_entity_form',
      3,
      5,
      8,
    ),
    'access callback' => 'maps_import_converter_access',
    'access arguments' => array(5, 'mapping'),
    'load arguments' => array(3, 5),
    'type' => MENU_LOCAL_ACTION,
    'file' => 'admin/maps_import.mapping.inc',
  );

  $items['admin/maps-suite/profiles/%maps_import_profile/' . $type . '/%maps_import_converter/mapping/add_delayed_entity'] = array(
    'title' => 'Add delayed entity',
    'page callback' => 'drupal_get_form',
    'page arguments' => array(
      'maps_import_converter_mapping_add_delayed_entity_form',
      3,
      5,
      8,
    ),
    'access callback' => 'maps_import_converter_access',
    'access arguments' => array(5, 'mapping'),
    'load arguments' => array(3, 5),
    'type' => MENU_LOCAL_ACTION,
    'file' => 'admin/maps_import.mapping.inc',
  );

  return $items;
}

/**
 * Build the module menu.
 */
function _maps_import_menu() {
  $items = array();

  $items['admin/maps-suite/profiles/%maps_import_profile/languages'] = array(
    'title' => 'Languages',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('maps_import_settings_languages_form', 3),
    'access callback' => 'maps_import_access',
    'access arguments' => array(3, 'language', TRUE),
    'type' => MENU_LOCAL_TASK,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'admin/maps_import.language.inc',
    'weight' => 4,
  );

  $items['admin/maps-suite/profiles/%maps_import_profile/statuses'] = array(
    'title' => 'Statuses',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('maps_import_settings_statuses_form', 3),
    'access callback' => 'maps_import_access',
    'access arguments' => array(3, 'status', '0'),
    'type' => MENU_LOCAL_TASK,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'admin/maps_import.status.inc',
    'weight' => 5,
  );

  $items['admin/maps-suite/profiles/%maps_import_profile/media_types'] = array(
    'title' => 'Media types',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('maps_import_settings_media_types_form', 3),
    'access callback' => 'maps_import_access',
    'access arguments' => array(3, 'media_type', TRUE),
    'type' => MENU_LOCAL_TASK,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'admin/maps_import.media_types.inc',
    'weight' => 6,
  );

  $items['admin/maps-suite/profiles/%maps_import_profile/import'] = array(
    'title' => 'Manual import',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('maps_import_import_form', 3),
    'access arguments' => array('administer maps suite'),
    'type' => MENU_LOCAL_TASK,
    'file' => 'admin/maps_import.admin.inc',
    'weight' => 10,
  );

  $i = 1;
  foreach (array('object', 'media') as $type) {
    $items += maps_import_menu_converters($type, $i);
    $i++;
  }

  $items['admin/maps-suite/profiles/%maps_import_profile/object/%maps_import_converter/mapping/add_object_media'] = array(
    'title' => 'Add object / media mapping',
    'page callback' => 'maps_import_converter_mapping_add_object_media_form',
    'page arguments' => array(3, 5),
    'access callback' => 'maps_import_converter_access',
    'access arguments' => array(5, 'mapping'),
    'load arguments' => array(3, 5),
    'type' => MENU_LOCAL_ACTION,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'admin/maps_import.mapping.inc',
  );

  $items['admin/maps-suite/profiles/%maps_import_profile/object/%maps_import_converter/mapping/%maps_import_mapping_item/edit_object_media'] = array(
    'page callback' => 'drupal_get_form',
    'page arguments' => array(
      'maps_import_converter_mapping_edit_object_media_form',
      3,
      5,
      7,
    ),
    'access callback' => 'maps_import_converter_access',
    'access arguments' => array(5, 'mapping'),
    'load arguments' => array(3, 5, 7),
    'file' => 'admin/maps_import.mapping.inc',
  );

  $items['admin/maps-suite/profiles/%maps_import_profile/settings/media'] = array(
    'title' => 'Settings',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('maps_import_converter_media_settings_form', 3),
    'access callback' => 'maps_import_access',
    'access arguments' => array(3),
    'load arguments' => array(3),
    'type' => MENU_CALLBACK,
    'file' => 'admin/maps_import.converter.inc',
  );

  $items['admin/maps-suite/profiles/%maps_import_profile/libraries'] = array(
    'title' => 'Libraries',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('maps_import_libraries_overview', 3),
    'access callback' => 'maps_import_access',
    'access arguments' => array(3),
    'type' => MENU_LOCAL_TASK,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'admin/maps_import.library.inc',
    'weight' => 9,
  );

  $items['admin/maps-suite/profiles/%maps_import_profile/libraries/%maps_import_library/add'] = array(
    'title' => 'Create a new map',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('maps_import_libraries_add_form', 3, 5, 6),
    'access callback' => 'maps_import_library_access',
    'access arguments' => array(3, 5),
    'type' => MENU_VISIBLE_IN_BREADCRUMB,
    'file' => 'admin/maps_import.library.inc',
  );

  $items['admin/maps-suite/profiles/%maps_import_profile/libraries/%maps_import_library/edit'] = array(
    'title' => 'Edit the map',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('maps_import_libraries_add_form', 3, 5, 6),
    'access callback' => 'maps_import_library_access',
    'access arguments' => array(3, 5),
    'type' => MENU_VISIBLE_IN_BREADCRUMB,
    'file' => 'admin/maps_import.library.inc',
  );

  $items['admin/maps-suite/profiles/%maps_import_profile/libraries/%maps_import_library/delete'] = array(
    'title' => 'Delete the map',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('maps_import_libraries_delete_confirm_form', 3, 5),
    'access callback' => 'maps_import_library_access',
    'access arguments' => array(3, 5),
    'type' => MENU_VISIBLE_IN_BREADCRUMB,
    'file' => 'admin/maps_import.library.inc',
  );

  $items['admin/maps-suite/settings/fields'] = array(
    'title' => 'Fields validation',
    'type' => MENU_LOCAL_TASK,
    'page callback' => 'drupal_get_form',
    'page arguments' => array('maps_import_validation_overview'),
    'access arguments' => array('administer maps suite'),
    'file' => 'admin/maps_import.validation.inc',
  );

  $items['admin/maps-suite/lock/break'] = array(
    'title' => 'Break the lock',
    'description' => 'Administer MaPS Profiles: Web Services profiles, import configuration, entities mapping, etc.',
    'page callback' => 'maps_import_break_lock',
    'access arguments' => array('administer maps suite'),
    'type' => MENU_CALLBACK,
    'file' => 'maps_import.module',
  );

  return $items;
}
