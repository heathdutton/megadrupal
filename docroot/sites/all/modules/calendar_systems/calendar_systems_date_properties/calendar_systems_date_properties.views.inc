<?php

/**
 * Implementation of hook_views_handlers().
 */
function calendar_systems_date_properties_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'calendar_systems_date_properties') . '/includes',
    ),
    'handlers' => array(),
  );
}

function calendar_systems_date_properties_views_data_alter(&$data) {
  $entity_types = _calendar_systems_date_properties_entity_types();
  foreach ($entity_types as $entity_type => $entity_type_info) {
    if ($entity_type_info['integrated'] && isset($data[$entity_type_info['base table']])) {

      foreach ($entity_type_info['selected_properties'] as $property_name => $property) {
        $calendar_systems = _calendar_systems_date_properties_get_active_calendar_systems();
        foreach ($calendar_systems as $calendar_system) {
          foreach ($calendar_system['parts'] as $calendar_system_part_name => $calendar_system_part) {
            $new_property_label = t($entity_type_info['properties'][$property_name]['label']) . ' ' . t($calendar_system['name']) . ' ' .t($calendar_system_part['label']);
            $new_property_name = $property_name . '_' . $calendar_system['name'] . '_' .$calendar_system_part_name;

            $data[$entity_type_info['base table']][$new_property_name] = array(
              'title' => $new_property_label,
              'help' => t('Date part for date based calculation'),
              'field' => array(
                'handler' => 'views_handler_field_numeric',
                'click sortable' => TRUE,
              ),
              'filter' => array(
                'handler' => 'views_handler_filter_numeric',
              ),
              'argument' => array(
                'handler' => 'views_handler_argument_numeric',
              ),
              'sort' => array(
                'handler' => 'views_handler_sort',
              ),
            );
          }
        }
      }


    }
  }
}