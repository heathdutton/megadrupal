<?php

/**
 * @file
 * Contains Drupal\smartling\FieldProcessors\ImageFieldProcessor.
 */

namespace Drupal\smartling\FieldProcessors;

class ImageFieldProcessor extends BaseFieldProcessor {

  /**
   * {@inheritdoc}
   */
  public function getSmartlingContent() {
    $data = array();

    if (!empty($this->entity->{$this->fieldName}[$this->sourceLanguage])) {
      foreach ($this->entity->{$this->fieldName}[$this->sourceLanguage] as $delta => $value) {
        $data[$delta]['alt-img'] = $value['alt'];
        $data[$delta]['title-img'] = $value['title'];
        $data[$delta]['fid-img'] = $value['fid'];
      }
    }

    return $data;
  }

  /**
   * {@inheritdoc}
   */
  public function fetchDataFromXML(\DomXpath $xpath) {
    $data = array();
    $quantity_value = $xpath->query('//string[@id="' . $this->fieldName . '-alt-img-0' . '"][1]')
      ->item(0);

    if (!$quantity_value) {
      return NULL;
    }

    $quantity = $quantity_value->getAttribute('quantity');

    for ($i = 0; $i < $quantity; $i++) {
      $altField = $xpath->query('//string[@id="' . $this->fieldName . '-alt-img-' . $i . '"][1]')->item(0);
      $titleField = $xpath->query('//string[@id="' . $this->fieldName . '-title-img-' . $i . '"][1]')->item(0);

      $fid = $altField->getAttribute('fid');
      $file_img = $this->drupal_wrapper->fileLoad($fid);

      if ($file_img) {
        $data[$i] = (array) $file_img;
        $data[$i]['alt'] = $this->processXMLContent((string) $altField->nodeValue);
        $data[$i]['title'] = $this->processXMLContent((string) $titleField->nodeValue);
      }
    }

    return $data;
  }

  public function putDataToXML($xml, $localize, $data) {
    $quantity = count($data);
    foreach ($data as $key => $value) {
      $extra_attributes = array('fid' => $value['fid-img']);

      $alt_field_name = $this->fieldName . '-alt-img-' . $key;
      $alt_string = $this->buildXMLString($xml, $alt_field_name, $key, $quantity, $value['alt-img'], $extra_attributes);

      $title_field_name = $this->fieldName . '-title-img-' . $key;
      $title_string = $this->buildXMLString($xml, $title_field_name, $key, $quantity, $value['title-img'], $extra_attributes);

      $localize->appendChild($alt_string);
      $localize->appendChild($title_string);
    }
  }

}
