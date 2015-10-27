<?php

namespace Drupal\smartling\EntityConversionUtils;

class NodeConversionUtil extends EntityConversionUtil {
  protected $settings;
  protected $field_api_wrapper;
  protected $drupal_api_wrapper;
  protected $smartling_utils;
  protected $entity_api_wrapper;

  public function __construct($settings, $entity_api_wrapper, $field_api_wrapper, $drupal_api_wrapper, $smartling_utils) {
    $this->settings = $settings;
    $this->entity_api_wrapper = $entity_api_wrapper;
    $this->field_api_wrapper = $field_api_wrapper;
    $this->drupal_api_wrapper = $drupal_api_wrapper;
    $this->smartling_utils = $smartling_utils;
  }

  /*
   * Legend:
   * def - default language
   * <no def> - node doesn't contain fields with default language
   * OK - no conversion required
   *
   * Conversion table
   *
   * -----------------------------------------------------------------------------
   * node lang. | fields lang |    to node method    |   to fields method
   *            |             | node lng | field lng | node lng | field lng
   * -----------------------------------------------------------------------------
   *    UND     |     UND     |   def    |   UND     |   def    |    def
   * -----------------------------------------------------------------------------
   *    def     | def/es/it/fr|   def    |   UND     |   OK     |    OK
   * -----------------------------------------------------------------------------
   *  def/es/it |     UND     |    OK    |   OK      |   def    |    def
   * -----------------------------------------------------------------------------
   *     def    |  <no def>   |        error         |       error
   * -----------------------------------------------------------------------------
   *
   * *def - default language of the site. Usually it's english.
   */
  public function convert(&$entity, $entity_type = 'node') {
    $allowed_fields = $this->settings->getFieldsSettingsByBundle($entity_type, $entity->type);
    $default_lang = $this->drupal_api_wrapper->getDefaultLanguage();
    if ((empty($allowed_fields)) || ($default_lang == LANGUAGE_NONE)) {
      return FALSE;
    }

    $field_langs = $this->field_api_wrapper->fieldLanguage('node', $entity);
    if (!in_array($default_lang, $field_langs) && !in_array(LANGUAGE_NONE, $field_langs)) {
      return FALSE;
    }

    if ($this->smartling_utils->isNodesMethod($entity->type)) {
      $this->updateToNodeTranslateMethod($entity, 'node', $default_lang, $allowed_fields);
    }
    else {
      $this->updateToFieldsTranslateMethod($entity, 'node', $default_lang, $allowed_fields);
    }

    //some magic transformations so that "title" module could catch up the title.
    $this->entity_api_wrapper->entitySave($entity_type, $entity);
    $id = $this->entity_api_wrapper->getID($entity_type, $entity);
    $entity = $this->entity_api_wrapper->entityLoadSingle($entity_type, $id);
  }

  function updateToNodeTranslateMethod($node, $entity_type, $default_language, $allowed_fields) {
    $field_langs = $this->field_api_wrapper->fieldLanguage($entity_type, $node);

    foreach($field_langs as $field => $lang) { // go through ALL field of this node
      if (($lang == $default_language) && (in_array($field, $allowed_fields))) { // if the field is in the wrong language
        $items = $this->field_api_wrapper->fieldGetItems($entity_type, $node, $field, $lang); // get all field values
        if (!empty($items)) {
          $node->{$field}[LANGUAGE_NONE] = $items; // put it under language neutral
          unset($node->{$field}[$lang]); // remove the old language
        }
      }
    }
    $node->language = $default_language;// set the node language to neutral
  }

}