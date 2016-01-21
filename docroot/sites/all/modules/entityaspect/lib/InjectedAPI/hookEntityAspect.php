<?php


/**
 * API object, to be injected in hook_entityaspect().
 */
class entityaspect_InjectedAPI_hookEntityAspect {

  protected $data;
  protected $types = array();

  function __construct(&$data) {
    $this->data =& $data;
  }

  function type($entity_type) {
    if (!isset($this->types[$entity_type])) {
      $this->types[$entity_type] =
        new entityaspect_InjectedAPI_hookEntityAspect_Type($entity_type, $this->data)
      ;
    }
    return $this->types[$entity_type];
  }
}
