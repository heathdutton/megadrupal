<?php

/**
 * @file
 * Holds entity related classes.
 */

/**
 * The while entity controller.
 */
class WhileController extends EntityAPIController {

  /**
   * Overridden to add in more default values.
   */
  public function create(array $values = array()) {
    $values += array(
      'created' => REQUEST_TIME,
      'langcode' => LANGUAGE_NONE,
    );
    return parent::create($values);
  }
}


/**
 * A while entity.
 */
class WhileEntity extends Entity {

  public $created;
  public $changed;
  public $id;
  public $type;
  public $label = '';
  public $status = 1;

  public function __construct($values = array()) {
    // Support creation with the type object also.
    if (isset($values['type']) && is_object($values['type'])) {
      $values['type'] = $values['type']->name;
    }
    parent::__construct($values, 'while');
  }

  /**
   * Overridden to care about created and changed times.
   */
  public function save() {
    // Do not automatically set a created values for already existing entities.
    if (empty($this->created) && (!empty($this->is_new) || !$this->id)) {
      $this->created = REQUEST_TIME;
    }

    $while_type = $this->type();
    if ($while_type->supportsRevisions()) {
      $this->is_new_revision = isset($this->options) && !empty($this->options['create_revision']);
    }
    $this->changed = REQUEST_TIME;

    parent::save();
  }

  /**
   * Gets the associated while type object.
   *
   * @return WhileType
   */
  public function type() {
    return while_type_load($this->type);
  }

  /**
   * Implements a custom default URI
   */
  public function defaultUri() {
    return array(
      'path' => while_get_string('base path') . '/' . $this->id,
    );
  }
}

/**
 * A while type entity.
 */
class WhileType extends Entity {

  public $label = '';
  public $weight = 0;

  /**
   * Helper method to check if the while type supports revisions.
   *
   * @return bool
   *   TRUE if revision support is enabled, FALSE if not.
   */
  public function supportsRevisions() {
    return !empty($this->data['supports_revisions']);
  }

}