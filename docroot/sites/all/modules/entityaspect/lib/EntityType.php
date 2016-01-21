<?php


/**
 * Wrapper class for static information about an entity type
 */
class entityaspect_EntityType {

  protected $info;
  protected $primary;

  function __construct($entity_type) {
    $this->info = entity_get_info($entity_type);
    $this->primary = $this->info['entity keys']['id'];
  }

  function entityToId($entity) {
    return @$entity->{$this->primary};
  }
}
