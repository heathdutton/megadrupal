<?php

namespace Drupal\soauth;

use Drupal\soauth\Common\Storage;
use Drupal\soauth\Error\SoAuthError;


/**
 * Class Service
 * @author Raman Liubimau <raman@cmstuning.net>
 */
class Service {
  
  /**
   * Registered providers.
   * @var array
   */
  private $cache = array();
  
  /**
   * Service class instance.
   * @var Service
   */
  static private $instance = NULL;
  
  /**
   * Get Service instance.
   * @return Service
   */
  static public function getInstance() {
    if (is_null(self::$instance)) {
      return (self::$instance = new self());
    }
    return self::$instance;
  }
  
  /**
   * Construct Service
   */
  private function __construct() {
    $this->registerProviders();
  }
  
  /**
   * Register SoAuth providers.
   */
  private function registerProviders() {
    foreach (module_invoke_all('soauth_provider_load') as $provider) {
      $this->registerProvider($provider);
    }
  }
  
  /**
   * Register SoAuth provider.
   * @param AbstractBaseProvider $provider
   */
  public function registerProvider($provider) {
    $this->cache[ $provider->getName() ] = $provider;
  }
  
  /**
   * Get provider by name.
   * @param string $name
   * @return Provider
   * @throws SoAuthError
   */
  public function getProvider($name) {
    if (!isset($this->cache[$name])) {
      throw new SoAuthError('Unknown provider"'.$name.'"');
    }
    return $this->cache[$name];
  }
  
  /**
   * Get all providers.
   * @return array
   */
  public function getProviders() {
    return $this->cache;
  }
  
  /**
   * Get global settings
   * @return Storage
   */
  public function getSettings() {
    return new Storage('soauth');
  }
}
