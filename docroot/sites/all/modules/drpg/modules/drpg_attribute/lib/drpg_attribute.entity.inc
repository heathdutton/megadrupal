<?php

/**
 * @file
 * Defines the attribute entity.
 */

class DrpgAttribute extends Entity {

  // @codingStandardsIgnoreStart
  public $attribute_id = NULL;
  public $type = NULL;
  public $entity_id = 0;
  public $attribute_name = NULL;
  public $value = 0;
  public $created = '';
  public $updated = '';
  // @codingStandardsIgnoreEnd

  /**
   * Override parent constructor.
   */
  public function __construct(array $values = array()) {
    parent::__construct($values, 'drpg_attribute');

    // Check for new attribute.
    if (!$this->attribute_id) {
      $this->created = time();
    }
  }

  /**
   * Specifies the default entity label.
   */
  protected function defaultLabel() {
    return $this->type . ' ' . $this->entity_id . ' ' . $this->attribute_name;
  }

  /**
   * Specifies the entity URI.
   */
  protected function defaultUri() {
    return array('path' => 'admin/drpg/attributes/manage/' . $this->internalIdentifier());
  }

}
