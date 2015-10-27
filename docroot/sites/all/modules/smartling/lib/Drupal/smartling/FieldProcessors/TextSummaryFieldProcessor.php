<?php

/**
 * @file
 * Contains Drupal\smartling\FieldProcessors\TextSummaryFieldProcessor.
 */

namespace Drupal\smartling\FieldProcessors;

class TextSummaryFieldProcessor extends BaseFieldProcessor {

  /**
   * {@inheritdoc}
   */
  public function getSmartlingContent() {
    $data = array();

    if (!empty($this->entity->{$this->fieldName}[$this->sourceLanguage])) {
      foreach ($this->entity->{$this->fieldName}[$this->sourceLanguage] as $delta => $value) {
        $data[$delta]['body'] = $value['value'];
        $data[$delta]['summary'] = $value['summary'];
        $data[$delta]['format'] = $value['format'];
      }
    }

    return $data;
  }


  /**
   * {@inheritdoc}
   */
  public function fetchDataFromXML(\DomXpath $xpath) {
    $data = array();
    $quantity_value = $xpath->query('//string[@id="' . $this->fieldName . '-body-0' . '"][1]')
      ->item(0);

    if (!$quantity_value) {
      return NULL;
    }

    $quantity = $quantity_value->getAttribute('quantity');

    for ($i = 0; $i < $quantity; $i++) {
      $bodyField = $xpath->query('//string[@id="' . $this->fieldName . '-body-' . $i . '"][1]')->item(0);
      $summaryField = $xpath->query('//string[@id="' . $this->fieldName . '-summary-' . $i . '"][1]')->item(0);
      $format = $bodyField->getAttribute('format');

      $data[$i]['value'] = $this->processXMLContent((string) $bodyField->nodeValue);
      $data[$i]['summary'] = $this->processXMLContent((string) $summaryField->nodeValue);
      $data[$i]['format'] = (string) $format;
      // @todo Copy fromat from the original field while xml file doesn't contain format
      // Otherwise you will get bug imediatelly with FullHtml fields
    }

    return $data;
  }

  public function putDataToXML($xml, $localize, $data) {
    // Field body-summary.
    $quantity = count($data);
    foreach ($data as $key => $value) {
      $value['body'] = $this->filterInvalidCharacters($value['body']);
      $value['summary'] = $this->filterInvalidCharacters($value['summary']);
      $string = $xml->createElement('string');
      $string_val = $xml->createTextNode($value['body']);
      $string_attr = $xml->createAttribute('id');
      $string_attr->value = $this->fieldName . '-body-' . $key;
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

      $string = $xml->createElement('string');
      $string_val = $xml->createTextNode($value['summary']);
      $string_attr = $xml->createAttribute('id');
      $string_attr->value = $this->fieldName . '-summary-' . $key;
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