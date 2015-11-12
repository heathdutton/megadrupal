<?php

/**
 * @file
 * Provides info about the node entity.
 */

/**
 * Implements hook_entity_property_info_alter().
 *
 * Make the synchronization fields available for the entity (and Rules) module.
 */
function ls_answ_entity_property_info_alter(&$info) {
  $ls_fields = array(
    'ls_status',
    'ls_sync',
    'ls_last_import',
    'ls_sid',
    'ls_lang',
    'ls_aid',
    'ls_token',
  );

  module_load_include('inc', 'ls_answ', 'ls_answ');
  // Retrieve all answers content types.
  $query = db_query("SELECT type FROM {node_type} WHERE module = :module", array(':module' => 'ls_answ'));
  while ($record = $query->fetchAssoc()) {
    // Retrieve the corresponding variables name.
    $properties = &$info['node']['bundles'][$record['type']]['properties'];
    foreach ($ls_fields as $ls_field) {
      $property = $ls_field;
      $properties[$property] = array(
        'label' => ls_answ_display_sync_headers($ls_field),
        'description' => ls_answ_display_sync_headers_description($ls_field),
        //'type' => 'integer', 
        //'getter callback' => 'entity_property_poll_node_get_properties',
      );
      switch ($property) {
        case 'ls_sync':
        case 'ls_status':
        case 'ls_sid':
        case 'ls_aid':
          $properties[$property]['type'] = 'integer';
          //$properties[$property]['type'] = 'list<integer>';
          break;
        case 'ls_lang':
        case 'ls_token':
          $properties[$property]['type'] = 'token';
          break;
        case 'ls_last_import':
          $properties[$property]['type'] = 'date';
          break;
          //case 'ls_url':
          //$properties[$property]['type'] = 'uri';
          //break;
          //case 'survey':
          //$properties[$property]['type'] = 'node';
          //$properties[$property]['getter callback'] = 'entity_property_ls_answ_node_get_properties',
          //break;
      }
      switch ($property) {
        case 'ls_sync':
        case 'ls_status':
        case 'ls_sid':
        case 'ls_lang':
          $properties[$property]['options list'] = 'ls_answ_metadata_' . $property . '_options_list';
          break;
      }
    }
  }
  //'getter callback' => 'entity_metadata_locale_get_languages',
  //'options list' => 'entity_metadata_language_list',
}
