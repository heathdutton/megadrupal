<?php

/**
 * @file
 * Contains FlysystemPluginBase.
 */

namespace Drupal\flysystem\Plugin;

use League\Flysystem\Util;

/**
 * Base class for plugins.
 */
abstract class FlysystemPluginBase implements FlysystemPluginInterface {

  /**
   * {@inheritdoc}
   */
  public static function create(array $configuration) {
    return new static($configuration);
  }

  /**
   * {@inheritdoc}
   */
  public function getExternalUrl($uri) {
    $path = str_replace('\\', '/', $this->getTarget($uri));

    return url('_flysystem/' . $this->getScheme($uri) . '/' . $path, array('absolute' => TRUE));
  }

  /**
   * Returns the target file path of a URI.
   *
   * @param string $uri
   *   The URI.
   *
   * @return string
   *   The file path of the URI.
   */
  protected function getTarget($uri) {
    return Util::normalizePath(substr($uri, strpos($uri, '://') + 3));
  }

  /**
   * Returns the scheme from the internal URI.
   *
   * @param string $uri
   *   The URI.
   *
   * @return string
   *   The scheme.
   */
  protected function getScheme($uri) {
    return substr($uri, 0, strpos($uri, '://'));
  }

}
