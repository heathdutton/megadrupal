<?php


class entityaspect_EntitySystem {

  protected $cache = array();

  function entityType($entity_type) {
    if (!is_string($entity_type)) {
      ddebug_backtrace();
      throw new Exception('$entity_type must be a string.');
    }
    if (!isset($this->cache[$entity_type])) {
      $this->cache[$entity_type] = $obj = new entityaspect_EntityType($entity_type);
    }
    return $this->cache[$entity_type];
  }

  function entityToId($entity_type, $entity) {
    return $this->entityType($entity_type)->entityToId($entity);
  }
}
