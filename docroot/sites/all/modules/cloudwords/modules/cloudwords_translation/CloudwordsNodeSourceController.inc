<?php

/**
 * @file
 * Contains CloudwordsNodeSourceController.
 */

class CloudwordsNodeSourceController implements CloudwordsSourceControllerInterface {

  protected $objectInfo = array();
  protected $stringInfo = array();
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
    return 'Content';
  }

  /**
   * Implements CloudwordsSourceControllerInterface::textGroup().
   */
  public function textGroup() {
    return 'node';
  }

  /**
   * Implements CloudwordsSourceControllerInterface::textGroupLabel().
   */
  public function textGroupLabel() {
    return 'Node';
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
    $translations = translation_node_get_translations($translatable->objectid);
    if (isset($translations[$translatable->language])) {
      return array(
        'path' => 'node/' . $translations[$translatable->language]->nid . '/edit',
      );
    }
    // The translation doesn't exist. Return the source title.
    $node = node_load($translatable->objectid);
    return array(
      'path' => 'node/' . $node->nid . '/edit',
    );
  }

  /**
   * Implements CloudwordsSourceControllerInterface::data().
   */
  public function data(CloudwordsTranslatable $translatable) {
    $node = node_load($translatable->objectid);
    $type = node_type_get_type($node);
    // Get all the fields that can be translated and arrange their values into
    // a specific structure.
    $structure = array('#label' => node_type_get_name($node));
    $structure['node_title']['#label'] = $type->title_label;
    $structure['node_title']['#text'] = $node->title;
    $structure += cloudwords_field_get_source_data('node', $node);
    return $structure;
  }

  /**
   * Implements CloudwordsSourceControllerInterface::save().
   */
  public function save(CloudwordsTranslatable $translatable) {
    if ($node = node_load($translatable->objectid)) {
      if (empty($node->tnid)) {
        // We have no translation source nid, this is a new set, so create it.
        $node->tnid = $node->nid;
        node_save($node);
      }
      $translations = translation_node_get_translations($node->tnid);
      if (isset($translations[$translatable->language])) {
        // We have already a translation for the source node for the target
        // language, so load it.
        $tnode = node_load($translations[$translatable->language]->nid);
      }
      else {
        // We don't have a translation for the source node yet, so create one.
        $tnode = clone $node;
        unset($tnode->nid, $tnode->vid, $tnode->uuid, $tnode->vuuid);
        $tnode->language = $translatable->language;
        $tnode->translation_source = $node;
      }
      // Time to put the translated data into the node.

      if(cloudwords_translation_supported_type($node->type) == TRANSLATION_ENABLED){
        $this->updateNodeTranslation($tnode, $translatable->getData(), $translatable->language, $translatable->getSetting('node_status'));
      }elseif(defined('ENTITY_TRANSLATION_ENABLED') && cloudwords_translation_supported_type($node->type) == ENTITY_TRANSLATION_ENABLED){
        $this->updateEntityTranslation($node, $translatable->getData(), $translatable->language, $translatable->getSetting('node_status'));
      }

    }
  }

  /**
   * Updates a node translation.
   *
   * @param stdClass $node
   *   The translated node object (the target).
   * @param array $data
   *   An array with the structured translated data.
   * @param string $language
   *   The target language.
   * @param string $nodeStatus
   *   The target node status.
   *
   * @see CloudwordsTranslatable::getData()
   */
  protected function updateNodeTranslation(stdClass $node, array $data, $language, $nodeStatus) {
    // Special case for the node title, but only if using node translations
    
    if (isset($data['node_title']['#translation']['#text'])) {
      $node->title = $data['node_title']['#translation']['#text'];
      unset($data['node_title']);
    }

    $node->status = $nodeStatus;

    cloudwords_field_populate_entity($node, $data, $language, $node->language);
    // Reset translation field, which determines outdated status.
    $node->translation['status'] = 0;

    cloudwords_translation_before_node_save($node, $data, $node->language, 'node_translation');

    node_save($node);

    cloudwords_translation_after_node_save($node, $data, $node->language, 'node_translation');
  }

  /**
   * Updates an entity translation.
   *
   * @param stdClass $node
   *   The translated node object (the target).
   * @param array $data
   *   An array with the structured translated data.
   * @param string $language
   *   The target language.
   * @param string $nodeStatus
   *   The target node status.
   *
   * @see CloudwordsTranslatable::getData()
   */
  protected function updateEntityTranslation(stdClass $node, array $data, $language, $nodeStatus) {
    // Special case for the node title, but only if using node translations
    
    if (isset($data['node_title']['#translation']['#text'])) {
      unset($data['node_title']);
    }

    $node->status = $nodeStatus;

    cloudwords_field_populate_entity($node, $data, $language, $node->language);
    // Reset translation field, which determines outdated status.
    $node->translation['status'] = 0;

    $handler = entity_translation_get_handler('node', $node);
    $translation = array(
      'translate' => 0,
      'status' => $node->status,
      'language' => $language,
      'source' => $node->language,
    );
    $handler->setTranslation($translation, $node);
    
    $node->translations->data[$language] = $translation;
    
    cloudwords_translation_before_node_save($node, $data, $node->language, 'entity_translation');
    
    node_save($node);
  }

}
