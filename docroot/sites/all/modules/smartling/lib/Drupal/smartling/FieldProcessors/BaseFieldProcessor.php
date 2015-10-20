<?php

/**
 * @file
 * Contains Drupal\smartling\FieldProcessors\BaseFieldProcessor.
 */

namespace Drupal\smartling\FieldProcessors;

/**
 * Handle business logic for different drupal field types.
 *
 * @package Drupal\smartling\FieldProcessors
 */
abstract class BaseFieldProcessor {

  protected $entityType;
  protected $entity;
  protected $sourceLanguage;
  protected $targetLanguage;
  protected $fieldName;

  protected $smartling_submission;

  protected $drupal_wrapper;
  
  public function __construct($field_name, $entity, $entity_type, $smartling_submission, $source_language, $target_language, $drupal_wrapper) {
    $this->entity = $entity;
    $this->entityType = $entity_type;
    $this->sourceLanguage = $source_language;
    $this->targetLanguage = $target_language;
    $this->fieldName = $field_name;
    $this->smartling_submission = $smartling_submission;

    $this->drupal_wrapper = $drupal_wrapper;
  }

  public function setSmartlingEntity($smartling_submission) {
    $this->smartling_submission = $smartling_submission;

    return $this;
  }

  /**
   * Runs specific smartling alters.
   *
   * @param $value string
   * @param bool $reset
   *
   * @see \Drupal\smartling\Alters\*
   *
   * @return string
   */
  public function processXMLContent($value, $reset = FALSE) {
    $handlers = & $this->drupal_wrapper->drupalStatic(__FUNCTION__);
    if (!isset($actions) || $reset) {
      $handlers = $this->drupal_wrapper->moduleInvokeAll('smartling_data_processor_info');
      $this->drupal_wrapper->alter('smartling_data_processor_info', $handlers);
    }

    foreach ($handlers as $parser => $processors) {
      if (!class_exists($parser)) {
        continue;
      }

      $processors_objs = array();
      foreach ($processors as $proc) {
        if (class_exists($proc) && in_array('Drupal\\smartling\\Alters\\SmartlingContentProcessorInterface', class_implements($proc))) {
          $processors_objs[] = new $proc();
        }
      }

      if (!empty($processors_objs)) {
        $parser = new $parser($processors_objs);
        $value = $parser->parse($value, $this->sourceLanguage, $this->fieldName, $this->entity);
      }
    }

    return $value;
  }

  /**
   * Converts drupal field format to smartling data.
   *
   * @return array
   *   Drupal field structure under language key ready to be put into drupal content entity.
   */
  abstract public function getSmartlingContent();

  /**
   * Fetch translation data from xml based on structure for particular field.
   *
   * @param \DomXpath $xpath
   *
   * @return array
   *   Drupal field structure under language key ready to be put into smartling entity.
   */
  abstract public function fetchDataFromXML(\DomXpath $xpath);


  public function filterInvalidCharacters($text) {
    return preg_replace('/[^\x{0009}\x{000a}\x{000d}\x{0020}-\x{D7FF}\x{E000}-\x{FFFD}]+/u', "", $text);
  }

  public function putDataToXML($xml, $localize, $data) {
    $quantity = count($data);
    $string = $xml->createElement('string');
    foreach ($data as $key => $value) {
      foreach ($value as $item_key => $item) {
        $item = $this->filterInvalidCharacters($item);
        $string_val = $xml->createTextNode($item);
        $string_attr = $xml->createAttribute('id');
        $string_attr->value = $this->fieldName . '-' . $item_key . '-' . $key;
        $string->appendChild($string_attr);
        $string->appendChild($string_val);
        $string_attr = $xml->createAttribute('quantity');
        $string_attr->value = $quantity;
        $string->appendChild($string_attr);
        $localize->appendChild($string);
      }
    }
  }

  public function setDrupalContentFromXML($fieldValue) {
    $this->entity->{$this->fieldName}[$this->targetLanguage] = $fieldValue;
  }

  /**
   * Prepare default field data for translatable field before applying new translation.
   *
   * @param array $fieldData
   *   Field data under language key.
   *   Array(
   *     $delta => array(
   *       'value' => $value,
   *     )
   *   )
   *
   * @return array
   */
  public function prepareBeforeDownload(array $fieldData) {
    return $fieldData;
  }

  // @todo This is processor for single field. And it already knows its name and has reference to entity
  // Pls remove both parameters
  public function cleanBeforeClone($entity) {
    return NULL;
  }

  /**
   * @param \DOMDocument $xml
   * @param string $field_name
   * @param string|integer $delta
   * @param string|integer $quantity
   * @param string $value
   * @param array $extra_attributes
   * @return \DOMElement
   */
  protected function buildXMLString($xml, $field_name, $delta, $quantity, $value, $extra_attributes = array()) {
    $value = $this->filterInvalidCharacters($value);
    $string = $xml->createElement('string', $value);
    $string->setAttribute('id', $field_name);
    $string->setAttribute('delta', $delta);
    $string->setAttribute('quantity', $quantity);

    foreach ($extra_attributes as $attribute_name => $attribute_value) {
      $string->setAttribute($attribute_name, $attribute_value);
    }

    return $string;
  }

}
