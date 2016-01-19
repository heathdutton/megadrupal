<?php

/**
 * @file
 * Contains Drupal\smartling\FieldProcessors\TitlePropertyFieldProcessor.
 */

namespace Drupal\smartling\FieldProcessors;

class DescriptionPropertyFieldProcessor extends TitlePropertyFieldProcessor {

  protected $propertyName = 'description';

  /**
   * {@inheritdoc}
   */
  public function getSmartlingContent() {
    return $this->entity->{$this->propertyName};
  }

  public function fetchDataFromXML(\DomXpath $xpath) {
    $quantity_value = $xpath->query('//string[@id="' . $this->fieldName . '-0' . '"][1]')
      ->item(0);

    if (!$quantity_value) {
      return NULL;
    }

    $field = $xpath->query('//string[@id="' . $this->fieldName . '-0"][1]')
      ->item(0);
    return $this->processXMLContent((string) $field->nodeValue);
  }

  public function setDrupalContentFromXML($fieldValue) {
    $this->entity->{$this->propertyName} = $fieldValue;
  }

}