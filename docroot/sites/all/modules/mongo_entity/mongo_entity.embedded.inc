<?php

/**
 * @file
 * Defines entity class and controller class for embedded MongoDB documents
 *
 * Due to limitations in the current version of MongoDB, an embedded entity
 * can only be stored one level down.
 *
 */

/**
 * A controller class for entities stored as subdocuments in a collection.
 */
class MongoEmbeddedEntityController extends MongoEntityController {

  /**
   * {@inheritdoc}
   */
  public function __construct($entity_type) {
    parent::__construct($entity_type);
  }

  function getEmbeddedProperties() {

    // @todo Cache this!

    $return = array();

    foreach (entity_get_property_info() as $entity_type => $property_info) {
      $properties = array();
      if (isset($property_info['properties'])) {
        $properties += $property_info['properties'];
      }
      if (isset($property_info['bundles'])) {
        foreach ($property_info['bundles'] as $bundle => $bundle_property_info) {
          if (isset($bundle_property_info['properties'])) {
            $properties += $bundle_property_info['properties'];
          }
        }
      }
      foreach ($properties as $property_name => $info) {

        if (isset($info['type'])) {

          if (preg_match('/^list\<(.+)\>$/', $info['type'], $matches)) {
            $type = $matches[1];
          }
          else {
            $type = $info['type'];
          }

          if ($type == $this->entityType) {
            // Check that the parent entity type is a MongoEntity, but not an embedded MongoEntity
            if (($controller = _mongo_entity_get_controller($entity_type)) && is_a($controller, 'MongoEntityController') && !is_a($controller, 'MongoEmbeddedEntityController')) {
              $return[$property_name][] = $entity_type;
            }
          }
        }
      }
    }

    return $return;

  }

  /**
   * {@inheritdoc}
   */
  public function load($ids = array(), $conditions = array()) {

    $entities = array();

    $embedded_properties = $this->getEmbeddedProperties();

    foreach ($embedded_properties as $name => $entity_types) {
      $field = field_info_field($name);
      foreach ($entity_types as $entity_type) {
        if ($ids) {
          // @todo use a method here
          $collection = mongodb_collection($entity_type);

          // $elemMatch only works if only one ID is passed and the field has multiple values
          if (count($ids) == 1 && !empty($field) && $field['cardinality'] != 1) {
            $id = current($ids);
            $cursor = $collection->find(array("{$name}._id" => $id), array($name => array('$elemMatch' => array('_id' => $id))));
          }
          else {
            $cursor = $collection->find(array("{$name}._id" => array('$in' => $ids)));
          }

          // Reduce the matched results to items with matching _ids
          foreach ($cursor as $doc) {

            if (isset($doc[$name]['_id'])) {
              $items = array($doc[$name]);
            }
            else {
              $items = $doc[$name];
            }

            foreach ($items as $item) {
              if (in_array($item['_id'], $ids)) {
                $id = $item['_id'];
                $item['_parent'] = $doc['_id'];
                // The field name of this embedded entity
                // @todo use dot notation for secondary embedded entities
                if (!empty($field)) {
                  $item['_field'] = $name;
                }
                else {
                  $item['_property'] = $name;
                }
                // The entity type that this embedded entity is stored in
                $item['_collection'] = $entity_type;
                // @todo set the embedded entity's parent collection, too, somehow
                $entities[$id] = $this->buildEntity($item);
              }
            }
          }
        }

        else if ($conditions) {

          $result = _mongo_entity_embedded_entity_query($entity_type, $name, $conditions);

          foreach ($result['result'] as $doc) {
            $item = $doc[$field_name];
            $id = $item['_id'];
            $item['_parent'] = $doc['_id'];
            if (!empty($field)) {
              $item['_field'] = $name;
            }
            else {
              $item['_property'] = $name;
            }
            $item->_collection = $entity_type;
            // @todo Each one of these may be a different entity type, I guess
            $entities[$id] = $this->buildEntity($item);
          }

        }

      }
    }

    // Ensure that the returned array is ordered the same as the original
    // $ids array if this was passed in and remove any invalid ids.
    $passed_ids = is_array($ids) ? array_flip($ids) : FALSE;
    if ($passed_ids && $passed_ids = array_intersect_key($passed_ids, $entities)) {
      foreach ($passed_ids as $id => $value) {
        $passed_ids[$id] = $entities[$id];
      }
      $entities = $passed_ids;
    }

    return $entities;

  }

