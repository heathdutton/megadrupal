<?php

/**
 * @file
 * Contains the IdealAdvancedConfigHandler class.
 */

/**
 * Ideal advanced config handler.
 *
 * Defines the configuration object. The configuration handler loads the ideal
 * settings and verifies the installation.
 */
class IdealAdvancedConfigHandler {

  /**
   * The array with all supplied ideal settings.
   *
   * @var array
   */

  protected $settings;

  /**
   * Load the settings from database and set to property.
   *
   * @param int $config_id
   *   The ideal configuration ID.
   */
  public function configurationLoad($config_id) {
    if ($config = entity_load('ideal_advanced_config', array($config_id))) {
      foreach ($config[$config_id] as $field_name => $setting) {
        $this->settings[$field_name] = $setting;
      }
    }
    else {
      $this->settings = $this->defaultConfiguration();
      watchdog('iDEAL', 'The corresponding ideal configuration does not exist', array(), WATCHDOG_ERROR);
    }
  }

  /**
   * Add an new ideal configuration.
   *
   * See ideal_advanced_default_configuration for all defined settings.
   *
   * @param array $settings
   *   An array with ideal configuration settings.
   */
  public function configurationAdd(array $settings) {
    $configuration = entity_create('ideal_advanced_config', $settings);
    $configuration->save();
  }

  /**
   * Edit the settings from an ideal configuration.
   *
   * See ideal_advanced_default_configuration for all defined settings.
   *
   * @param array $settings
   *  An array with ideal configuration settings.
   * @param int $config_id
   *   The ideal configuration ID.
   */
  public function configurationEdit(array $settings, $config_id) {
    $entity = entity_load('ideal_advanced_config', array($config_id));
    $configuration = $entity[$config_id];

    $configuration->title = $settings['title'];
    $configuration->merchant_id = $settings['merchant_id'];
    $configuration->sub_id = $settings['sub_id'];
    $configuration->private_key_path = $settings['private_key_path'];
    $configuration->private_key_password = $settings['private_key_password'];
    $configuration->private_certificate_path = $settings['private_certificate_path'];
    $configuration->ideal_server_url = $settings['ideal_server_url'];
    $configuration->public_certificate_path = $settings['public_certificate_path'];
    $configuration->expiration_period = $settings['expiration_period'];
    $configuration->debug_mode = $settings['debug_mode'];
    $configuration->icon_path = $settings['icon_path'];

    $configuration->save();
  }

  /**
   * Return the complete array with ideal settings.
   *
   * @param int $config_id
   *   The config ID can be supplied. You can also load the configuration
   *   in to the class first with configurationLoad.
   *
   * @return array
   *   The ideal advanced settings.
   *   - id
   *   - title
   *   - merchant_id
   *   - sub_id
   *   - private_key_path
   *   - private_key_password
   *   - private_certificate_path
   *   - ideal_server_url
   *   - public_certificate_path
   *   - expiration_period
   *   - debug_mode.
   */
  public function getSettings($config_id = NULL) {
    if ($config_id) {
      $this->configurationLoad($config_id);
    }

    return $this->settings;
  }

  /**
   * Validate the overall configuration of the ideal connector module.
   *
   * @param int $config_id
   *   The config ID can be supplied. You can also load the configuration
   *   in to the class first with configurationLoad.
   *
   * @return bool
   *   True on correct config, false on error.
   */
  public function configValidate($config_id = NULL) {
    $incomplete = FALSE;
    $file_error = FALSE;
    $messages = array();

    if ($config_id) {
      $this->configurationLoad($config_id);
    }

    $library = libraries_detect('ideal_connector');

    if (isset($library['error'])) {
      $messages[] = t('Could not load the ideal connector library: @error. Please install the library in libraries/ideal_connector.', array('@error' => $library['error']));
    }
    if (!function_exists('curl_version')) {
      $messages[] = t('PHP cURL is not enabled or installed.');
    }
    if (!function_exists('openssl_open')) {
      $messages[]  = t('OpenSSL is not enabled or installed.');
    }
    if (empty($this->settings)) {
      $messages[] = t('No ideal settings are supplied to the idealAdvancedConfigHandler.');
    }
    if (empty($messages) && !$this->settingsComplete($this->settings)) {
      $messages[] = t('The ideal settings are incomplete.');
      $incomplete = TRUE;
    }
    if (!$incomplete && !$this->pathValidate($this->settings['private_certificate_path'])) {
      $messages[] = t('Could not load private certificate.');
    }
    if (!$incomplete && !$this->pathValidate($this->settings['public_certificate_path'])) {
      $messages[] = t('Could not load public certificate');
      $file_error = TRUE;
    }
    if (!$incomplete && !$this->pathValidate($this->settings['private_key_path'])) {
      $messages[] = t('Could not load private key.');
      $file_error = TRUE;
    }
    if (!$incomplete
        && !$file_error
        && ($key_validate = $this->keyValidate($this->settings['private_key_path'], $this->settings['private_key_password']))
        && isset($key_validate['error'])) {
      $messages[] = t('There is an issue with the private key: @error', array('@error' => implode(' ', $key_validate['error'])));
    }

    if (!empty($messages)) {
      $message = implode(' ', $messages);
      $config = isset($this->settings['title']) ? $this->settings['title'] : $this->settings['id'];
      watchdog('iDEAL', 'Error in: %config, message: %msg', array('%msg' => $message, '%config' => $config), WATCHDOG_ERROR);
      drupal_set_message($message, 'error', FALSE);

      return FALSE;
    }

    return TRUE;
  }

