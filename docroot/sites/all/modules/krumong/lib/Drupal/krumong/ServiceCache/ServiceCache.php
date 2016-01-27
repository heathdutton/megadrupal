<?php

namespace Drupal\krumong\ServiceCache;


/**
 * ServiceCache + ServiceFactory is the little brother of a DIC.
 * It is perfectly sufficient to manage the dependencies within a single module.
 */
class ServiceCache {

  protected $cache = array();
  protected $factory;

  function __construct($factory) {
    $this->factory = $factory;
  }

  function __get($key) {
    if (!isset($this->cache[$key])) {
      $this->cache[$key] = $this->factory->createService($key, $this);
    }
    return $this->cache[$key];
  }
}
