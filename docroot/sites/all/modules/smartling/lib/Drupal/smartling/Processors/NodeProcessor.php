<?php

/**
 * @file
 * Contains Drupal\smartling\Processors\NodeProcessor.
 */

namespace Drupal\smartling\Processors;

class NodeProcessor extends GenericEntityProcessor {

  protected $field_api_wrapper;

  public function __construct($smartling_submission, $field_processor_factory, $smartling_settings, $log, $entity_api_wrapper, $smartling_utils, $field_api_wrapper) {
    parent::__construct($smartling_submission, $field_processor_factory, $smartling_settings, $log, $entity_api_wrapper, $smartling_utils);

    $this->field_api_wrapper = $field_api_wrapper;
  }

  /**
   * {inheritdoc}
   */
  protected function addTranslatedFieldsToNode($node) {
    $field_values = array();
    foreach ($this->getTranslatableFields() as $field_name) {
      if (!empty($node->{$field_name}[LANGUAGE_NONE])) {
        $fieldProcessor = $this->fieldProcessorFactory->getProcessor($field_name, $node, $this->drupalEntityType,
          $this->smartling_submission->getEntity(), $this->targetFieldLanguage);

        $val = $fieldProcessor->cleanBeforeClone($node);
        if (!empty($val)) {
          $field_values[$field_name] = $val;
        }
      }
    }

    $this->entity_api_wrapper->nodeObjectPrepare($node);
    $this->entity_api_wrapper->entitySave('node', $node);

    foreach ($this->getTranslatableFields() as $field_name) {
      if (!empty($field_values[$field_name])) {
        $node->{$field_name} = $field_values[$field_name];
      }
    }

    foreach ($this->getTranslatableFields() as $field_name) {
      // Run all translatable fields through prepareBeforeDownload
      // to make sure that all related logic was triggered.
      if (!empty($this->contentEntity->{$field_name}[LANGUAGE_NONE])) {
        $fieldProcessor = $this->fieldProcessorFactory->getProcessor($field_name, $node, $this->drupalEntityType,
          $this->smartling_submission->getEntity(), $this->targetFieldLanguage);

        // @TODO get rid of hardcoded language.
        $fieldProcessor->prepareBeforeDownload($this->contentEntity->{$field_name}[LANGUAGE_NONE]);
      }
    }

    return $node;
  }

  protected function prepareDrupalEntity() {
    if (!$this->isOriginalEntityPrepared && $this->smartling_utils->isNodesMethod($this->smartling_submission->getBundle())) {
      $this->isOriginalEntityPrepared = TRUE;
      // Translate subnode instead of main one.
      $this->ifFieldMethod = FALSE;
      $tnid = $this->contentEntity->tnid ?: $this->contentEntity->nid;
      $translations = $this->entity_api_wrapper->translationNodeGetTranslations($tnid);
      if (isset($translations[$this->drupalTargetLocale])) {
        $this->smartling_submission->setRID($translations[$this->drupalTargetLocale]->nid);

        $node = $this->entity_api_wrapper->entityLoadSingle('node', $this->smartling_submission->getRID()); //node_load($this->smartling_submission->rid);
        $node->translation_source = $this->contentEntity;

        //$node = node_load($node->nid);
        $node = $this->entity_api_wrapper->entityLoadSingle('node', $node->nid);
        $node = $this->addTranslatedFieldsToNode($node);

        $this->contentEntity = $node;
        $this->contentEntityWrapper->set($this->contentEntity);
      }
      else {
        // If node not exist, need clone.
        $node = clone $this->contentEntity;
        unset($node->nid);
        unset($node->vid);
        $this->entity_api_wrapper->nodeObjectPrepare($node);
        $node->language = $this->drupalTargetLocale;
        $node->uid = $this->smartling_submission->getSubmitter();
        $node->tnid = $this->contentEntity->nid;

        // @todo Do we need this? clone should do all the stuff.
        $node_fields = $this->field_api_wrapper->fieldInfoInstances('node', $this->contentEntity->type);
        foreach ($node_fields as $field) {
          $node->{$field['field_name']} = $this->contentEntity->{$field['field_name']};
        }

        $node->translation_source = $this->contentEntity;

        $node = $this->addTranslatedFieldsToNode($node);
        $node = $this->entity_api_wrapper->entityLoadSingle('node', $node->nid);
        // Second saving is done for Field Collection field support
        // that need host entity id.
        //node_save($node);

        // Update reference to drupal content entity.
        $this->contentEntity = $node;
        $this->smartling_submission->setRID($node->nid);
      }
    }
  }

  public static function supportedType($bundle) {
    $transl_method = variable_get('language_content_type_' . $bundle, NULL);
    return in_array($transl_method, array(
      SMARTLING_NODES_METHOD_KEY,
      SMARTLING_FIELDS_METHOD_KEY
    ));
  }

  protected function getOriginalEntity($entity) {
    return smartling_get_original_node($entity);
  }
}