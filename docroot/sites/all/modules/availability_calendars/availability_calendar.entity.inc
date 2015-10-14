<?php

/**
 * @file Contains Entity API and Search API hooks and callbacks.
 *
 * @todo:
 * - Can the hybrid approach remain in place: calendar as a field and calendar
 *   as an entity without introducing too much overhead?
 * - Move field instance settings into the entity data model:
 *   - default state: without knowing the default state, a calendar on its own
 *     cannot be rendered or queried about availability.
 *   - allocation type: without knowing this setting, user interaction with the
 *     calendar is difficult, especially selecting end dates.
 * - Make a calendar entity renderable on its own.
 * - Move calendar functionality into an entity and entityController class.
 * - Define additional properties on the calendar like:
 *   - availability
 *   - isAvailable
 * - the definition values 'allocation_type' and 'default_state' for the
 *   (filtered) availability can be empty if we are filtering on calendar
 *   entities instead of calendar fields.
 */

/**
 * Implements hook_entity_info().
 *
 * Defines the "internals" of this module's data model to Drupal core and Entity
 * API (from contrib):
 * - availability_calendar_calendar is defined as entity.
 * - availability_calendar_availability cannot be defined as entity as it does
 *   not have a numeric primary key. Will be defined as property of a calendar
 *   though (in a future version).
 * - availability_calendar_state could be defined as configuration entities, so
 *   it could be exported, but has not been defined as such yet.
 *
 * Both Drupal core and Entity API keys and values are defined here.
 */
function availability_calendar_entity_info() {
  $return = array(
    'availability_calendar_calendar' => array(
      'module' => 'availability_calendar',
      'label' => t('Availability Calendar'),
      'plural label' => t('Availability Calendars'),
      'description' => t('An entity type used to store date based availability.'),
      'base table' => 'availability_calendar_calendar',
      'revision table' => null,
      'entity keys' => array(
        'id' => 'cid',
      ),
      'fieldable' => FALSE,
      'view modes' => array(
        'full' => array(
          'label' => t('Full content'),
          'custom settings' => FALSE,
        ),
      ),
      'controller class' => 'EntityAPIController',
      'entity class' => 'Entity',
      'label callback' => 'entity_class_label',
      'uri callback' => 'entity_class_uri',
      'views controller class' => 'AvailabilityCalendarViewsController',
      //'rules controller class' => '',
    ),
  );
  return $return;
}

/**
 * Implements hook_entity_property_info().
 *
 * Defines the labels and types of the properties of the
 * 'availability_calendar_calendar' entity.
 */
function availability_calendar_entity_property_info() {
  $return = array(
    'availability_calendar_calendar' => array(
      'properties' => array(
        // Schema fields. We override to define the correct type and textual
        // information.
        'cid' => array(
          'label' => t('Calendar ID'),
          'description' => t('The unique ID of the calendar.'),
          'type' => 'integer',
          'schema field' => 'cid'
        ),
        'created' => array(
          'label' => t('Created date'),
          'description' => t('The date the calendar was created.'),
          'type' => 'date',
          'schema field' => 'created'
        ),
        'changed' => array(
          'label' => t('Last updated date'),
          'description' => t('The date the calendar was last updated.'),
          'type' => 'date',
          'schema field' => 'changed'
        ),
        // Virtual, non stored properties.
        // Filtered availability: should only be used in Search API indices.
        'filtered_availability' => array(
          'label' => t('Filtered availability'),
          'description' => t('Availability for this calendar, filtered for search optimization.'),
          'type' => 'list<date>',
          'getter callback' => 'availability_calendar_calendar_entity_property_filtered_availability_get',
        ),
      ),
    ),
  );
  return $return;
}

/**
 * Callback to alter the property info for fields, in our case the Availability
 * Calendar field.
 *
 * @param array $info
 * @param string $entity_type
 * @param array $field
 * @param array $instance
 * param array $field_type
 */
function availability_calendar_field_property_info_alter(&$info, $entity_type, $field, $instance/*, $field_type*/) {
  $name = $field['field_name'];
  $property = &$info[$entity_type]['bundles'][$instance['bundle']]['properties'][$name];
  if ($field['cardinality'] != 1) {
    $property['type'] = "list<{$property['type']}>";
  }
  $property['getter callback'] = 'entity_metadata_field_verbatim_get';
  $property['setter callback'] = 'entity_metadata_field_verbatim_set';
  $property['property info'] =  array(
    'cid' => array(
      'type' => 'availability_calendar_calendar',
      'label' => t('Availability Calendar'),
      'description' => t('The reference to the Availability Calendar.'),
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
    ),
  );
  if (!empty($instance['settings']['allow_disable'])) {
    $property['property info']['enabled'] = array(
      'type' => 'boolean',
      'label' => t('Enabled'),
      'description' => t('If the calendar is enabled'),
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
    );

  }
  if (!empty($instance['settings']['add_name'])) {
    $property['property info']['name'] =  array(
      'type' => 'string',
      'label' => t('Name'),
      'description' => t('The name of the calendar'),
      'sanitized' => FALSE,
      'sanitize' => 'check_plain',
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
    );
  }
}

