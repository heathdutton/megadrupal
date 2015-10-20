<?php

/**
 * @file
 * Contains Drupal\smartling\FieldProcessors\TitlePropertyFieldProcessor.
 */

namespace Drupal\smartling\FieldProcessors;

class TitlePropertyFieldProcessor extends BaseFieldProcessor {

  /**
   * {@inheritdoc}
   */
  public function getSmartlingContent() {
    return array(entity_label($this->entityType, $this->entity));
  }

  public function setDrupalContentFromXML($fieldValue) {
    $this->entity->title = $fieldValue;
  }

  public function fetchDataFromXML(\DomXpath $xpath) {
    $data = array();
    $quantity_value = $xpath->query('//string[@id="' . $this->fieldName . '-0' . '"][1]')
      ->item(0);

    if (!$quantity_value) {
      return NULL;
    }

    $quantity = $quantity_value->getAttribute('quantity');

    for ($i = 0; $i < $quantity; $i++) {
      $field = $xpath->query('//string[@id="' . $this->fieldName . '-' . $i . '"][1]')
        ->item(0);
      $data = $this->processXMLContent((string) $field->nodeValue);
      //$data[$this->sourceLanguage][$i]['value'] = $this->processXMLContent((string) $field->nodeValue);
    }

    return $data;
  }

  public function putDataToXML($xml, $localize, $data) {
    $quantity = count($data);
    foreach ($data as $key => $value) {
      $value = $this->filterInvalidCharacters($value);
      $string = $xml->createElement('string');
      $string_val = $xml->createTextNode($value);
      $string_attr = $xml->createAttribute('id');
      $string_attr->value = $this->fieldName . '-' . $key;
      $string->appendChild($string_attr);
      $string->appendChild($string_val);
      // Set quantity.
      $string_attr = $xml->createAttribute('quantity');
      $string_attr->value = $quantity;
      $string->appendChild($string_attr);
      $localize->appendChild($string);
    }
  }

}