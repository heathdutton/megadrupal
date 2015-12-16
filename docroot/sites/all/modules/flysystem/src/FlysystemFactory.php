<?php

/**
 * @file
 * Contains \Drupal\flysystem\FlysystemFactory.
 */

namespace Drupal\flysystem;

use Drupal\flysystem\DrupalFlysystemCache;
use Drupal\flysystem\Flysystem\Adapter\ReplicateAdapter;
use League\Flysystem\Cached\CachedAdapter;
use League\Flysystem\Filesystem;

/**
 * A factory for Flysystem filesystems.
 */
class FlysystemFactory {

  /**
   * Default settings.
   *
   * @var array
   */
  protected $defaults = array(
    'driver' => '',
    'config' => array(),
    'replicate' => FALSE,
    'cache' => FALSE,
  );

  /**
   * A cache of filesystems.
   *
   * @var \League\Flysystem\FilesystemInterface[]
   */
  protected $filesystems = array();

  /**
   * Created plugins.
   *
   * @var \Drupal\flysystem\Plugin\FlysystemPluginInterface[]
   */
  protected $plugins = array();

  /**
   * Settings for stream wrappers.
   *
   * @var array
   */
  protected $settings;

  /**
   * Constructs a FlysystemFactory object.
   *
   * @param array $settings
   *   The settings from settings.php.
   */
  public function __construct(array $settings) {
    // Apply defaults.
    foreach ($settings as $scheme => $configuration) {
      $this->settings[$scheme] = $configuration + $this->defaults;
    }
  }

  /**
   * Returns the filesystem for a given scheme.
   *
   * @param string $scheme
   *   The scheme.
   *
   * @return \League\Flysystem\FilesystemInterface
   *   The filesystem for the scheme.
   */
  public function getFilesystem($scheme) {
    if (!isset($this->filesystems[$scheme])) {
      $this->filesystems[$scheme] = new Filesystem($this->getAdapter($scheme));
    }

    return $this->filesystems[$scheme];
  }

  /**
   * Returns the plugin for a scheme.
   *
   * @param string $scheme
   *   The scheme.
   *
   * @return \Drupal\flysystem\Plugin\FlysystemPluginInterface
   *   The plugin.
   */
  public function getPlugin($scheme) {
    if (!isset($this->plugins[$scheme])) {
      $settings = $this->getSettings($scheme);

      $this->plugins[$scheme] = flysystem_get_plugin($settings['driver'], $settings['config']);
    }

    return $this->plugins[$scheme];
  }

  /**
   * Calls FlysystemPluginInterface::ensure() on each plugin.
   *
   * @param bool $force
   *   (optional) Wheter to force the insurance. Defaults to false.
   *
   * @return array
   *   Errors keyed by scheme.
   */
  public function ensure($force = FALSE) {
    $errors = array();

    foreach ($this->settings as $scheme => $configuration) {

      foreach ($this->getPlugin($scheme)->ensure($force) as $error) {

        $errors[$scheme][] = $error;
        watchdog('flysystem', $error['message'], $error['context'], $error['severity']);
      }
    }

    return $errors;
  }

  /**
   * Prevents the class from being serialized.
   */
  public function __sleep() {
    $message = sprintf('%s can not be serialized. This probably means you are serializing an object that has an indirect reference to the %s object. Adjust your code so that is not necessary.', __CLASS__, __CLASS__);
    throw new \LogicException($message);
  }

  /**
   * Returns the adapter for a scheme.
   *
   * @param string $scheme
   *   The scheme to find an adapter for.
   *
   * @return \League\Flysystem\AdapterInterface
   *   The correct adapter from settings.
   */
  protected function getAdapter($scheme) {
    $settings = $this->getSettings($scheme);

    $adapter = $this->getPlugin($scheme)->getAdapter();

    if ($settings['replicate']) {
      $replica = $this->getAdapter($settings['replicate']);
      $adapter = new ReplicateAdapter($adapter, $replica);
    }

    if ($settings['cache']) {
      $cache = new DrupalFlysystemCache('flysystem:' . $scheme);
      $adapter = new CachedAdapter($adapter, $cache);
    }

    return $adapter;
  }

  /**
   * Finds the settings for a given scheme.
   *
   * @param string $scheme
   *   The scheme.
   *
   * @return array
   *   The settings array from settings.php.
   */
  protected function getSettings($scheme) {
    return isset($this->settings[$scheme]) ? $this->settings[$scheme] : $this->defaults;
  }

}
