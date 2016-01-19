<?php

/**
 * @file
 * Field validation Australian Medicare card number validation.
 *
 * Validation information taken from NEHTA: "Healthcare Identifier Service - Use
 * of Healthcare Identifiers in Health Software Systems Conformance Requirements
 * v2.0" - Appendix C: Medicare card number check algorithm.
 * http://www.nehta.gov.au/implementation-resources/national-infrastructure/EP-1060-2011/NEHTA-1080-2012
 */

$plugin = array(
  'label' => t('Medicare card number'),
  'description' => t('Verifies that user-entered values are valid Australian Medicare card numbers.'),
  'handler' => array(
    'class' => 'fv_au_medicare_card_number_validator',
  ),
);

class fv_au_medicare_card_number_validator extends fv_au_check_digit_validator {

  /**
   * Save arguments locally.
   */
  function __construct($entity_type = 'node', $entity = NULL, $field = '', $instance = NULL, $langcode = 'und', $items = array(), $delta = 0, $item = array(), $value = '', $rule = NULL, &$errors = array()) {
    $this->mask = '/^\d{4} \d{5} \d$/';
    $this->example = t('1111 11111 1');
    $this->lengths = array(10);
    $this->weights = array(1, 3, 7, 9, 1, 3, 7, 9);
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
        // First number must be between 2 and 6.
        if (!in_array($digits[0], range(2, 6))) {
          $this->set_error();
        }
        else {
          $issue_number = array_pop($digits);
          $check_digit = array_pop($digits);

          // Must pass the check digit test.
          if ($check_digit != $this->get_check_digit_remainder($digits)) {
            $this->set_error();
          }
        }
      }
    }
  }
}
