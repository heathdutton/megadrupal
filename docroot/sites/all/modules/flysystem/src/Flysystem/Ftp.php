<?php

/**
 * @file
 * Contains \Drupal\flysystem\Flysystem\Ftp.
 */

namespace Drupal\flysystem\Flysystem;

use Drupal\flysystem\Flysystem\Adapter\MissingAdapter;
use Drupal\flysystem\Plugin\FlysystemPluginBase;
use League\Flysystem\Adapter\Ftp as FtpAdapter;

/**
 * Drupal plugin for the "FTP" Flysystem adapter.
 */
class Ftp extends FlysystemPluginBase {

  /**
   * Plugin configuration.
   *
   * @var array
   */
  protected $configuration;

  /**
   * Constructs an Ftp object.
   *
   * @param array $configuration
   *   Plugin configuration array.
   */
  public function __construct(array $configuration) {
    $this->configuration = $configuration;

    if (empty($this->configuration['host'])) {
      $this->configuration['host'] = '127.0.0.1';
    }
  }

  /**
   * {@inheritdoc}
   */
  public function getAdapter() {
    try {
      $adapter = new FtpAdapter($this->configuration);
      $adapter->connect();
    }

    catch (\RuntimeException $e) {
      // A problem connecting to the server.
      $adapter = new MissingAdapter();
    }

    return $adapter;
  }

  /**
   * {@inheritdoc}
   */
  public function ensure($force = FALSE) {
    if ($this->getAdapter() instanceof FtpAdapter) {
      return array();
    }

    return array(array(
      'severity' => WATCHDOG_ERROR,
      'message' => 'There was an error connecting to the FTP server %host:%port.',
      'context' => array(
        '%host' => $this->configuration['host'],
        '%port' => isset($this->configuration['port']) ? $this->configuration['port'] : 21,
      ),
    ));
  }

}
