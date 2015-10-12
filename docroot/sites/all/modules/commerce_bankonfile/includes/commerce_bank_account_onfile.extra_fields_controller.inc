<?php

/**
 * Bank Account on File extra fields controller.
 *
 * Defines extra fields rendered by EntityAPIController.
 */
class CommerceBankAccountFileExtraFieldsController extends EntityDefaultExtraFieldsController {

  /**
   * Implements EntityExtraFieldsControllerInterface::fieldExtraFields().
   */
  public function fieldExtraFields() {
  $properties = array('bank_account_type', 'bank_account_name', 'bank_account_number', 'aba_code'/*, 'card_exp'*/);

    $extra = array();
    foreach ($properties as $index => $name) {
      $property_info = $this->propertyInfo['properties'][$name];
      $extra_field = $this->generateExtraFieldInfo($name, $property_info);
      $extra_field['#weight'] = $index;
      $extra[$this->entityType][$this->entityType]['display'][$name] = $extra_field;
    }

    return $extra;
  }
}
