<?php

/**
 * @file
 * Contains CloudwordsNodeSourceController.
 */

class CloudwordsEntitySourceController implements CloudwordsSourceControllerInterface {

  protected $objectInfo = array();
  protected $stringInfo = array();
  protected $bundle = array();  
  protected $type;

  /**
   * Implements CloudwordsSourceControllerInterface::__construct().
   */
  public function __construct($type) {
    $this->type = $type;
  }

  /**
   * Implements CloudwordsSourceControllerInterface::typeLabel().
   */
  public function typeLabel() {
    return 'Entity';
  }

  /**
   * Implements CloudwordsSourceControllerInterface::textGroup().
   */
  public function textGroup() {
    return 'entity_bundle';
  }

  /**
   * Implements CloudwordsSourceControllerInterface::textGroupLabel().
   */
  public function textGroupLabel() {
    //TODO need to get the label of the entity bundle
    return 'Entity Bundle Label';
  }

  /**
   * Implements CloudwordsSourceControllerInterface::targetLabel().
   */
  public function targetLabel(CloudwordsTranslatable $translatable) {
    $translations = translation_node_get_translations($translatable->objectid);
    if (isset($translations[$translatable->language])) {
      return $translations[$translatable->language]->title;
    }
    return $translatable->label;
  }

  /**
   * Implements CloudwordsSourceControllerInterface::uri().
   */
  public function uri(CloudwordsTranslatable $translatable) {
    $entity = entity_load_single($translatable->type, $translatable->objectid);
    $uri = entity_uri($translatable->type, $entity);
    //TODO get language path if one exists
    if (isset($uri['path'])) {
      return array(
        'path' => $uri['path'],
      );
    }
  }

  /**
   * Implements CloudwordsSourceControllerInterface::data().
   */
  public function data(CloudwordsTranslatable $translatable) {

    $entity = entity_load_single($translatable->type, $translatable->objectid);
       
    $type = $translatable->type;
    // Get all the fields that can be translated and arrange their values into
    // a specific structure.
    $structure = array('#label' => $translatable->typeLabel());
    //$structure['entity_title']['#label'] = $type;
    //$structure['enitty_title']['#text'] = $entity->title;
    //$structure += cloudwords_field_get_source_data($translatable->textgroup, $entity, TRUE);
    
    
    
    $structure += cloudwords_field_get_source_data($translatable->type, $entity, TRUE);
    return $structure;
  }

  /**
   * Implements CloudwordsSourceControllerInterface::save().
   */
  public function save(CloudwordsTranslatable $translatable) {
    if ($entity = entity_load_single($translatable->type, $translatable->objectid)) {
      $this->updateEntityTranslation($entity, $translatable->getData(), $translatable->language, $translatable->getSetting('node_status'), $translatable->type);
    }
  }

  /**
   * Updates an entity translation.
   *
   * @param stdClass $entity
   *   The translated entity object (the target).
   * @param array $data
   *   An array with the structured translated data.
   * @param string $language
   *   The target language.
   * @param string $entityStatus
   *   The target entity status.
   *
   * @see CloudwordsTranslatable::getData()
   */
  protected function updateEntityTranslation($entity, array $data, $language, $entityStatus, $entity_type) {
    // Special case for the node title, but only if using node translations
    
    if (isset($data['node_title']['#translation']['#text'])) {
      unset($data['node_title']);
    }

    $entity->status = $entityStatus;

    $entity->language = (isset($entity->language)) ? $entity->language : $entity->translations->original;
    cloudwords_field_populate_entity($entity, $data, $language, $entity->language);
    // Reset translation field, which determines outdated status.
    //$entity->translation['status'] = 0;

    
    $handler = entity_translation_get_handler($entity_type, $entity);

    $translation = array(
      'translate' => 0,
      'status' => $entity->status,
      'language' => $language,
      'source' => $entity->language,
    );
    $handler->setTranslation($translation, $entity);
    
    $entity->translations->data[$language] = $translation;
    
    cloudwords_translation_before_entity_save($entity, $data, $entity->language, 'entity_translation');

    entity_save($entity_type, $entity);
  }

}