  /**
   * Check if all the settings are supplied.
   *
   * @param array $settings
   *   Array with ideal settings.
   *
   * @return bool True on complete.
   *   True on complete.
   */
  public function settingsComplete(array $settings) {
    $mandatory = $this->mandatorySettings();

    if (empty($settings)) {
      return FALSE;
    }

    foreach ($settings as $name => $value) {
      if (in_array($name, $mandatory) && empty($value)) {
        return FALSE;
      }
    }

    return TRUE;
  }

  /**
   * Validate if a value is numeric.
   *
   * @param int $value
   *   Int value to check.
   *
   * @return bool
   *   True on success.
   */
  public function numericValidate($value) {
    return is_numeric($value);
  }

  /**
   * Validate if a file exists in the private folder and is readable.
   *
   * @param string $value
   *   The path to a file.
   *
   * @return bool
   *   True on success.
   */
  public function pathValidate($value) {
    return is_readable(IDEAL_ADVANCED_BASE_PATH . $value);
  }

  /**
   * Validate if a path has a correct format.
   *
   * @param string $value
   *   The url to check.
   *
   * @return bool
   *   True on success.
   */
  public function urlValidate($value) {
    if (filter_var($value, FILTER_VALIDATE_URL, array('flags' => FILTER_FLAG_PATH_REQUIRED))) {
      return TRUE;
    }

    return FALSE;
  }

  /**
   * Validates the private key.
   *
   * Validates that the key is a 2048-bit RSA key. On success the bits and type
   * are returned. The error array is set on failure. This array contains error
   * messages in the form of strings.
   *
   * @param string $private_key_path
   *   Path to the private certificate key relative to the private folder.
   * @param string $private_key_password
   *   Password for corresponding private key.
   *
   * @return array
   *   An array with key specifications on success. Otherwise a 'error' key is
   *   set with messages as value.
   */
  public function keyValidate($private_key_path, $private_key_password) {
    $message = array();

    if ($key = openssl_pkey_get_private(file_get_contents(IDEAL_ADVANCED_BASE_PATH . $private_key_path), $private_key_password)) {
      $key_details = openssl_pkey_get_details($key);
      $message['bits'] = $key_details['bits'];
      $message['type'] = $key_details['type'];

      if ($key_details['bits'] != 2048) {
        $message['error'][] = 'The private key is not 2048 bits.';
      }
      if ($key_details['type'] != OPENSSL_KEYTYPE_RSA) {
        $message['error'][] = 'The private key type is not RSA.';
      }
    }
    else {
      $message['error'][] = 'The private key or password is not correct.';
    }

    return $message;
  }

  /**
   * Get all the required field names from the ideal advanced admin form.
   *
   * @return array
   *   All the required configuration field names.
   */
  protected function mandatorySettings() {
    module_load_include('inc', 'ideal_advanced', 'form/ideal_advanced_admin_form');
    $form_fields = ideal_advanced_admin_form_fields($this->defaultConfiguration());
    $required = array();

    foreach ($form_fields as $name => &$form_field) {
      // Move the fields from a fieldset to the root of the array.
      if (isset($form_field['#type']) && $form_field['#type'] == 'fieldset') {
        foreach ($form_field as $form_field_name => $fieldset_field) {
          if (is_array($fieldset_field)) {
            $form_fields[$form_field_name] = $fieldset_field;
          }
        }
        // Reset the array to check if there are any sub fieldsets.
        unset($form_fields[$name]);
        reset($form_fields);
      }

      if (isset($form_field['#required']) && $form_field['#required'] && !in_array($name, $required)) {
        $required[] = $name;
      }
    }

    return $required;
  }

  /**
   * Defines de default configuration settings for iDEAL.
   *
   * @return array
   *   Array with default settings.
   */
  public function defaultConfiguration() {
    $settings = array(
      'title' => '',
      'merchant_id' => '',
      'sub_id' => '',
      'private_key_path' => '',
      'private_key_password' => '',
      'private_certificate_path' => '',
      'ideal_server_url' => '',
      'public_certificate_path' => '',
      'expiration_period' => 15,
      'debug_mode' => 1,
      'icon_path' => '',
    );

    return $settings;
  }

}
