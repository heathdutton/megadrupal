<?php
/**
 * @file
 * XCMetadataEntity class definition
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

class XCMetadataEntity extends XCMetadata {
  public $type;
  public $node_type;
  public $title;
  public $description;
  public $module;
  public $entities = array();
  public $fields = array();
  public $attributes = array();
  public $namespaces = array();

  /**
   * Instantiate an object representating a metadata entity type definition.
   * The object is created regardless of whether the definition has been "set"
   * to exist in the database.
   *
   * If the definiton has been "set" the constructor will first query and cache
   * the version in the database. Afterwards, unless the cache is cleared or
   * rebuilt, all subsequent calls to instantiate this object will return the
   * version cached during the page load cycle.
   *
   * Note that this version may or may not have been "set" to exist in the
   * database either.
   *
   * If the definition has not been "set", it is still cached. The only
   * difference is that there is no version in the database.
   *
   * This is how the Metadata module handles its objects. All calls to
   * instantiate a definition with similar properties will return a copy (not
   * a reference) of the most updated version of the same definition, if
   * possible.
   *
   * @param $type
   *    Entity type
   * @param $module
   *    Module machine name
   */
  public function __construct($type, $module = '') {
    static $sql_cache = array();
    timer_start('XCMetadataEntity');
    timer_start('XCMetadataEntity/' . $type);

    // $qualified = _object_to_qualified_name(array('type' => $type), 'entity');
    $_cached = xc_metadata_cache_get('entity', $type);

    if (!empty($_cached)) {
      $object = $_cached;
    }
    elseif (!empty($type)) {
      if (!isset($sql_cache['type'])) {
        $sql_cache['type'] = "SELECT type, node_type, title, description, module
                FROM {xc_metadata_entity}
                WHERE type = ':type'";
        if (!empty($module)) {
          $sql_cache['type'] .= " AND module = '" . $module  . "'";
        }
      }
      $query_temp = str_replace(":type", $type, $sql_cache['type']);
      $object = db_query($query_temp)->fetchObject();
      if (!empty($object)) {
        $entities = array();
        $fields = $field_qualifieds = array();
        $attributes = $attribute_qualifieds = array();
        $namespaces = array();

        if (!isset($sql_cache['parent'])) {
          $sql_cache['parent'] = "SELECT parent FROM {xc_metadata_entity_entities} WHERE child = '%s'";
        }
        $result = db_query('SELECT parent FROM {xc_metadata_entity_entities} WHERE child = :child', array(':child' => $type));
        foreach ($result as $entity) {
          $entities['parents'][] = array('type' => $entity->parent);
        }

        if (!isset($sql_cache['child'])) {
          $sql_cache['child'] = "SELECT child FROM {xc_metadata_entity_entities} WHERE parent = '" . $type . "'";
        }
        $result = db_query($sql_cache['parent']);
        foreach ($result as $entity) {
          $entities['children'][] = array('type' => $entity->child);
        }

        if (!isset($sql_cache['field'])) {
          $sql_cache['field'] = 'SELECT DISTINCT field FROM {xc_metadata_field_entities} WHERE entity = :entity';
        }
        $result = db_query($sql_cache['field'], array(':entity' => $type));
        foreach ($result as $field) {
          $field_qualifieds[] = $field->field;
          $field_array = _qualified_name_to_array($field->field, 'field');
          $fields[] = $field_array;
          if (!empty($field_array['namespace'])) {
            $namespaces[] = $field_array['namespace'];
          }
        }

        if (!isset($sql_cache['attribute'])) {
          $query = "SELECT DISTINCT attribute FROM {xc_metadata_field_attributes} WHERE field IN ('". implode("','", $field_qualifieds) ."')";
        }
        // TODO Please convert this statement to the D7 database API syntax.
        $result = db_query($query);
        foreach ($result as $attribute) {
          $attribute_array = _qualified_name_to_array($attribute->attribute,
            'attribute');
          $attribute_qualifieds[] = $attribute->attribute;
          $attributes[] = $attribute_array;
          if (!empty($attribute_array['namespace'])) {
            $namespaces[] = $attribute_array['namespace'];
          }
        }
        $object->entities = $entities;
        $object->fields = $fields;
        $object->attributes = $attributes;
        foreach (array_unique($namespaces) as $namespace) {
          $object->namespaces[] = _qualified_name_to_array($namespace, 'namespace');
        }
        if (empty($object->namespaces)) {
          $object->namespaces = array();
        }
        xc_metadata_cache_set('entity', $type, $object);
      }
    }

    if (!empty($object)) {
      $this->type = $object->type;
      $this->node_type = $object->node_type;
      $this->title = $object->title;
      $this->description = $object->description;
      $this->module = $object->module;
      $this->entities = $object->entities;
      $this->fields = $object->fields;
      $this->attributes = $object->attributes;
      $this->namespaces = $object->namespaces;
      $this->set = TRUE;
    }
    else {
      $this->type = $type;
      $this->module = $module;
    }

    if (empty($this->node_type)) {
      $this->node_type = 'xc_' . $this->type;
    }

    timer_stop('XCMetadataEntity/' . $type);
    timer_stop('XCMetadataEntity');
  }

  /**
   * Get metadata entity type definitions for all entities that can be either
   * a parent or a child to entities of of this type.
   *
   * @see xc_metadata_entity_get_entities()
   * @return
   *    An array of XCMetadataEntity objects
   */
  public function get_entities() {
    return xc_metadata_entity_get_entities($this);
  }

  /**
   * Get metadata field type definitions for all fields that entities of this
   * type can contain.
   *
   * @see xc_metadata_entity_get_fields()
   * @return
   *    An array of XCMetadataField objects
   */
  public function get_fields() {
    return xc_metadata_entity_get_fields($this);
  }

  /**
   * Get metadata attribute type definitions for all attribute types of all
   * fields that entities of this type can contain.
   *
   * @see xc_metadata_entity_get_attributes()
   * @return
   *    An array of XCMetadataAttribute objects
   */
  public function get_attributes() {
    return xc_metadata_entity_get_attributes($this);
  }

  /**
   * Get metadata namespace type definitions for all namespace types of all
   * fields and attributes that entities of this type can contain.
   *
   * @see xc_metatdata_entity_get_namespaces()
   * @return
   *    An array of XCMetadataNamespace objects
   */
  public function get_namespaces() {
    return xc_metadata_entity_get_namespaces($this);
  }

  /**
   * Set to the database the relationship definition for existing entity types
   * of entities that can either be a parent or a child to entitues of this
   * type by inserting or updating the properties of this object.
   *
   * Also sets this entity type definition to the databse if not already set.
   * Note that an empty array results in no entity type relationships.
   *
   * @see xc_metadata_entity_set_entities()
   * @param $reset
   *    TRUE to clear and rebuild the metadata definitions cache
   * @param $force
   *    FALSE to validate the definition before creation; TRUE to create the
   *    definition regardless of any errors that may occur during the process
   */
  public function set_entities($reset = TRUE, $force = FALSE) {
    xc_metadata_entity_set_entities($this, $this->entities, $reset, $force);
  }

  /**
   * Set to the databse the entity type definition either by inserting or
   * updating the properties of this object.
   *
   * @see xc_metadata_entity_set()
   * @see xc_metadata_entity_set_entities()
   * @param $reset
   *    TRUE to clear and rebuild the metadata definitions cache
   * @param $force
   *    FALSE to validate the definition before creation; TRUE to create the
   *    definition regardless of any errors that may occur during the process
   */
  public function set_definition($reset = TRUE, $force = FALSE) {
    xc_metadata_entity_set($this, $reset);
    xc_metadata_entity_set_entities($this, $this->entities, $reset, $force);
  }

  /**
   * Unset from the database the entity type definition by deleting any
   * existing defition set to the database definition and removing the object
   * from the cache.
   *
   * @see xc_metadata_entity_unset()
   * @param $reset
   *    TRUE to clear and rebuild the metadata definitions cache
   * @param $force
   *    FALSE to validate the definition before removal; TRUE to remove the
   *    definition regardless of any errors that may occur during the process
   */
  public function unset_definition($reset = TRUE, $force = FALSE) {
    xc_metadata_entity_unset($this, $reset, $force);
  }
}