function availability_calendar_calendar_entity_property_filtered_availability_get($entity, $options, $property_name, $entity_type, &$property_info) {
  $result = NULL;
  module_load_include('inc', 'availability_calendar');
  $cid = $entity->cid;
  $from = new DateTime();
  $to = new DateTime();
  $months_to_index = variable_get('availability_calendar_months_to_index', 13);
  //PHP5.3:$to->add(new DateInterval("P{$months_to_index}M"));
  $to->modify("+$months_to_index months");
  $default_state = availability_calendar_get_field_setting('default_state');
  if ($default_state === NULL) {
    // We could use info from the field filter, but as we have no idea of that
    // context, we just walk through all fields. Performance hit should be
    // minimal in "normal" set-ups.
    // @todo: this will fail if no field refers to this calendar but this is not yet supported.
    $field = availability_calendar_get_referring_field($cid);
    if (!empty($field)) {
      $field_info = field_info_field($field);
      $default_state = $field_info['settings']['default_state'];
    }
  }
  $availability = availability_calendar_get_availability($cid, $from , $to, $default_state);
  $non_available_states = availability_calendar_get_states(FALSE);
  $result = array();
  foreach ($availability as $date => $sid) {
    if (array_key_exists($sid, $non_available_states)) {
      $timestamp = new DateTime($date);
      $timestamp = (int) $timestamp->format('U');
      $result[] = $timestamp;
    }
  }
  return $result;
}

/**
 * Returns the value for an availability calendar field setting.
 *
 * A value is only returned if all availability calendar fields have the same
 * value for the requested setting. If not, null is returned.
 *
 * @param string $setting
 * @return mixed
 */
function availability_calendar_get_field_setting($setting) {
  // @todo: As of Drupal 7.22?: $instances = field_info_field_map();
  $settings = &drupal_static(__FUNCTION__);
  if ($settings === NULL) {
   $settings = array();
  }
  if (!array_key_exists('setting', $settings)) {
    $settings[$setting] = NULL;
    $instances = field_info_fields();
    foreach ($instances as $field) {
      if ($field['type'] === 'availability_calendar') {
        if ($settings[$setting] === NULL) {
          // first field of this type: get value for the requested setting
          $settings[$setting] = $field['settings'][$setting];
        }
        else if ($settings[$setting] !== $field['settings'][$setting]) {
          // Multiple calendar fields with different values for the requested
          // setting: we cannot return THE value for this setting; return null.
          $settings[$setting] = NULL;
          break;
        }
        // else: multiple calendar fields, but so far all these fields have the
        // same value for this setting. Continue.
      }
    }
  }
  return $settings[$setting];
}

/**
 * Returns the field name of the field that refers to the given calendar id.
 *
 * The search can be restricted to a given set of fields by passing in a 2nd
 * parameter.
 *
 * @param int $cid
 * @param array|NULL $restrict
 *   An array of fields to restrict the search to. NULL if the search should
 *   not be restricted. An empty array restricts the search to NO fields at
 *   all and thus will always return FALSE!
 *
 * @return string|false
 *   The name of the field or false if no field refers to this calendar.
 */
function availability_calendar_get_referring_field($cid, $restrict = NULL) {
  $fields = is_array($restrict) ? $restrict : $this->get_fields_by_type('availability_calendar_calendar');
  foreach ($fields as $field_name) {
    $query = new EntityFieldQuery();
    $query->fieldCondition($field_name, 'cid', $cid, '=');
    $result = $query->execute();
    if (!empty($result)) {
      return $field_name;
    }
  }
  return FALSE;
}

/**
 * Implements hook_search_api_data_type_info().
 *
 * We define a new data type for indexed properties that should be used to
 * filter on availability.
 *
 * @return array
 */
function availability_calendar_search_api_data_type_info() {
  return array(
    'availability_calendar_availability' => array(
      'name' => t('Availability'),
      'fallback' => 'date',
    ),
  );
}

/**
 * Implements hook_search_api_alter_callback_info().
 */
function availability_calendar_search_api_alter_callback_info() {
  $callbacks['availability_calendar_alter_field_filter'] = array(
    'name' => t('Field filter'),
    'description' => t('Exclude items from indexing based on the fields that refer to them.'),
    'class' => 'AvailabilityCalendarAlterFieldFilter',
    // Filters should be executed first.
    'weight' => -10,
  );
  return $callbacks;
}
