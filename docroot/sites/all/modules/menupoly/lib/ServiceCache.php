<?php

/**
 * Class menupoly_ServiceCache
 *
 * @property menupoly_ModuleInfo $info
 * @property menupoly_BlockInfo $blocks
 * @property menupoly_AccessChecker $accessChecker
 * @property menupoly_Main $main
 * @property menupoly_SettingsProcessor $settingsProcessor
 * @property array $trailPaths
 */
class menupoly_ServiceCache {

  /**
   * @var menupoly_ServiceFactory
   */
  protected $factory;

  /**
   * @var mixed[]
   */
  protected $cache = array();

  /**
   * @param menupoly_ServiceFactory $factory
   */
  function __construct($factory) {
    $this->factory = $factory;
  }

  /**
   * @param string $key
   *
   * @return object
   */
  function __get($key) {
    return $this->get($key);
  }

  /**
   * @param string $type
   *
   * @return menupoly_MenuTreeSource_Interface
   */
  function menuTreeSource($type) {
    $key = 'menuTreeSource:' . $type;
    if (!isset($this->cache[$key])) {
      $this->cache[$key] = $this->factory->call_1_menuTreeSource($this, $type);
    }
    return $this->cache[$key];
  }

  /**
   * @param string $key
   *
   * @return mixed
   * @throws Exception
   */
  function get($key) {
    if (!isset($this->cache[$key])) {
      $method = 'get_' . $key;
      if (method_exists($this->factory, $method)) {
        $service = $this->factory->$method($this);
      }
      else {
        throw new Exception("Method $method not provided by service factory.");
      }
      $this->cache[$key] = isset($service) ? $service : FALSE;
    }
    return $this->cache[$key];
  }

  /**
   * @param string $key
   */
  function reset($key) {
    unset($this->cache[$key]);
  }
}
