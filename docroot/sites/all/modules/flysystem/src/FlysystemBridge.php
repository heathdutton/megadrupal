<?php

/**
 * @file
 * Contains \Drupal\flysystem\FlysystemBridge.
 */

namespace Drupal\flysystem;

use League\Flysystem\Util;
use Twistor\FlysystemStreamWrapper;

/**
 * An adapter for Flysystem to \DrupalStreamWrapperInterface.
 */
class FlysystemBridge extends FlysystemStreamWrapper implements \DrupalStreamWrapperInterface {

  /**
   * {@inheritdoc}
   */
  public function getUri() {
    return $this->uri;
  }

  /**
   * {@inheritdoc}
   */
  public function setUri($uri) {
    $this->uri = $uri;
  }

  /**
   * {@inheritdoc}
   */
  public function getExternalUrl() {
    return flysystem_factory()
      ->getPlugin($this->getProtocol())
      ->getExternalUrl($this->uri);
  }

  /**
   * {@inheritdoc}
   */
  public static function getMimeType($uri, $mapping = NULL) {
    return \DrupalLocalStreamWrapper::getMimeType($uri, $mapping);
  }

  /**
   * {@inheritdoc}
   */
  public function chmod($mode) {
    return $this->stream_metadata($this->uri, STREAM_META_ACCESS, $mode);
  }

  /**
   * {@inheritdoc}
   */
  public function realpath() {
    return FALSE;
  }

  /**
   * {@inheritdoc}
   */
  public function dirname($uri = NULL) {
    if (!isset($uri)) {
      $uri = $this->uri;
    }

    list($scheme, $target) = explode('://', $uri, 2);

    return $scheme . '://' . ltrim(Util::dirname($target), '\/');
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
  protected function getFilesystemForScheme($scheme) {
    if (!isset(static::$filesystems[$scheme])) {
      static::$filesystems[$scheme] = flysystem_factory()->getFilesystem($scheme);
      static::$config[$scheme] = static::$defaultConfiguration;
      static::$config[$scheme]['permissions']['dir']['public'] = 0777;
      static::registerPlugins($scheme, static::$filesystems[$scheme]);
    }

    return static::$filesystems[$scheme];
  }

  /**
   * {@inheritdoc}
   */
  protected function getFilesystem() {
    if (!isset($this->filesystem)) {
      $this->filesystem = $this->getFilesystemForScheme($this->getProtocol());
    }

    return $this->filesystem;
  }

}
