<?php

/**
 * @file
 * Contains GoogleDfpPluginInterface.
 */

namespace Drupal\google_dfp;

/**
 * Defines an interface for plugins defining ad objects.
 */
interface PluginInterface {

  /**
   * Sets the plugin configuration.
   *
   * @param array $configuration
   *   (optional) The plugin configuration. If an empty array is passed the
   *   plugin should set its own default configuration.
   */
  public function setConfiguration(array $configuration = array());

  /**
   * Sets the plugin configuration.
   *
   * @param string $key
   *   (Optional) The configuration key. Defaults to NULL. If not provided,
   *   should return all configuration values.
   *
   * @return array|mixed
   *   The configuration value or values.
   */
  public function getConfiguration($key = NULL);

  /**
   * Returns the plugin id.
   *
   * @return string
   *   The plugin id.
   */
  public function getId();

  /**
   * Returns the plugin title.
   *
   * @return string
   *   The plugin title.
   */
  public function getTitle();

  /**
   * Returns the settings form.
   *
   * @param array $form
   *   The entire form.
   * @param array $form_state
   *   The form state.
   *
   * @return array
   *   Form API array for the settings page.
   */
  public function settingsForm(&$form, &$form_state);

  /**
   * React to the settings form submission.
   *
   * @param array $form
   *   The entire form.
   * @param array $form_state
   *   The form state.
   */
  public function settingsFormSubmit(&$form, &$form_state);

  /**
   * Creates an instance of this plugin.
   *
   * @param string $id
   *   The plugin id.
   * @param array $configuration
   *   The plugin configuration.
   *
   * @return \Drupal\google_dfp\PluginInterface
   *   An object implementing Drupal\google_dfp\PluginInterface.
   */
  public static function createInstance($id, array $configuration);

}
