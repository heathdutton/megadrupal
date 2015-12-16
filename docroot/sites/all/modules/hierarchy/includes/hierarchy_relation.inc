<?php

/**
 * @file
 * A class used for hierarchy relations.
 */

class HierarchyRelation extends Entity {

  /**
   * Entity ID.
   *
   * @var integer
   */
  public $rid = NULL;

  /**
   * The name of the hierarchy position field on the host entity.
   *
   * @var string
   */
  public $field_name = NULL;

  /**
   * The weight of item among its siblings.
   *
   * @var integer
   */
  public $weight = 0;

  /**
   * The host entity object.
   *
   * @var object
   */
  protected $hostEntity = NULL;

  /**
   * The host entity ID.
   *
   * @var integer
   */
  protected $hostEntityId = NULL;

  /**
   * The host entity type.
   *
   * @var string
   */
  protected $hostEntityType = NULL;

  public function __construct($values = array()) {
    parent::__construct($values, 'hierarchy_relation');

    if (isset($this->field_name)) {
      $field_info = field_info_field($this->field_name);
      if ((!$field_info) || ($field_info['type'] != 'hierarchy_position')) {
        throw new Exception("Invalid field name given: {$this->field_name} is not a Hierarchy position field.");
      }
    }
  }

  public function defaultLabel() {
    return '';
  }

  public function buildContent($view_mode = 'full', $langcode = NULL) {
    // No content.
    return array();
  }

  public function hostEntity() {
    if ($this->fetchHostDetails()) {
      if (!isset($this->hostEntity)) {
        $this->hostEntity = entity_load_single($this->hostEntityType, $this->hostEntityId);
      }

      return $this->hostEntity;
    }

    return NULL;
  }

  public function hostEntityType() {
    if ($this->fetchHostDetails()) {
      return $this->hostEntityType;
    }

    return NULL;
  }

  protected function fetchHostDetails() {
    if (!isset($this->hostEntityId)) {
      if ($this->rid) {
        $query = new EntityFieldQuery();
        $query->addTag('DANGEROUS_ACCESS_CHECK_OPT_OUT');
        $query->fieldCondition($this->field_name, 'relation_id', $this->rid);
        $result = $query->execute();
        list($this->hostEntityType, $data) = each($result);
        $this->hostEntityId = $data ? key($data) : FALSE;
      }
      else {
        $this->hostEntityId = FALSE;
      }
    }
    return !empty($this->hostEntityId) || !empty($this->hostEntity);
  }

  /**
   * Deletes the relation and the reference in the host entity.
   */
  public function delete($host_entity_item_deleted = FALSE) {
    if (!$host_entity_item_deleted) {
      throw new Exception('Hierarchy relation entity is not allowed to be deleted directly.');
    }
    parent::delete();
  }

}
