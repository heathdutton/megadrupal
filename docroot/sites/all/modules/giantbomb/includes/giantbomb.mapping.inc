<?php
/**
 * @file
 * Provide information about properties of types on the Giant Bomb API.
 */

/**
 * Find the field name and column a Giant Bomb property stores data.
 *
 * This is only valid for the current entity and bundle setting. It is not
 * possible to rely on this to get field names of orphaned types.
 *
 * @param string $endpoint
 *   A Giant Bomb endpoint name.
 * @param string $object
 *   A Giant Bomb object type.
 * @param string $field
 *   Allowed values: constants GIANTBOMB_FIELD_*
 * 
 * @return array|bool
 *   Return array(field_name, field_column), or FALSE.
 */
function giantbomb_field_name($endpoint, $object, $field) {
  $fast = &drupal_static(__FUNCTION__, array());

  if (isset($fast[$endpoint][$object])) {
    if (isset($fast[$endpoint][$object][$field])) {
      return $fast[$endpoint][$object][$field];
    }
    else {
      return FALSE;
    }
  }

  $fast[$endpoint][$object] = array();

  $object_info = giantbomb_type_load($endpoint, $object);

  if (empty($object_info->entity_type) || empty($object_info->bundle)) {
    return FALSE;
  }

  $instances = field_info_instances($object_info->entity_type, $object_info->bundle);
  foreach ($instances as $field_name => $instance) {
    $id = $instance['id'];
    if (isset($object_info->settings['field_mapping'][$id])) {
      foreach ($object_info->settings['field_mapping'][$id] as $column => $property) {
        $property_name = $property['property'];
        if (in_array($property_name, array(GIANTBOMB_FIELD_ENDPOINT, GIANTBOMB_FIELD_TYPE, GIANTBOMB_FIELD_ID, GIANTBOMB_FIELD_NEXT_UPDATE))) {
          $fast[$endpoint][$object][$property_name] = array($field_name, $column);
        }
      }
    }
  }

  return isset($fast[$endpoint][$object][$field]) ? $fast[$endpoint][$object][$field] : FALSE;
}

function giantbomb_endpoint($endpoint) {
  $endpoints = array(
    'giantbomb' => 'GiantBombEndpoint',
  );

  return isset($endpoints[$endpoint]) ? $endpoints[$endpoint] : FALSE;
}

/**
 * Get the property type handler for a property.
 *
 * @param string $data_type
 *   Data type as found in property definition.
 * @return string|bool
 *   Class name, or FALSE.
 */
function giantbomb_property_type($data_type) {
  $data_types = array(
    'text' => 'GiantBombTextProperty',
    'html' => 'GiantBombHTMLProperty',
    'date' => 'GiantBombDateProperty',
    'url' => 'GiantBombURLProperty',
    'int' => 'GiantBombIntProperty',
    'image' => 'GiantBombImageProperty',
    'images' => 'GiantBombImagesProperty',
  );

  return isset($data_types[$data_type]) ? $data_types[$data_type] : FALSE;
}

/**
 * Return properties for a GiantBomb object type.
 *
 * @param string $endpoint
 * @param string $object
 */
function giantbomb_object_properties($endpoint, $object) {
  $endpoint_class = giantbomb_endpoint($endpoint);
  $endpoint_object = new $endpoint_class;
  $properties = $endpoint_object->object_properties($object);
  return isset($properties) ? $properties : FALSE;
}

/**
 * Find list name of a property, if a property is a list.
 *
 * For example:
 *
 *  A 'game' has 'killed_characters', which contain 'character' objects.
 *
 *  giantbomb_property_is_list('giantbomb', 'game', 'killed_characters') 
 *  would return 'characters'.
 *
 *  Whereas
 *
 *  giantbomb_property_is_list('giantbomb', 'character', 'killed_characters') 
 *  would return FALSE because a character does not have killed characters.
 *
 * @param string $endpoint
 * @param string $object
 *   Singular object type.
 * @param string $property
 *   Property name from API request.
 * @return string|property
 */