  /**
   * {@inheritdoc}
   */
  public function save($entity) {

    // @todo check for _parent, _collection, and _field

    if (empty($entity->_parent) || empty($entity->_collection)) {
      throw new Exception('Unable to save MongoEmbeddedEntity without parent or collection properties');
    }

    $document = $this->buildDocument($entity);
    $collection = mongodb_collection($entity->_collection);

    $this->invoke('presave', $entity);

    if ($entity->is_new) {
      throw new Exception('Unable to insert new MongoEmbeddedEntity');
    }
    else {

      if (isset($entity->_field) && ($field = field_info_field($entity->_field))) {

        $query = array('_id' => $entity->_parent, "{$entity->_field}._id" => $entity->_id);
        if ($field['cardinality'] == 1) {
          // Set the subdocument directly
          $collection->update($query, array('$set' => array($entity->_field => $document)));
        }
        else {
          // Use a positional operator
          $collection->update($query, array('$set' => array("{$entity->_field}.$" => $document)));
        }
        $this->invoke('update', $entity);
        $return = SAVED_UPDATED;

      }
      else if (isset($entity->_property)) {

        $query = array('_id' => $entity->_parent, "{$entity->_property}._id" => $entity->_id);

        // @todo _collection may not be the actual entity type
        $info = entity_get_all_property_info($entity->_collection)[$entity->_property];
        if (preg_match('/^list\<(.+)\>$/', $info['type'], $matches)) {
          $collection->update($query, array('$set' => array("{$entity->_property}.$" => $document)));
        }
        else {
          $collection->update($query, array('$set' => array($entity->_property => $document)));
        }
        $this->invoke('update', $entity);
        $return = SAVED_UPDATED;

      }
      else {
        // @todo throw Exception
      }
    }

    // Save fields.
    $function = "field_attach_update";
    $function($this->entityType, $entity);

    unset($entity->is_new);
    unset($entity->original);
    return $return;

  }

  /**
   * {@inheritdoc}
   */
  public function delete($ids) {

    $ids = array_map('intval', $ids);

    // Delete embedded fields
    $embedded_properties = $this->getEmbeddedProperties();
    foreach ($embedded_properties as $name => $entity_types) {

      // Find a field with this name
      $field = field_info_field($name);
      foreach ($entity_types as $entity_type) {

        // Find a property with this name
        $property_info = entity_get_property_info($entity_type);
        $property = NULL;

        if (isset($property_info['properties'][$name])) {
          $property = $property_info['properties'][$name];
        }
        elseif (isset($property_info['bundles'])) {
          foreach ($property_info['bundles'] as $bundle => $bundle_property_info) {
            if (isset($bundle_property_info['properties'][$name])) {
              $property = $bundle_property_info['properties'][$name];
            }
          }
        }

        $is_list = FALSE;

        if (!empty($property)) {

          // Check whether this property can be an array
          if (isset($property['field']) && $property['field']) {
            $is_list = $field['cardinality'] > 1;
          }
          else {
            $is_list = preg_match('/^list\<(.+)\>$/', $property['type']);
          }

          $collection = mongodb_collection($entity_type);

          if ($is_list == 1) {

            $collection->update(
              array(),
              array(
                '$pull' => array(
                  $name => array(
                    '_id' => array('$in' => $ids),
                  ),
                ),
              ),
              array('multiple' => TRUE)
            );

          } else {

            // @todo Should this set the entity to null, or delete the field entirely?
            $collection->update(
              array(),
              array(
                '$unset' => array(
                  $name => array(
                    '_id' => array('$in' => $ids),
                  ),
                ),
              ),
              array('multiple' => TRUE)
            );
          }
        }
      }
    }


  }

}
