<?php
/**
 * @file
 * Field validation must be empty validator.
 *
 */
$plugin = array(
  'label' => t('Required'),
  'description' => t("Makes a field required."),
  'handler' => array(
    'class' => 'property_validation_required_validator',
  ),
);

class property_validation_required_validator extends property_validation_validator {

  /**
   * Validate field. 
   */
  public function validate() {
    if (empty($this->value)) {
      $this->set_error();
    }
  }

}
