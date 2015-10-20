<?php

/**
 * @file
 * Contains Drupal\smartling\FieldProcessors\TitlePropertyFieldProcessor.
 */

namespace Drupal\smartling\FieldProcessors;

use Drupal\smartling\FieldProcessorFactory;

class FieldCollectionFieldProcessor extends BaseFieldProcessor {

  protected $fieldFactory;
  protected $entity_api_wrapper;
  protected $field_api_wrapper;

  public function __construct($field_name, $entity, $entity_type, $smartling_submission, $source_language, $target_language, $drupal_wrapper, $field_processor_factory, $entity_api_wrapper, $field_api_wrapper) {
    parent::__construct($field_name, $entity, $entity_type, $smartling_submission, $source_language, $target_language, $drupal_wrapper);

    $this->fieldFactory = $field_processor_factory;
    $this->entity_api_wrapper = $entity_api_wrapper;
    $this->field_api_wrapper = $field_api_wrapper;

    return $this;
  }

  protected function fieldCollectionItemLoad($id) {
    return field_collection_item_load($id);
  }

  protected function getProcessor($field_name, $entity, $smartling_submission, $target_anguage, $source_language = NULL) {
    return $this->fieldFactory->getProcessor($field_name, $entity, 'field_collection_item', $smartling_submission, $target_anguage, $source_language);
  }
  /**
   * {@inheritdoc}
   */
  public function getSmartlingContent() {
    $data = array();

    if (!empty($this->entity->{$this->fieldName}[$this->sourceLanguage])) {
      foreach ($this->entity->{$this->fieldName}[$this->sourceLanguage] as $delta => $value) {
        $fid = intval(@$value['value']);
        if (0 == $fid) {
          continue;
        }

        $entity = $this->fieldCollectionItemLoad($fid);

        foreach ($this->getTranslatableFields() as $field_name) {
          /* @var $fieldProcessor \Drupal\smartling\FieldProcessors\BaseFieldProcessor */
          //$fieldProcessor = $this->getProcessor($field_name, $entity, $this->smartling_submission, $this->targetLanguage, $this->smartling_submission->original_language);
          $fieldProcessor = $this->getProcessor($field_name, $entity, $this->smartling_submission, $this->targetLanguage, $this->sourceLanguage);

          if ($fieldProcessor) {
            $data[$fid][$field_name] = $fieldProcessor->getSmartlingContent();
          }
        }
      }
    }

    return $data;
  }

  protected function getTranslatableFields() {
    // @todo Inject via DIC.
    return smartling_settings_get_handler()->getFieldsSettingsByBundle('field_collection_item', $this->fieldName);//$this->entity->field_name);
  }

  protected function importSmartlingXMLToFieldCollectionEntity(\DomXpath $xpath) {
    $point = null;
    foreach ($this->getTranslatableFields() as $field_name) {
      $fieldProcessor = $this->getProcessor($field_name, $this->entity, $this->smartling_submission, $this->targetLanguage);
      $fieldValue = $fieldProcessor->fetchDataFromXML($xpath);
      $fieldProcessor->setDrupalContentFromXML($fieldValue);
    }

    unset($fieldProcessor);
    $this->entity_api_wrapper->entitySave('field_collection_item', $this->entity);
  }

  public function fetchDataFromXML(\DomXpath $xpath) {
    $result = array();
    $data = $xpath->query('/data/localize/field_collection[@id="' . $this->fieldName . '"]');

    if (!$data->length) {
      $data = $xpath->query('//field_collection[@id="' . $this->fieldName . '"]');

      if (!$data->length) {
        return FALSE;
      }
    }

    $delta = 0;
    foreach ($data as $field_collection_tag) {
      $eid = @$this->entity->{$this->fieldName}[$this->targetLanguage][$delta]['value'];
      if (empty($eid)) {
        continue;
      }
      $parentEntity = clone $this->entity;
      $this->entity = $this->fieldCollectionItemLoad($eid);
    //  $this->entity = entity_load_single('field_collection_item', $eid);
    //  $host_entity = $this->entity->hostEntity();
      $doc = new \DOMDocument();
      $nested_item = $field_collection_tag->cloneNode(TRUE);
      $doc->appendChild($doc->importNode($nested_item, TRUE));
      $nested_xpath = new \DomXpath($doc);
      $this->importSmartlingXMLToFieldCollectionEntity($nested_xpath);
      $delta++;
      $this->entity = $parentEntity;
    }

    return array(array('value' => $eid));
  }


