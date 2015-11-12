<?php

/**
 * Card on File extra fields controller.
 *
 * Defines extra fields rendered by EntityAPIController.
 */
class CommerceCardOnFileExtraFieldsController extends EntityDefaultExtraFieldsController {

  /**
   * Implements EntityExtraFieldsControllerInterface::fieldExtraFields().
   */
  public function fieldExtraFields() {
    $properties = array('card_type', 'card_name', 'card_number', 'card_exp');

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
