<?php

/**
 * @file
 * User authentication classes and interfaces.
 */

/**
 * Main class to deal with collected plugins.
 */
class UserAuthAPI {

  protected $plugins = array();
  protected $plugin_info;
  protected $form_values = array();
  protected $validation_result = NULL;

  function __construct($form_values = array()) {
    $this->form_values = $form_values;
  }

  /**
   * Searches available classes for ones implementing the
   * UserAuthValidatePlugin interface and stores them as a ReflectionClass.
   */
  function getPluginInfo() {
    // Return plugins array if they have already been collected.
    if (isset($this->plugin_info)) {
      return $this->plugin_info;
    }

    // If it's cached, get that and use it.
    if ($cache = cache_get('user_auth_api:plugin_info')) {
      $this->plugin_info = $cache->data;
    }
    // Otherwise, rebuild data.
    else {
      $plugins = array();
      foreach (module_invoke_all('user_auth_api_info') as $key => $class) {
        // Invoke reflection class to do our plugin search.
        $reflection = new ReflectionClass($class);
        if ($reflection->implementsInterface('UserAuthAPIPlugin')) {
          $plugin = $reflection->newInstance();
          $plugins[$key] = array(
            'class' => $class,
            'weight' => $plugin->defaultWeight(),
            'external' => $plugin->isExternal(),
          );
        }
      }

      // Alter.
      drupal_alter('user_auth_api_info', $plugins);

      $this->plugin_info = $plugins;
      // Set this in the cache.
      cache_set('user_auth_api:plugin_info', $plugins);
    }

    return $this->plugin_info;
  }

  /**
   * Create plugin instances for all plugins and add to the data.
   */
  function instantiatePlugins() {
    $plugins = $this->getPluginInfo();
    foreach ($plugins as $plugin_name => &$plugin_info) {
      $plugin_info['plugin'] = new $plugin_info['class']();
    }

    return $plugins;
  }

  /**
   * Sort plugins by weight and invoke validate method on each.
   */
  function invokePlugins(array $exclude = array()) {
    $plugins = $this->instantiatePlugins();
    $this->sortPluginsByWeight($plugins);

    foreach ($plugins as $plugin_name => $plugin_info) {
      if (in_array($plugin_name, $exclude)) {
        continue;
      }
      $plugin = $plugin_info['plugin'];
      // Call the validate method on each available class.
      $validation_result = $plugin->validate($this->form_values);
      if (($validation_result === TRUE) || ($validation_result === FALSE)) {
        $this->validation_result = $validation_result;
        return new UserAuthAPIValidateResponse($validation_result, $plugin);
      }
    }

    return new UserAuthAPIValidateResponse($this->validation_result);
  }

  /**
   * Sort plugins array by weight method on each plugin.
   *
   * @return
   *  Sorted plugins array.
   */
  static function sortPluginsByWeight(array &$plugins) {
    uasort($plugins, 'drupal_sort_weight');
  }

} // UserAuthAPI.

/**
 * Response object to return containing validation info and plugin.
 */
class UserAuthAPIValidateResponse {

  protected $validation_result = NULL;
  protected $plugin;

  function __construct($validation_result, $plugin = NULL) {
    $this->validation_result = $validation_result;
    $this->plugin = $plugin;
  }

  function getValidationResult() {
    return $this->validation_result;
  }

  function getPlugin() {
    return $this->plugin;
  }

} // UserAuthValidateResponse.

/**
 * Interface for defining validation plugins.
 */
interface UserAuthAPIPlugin {

  /**
   * Define whether to plugin uses external validation or not.
   * If yes, user_external_login_register will be called to validate accounts.
   */
  function isExternal();

  /**
   * Main method to decide validation result.
   */
  function validate($form_values);

  /**
   * This method will be called if validation fails.
   */
  function validateError($form, $form_values);

  /**
   * This method will be called if validation succeeds.
   */
  function validateSuccess($account, $form_values);

  /**
   * The default weight of the plugin. This affects the order it is called
   * if there are multiple plugins.
   */
  function defaultWeight();

} // UserAuthValidatePlugin
