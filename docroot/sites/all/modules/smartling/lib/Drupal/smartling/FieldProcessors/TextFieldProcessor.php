<?php

/**
 * @file
 * Contains Drupal\smartling\FieldProcessors\TextFieldProcessor.
 */

namespace Drupal\smartling\FieldProcessors;

class TextFieldProcessor extends BaseFieldProcessor {

  /**
   * {@inheritdoc}
   */
  public function getSmartlingContent() {
    $data = array();

    if (!empty($this->entity->{$this->fieldName}[$this->sourceLanguage])) {
      foreach ($this->entity->{$this->fieldName}[$this->sourceLanguage] as $delta => $value) {
        $data[$delta]['value'] = isset($value['value']) ? $value['value'] : '';
        $data[$delta]['format'] = isset($value['format']) ? $value['format'] : '';
      }
    }

    return $data;
  }

  /**
   * {@inheritdoc}
   */
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
      $format = $field->getAttribute('format');
      $data[$i]['value'] = $this->processXMLContent((string) $field->nodeValue);
      $data[$i]['format'] = (string) $format;
      // @todo Copy format from the original field while xml file doesn't contain format
      // Otherwise you will get bug imediatelly with FullHtml fields
    }

    return $data;
  }

  public function putDataToXML($xml, $localize, $data) {
    // Field text.
    $quantity = count($data);
    foreach ($data as $key => $value) {
      $value['value'] = $this->filterInvalidCharacters($value['value']);
      $string = $xml->createElement('string');
      $string_val = $xml->createTextNode($value['value']);
      $string_attr = $xml->createAttribute('id');
      $string_attr->value = $this->fieldName . '-' . $key;
      $string->appendChild($string_attr);
      $string->appendChild($string_val);
      // Set quantity.
      $string_attr = $xml->createAttribute('quantity');
      $string_attr->value = $quantity;
      $string->appendChild($string_attr);
      $localize->appendChild($string);

      // Set format.
      if (isset($value['format'])) {
        $string_attr = $xml->createAttribute('format');
        $string_attr->value = $value['format'];
        $string->appendChild($string_attr);
        $localize->appendChild($string);
      }

      // Set quantity.
      $string_attr = $xml->createAttribute('quantity');
      $string_attr->value = $quantity;
      $string->appendChild($string_attr);
      $localize->appendChild($string);
    }
  }

  public function setDrupalContentFromXML($fieldValue) {
    if (is_array($fieldValue)) {
      $elem = current($fieldValue);
      if (isset($elem['value'])) {
        $this->entity->{$this->fieldName}[$this->targetLanguage] = $fieldValue;
      }
      else {
        foreach ($fieldValue as $delta => $val) {
          $this->entity->{$this->fieldName}[$this->targetLanguage][$delta] = array('value' => $val);
        }
      }
    }
    else {
      $this->entity->{$this->fieldName}[$this->targetLanguage] = array(array('value' => $fieldValue));
    }

  }
}