  protected function clone_fc_items($entity_type, &$entity, $fc_field, $language = LANGUAGE_NONE){
    $entity_wrapper = $this->entity_api_wrapper->entityMetadataWrapper($entity_type, $entity);
    $old_fc_items = $entity_wrapper->{$fc_field}->value();
    if (!is_array($old_fc_items)) {
      $old_fc_items = array($old_fc_items);
    }
    if (empty($old_fc_items)) {
      foreach ($entity->{$fc_field}[$language] as $item_key => $item_value) {
        $fc_item = $this->fieldCollectionItemLoad($item_value['value']);
          $old_fc_items[$item_key] = $fc_item;
      }
    }

    $field_info_instances = $this->field_api_wrapper->fieldInfoInstances();
    $field_names = $this->drupal_wrapper->elementChildren($field_info_instances['field_collection_item'][$fc_field]);
    unset($entity->{$fc_field}[$language]);
    $result = array();
    foreach ($old_fc_items as $old_fc_item) {
      //$old_fc_item_wrapper = entity_metadata_wrapper('field_collection_item', $old_fc_item);
      $new_fc_item = $this->entity_api_wrapper->entityCreate('field_collection_item', array('field_name' => $fc_field));
      $new_fc_item->setHostEntity($entity_type, $entity, $language);
      foreach ($field_names as $field_name) {
        if (!empty($old_fc_item->{$field_name})){
          $new_fc_item->{$field_name} = $old_fc_item->{$field_name};
        }
      }
        $new_fc_item_wrapper = $this->entity_api_wrapper->entityMetadataWrapper('field_collection_item', $new_fc_item);
        $new_fc_item_wrapper->save();
        // field_attach_update($entity_type, $entity);
        $result[] = array('value' => $new_fc_item_wrapper->getIdentifier(), 'revision_id' => $new_fc_item_wrapper->getIdentifier());

      //Now check if any of the fields in the newly cloned fc item is a field collection and recursively call this function to properly clone it.
      foreach ($field_names as $field_name) {
        if (!empty($new_fc_item->{$field_name})){
          $field_info = $this->field_api_wrapper->fieldInfoField($field_name);
          if ($field_info['type'] == 'field_collection'){
            $this->clone_fc_items('field_collection_item', $new_fc_item, $field_name, $language);
          }
        }
      }
        $new_fc_item = $this->fieldCollectionItemLoad( $new_fc_item_wrapper->getIdentifier());

    }

    return $result;
  }

  public function prepareBeforeDownload(array $fieldData) {
    return $this->clone_fc_items($this->entityType, $this->entity, $this->fieldName);
  }

  public function setDrupalContentFromXML($fieldValue) {
    return;
    $content = $fieldValue;

//    //$values = $this->entity->{$this->fieldName}[LANGUAGE_NONE];
//    if (empty($values)) {
//      return;
//    }

    //$id = current($values);
    foreach($content as $id => $val) {
      $this->saveContentToEntity($id, $val);
      //$id = next($values);
    }
  }

  protected function saveContentToEntity($id, $value) {
    $entity = $this->fieldCollectionItemLoad($id);

    $fieldProcessorFactory = $this->fieldFactory;
    foreach ($value as $field_name => $fieldValue) {
      $smartling_submission = clone $this->smartling_submission;
      $fieldProcessor = $fieldProcessorFactory->getProcessor($field_name, $entity, 'field_collection_item', $smartling_submission, LANGUAGE_NONE);
      $fieldProcessor->setDrupalContentFromXML($fieldValue);
      unset($fieldProcessor);
    }

    //@todo: check if this unset makes any sense.
    unset($fieldProcessorFactory);
    $this->entity_api_wrapper->entitySave('field_collection_item', $entity);
  }

  public function cleanBeforeClone($entity) {
    $val = '';
    $field_name = $this->fieldName;
    if (isset($entity->{$field_name})) {
      $val = $entity->{$field_name};
      unset($entity->{$field_name});
    }
    return $val;
  }

  public function putDataToXML($xml, $localize, $data, $fieldName = NULL) {
    foreach ($data as $entity_id => $field_collection) {
      $collection = $xml->createElement('field_collection');
      $attr = $xml->createAttribute('id');
      $attr->value = !empty($fieldName) ? $fieldName : $this->fieldName;
      $collection->appendChild($attr);
      $attr = $xml->createAttribute('eid');
      $attr->value = $entity_id;
      $collection->appendChild($attr);

      foreach ($field_collection as $field_name => $value) {
        foreach ($value as $delta => $item) {
          // If field value is an array and value key is valid field name
          // then process it as nested field collection.
          if (is_array($item) && $this->isFieldOfType($field_name, 'field_collection')) {
            $this->putDataToXML($xml, $collection, array($delta => $item), $field_name);
          }
          else {
            $fieldProcessor = $this->fieldFactory->getProcessor($field_name, $this->entity, $this->entityType, $this->smartling_submission, $this->targetLanguage);
            $fieldProcessor->putDataToXml($xml, $collection, array($delta => $item));
          }

          $localize->appendChild($collection);
        }
      }
    }
  }

  protected function buildSingleStringTag($xml, $entity_id, $field_name, $delta, $quantity, $value) {
    $string = $xml->createElement('string');

    $string_attr = $xml->createAttribute('eid');
    $string_attr->value = $entity_id;
    $string->appendChild($string_attr);

    $string_attr = $xml->createAttribute('id');
    $string_attr->value = $field_name;
    $string->appendChild($string_attr);

    $string_attr = $xml->createAttribute('delta');
    $string_attr->value = $delta;
    $string->appendChild($string_attr);

    $string_val = $xml->createTextNode($value);
    $string->appendChild($string_val);

    $string_attr = $xml->createAttribute('quantity');
    $string_attr->value = $quantity;
    $string->appendChild($string_attr);

    return $string;
  }


  protected function isFieldOfType($field_name, $field_type) {
    $field = $this->field_api_wrapper->fieldInfoField($field_name);

    return isset($field) && $field['type'] == $field_type;
  }
}



