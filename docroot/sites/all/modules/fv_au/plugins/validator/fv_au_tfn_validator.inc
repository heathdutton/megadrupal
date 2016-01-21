<?php

/**
 * @file
 * Field validation Australian Tax File Number (TFN) validation.
 *
 * For validation information see
 * http://en.wikipedia.org/wiki/Tax_File_Number
 */

$plugin = array(
  'label' => t('TFN'),
  'description' => t('Verifies that user-entered values are valid Australian Tax File Numbers (TFNs).'),
  'handler' => array(
    'class' => 'fv_au_tfn_validator',
  ),
);

class fv_au_tfn_validator extends fv_au_check_digit_validator {

  /**
   * Save arguments locally.
   */
  function __construct($entity_type = 'node', $entity = NULL, $field = '', $instance = NULL, $langcode = 'und', $items = array(), $delta = 0, $item = array(), $value = '', $rule = NULL, &$errors = array()) {
    // @todo: I'm not sure what the mask is supposed to be for 8 digit TFNs.
    $this->mask = '/^\d{2,3} \d{3} \d{3}$/';
    $this->example = t('111 111 111');
    $this->lengths = array(8, 9);
    $this->weights = array(1, 4, 3, 7, 5, 8, 6, 9, 10);
    $this->divisor = 11;
    parent::__construct($entity_type, $entity, $field, $instance, $langcode, $items, $delta, $item, $value, $rule, $errors);
  }

  /**
   * Validate field.
   */
  public function validate() {
    if ($this->value !== '' && !is_null($this->value)) {
      $value = $this->value;
      if ($this->common_validation($value)) {
        $digits = str_split($value);
        // Must pass the check digit test.
        if ($this->get_check_digit_remainder($digits)) {
          $this->set_error();
        }
      }
    }
  }
}