function giantbomb_property_is_list($endpoint, $object, $property) {
  $lists = array_keys(giantbomb_types_all($endpoint, 'list'));

  if (in_array($property, $lists)) {
    return $property;
  }

  $endpoint_class_name = giantbomb_endpoint($endpoint);
  if (class_exists($endpoint_class_name)) {
    $endpoint_object = new $endpoint_class_name;
    return $endpoint_object->property_is_list($object, $property);
  }
}

/**
 *
 * @param string $endpoint
 *   A Giant Bomb endpoint name.
 * @param string $object
 *   A Giant Bomb object.
 * @param array $settings
 *   Valid keys:
 *     - array load
 *     - array related_create:
 *       Create related objects of these types. (Array containing the ID of
 *       the list type)
 *     - array related_update:
 *       Update related objects of these types. (Array containing the ID of
 *       the list type): Load all these resource types on this page.
 *       (objects and or lists). Lists will queue all lists.
 *     - int offset: How many objects into list to start loading.
 */
function giantbomb_process_job_list($endpoint, $object, $settings = array()) {
  $args = array();

  $object_info = giantbomb_type_load($endpoint, $object);
  if (!$object_info) {
    return FALSE;
  }

  // Merge defaults
  $settings += array(
    'related_create' => array(),
    'related_update' => array(),
    'offset' => 0,
    'load' => array(),
  );
  $settings['load'] += array(
    'lists' => TRUE,
    'objects' => TRUE,
  );

  $args['offset'] = $settings['offset'];

  $request_list = array(
    'endpoint' => $endpoint,
    'object' => $object,
    'settings' => $settings,
  );

  $request_object = array(
    'endpoint' => $endpoint,
    'object' => $object,
    'settings' => array(
      'create' => in_array($object_info->id, $settings['list_related_create']),
      'update' => in_array($object_info->id, $settings['list_related_update']),
      'related_create' => isset($settings['object_related_create']) ? $settings['object_related_create'] : array(),
      'related_update' => isset($settings['object_related_update']) ? $settings['object_related_update'] : array(),
      'relationships_create' => isset($settings['object_relationships_create']) ? $settings['object_relationships_create'] : array(),
    ),
  );

  // Fail if there is nothing to request
  if (!$request_object['settings']['create'] && !$request_object['settings']['update']) {
    return FALSE;
  }

  $query = giantbomb_api_query_list($endpoint, $object_info->list, $args);

  if ($query->status_code != GIANTBOMB_API_CODE_OK) {
    return FALSE;
  }

  // Add all remaining pages to queue.
  if ($settings['load']['lists']) {
    $queue_list = DrupalQueue::get('giantbomb_list');

    // Cancel recursive loading of lists
    $request_list['settings']['load']['lists'] = FALSE;

    if (!empty($query->number_of_page_results)) {
      $results_per_page = $query->number_of_page_results;
      $pages = ceil($query->number_of_total_results / $results_per_page);

      for ($i = 1; $i < $pages; $i++) {
        // This will skip first page
        $request_list['settings']['offset'] = ($i * $results_per_page);
        $queue_list->createItem($request_list);
      }
    }
  }

  if ($settings['load']['objects']) {
    // Add all objects on this page to queue
    $queue_object = DrupalQueue::get('giantbomb_object');
    foreach ($query->results as $result) {
      if (isset($result->id)) {
        $request_object['id'] = $result->id;
        $queue_object->createItem($request_object);
      }
    }
  }
}

