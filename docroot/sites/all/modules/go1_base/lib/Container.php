<?php
namespace Drupal\go1_base;

use Drupal\go1_base\Container\ServiceResolver;
use Drupal\go1_base\Container\ArgumentResolver;
use Drupal\go1_base\Helper\ConfigFetcher;
use Drupal\go1_base\Helper\Wrapper\Database as DB_Wrapper;
use Drupal\go1_base\Helper\Wrapper\Cache as Cache_Wrapper;
use Drupal\go1_base\Config\Resolver as Config_Resolver;
use Drupal\go1_base\Config\Config;

require_once go1_library('pimple') . '/lib/Pimple.php';

/**
 * Service Container/Locator.
 *
 * @see  https://github.com/mrsinguyen/go1_base/wiki/7.x-2.x-service-container
 */
class Container extends \Pimple {
  public function __construct() {
    parent::__construct(array(
      'container' => $this,
      // Dependencies for Container itself
      'wrapper.db' => function() { return new DB_Wrapper(); },
      'wrapper.cache' => function() { return new Cache_Wrapper(); },
      'config' => function() { return new Config(new Config_Resolver()); },
      'service.resolver' => function() { return new ServiceResolver(); },
      'argument.resolver' => function() { return new ArgumentResolver(); },
      'helper.config_fetcher' => function() { return new ConfigFetcher(); },
    ));
  }

  private function getServiceCallback($id, $def) {
    return function($c) use ($id, $def) {
      if (isset($c["{$id}:arguments"])) {
        $def['arguments'] = $c["{$id}:arguments"];
      }

      list($args, $calls) = $c['argument.resolver']->resolve($def);

      return $c['service.resolver']->convertDefinitionToService($def, $args, $calls);
    };
  }

  /**
   * @param  int $id
   * @param  array $def
   */
  public function initService($id, $def) {
    $callback = $this->getServiceCallback($id, $def);

    if (isset($def['reuse']) && !$def['reuse']) {
      $this[$id] = $this->factory($callback);
    }
    else {
      $this->offsetSet($id, $callback);
    }
  }

  /**
   * Get service by ID.
   *
   * @param  string $id Service ID.
   */
  public function offsetGet($id) {
    if (!$this->offsetExists($id)) {
      if ($def = $this['service.resolver']->getDefinition($id)) {
        $this->initService($id, $def);
      }
    }

    return parent::offsetGet($id);
  }

  /**
   * Find services by tag
   *
   * @param  string  $tag
   * @param  string  $return Type of returned services,
   * @return array
   */
  public function find($tag, $return = 'service_name') {
    $defs = go1_cache("go1c:tag:{$tag}, + 1 year", array($this['service.resolver'], 'fetchDefinitions'), array($tag));

    if ($return === 'service_name') {
      return $defs;
    }

    if ($return === 'service') {
      foreach ($defs as $k => $name) {
        unset($defs[$k]);
        $defs[$name] = $this[$name];
      }
    }

    return $defs;
  }

}
