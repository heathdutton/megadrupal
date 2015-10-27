<?php
/**
 * @file
 * XCMetadataAttribute class definition
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

class XCMetadataAttribute extends XCMetadata {
  public $name;
  public $namespace;
  public $field;
  public $title;
  public $description;
  public $module;
  public $fields = array();
  public $type;
  public $size;
  public $max_size;
  public $required;
  public $default_value;
  public $possible_values = array();

  /**
   * Instantiate an object representating a metadata attribute type definition.
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
   * @param $name
   *    Attribute name
   * @param $namespace
   *    Attribute namespace
   * @param $field
   *    The name of the field to which the attribute belongs
   * @param $module
   *    Module machine name (Optional)
   */
  public function __construct($name, $namespace = '', $field = '', $module = '') {
    $qualified_name = _object_to_qualified_name(
      array(
        'name' => $name,
        'namespace' => $namespace
      ),
      'attribute'
    );
    $cache_key = $field . '@' . $qualified_name;
    $_cached = xc_metadata_cache_get('attribute', $cache_key);
    if (!empty($_cached)) {
      $object = $_cached;
    }
    elseif (!empty($name)) {
      $sql = "SELECT name, namespace, field, title, description, module, type,
                size, max_size, required, default_value, possible_values
              FROM {xc_metadata_attribute}
              WHERE name = '".$name."'
                AND namespace = '".$namespace."'
                AND field = '".$field."'";
      $sql .= empty($module) ? '' : " AND module = '".$module."'";

      $object = db_query($sql)->fetchObject();
      if (!empty($object)) {
        $possible_values = unserialize($object->possible_values);
        if (is_null($possible_values)) {
          $possible_values = array();
        }
        $object->possible_values = $possible_values;

        $fields = array();
        $sql = "SELECT DISTINCT field
                 FROM {xc_metadata_field_attributes}
                 WHERE attribute = '".$qualified_name."'";
        $result = db_query($sql);
        foreach($result as $data) {
          $fields[] = array('type' => $data->field);
        }
        $object->fields = $fields;
        xc_metadata_cache_set('attribute', $cache_key, $object);
      }
    }

    if (!empty($object)) {
      $this->name            = $object->name;
      $this->namespace       = $object->namespace;
      $this->field           = $object->field;
      $this->title           = $object->title;
      $this->description     = $object->description;
      $this->module          = $object->module;
      $this->fields          = $object->fields;
      $this->type            = $object->type;
      $this->size            = $object->size;
      $this->max_size        = $object->max_size;
      $this->required        = $object->required;
      $this->default_value   = $object->default_value;
      $this->possible_values = $object->possible_values;
      $this->set = TRUE;
    }
    else {
      $this->name      = $name;
      $this->namespace = $namespace;
      $this->field     = $field;
      $this->module    = $module;
    }
  }

  public function get_qualified_name() {
    return _array_to_qualified_name(array(
      'name' => $this->name,
      'namespace' => $this->namespace), 'attribute');
  }

  /**
   * Get metadata field type definitions for all fields that can contain
   * attributes of this type.
   *
   * @see xc_metadata_attribute_get_fields()
   * @return
   *    An array of XCMetadataField objects
   */
  public function get_fields() {
    return xc_metadata_attribute_get_fields($this);
  }

  /**
   * Set to the databse the attribute type definition either by inserting or
   * updating the properties of this object.
   *
   * @see xc_metadata_attribute_set()
   * @param $reset
   *    TRUE to clear and rebuild the metadata definitions cache
   * @param $force
   *    FALSE to validate the definition before creation; TRUE to create the
   *    definition regardless of any errors that may occur during the process
   */
  public function set_definition($reset = TRUE, $force = FALSE) {
    xc_metadata_attribute_set($this, $reset, $force);
  }

  /**
   * Unset from the database the attribute type definition by deleting any
   * existing defition set to the database definition and removing the object
   * from the cache.
   *
   * @see xc_metadata_attribute_unset()
   * @param $reset
   *    TRUE to clear and rebuild the metadata definitions cache
   * @param $force
   *    FALSE to validate the definition before removal; TRUE to remove the
   *    definition regardless of any errors that may occur during the process
   */
  public function unset_definition($reset = TRUE, $force = FALSE) {
    xc_metadata_attribute_unset($this, $reset, $force);
  }
}
