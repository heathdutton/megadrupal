<?php
/**
 * Views support for Availability Calendar.
 *
 * Availability Calendar supports the views module, partly through the functions
 * here, partly through class AvailabilityCalendarViewsController (extends
 * EntityViewsController) in file availability_calendar_views_controller.inc.
 *
 * The hooks in this file:
 *
 * hook_field_views_data_alter():
 * (run after the field module implementation of hook_views_data has executed
 *  hook_field_views_data)
 * Alters the Views data at field level for our own availability calendar field:
 * - We add a relationship from field cid to availability_calendar_calendar.
 * - We define a filter and argument to filter on availability.
 *
 * hook_field_views_data_views_data_alter():
 * (run during the field module implementation of hook_views_data_alter to alter
 *  the views data on a per field basis)
 * - We add reverse relationships on availability calendars to the entities that
 *   have an availability calendar field.
 *
 * hook_views_data_alter():
 * (run after hook_views_data)
 * - We change the filter and argument handlers for fields in search indices
 *   that are of type "Availability" (availability_calendar_availability).
 *
 *  hook_views_plugins():
 * - Define an argument validator plugin for date ranges.
 */

/**
 * Implements hook_field_views_data_alter().
 *
 * The data structure mostly contains correct defaults. So we let Views/field
 * create the array for us first, then we alter it, as opposed to implementing
 * hook_field_views_data() ourselves.
 *
 * Data added:
 * - We add a relationship from field cid to availability_calendar_calendar.
 * - We define a filter and argument to filter on availability.
 */
function availability_calendar_field_views_data_alter(&$data, $field /*, $module*/) {
  if ($field['type'] == 'availability_calendar') {
    // Get some info for easy use later on.
    $field_name = $field['field_name'];
    $field_table_name = key($field['storage']['details']['sql']['FIELD_LOAD_CURRENT']);

    // field_..._cid is a reference to availability_calendar_calendar.
    $data[$field_table_name]["{$field_name}_cid"]['relationship'] = array(
      'handler' => 'views_handler_relationship',
      'base' => 'availability_calendar_calendar',
      'entity type' => 'availability_calendar_calendar',
      'base field' => 'cid',
      'label' => t('Reference to an availability calendar'),
      // We should only join to the availability_calendar_calendar table when
      // the calendar is enabled at the field level.
      'join extra' => array(
        array(
          'table' => $field_table_name,
          'field' => "{$field_name}_enabled",
          'value' => 1,
          'numeric' => TRUE,
        ),
      ),
    );

    // Define a filter and argument to filter on availability.
    $field_title = $data[$field_table_name][$field_name]['title'];
    $data[$field_table_name]['available'] = array(
      'group' => $data[$field_table_name][$field_name]['group'],
      'title' => t('@field_label is available', array('@field_label' => $field_title)),
      'title short' => t('@field_label available', array('@field_label' => $field_title)),
      'help' => t('Filters on availability during the defined period.'),
      'filter' => array(
        'real field' => "{$field_name}_cid",
        'handler' => 'availability_calendar_handler_filter_availability',
        'default_state' => $field['settings']['default_state'],
        'allocation_type' => $field['settings']['allocation_type'],
      ),
      'argument' => array(
        'real field' => "{$field_name}_cid",
        'handler' => 'availability_calendar_handler_argument_availability',
        'default_state' => $field['settings']['default_state'],
        'allocation_type' => $field['settings']['allocation_type'],
      ),
    );
  }
}

/**
 * Implements hook_field_views_data_views_data_alter().
 *
 * Field modules can implement hook_field_views_data_views_data_alter() to
 * alter the views data on a per field basis. This is weirdly named so as
 * not to conflict with the drupal_alter('field_views_data') in
 * field_views_data.
 *
 * We add:
 * - reverse relationships on availability calendars to entities that use it.
 *
 * Based on code copied from views/modules/file.field.inc
 */
function availability_calendar_field_views_data_views_data_alter(&$data, $field) {
  // Get some info for easy use later on.
  $field_name = $field['field_name'];
  $field_table_name = key($field['storage']['details']['sql']['FIELD_LOAD_CURRENT']);

  // Define reverse entity relations to entities using calendars via this field
  foreach ($field['bundles'] as $entity_type => $bundles) {
    $entity_info = entity_get_info($entity_type);
    $pseudo_field_name = "reverse_{$field_name}_$entity_type";

    list($label/*, $all_labels*/) = field_views_field_label($field_name);
    $entity = $entity_info['label'];
    if ($entity == t('Node')) {
      $entity = t('Content');
    }

    $data['availability_calendar_calendar'][$pseudo_field_name]['relationship'] = array(
      'title' => t('@entity using @field', array(
        '@entity' => $entity,
        '@field' => $label
      )),
      'help' => t('Relate each @entity with a @field set to this availability calendar.', array(
        '@entity' => $entity,
        '@field' => $label
      )),
      'handler' => 'views_handler_relationship_entity_reverse',
      'field_name' => $field_name,
      'field table' => $field_table_name,
      'field field' => "{$field_name}_cid",
      'base' => $entity_info['base table'],
      'base field' => $entity_info['entity keys']['id'],
      'label' => t('!field_name', array('!field_name' => $field_name)),
      'join_extra' => array(
        0 => array(
          'field' => 'entity_type',
          'value' => $entity_type,
        ),
        1 => array(
          'field' => 'deleted',
          'value' => 0,
          'numeric' => TRUE,
        ),
      ),
    );
  }
}

