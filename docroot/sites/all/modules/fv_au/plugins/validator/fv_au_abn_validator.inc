<?php

/**
 * @file
 * Field validation Australian Business Number (ABN) validation.
 *
 * For validation information see
 * http://www.ato.gov.au/businesses/content.aspx?doc=/content/13187.htm
 */

$plugin = array(
  'label' => t('ABN'),
  'description' => t('Verifies that user-entered values are valid Australian Business Numbers (ABNs).'),
  'handler' => array(
    'class' => 'fv_au_abn_validator',
  ),
);

class fv_au_abn_validator extends fv_au_check_digit_validator {

  /**
   * Save arguments locally.
   */
  function __construct($entity_type = 'node', $entity = NULL, $field = '', $instance = NULL, $langcode = 'und', $items = array(), $delta = 0, $item = array(), $value = '', $rule = NULL, &$errors = array()) {
    $this->mask = '/^\d{2} \d{3} \d{3} \d{3}$/';
    $this->example = t('11 111 111 111');
    $this->lengths = array(11);
    $this->weights = array(10, 1, 3, 5, 7, 9, 11, 13, 15, 17, 19);
    $this->divisor = 89;
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
        $digits[0]--;
        if ($this->get_check_digit_remainder($digits)) {
          $this->set_error();
        }
      }
    }
  }
}
