<?php
/**
 * @file
 * Contains the ElementType class
 */

class ElementType extends Entity {

	/**
   * Creates a new entity.
   *
   * @see entity_create()
   */
  public function __construct(array $values = array(), $entityType = NULL) {
  	parent::__construct($values, 'element_type');
  }

  /**
   * Returns the element identifier, i.e. the entities name or numeric id.
   *
   * @return
   *   The identifier of the element.
   *
   * @see entity_id()
   */
  public function identifier() {
    return isset($this->{$this->nameKey}) ? str_replace('_', '-', $this->{$this->nameKey}) : NULL;
  }

  /**
   * Override this in order to implement a custom default URI and specify
   * 'entity_class_uri' as 'uri callback' hook_entity_info().
   */
  protected function defaultUri() {
    return array('path' => 'admin/structure/element-types/manage/' . $this->identifier());
  }

}
