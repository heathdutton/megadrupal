<?php

/**
 * @file
 * Mediabox display views inc.
 */

/**
 * Implements hook_views_plugins().
 */
function mediabox_ui_views_plugins() {

  $plugins['style']['mediabox'] = array(
    'title' => t('Mediabox selector'),
    'theme' => 'mediaboxui_view',
    'type' => 'normal',
    'theme path' => drupal_get_path('module', 'mediabox_ui') . '/views/theme',
    //'path' => drupal_get_path('module', 'mediabox_ui') . '/views',
    'theme file' => 'mediabox_ui.theme.inc',
    'handler' => 'mediabox_ui_style_plugin',
    'uses options' => FALSE,
    'uses grouping' => FALSE,
    'uses row plugin' => TRUE,
    'uses fields' => TRUE,
  );

  $plugins['style']['mediabox_grid'] = array(
    'title' => t('Mediabox grid selector'),
    'theme' => 'mediaboxui_grid_view',
    'type' => 'normal',
    'theme path' => drupal_get_path('module', 'mediabox_ui') . '/views/theme',
    //'path' => drupal_get_path('module', 'mediabox_ui') . '/views',
    'theme file' => 'mediabox_ui.theme.inc',
    'handler' => 'mediabox_ui_grid_style_plugin',
    'uses options' => TRUE,
    'uses grouping' => FALSE,
    'uses row plugin' => TRUE,
    'uses fields' => TRUE,
  );

  return $plugins;
}

/**
 * Implements hook_field_views_data().
 */
function mediabox_ui_field_views_data($field) {
  $data = field_views_field_default_views_data($field);
  $entity_info = entity_get_info('mediabox_display');

  foreach ($data as $table_name => $table_data) {
    if (isset($entity_info['base table'])) {

      $field_name = $field['field_name'] . '_display_mid';
      $parameters = array('@entity' => t('Mediabox'), '!field_name' => $field['field_name']);

      $data[$table_name][$field_name]['relationship'] = array(
        'handler' => 'views_handler_relationship',
        'base' => $entity_info['base table'],
        'base field' => $entity_info['entity keys']['id'],
        'label' => t('@entity mediabox referenced from !field_name', $parameters),
        'group' => t('Mediabox reference'),
        'title' => t('Referenced mediabox'),
        'help' => t('A bridge to the @entity entity that is referenced via !field_name', $parameters),
      );
    }
  }

  return $data;
}
/**
 * Implements hook_field_views_data_views_data_alter().
 *
 * Views integration to provide reverse relationships on entityreference fields.
 */
function mediabox_ui_field_views_data_views_data_alter(&$data, $field) {

  foreach ($field['bundles'] as $entity_type => $bundles) {
    $target_entity_info = entity_get_info('mediabox_display');

    if (isset($target_entity_info['base table'])) {
      $entity_info = entity_get_info($entity_type);
      $entity = $entity_info['label'];
      if ($entity == t('Node')) {
        $entity = t('Content');
      }

      // always mediabox
      $target_entity = $target_entity_info['label'];

      $pseudo_field_name = 'reverse_' . $field['field_name'] . '_' . $entity_type;
      $replacements = array('@entity' => $entity, '@target_entity' => $target_entity, '!field_name' => $field['field_name']);

      $data[$target_entity_info['base table']][$pseudo_field_name]['relationship'] = array(
        'handler' => 'views_handler_relationship_entity_reverse',
        'field_name' => $field['field_name'],
        'field table' => _field_sql_storage_tablename($field),
        'field field' => $field['field_name'] . '_display_mid',
        'base' => $entity_info['base table'],
        'base field' => $entity_info['entity keys']['id'],
        'label' => t('@entity referencing @target_entity from !field_name', $replacements),
        'group' => t('Mediabox display Reference'),
        'title' => t('Referencing mediabox display'),
        'help' => t('A bridge to the @entity entity that is referencing @target_entity via !field_name', $replacements),
      );
    }
  }

  // For all mediabox fields we need to change delta handlers because default
  // ones will create additional double joins and duplicates.
  $mediabox_fields_info = array();
  $fields_info = field_info_fields();
  foreach ($fields_info as $key => $field_info) {
    if ($field_info['type'] == 'mediabox') {
      $mediabox_fields_info[$key] = $field_info;
    }
  }
  foreach ($mediabox_fields_info as $key => $field_info) {
    $data['field_data_' . $key]['delta']['field']['handler'] = 'mediabox_delta_field_handler';
    $data['field_data_' . $key]['delta']['sort']['handler'] = 'mediabox_delta_sort_handler';
    $data['field_data_' . $key]['delta']['argument']['handler'] = 'mediabox_delta_argument_handler';
    $data['field_data_' . $key]['delta']['filter']['handler'] = 'mediabox_delta_filter_handler';
  }
  
}
