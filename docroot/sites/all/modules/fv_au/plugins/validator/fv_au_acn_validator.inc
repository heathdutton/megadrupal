<?php

/**
 * @file
 * Field validation Australian Company Number (ACN) validation.
 *
 * For validation information see
 * http://www.asic.gov.au/asic/asic.nsf/byheadline/australian+company+number+%28acn%29+check+digit
 */

$plugin = array(
  'label' => t('ACN'),
  'description' => t('Verifies that user-entered values are valid Australian Company Numbers (ACNs).'),
  'handler' => array(
    'class' => 'fv_au_acn_validator',
  ),
);

class fv_au_acn_validator extends fv_au_check_digit_validator {

  /**
   * Save arguments locally.
   */
  function __construct($entity_type = 'node', $entity = NULL, $field = '', $instance = NULL, $langcode = 'und', $items = array(), $delta = 0, $item = array(), $value = '', $rule = NULL, &$errors = array()) {
    $this->mask = '/^\d{3} \d{3} \d{3}$/';
    $this->example = t('111 111 111');
    $this->lengths = array(9);
    $this->weights = array(8, 7, 6, 5, 4, 3, 2, 1);
    $this->divisor = 10;
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
        if ($digits[8] != 10 - $this->get_check_digit_remainder($digits)) {
          $this->set_error();
        }
      }
    }
  }
}
