<?php

/**
 * @file
 * Contains \Drupal\flysystem\Flysystem\Missing.
 */

namespace Drupal\flysystem\Flysystem;

use Drupal\flysystem\Flysystem\Adapter\MissingAdapter;
use Drupal\flysystem\Plugin\FlysystemPluginBase;

/**
 * Drupal plugin for the "MissingAdapter" Flysystem adapter.
 */
class Missing extends FlysystemPluginBase {

  /**
   * {@inheritdoc}
   */
  public function getAdapter() {
    return new MissingAdapter();
  }

  /**
   * {@inheritdoc}
   */
  public function getExternalUrl($uri) {
    return '';
  }

  /**
   * {@inheritdoc}
   */
  public function ensure($force = FALSE) {
    return array(array(
      'severity' => WATCHDOG_ERROR,
      'message' => 'The Flysystem driver is missing.',
      'context' => array(),
    ));
  }

}