/**
 * Implements hook_views_data_alter().
 *
 * We change the filter and argument handlers for indexed fields from the type
 * "Availability". Fields that are declared to be of this type should either be
 * the cid field of an availability calendar field (field type = integer) or the
 * filtered availability property of an availability calendar entity
 * (field type = list<date>).
 *
 * @param array $data
 *
 * @see hook_views_data()
 */
function availability_calendar_views_data_alter(&$data) {
  $indices = module_exists('search_api') ? search_api_index_load_multiple(FALSE) : array();
  foreach ($indices as $index) {
    foreach (availability_calendar_get_search_api_index_availability_fields($index->id) as $search_field_name => $search_field) {
      $views_field_name = str_replace(":", "_", $search_field_name);
      $views_field = &$data['search_api_index_' . $index->machine_name][$views_field_name];
      // Change the filter handler.
      $views_field['filter']['handler'] = 'availability_calendar_handler_filter_indexed_availability';
      $views_field['filter']['allocation_type'] = availability_calendar_get_allocation_type($search_field_name);
      // Pass on additional information. cid fields will be joined to a table
      // with filtered availability in another index (should be created
      // separately). For filtered availability fields, the table itself will
      // be used.
      $views_field['filter']['filtered_availability_table'] = search_api_extract_inner_type($views_field['field']['type']) === 'integer'
        ? availability_calendar_get_search_api_availability_index_table($index->server)
        : '';
      // And change the argument handler accordingly.
      $views_field['argument']['handler'] = 'availability_calendar_handler_argument_indexed_availability';
      $views_field['argument']['filtered_availability_table'] = $views_field['filter']['filtered_availability_table'];
      $views_field['argument']['allocation_type'] = $views_field['filter']['allocation_type'];
    }
  }
}

function availability_calendar_get_allocation_type($search_field_name) {
  list($field_name) = explode(':', $search_field_name);
  $field_info = field_info_field($field_name);
  return $field_info['settings']['allocation_type'];
}

/**
 * Returns the availability field(s) of an Search API index (if it has any).
 *
 * @param integer $index_id
 *   The index_id of an index for which the availability fields should be found.
 *
 * @return array
 *   An array, possibly empty, with the names of the availability fields.
 */
function availability_calendar_get_search_api_index_availability_fields($index_id) {
  $result = array();
  $index = search_api_index_load($index_id);
  if (isset($index->options['fields'])) {
    foreach ($index->options['fields'] as $field_name => $field) {
      if (isset($field['real_type']) && search_api_extract_inner_type($field['real_type']) === 'availability_calendar_availability') {
        $result[$field_name] = $field;
      }
    }
  }
  return $result;
}

/**
 * Searches for an index that indexes availability calendars and returns the
 * table name of the table that contains the indexed availability.
 *
 * @param string $server_name
 *   The server on which the index should reside.
 *
 * @return string
 */
function availability_calendar_get_search_api_availability_index_table($server_name) {
  $indices = module_exists('search_api') ? search_api_index_load_multiple(FALSE) : array();
  foreach ($indices as $index) {
    if ($index->server === $server_name && $index->item_type === 'availability_calendar_calendar') {
      $server = search_api_server_load($server_name);
      $reflectionClass = new ReflectionClass(get_class($server));
      $reflectionProperty = $reflectionClass->getProperty('options');
      $reflectionProperty->setAccessible(true);
      $options = $reflectionProperty->getValue($server);

      // Get table that contains the filtered availability.
      if (isset($options['indexes'][$index->machine_name]['filtered_availability']['table'])) {
        return $options['indexes'][$index->machine_name]['filtered_availability']['table'];
      }
    }
  }
  return NULL;
}

/**
 * Implements hook_views_plugins().
 *
 * @return array
 */
function availability_calendar_views_plugins() {
  return array(
    'argument validator' => array(
      'availability_calendar_date_range' => array(
        'title' => t('Date range for Availability Calendar'),
        'handler' => 'availability_calendar_plugin_argument_validate_date_range',
      ),
    ),
  );
}
