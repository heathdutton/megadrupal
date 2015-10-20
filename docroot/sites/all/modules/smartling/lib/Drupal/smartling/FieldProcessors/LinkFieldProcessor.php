<?php

/**
 * @file
 * Contains Drupal\smartling\FieldProcessors\TextFieldProcessor.
 */

namespace Drupal\smartling\FieldProcessors;

class LinkFieldProcessor extends BaseFieldProcessor {

  /**
   * {@inheritdoc}
   */
  public function getSmartlingContent() {
    $data = array();

    if (!empty($this->entity->{$this->fieldName}[$this->sourceLanguage])) {
      foreach ($this->entity->{$this->fieldName}[$this->sourceLanguage] as $delta => $value) {
        $data[$delta]['title'] = isset($value['title']) ? $value['title'] : '';
        $data[$delta]['url'] = isset($value['url']) ? $value['url'] : '';
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
      $url = $field->getAttribute('url');
      $data[$i]['title'] = (string) $field->nodeValue;
      $data[$i]['url'] = $this->processXMLContent((string) $url);
    }

    return $data;
  }

  public function putDataToXML($xml, $localize, $data) {
    // Field text.
    $quantity = count($data);
    foreach ($data as $key => $value) {
      $value['title'] = $this->filterInvalidCharacters($value['title']);
      $string = $xml->createElement('string');
      $string_val = $xml->createTextNode($value['title']);
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
      if (isset($value['url'])) {
        $string_attr = $xml->createAttribute('url');
        $string_attr->value = $value['url'];
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
      if (isset($elem['title'])) {
        $this->entity->{$this->fieldName}[$this->targetLanguage] = $fieldValue;
      }
      else {
        foreach ($fieldValue as $delta => $val) {
          $this->entity->{$this->fieldName}[$this->targetLanguage][$delta] = array('title' => $val);
        }
      }
    }
    else {
      $this->entity->{$this->fieldName}[$this->targetLanguage] = array(array('title' => $fieldValue));
    }
  }
}