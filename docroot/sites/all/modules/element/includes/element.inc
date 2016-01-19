<?php
/**
 * @file
 * Contains the Element class
 */

class Element extends Entity {

	/**
   * Creates a new entity.
   *
   * @see entity_create()
   */
  public function __construct(array $values = array(), $entityType = NULL) {
  	parent::__construct($values, 'element');
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
    return array('path' => 'element/' . $this->identifier());
  }


  /**
   * Returns the uri of the entity just as entity_uri(). This is just a public
   * wrapper for $this->defaultUri()
   */
  public function uri() {
    return $this->defaultUri();
  }

}
