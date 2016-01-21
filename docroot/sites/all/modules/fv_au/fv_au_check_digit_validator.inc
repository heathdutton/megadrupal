<?php

/**
 * @file
 * Base validator class for check digit based validators.
 */

class fv_au_check_digit_validator extends field_validation_validator {

  /**
   * Options for the spaces handling setting.
   */
  const FV_AU_SPACES_NONE = 1;
  const FV_AU_SPACES_FORMATTED = 2;
  const FV_AU_SPACES_NONE_FORMATTED = 3;
  const FV_AU_SPACES_ALL = 4;

  /**
   * Positions of allowed spaces in properly formatted data.
   */
  protected $mask;

  /**
   * An example of properly formatted data.
   */
  protected $example;

  /**
   * An array of possible lengths for the data.
   */
  protected $lengths;

  /**
   * Weightings of each digit for checksum calculations.
   */
  protected $weights;

  /**
   * The number to divide the sum of products by in checksum calculations.
   */
  protected $divisor;

  /**
   * Provide additional settings.
   */
  function settings_form(&$form, &$form_state) {
    $default_settings = $this->get_default_settings($form, $form_state);

    $form['settings']['spaces_handling'] = array(
      '#title' => t('Allowed spaces'),
      '#type' => 'radios',
      '#options' => array(
        $this::FV_AU_SPACES_NONE => t('Do not allow spaces'),
        $this::FV_AU_SPACES_FORMATTED => t('Only allow spaces in proper formatting (Eg. @example)', array('@example' => $this->example)),
        $this::FV_AU_SPACES_NONE_FORMATTED => t('Allow no spaces or spaces in proper formatting (Eg. @example)', array('@example' => $this->example)),
        $this::FV_AU_SPACES_ALL => t('Allow spaces anywhere'),
      ),
      '#default_value' => isset($default_settings['spaces_handling']) ? $default_settings['spaces_handling'] : $this::FV_AU_SPACES_NONE,
    );

    parent::settings_form($form, $form_state);
  }

  /**
   * Common field validation.
   *
   * @param string $value
   *   The value being validated.
   *
   * @return bool
   *   Whether or not the value passed validation.
   */
  protected function common_validation(&$value) {
    // Handle spaces based on the validator settings.
    if (!$this->handle_spaces($value)) {
      $this->set_error();
      return FALSE;
    }
    // Must be digits.
    else if (!ctype_digit($value)) {
      $this->set_error();
      return FALSE;
    }
    // Must be the correct number of digits.
    else if (!in_array(strlen($value), $this->lengths)) {
      $this->set_error();
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Get the remainder for the check digit calculation of an ATO ID number.
   *
   * @param array $digits
   *   The digits of the number being checked.
   *
   * @return int
   *   The remainder of the checksum division.
   */
  protected function get_check_digit_remainder($digits) {
    $sum = 0;

    foreach ($digits as $i => $digit) {
      if (isset($this->weights[$i])) {
        $sum += $digit * $this->weights[$i];
      }
    }

    return $sum % $this->divisor;
  }

  /**
   * Handle the various options for allowing spaces in the value.
   *
   * @param array $value
   *   The value being validated.
   *
   * @return bool
   *   Whether or not the formatted value is in a valid format.
   */
  protected function handle_spaces(&$value) {
    $valid = TRUE;
    $settings = $this->rule->settings;
    $has_spaces = strpos($value, ' ') !== FALSE;

    if ($settings['spaces_handling'] == $this::FV_AU_SPACES_ALL) {
      // Strip all spaces out.
      $value = str_replace(' ', '', $value);
    }
    else if ($settings['spaces_handling'] == $this::FV_AU_SPACES_FORMATTED || ($settings['spaces_handling'] == $this::FV_AU_SPACES_NONE_FORMATTED && $has_spaces)) {
      // Check that spaces are only in the allowed places, then strip them out.
      if (preg_match($this->mask, $value)) {
        $value = str_replace(' ', '', $value);
      }
      else {
        $valid = FALSE;
      }
    }

    return $valid;
  }
}
