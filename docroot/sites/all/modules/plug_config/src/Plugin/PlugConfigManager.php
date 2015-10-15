<?php

/**
 * @file
 * Contains \Drupal\plug_field\PlugFieldManager.
 */

namespace Drupal\plug_config\Plugin;

use Drupal\Core\Plugin\DefaultPluginManager;
use Drupal\plug\Util\Module;

class PlugConfigManager extends DefaultPluginManager {

  /**
   * Constructs PlugConfigManager.
   *
   * @param \Traversable $namespaces
   *   An object that implements \Traversable which contains the root paths
   *   keyed by the corresponding namespace to look for plugin implementations.
   * @param \DrupalCacheInterface $cache_backend
   *   Cache backend instance to use.
   */
  public function __construct(\Traversable $namespaces, $cache_backend = NULL) {
    parent::__construct('Plugin/Config', $namespaces, 'Drupal\plug_config\Plugin\Config\ConfigInterface', '\Drupal\plug_config\Annotation\Config');
    if ($cache_backend instanceof \DrupalCacheInterface) {
      $this->setCacheBackend($cache_backend, 'config_plugins');
    }
    $this->alterInfo('config_plugin');
  }

  /**
   * PlugConfigManagerBase factory method.
   *
   * @param string $bin
   *   The cache bin for the plugin manager.
   * @param bool $all
   *   Include values for disabled modules.
   *
   * @return PlugConfigManager
   *   The created manager.
   */
  public static function create($bin = 'cache', $all = FALSE) {
    if ($all) {
      return new static(Module::getNamespaces($all));
    }
    return new static(Module::getNamespaces($all), _cache_get_object($bin));
  }

  /**
   * Create an uncached plugin manager that only considers the passed modules.
   *
   * @param array $modules
   *   The list of modules to consider for plugins.
   *
   * @return PlugConfigManager
   *   The created manager.
   */
  public static function scopedFactory(array $modules) {
    $namespaces = new \ArrayObject();
    foreach ($modules as $module) {
      $namespaces['Drupal\\' . $module] = drupal_get_path('module', $module) . '/src';
    }
    return new static($namespaces);
  }

  /**
   * PlugConfigManagerBase pseudo service.
   *
   * @param string $bin
   *   The cache bin for the plugin manager.
   *
   * @return PlugConfigManager
   *   The created manager.
   */
  public static function get($bin = 'cache') {
    $manager = &drupal_static(get_called_class() . '::get');
    if (!isset($manager)) {
      $manager = static::create($bin);
    }
    return $manager;
  }

  /**
   * Finds plugin definitions.
   *
   * @return array
   *   List of definitions to store in cache.
   */
  protected function findDefinitions() {
    $defaults = array(
      'exportable' => TRUE,
      'controller class' => '\Drupal\plug_config\Plugin\Config\DefaultConfigController',
      'controller ui class' => '\Drupal\plug_config\Plugin\Config\DefaultConfigUIController',
      'entity keys' => array(
        'id' => 'id',
        'label' => 'label',
        'name' => 'name',
      ),
    );
    return array_map(function($definition) use ($defaults) {
      foreach ($definition as $property_name => &$item) {
        if (($new_property = preg_replace('/_/', ' ', $property_name)) && $new_property != $property_name) {
          $definition[$new_property] = $definition[$property_name];
          unset($definition[$property_name]);
        }
      }

      $definition += $defaults;
      return $definition + array(
        'base table' => $definition['id'],
        'module' => $definition['provider'],
        'entity class' => $definition['class'],
        'access callback' => array($definition['class'], 'accessCallback'),
        'admin ui' => array(
          'controller class' => $definition['controller ui class'],
          'path' => $definition['path'],
          'file' => 'plug_config.module',
          'file path' => drupal_get_path('module', 'plug_config')
        ),
      );
    }, $this->findAllDefinitions());
  }

  /**
   * Finds plugin definitions.
   *
   * This is a copy of parent::findDefinitions but it won't unset plugins for
   * disabled modules.
   *
   * @return array
   *   List of definitions to store in cache.
   */
  protected function findAllDefinitions() {
    $definitions = $this->discovery->getDefinitions();
    foreach ($definitions as $plugin_id => &$definition) {
      $this->processDefinition($definition, $plugin_id);
    }
    if ($this->alterHook) {
      drupal_alter($this->alterHook, $definitions);
    }
    // If this plugin was provided by a module that does not exist, remove the
    // plugin definition.
    foreach ($definitions as $plugin_id => $plugin_definition) {
      // If the plugin definition is an object, attempt to convert it to an
      // array, if that is not possible, skip further processing.
      if (is_object($plugin_definition) && !($plugin_definition = (array) $plugin_definition)) {
        continue;
      }
    }
    return $definitions;
  }

}