/**
 * Prepares an entity, loads data from the API object into the entity,
 * and saves the entity.
 *
 * If the entity is new, then it will be created immediately. Otherwise
 * we'll check if it is time to update the existing entity. If not, we'll quit
 * immediately.
 *
 * @param array $settings
 *   Valid keys:
 *     - array related_create:
 *       Create related objects of these types. (Array of type ID's)
 *     - array related_update:
 *       Update related objects of these types. (Array of type ID's)
 *     - array relationships_create:
 *       For each related object, create relationships of these types (Array of
 *       type ID's)
 *     - array relate_to: Establish a relationship between the local object
 *       and another local object. 'relate' is an array containing multiple
 *       arrays: array('type' => string_object_type, 'id' => int_object_id). Where type
 *       id are the identifiers from the API.
 *     - bool create: Create the entity if it does not exist.
 *     - bool update: Update the entity if it exists locally.
 */
function giantbomb_process_job_object($endpoint, $object, $id, $settings = array()) {
  $object_info = giantbomb_type_load($endpoint, $object);

  // Endpoint
  $object_property_info = giantbomb_object_properties($endpoint, $object);
  if (!$object_property_info) {
    return;
  }

  // Merge defaults
  $settings += array(
    'create' => TRUE,
    'update' => TRUE,
    'object_related_create' => array(),
    'object_related_update' => array(),
    'object_relationships_create' => array(),
  );

  // Begin new lookup
  if (empty($object_info->entity_type) || empty($object_info->bundle)) {
    return FALSE;
  }

  $entity_type = $object_info->entity_type;
  $bundle = $object_info->bundle;
  $entity_info = entity_get_info($entity_type);
  $types = giantbomb_types_all($endpoint, 'list');
  $queue_object = DrupalQueue::get('giantbomb_object');


  // Does this object need to be created?
  list(, $entity) = giantbomb_local_entity_load($endpoint, $object, $id);
  $first_update = FALSE;

  if ($entity) {
    if (!$settings['update']) {
      return;
    }
  }
  // Build new entity
  else {
    if ($settings['create']) {
      $first_update = TRUE;
      $values = array(
        // Hopefully the only required property is bundle. Things may get messy.
        $entity_info['entity keys']['bundle'] => $bundle,
      );
      $entity = entity_create($entity_type, $values);
      _giantbomb_static_void($endpoint, $object, $id);
    }
    else {
      return;
    }
  }

  $wrapper = entity_metadata_wrapper($entity_type, $entity);

  if ($first_update) {
    $field_endpoint = giantbomb_field_name($endpoint, $object, GIANTBOMB_FIELD_ENDPOINT);
    $field_type = giantbomb_field_name($endpoint, $object, GIANTBOMB_FIELD_TYPE);
    $field_id = giantbomb_field_name($endpoint, $object, GIANTBOMB_FIELD_ID);

    $wrapper->{$field_endpoint[0]}->set($endpoint);
    $wrapper->{$field_type[0]}->set($object);
    $wrapper->{$field_id[0]}->set($id);
  }

  // Request object from API.
  $query = giantbomb_api_query_object($endpoint, $object, $id);

  if ($query->status_code != GIANTBOMB_API_CODE_OK) {
    return FALSE;
  }

  $property_values = array();
  $requested_object = array('endpoint' => $endpoint, 'type' => $object_info->id, 'id' => $id);

  // Loop through results
  foreach ($query->results as $property => $value) {
    // Related lists and objects.

    // Lists
    if (is_array($value) && $list = giantbomb_property_is_list($endpoint, $object, $property)) {
      if (isset($types[$list])) {
        $type = $types[$list];
        $create = in_array($type->id, $settings['object_related_create']);
        $update = in_array($type->id, $settings['object_related_update']);
        if ($create || $update) {
          foreach ($value as $related_object) {
            $request_object = array(
              'endpoint' => $endpoint,
              'object' => $type->object,
              'id' => $related_object->id,
              'settings' => array(
                'create' => $create,
                'update' => $update,
              ),
            );

            if (in_array($type->id, $settings['object_relationships_create'])) {
              $request_object['settings']['relate_to'][] = array(
                'type' => $object,
                'id' => $id,
              );
            }

            $queue_object->createItem($request_object);
          }
        }
      }
    }

    // Properties of object
    else {
      if (isset($object_property_info[$property]['data type'])) {
        $property_class = giantbomb_property_type($object_property_info[$property]['data type']);
        if ($property_class && class_exists($property_class) && $value) {
          $property_object = new $property_class($value, array(), $requested_object);
          if ($property_object->validate()) {
            $property_values[$property] = $property_object;
          }
        }  
      }
    }
  }

  // Entity Properties
  $entity_properties = array('label');
  foreach ($entity_properties as $entity_property) {
    if (isset($object_info->settings['entity_mapping'][$entity_property])) {
      $property = $object_info->settings['entity_mapping'][$entity_property];
      if (isset($property_values[$property]) && FALSE !== ($value = $property_values[$property]->value())) {
        if (isset($entity_info['entity keys'][$entity_property])) {
          $entity->{$entity_info['entity keys'][$entity_property]} = $value;
        }
      }
    }
  }

  // Fields
  $instances = field_info_instances($object_info->entity_type, $object_info->bundle);
  foreach ($instances as $field_name => $instance) {
    $id = $instance['id'];
    $field_name = $instance['field_name'];
    if (isset($object_info->settings['field_mapping'][$id])) {
      foreach ($object_info->settings['field_mapping'][$id] as $column => $property) {
        $property_name = $property['property'];
        if (isset($property_values[$property_name])) {
          $value = clone $property_values[$property_name];

          if (!empty($property['settings'])) {
            $value->settings_set($property['settings']);
          }

          $field_value = $value->value();
          if ($field_value !== FALSE) {
            $entity->{$field_name}[LANGUAGE_NONE][0][$column] = $field_value;
          }
        }
      }
    }
  }

  // Relationships
  // Create a relationship TO THIS entity.
  if (isset($settings['relate_to'])) {
    foreach ($settings['relate_to'] as $relationship) {
      $relate_type = giantbomb_type_load($endpoint, $relationship['type']);
      if ($relate_type) {
        // Relate if the relating object exists locally.
        list($relate_entity_type, $relate_entity_id) = giantbomb_local_entity($endpoint, $relationship['type'], $relationship['id']);
        if ($relate_entity_id) {
          // The entity relating to must have been created, before a
          // relationship can be established.

          // Relation
          entity_save($entity_type, $entity);
          list(, $entity_id) = entity_extract_ids($entity_type, $entity);
          if (!empty($object_info->settings['relationships']['relation'][$relate_type->id])) {
            $relation_type = $object_info->settings['relationships']['relation'][$relate_type->id];
            if ($entity_id) {
              $endpoints = array(
                array(
                  'entity_type' => $entity_type,
                  'entity_id' => $entity_id,
                ),
                array(
                  'entity_type' => $relate_entity_type,
                  'entity_id' => $relate_entity_id,
                ),
              );
              if (!relation_relation_exists($endpoints, $relation_type)) {
                $relation = relation_create($relation_type, $endpoints);
                relation_save($relation);
              }
            }
          }

          // Entityreference
          // Ensure related object is the same entity_type. Only valid for entityreference.
          /*
           if ($entity_type == $relate_entity_type) {
          if ($relationship['type'] == 'person') {
          $wrapper->{'entity_reference_field_name'}->set($relate_entity_id);
          }
          elseif ($relationship['type'] == 'game') {
          $wrapper->{'entity_reference_field_name'}->set($relate_entity_id);
          }
          }
          */
        }
      }
    }
  }

  // Determine next update value.
  $field_next_update = giantbomb_field_name($endpoint, $object, GIANTBOMB_FIELD_NEXT_UPDATE);
  if ($field_next_update) {
    // @todo: fix multi-value date fields.
    $wrapper->{$field_next_update[0]}->set(time() + $object_info->update_frequency);
  }

  entity_save($entity_type, $entity);
}