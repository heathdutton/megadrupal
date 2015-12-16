<?php

/**
 * @file
 * A class used for hierarchy types.
 */

class HierarchyType extends Entity {

  /**
   * Entity ID.
   *
   * @var integer
   */
  public $id = NULL;

  /**
   * The machine name of the hierarchy type.
   *
   * @var string
   */
  public $type = '';

  /**
   * The human-readable name of the hierarchy type.
   *
   * @var string
   */
  public $name = '';

  /**
   * The description of the hierarchy type.
   *
   * @var string
   */
  public $description = NULL;

  /**
   * The entity type of hierarchy items.
   *
   * @var string
   */
  public $entity_type = NULL;

  /**
   * Hierarchy uniqueness (singular or multiple).
   *
   * @var int
   */
  public $is_unique = 1;

  /**
   * Hierarchy ordering (hierarchy items are weighted).
   *
   * @var int
   */
  public $is_ordered = 1;

  /**
   * Serialized array with misc options.
   *
   * @var array
   */
  public $data = array();

  public function __construct($values = array()) {
    parent::__construct($values, 'hierarchy_type');
  }

  public function save() {
    if (empty($this->uuid)) {
      $this->uuid = hierarchy_generate_uuid();
    }

    $is_new = empty($this->id);

    parent::save();

    if ($is_new && $this->is_unique && (!hierarchy_exists_for_type($this->type))) {
      // Create unique hierarchy
      // when the corresponding hierarchy type is created.
      $hierarchy = new Hierarchy(array('type' => $this->type, 'name' => $this->name));
      $hierarchy->save();
    }
  }

}
