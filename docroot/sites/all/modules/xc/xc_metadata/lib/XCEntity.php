<?php
/**
 * @file
 * XCEntity class definition
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

class XCEntity {

  /** The type of entity */
  public $metadata_type;

  /** The ID of entity */
  public $metadata_id;

  /** A unique identifier **/
  public $identifier;

  /** The Drupal node type */
  public $node_type = NULL;

  /** The Drupal node ID */
  public $node_id = NULL;

  /** The content of the object as array */
  public $metadata = array();

  /** The source data in its original format */
  public $raw;

  /** The name of the format of the original data */
  public $format;

  /** The creation timestap */
  public $timestamp;

  /** The update timestap */
  public $updated;

  /** The relationships of the entity */
  public $relationships = array();

  /** Has the entity been built? */
  public $built = FALSE;

  /** Has the entity been stored? */
  public $stored = FALSE;

  /** Has the entity been stored? */
  public $deleted = FALSE;

  /** Locks on the metadata **/
  public $locks = array();

  /** Source of metadata **/
  public $source_id = NULL;

  /** Location of metadata **/
  public $locations = array();

  /** Additional properties **/
  public $properties = array();

  /**
   * Creates a new entity
   * @param $arg (mixed)
   *   If it is a string, it will be the identifier.
   *   If it is an array, or an object, it contains the properties of the XCEntity.
   *   The possible keys:
   *   metadata_id, node_id, nid, identifier, metadata_type, node_type, metadata,
   *   raw, format, created, updated, relationships, built, stored, deleted,
   *   locks, source_id, locations, properties
   * @param $build (Boolean)
   *   Flag whether to build the entity
   * @param $store (Boolean)
   *   Flag whether to store the entity
   * @param $params (Array)
   *   Other parameters
   *
   * @return XCEntity
   *   The newly created entity
   */
  public function __construct($arg = NULL, $build = FALSE, $store = FALSE, $params = array()) {
    global $_oaiharvester_statistics;

    $trace_time = (isset($params['trace_time']) && $params['trace_time'] == TRUE) ? TRUE : FALSE;
    if ($trace_time) {
      $t_start = microtime(TRUE);
    }
    if (is_array($arg) || is_object($arg)) {
      $properties = is_object($arg) ? get_object_vars($arg) : $arg;

      // Attempt to instantiate an existing metadata object from whatever
      // is in the array or object
      if ($trace_time) {
        $t1 = microtime(TRUE);
      }

      if (!empty($properties['metadata_id'])) {
        $metadata_id = (int) $properties['metadata_id'];
      }
      if ($trace_time) {
        $t2 = microtime(TRUE);
        xc_oaiharvester_statistics_set($params['caller'] . '/metadata_id', abs($t2 - $t1));
      }

      if (empty($metadata_id)
           && !empty($properties['identifier_int'])
           && (empty($params['no_retrieve']) || $params['no_retrieve'] == FALSE)) {
        $metadata_id = xc_metadata_metadata_id_from_identifier_int($properties['identifier']);
      }
      if ($trace_time) {
        $t2a = microtime(TRUE);
        xc_oaiharvester_statistics_set($params['caller'] . '/identifier', abs($t2a - $t2));
      }

      if (empty($metadata_id)
           && !empty($properties['identifier'])
           && (empty($params['no_retrieve']) || $params['no_retrieve'] == FALSE)) {
        $metadata_id = _identifier_to_metadata_id($properties['identifier']);
      }
      if ($trace_time) {
        $t3 = microtime(TRUE);
        xc_oaiharvester_statistics_set($params['caller'] . '/identifier', abs($t3 - $t2a));
      }

      if (empty($metadata_id) && !empty($properties['node_id'])) {
        $metadata_id = _node_id_to_metadata_id($properties['node_id']);
      }
      if ($trace_time) {
        $t4 = microtime(TRUE);
        xc_oaiharvester_statistics_set($params['caller'] . '/node_id', abs($t4 - $t3));
      }

      if (empty($metadata_id) && !empty($properties['nid'])) {
        $metadata_id = _node_id_to_metadata_id($properties['nid']);
      }
      if ($trace_time) {
        $t5 = microtime(TRUE);
        xc_oaiharvester_statistics_set($params['caller'] . '/nid', abs($t5 - $t4));
      }
    }
    else {
      // Otherwise assume the argument is a metadata id
      $metadata_id = (int) $arg;
      $properties = array();
    }
    if ($trace_time) {
      $t_start2 = microtime(TRUE);
      xc_oaiharvester_statistics_set($params['caller'] . '/init', abs($t_start2 - $t_start));
    }

    if (!empty($metadata_id)) {
      $this->metadata_id = $metadata_id;

      if ($trace_time) {
        $t_qual = microtime(TRUE);
      }
      $qname = _object_to_qualified_name($this, 'metadata');
      if ($trace_time) {
        $t_cached = microtime(TRUE);
        xc_oaiharvester_statistics_set($params['caller'] . '/_object_to_qualified_name', abs($t_cached - $t_qual));
      }
      $_cached = xc_cache_get('metadata', $qname, TRUE);
      if ($trace_time) {
        $t_cached2 = microtime(TRUE);
        xc_oaiharvester_statistics_set($params['caller'] . '/xc_cache_get', abs($t_cached2 - $t_cached));
      }
      if (!empty($_cached)) {
        $this->metadata_type = $_cached->metadata_type;
        $this->node_type     = $_cached->node_type;
        $this->node_id       = $_cached->node_id;
        $this->metadata      = $_cached->metadata;
        $this->raw           = $_cached->raw;
        $this->format        = $_cached->format;
        $this->created       = $_cached->created;
        $this->updated       = $_cached->updated;
        $this->relationships = $_cached->relationships;
        $this->built         = $_cached->built;
        $this->stored        = $_cached->stored;
        $this->deleted       = $_cached->deleted;
        $this->locks         = $_cached->locks;
        $this->source_id     = $_cached->source_id;
        $this->locations     = $_cached->locations;
        $this->properties    = $_cached->properties;
        $this->identifier_int = $_cached->identifier_int;
      }
      else {
        $this->retrieve(NULL, $params);
        if ($trace_time) {
          $t_retrieve = microtime(TRUE);
          xc_oaiharvester_statistics_set($params['caller'] . '/xc_cache_get', abs($t_retrieve - $t_cached));
        }
      }
    }


    // Update the cached or loaded XCEntity with whatever was passed
    // to the constructor from an object or array
    foreach ($properties as $key => $value) {
      $this->$key = $value;
    }

    // If building requested
    if (!$this->built && $build) {
      if ($trace_time) {
        $t_build = microtime(TRUE);
      }
      $this->build($params);
      if ($trace_time) {
        $t_build2 = microtime(TRUE);
        xc_oaiharvester_statistics_set($params['caller'] . '/build', abs($t_build2 - $t_build));
      }
    }

    // If storage requested
    if ($this->stored && $store) {
      if ($trace_time) {
        $t_store = microtime(TRUE);
      }
      $this->store(NULL, $params);
      if ($trace_time) {
        $t_store2 = microtime(TRUE);
        xc_oaiharvester_statistics_set($params['caller'] . '/store', abs($t_store2 - $t_store));
      }
    }
  }

  public static function load($metadata_id) {
    return new XCEntity(array('metadata_id' => $metadata_id));
  }

  public static function loadNodeID($node_id) {
    return new XCEntity(array('node_id' => $node_id));
  }

  public function get_properties() {
    return xc_entity_load_properties($this);
  }

  public function get_relationships() {
    return xc_entity_load_relationships($this);
  }

  public function get_source() {
    xc_source_get($this->source_id);
  }

  public function get_default_location() {
    xc_source_get_default_location($this->source_id);
  }

  public function get_all_locations() {
    xc_source_get_locations($this->source_id);
  }

  public function get_definition() {
    return xc_metadata_entity_get($this->metadata_type);
  }

  public function get_node() {
    return node_load($this->node_id);
  }

  public function set_relationships() {
    $object = xc_entity_set_relationships($this);
    if ($object) {
      return $this;
    }
  }

  /**
   * Call _xc_build, which constructs metadata objects from its raw input data
   * based on the raw import format.
   */
  public function build($params = array()) {
    _xc_build($this, $this->format, $params);
  }

  /**
   * Call _xc_transform
   * @see _xc_transform
   * @param $format
   * @return unknown_type
   */
  public function transform($format = '', $params = array()) {
    return _xc_transform($this, $format, $params);
  }

  public function store($locations = array(), $params = array()) {
    _xc_store($this, $locations, $params);
  }

  /**
   * Retrieve metadata instances
   * @param (Array) $locations
   *   The locations from this item should be retrieved
   * @param (Array) $params
   *   Other parameters
   * @return unknown_type
   */
  public function retrieve($locations = array(), $params = array()) {
    _xc_retrieve($this, $locations, $params);
  }

  public function remove($locations = array(), $params = array()) {
    _xc_remove($this, $locations, $params);
  }

  public function alter($params = array()) {
    _xc_alter($this, $params);
  }

  public function validate() { }

  public function clean() { }

  public function fetch_field($field, $delta = 0) {
    return xc_fetch_field($this, $field, $delta);
  }

  public function fetch_attribute($field, $attribute, $delta = 0) {
    return xc_fetch_field($this, $field, $attribute, $delta);
  }

  public function alter_field($field, $delta = 0, $alter = NULL, $value = NULL) {
    xc_alter_field($this, $field, $delta, $alter, $value);
  }

  public function alter_attribute($field, $attribute, $delta = 0, $alter = NULL, $value = NULL) {
    xc_alter_field($this, $field, $attribute, $delta, $alter, $value);
  }

  public function is_locked() {
    // TODO: Do a little more than just say if its locked
  }

}
