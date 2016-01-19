<?php

/**
 * @file
 * Contains \Drupal\integration\Backend\AbstractBackend.
 */

namespace Drupal\integration\Backend;

use Drupal\integration\Backend\Authentication\AuthenticationInterface;
use Drupal\integration\ConfigurablePluginInterface;
use Drupal\integration\Configuration\AbstractConfiguration;

/**
 * Class AbstractBackend.
 *
 * @package Drupal\integration\Backend
 */
abstract class AbstractBackend implements BackendInterface, ConfigurablePluginInterface {

  /**
   * Configuration component.
   *
   * @var Configuration\BackendConfiguration
   */
  private $configuration;

  /**
   * Formatter component.
   *
   * @var Formatter\FormatterInterface
   */
  private $formatter;

  /**
   * Authentication component.
   *
   * @var AuthenticationInterface
   */
  private $authentication;

  /**
   * Constructor.
   *
   * @param Configuration\BackendConfiguration $configuration
   *    Configuration object.
   * @param Formatter\FormatterInterface $formatter
   *    Formatter object.
   * @param AuthenticationInterface $authentication
   *    Authentication handler object.
   */
  public function __construct(Configuration\BackendConfiguration $configuration, Formatter\FormatterInterface $formatter, AuthenticationInterface $authentication) {
    $this->setConfiguration($configuration);
    $this->setFormatterHandler($formatter);
    $this->setAuthenticationHandler($authentication);
  }

  /**
   * {@inheritdoc}
   */
  public function getConfiguration() {
    return $this->configuration;
  }

  /**
   * {@inheritdoc}
   */
  public function setConfiguration(AbstractConfiguration $configuration) {
    $this->configuration = $configuration;
  }

  /**
   * {@inheritdoc}
   */
  public function getFormatterHandler() {
    return isset($this->formatter) ? $this->formatter : '';
  }

  /**
   * {@inheritdoc}
   */
  public function setFormatterHandler(Formatter\FormatterInterface $formatter) {
    $this->formatter = $formatter;
  }

  /**
   * {@inheritdoc}
   */
  public function getAuthenticationHandler() {
    return $this->authentication;
  }

  /**
   * {@inheritdoc}
   */
  public function setAuthenticationHandler(AuthenticationInterface $authentication) {
    $this->authentication = $authentication;
  }

}
