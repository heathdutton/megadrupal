<?php
namespace Drupal\smartling\EntityConversionUtils;

class EntityConversionUtil implements EntityConversionInterface {
  protected $settings;
  protected $entity_api_wrapper;
  protected $field_api_wrapper;
  protected $drupal_api_wrapper;
  protected $smartling_utils;

  public function __construct($settings, $entity_api_wrapper, $field_api_wrapper, $drupal_api_wrapper, $smartling_utils) {
    $this->settings = $settings;
    $this->field_api_wrapper = $field_api_wrapper;
    $this->drupal_api_wrapper = $drupal_api_wrapper;
    $this->smartling_utils = $smartling_utils;
    $this->entity_api_wrapper = $entity_api_wrapper;
  }

  public function convert(&$entity, $entity_type) {
    $default_language = $this->drupal_api_wrapper->getDefaultLanguage();
    if ($default_language == LANGUAGE_NONE) {
      return FALSE;
    }
    $bundle = $this->entity_api_wrapper->getBundle($entity_type, $entity);
    $allowed_fields = $this->settings->getFieldsSettingsByBundle($entity_type, $bundle);

    if (empty($allowed_fields)) {
      return FALSE;
    }

    $this->updateToFieldsTranslateMethod($entity, $entity_type, $default_language, $allowed_fields);

    //some magic transformations so that "title" module could catch up the title.
    $this->entity_api_wrapper->entitySave($entity_type, $entity);
    $id = $this->entity_api_wrapper->getID($entity_type, $entity);
    $entity = $this->entity_api_wrapper->entityLoadSingle($entity_type, $id);
  }

  function updateToFieldsTranslateMethod($entity, $entity_type, $default_language, $allowed_fields) {
    $field_langs = $this->field_api_wrapper->fieldLanguage($entity_type, $entity);

    foreach($field_langs as $field => $lang) { // go through ALL field of this node
      if (($lang == LANGUAGE_NONE) && (in_array($field, $allowed_fields))) { // if the field is in the wrong language
        $items = $this->field_api_wrapper->fieldGetItems($entity_type, $entity, $field, $lang); // get all field values
        if (!empty($items)) {
          $entity->{$field}[$default_language] = $items; // put it under language neutral
          unset($entity->{$field}[$lang]); // remove the old language
        }
      }
    }
    $entity->language = $default_language;// set the node language to neutral
  }


}