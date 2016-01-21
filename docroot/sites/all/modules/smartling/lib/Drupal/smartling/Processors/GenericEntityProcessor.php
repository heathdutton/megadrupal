<?php

/**
 * @file
 * Contains Drupal\smartling\Processors\GenericEntityProcessor.
 *
 * @todo rename namespace to EntityProcessor or something else.
 */

namespace Drupal\smartling\Processors;

use DOMXPath;
use Drupal\smartling\FieldProcessorFactory;
use Drupal\smartling\Log\SmartlingLog;

/**
 * Contains smartling entity and provide main Smartling connector business logic.
 *
 * @package Drupal\smartling\Processors
 */
class GenericEntityProcessor implements EntityProcessorInterface {

  /**
   * Contains Smartling data entity.
   *
   * Instance of SmartlingEntityData if processor was created from newly created
   * smartling data entity that has such type in other cases - just stdClass.
   *
   * @var \stdClass|\SmartlingEntityData
   */
  public $smartling_submission;

  /**
   * Contains Smartling data referenced drupal content entity, e.g. Node, User.
   *
   * @var \stdClass|\Entity
   *
   * @see smartling_entity_load().
   */
  public $contentEntity;

  /**
   * Abstracted wrapper for drupal content entity.
   *
   * @var \EntityDrupalWrapper
   */
  public $contentEntityWrapper;

  /**
   * Contain drupal content entity type.
   *
   * @var string
   */
  protected $drupalEntityType;

  /**
   * Contains Smartling log object.
   *
   * @var \Drupal\smartling\Log\SmartlingLog
   */
  protected $log;

  /**
   * @var string
   * @todo Rename suffix Language to Locale to make consistent with other properties
   */
  protected $targetFieldLanguage;

  /**
   * @var string
   */
  protected $drupalOriginalLocale;

  /**
   * Contains locale in drupal format, e.g. 'en', 'und'.
   *
   * @var string
   */
  protected $drupalTargetLocale;

  /**
   * Contains if drupal content bundle has "Entity translation" mode.
   *
   * @var bool
   */
  protected $ifFieldMethod;

  /**
   * @var FieldProcessorFactory
   */
  protected $fieldProcessorFactory;

  /**
   * Helper internal flag to avoid duplicated execution.
   *
   * @var bool
   *
   * @see self::prepareOriginalEntity()
   */
  protected $isOriginalEntityPrepared;

  protected $entity_api_wrapper;
  protected $smartling_utils;
  protected $smartling_settings;


  /**
   * Translation handler factory.
   *
   * @param string $entity_type
   *   Entity type.
   * @param object $entity
   *   Entity.
   *
   * @return object
   *   Return translation handler object.
   */
  protected function getEntityTranslationHandler($entity_type, $entity) {
    $entity_info = $this->entity_api_wrapper->entityGetInfo($entity_type);
    $class = 'SmartlingEntityTranslationDefaultHandler';
    // @todo remove fourth parameter once 3rd-party translation handlers have
    // been fixed and no longer require the deprecated entity_id parameter.
    $handler = new $class($entity_type, $entity_info, $entity, NULL);
    return $handler;
  }

  /**
   * Create GenericEntityProcessor instance.
   *
   * @param object $smartling_submission
   *   Smartling data entity.
   * @param FieldProcessorFactory $field_processor_factory
   *   Factory instance for all field specific logic.
   * @param SmartlingApiWrapper $smartling_api
   *   Smartling API wrapper for Drupal.
   * @param SmartlingLog $log
   *   Smartling log object.
   *
   */
  public function __construct($smartling_submission, $field_processor_factory, $smartling_settings, $log, $entity_api_wrapper, $smartling_utils) {
    $this->smartling_submission = $smartling_submission;
    $this->drupalTargetLocale = $smartling_submission->getTargetLanguage();
    $this->drupalOriginalLocale = $smartling_submission->getOriginalLanguage();

    $this->log = $log;
    $this->fieldProcessorFactory = $field_processor_factory;
    $this->entity_api_wrapper = $entity_api_wrapper;
    $this->smartling_utils = $smartling_utils;
    $this->smartling_settings = $smartling_settings;


    $this->contentEntity = $this->entity_api_wrapper->entityLoadSingle($this->smartling_submission->getEntityType(), $this->smartling_submission->getRID());
    $this->drupalEntityType = $this->smartling_submission->getEntityType();
    $this->contentEntityWrapper = $this->entity_api_wrapper->entityMetadataWrapper($this->drupalEntityType, $this->contentEntity);
    $this->ifFieldMethod = $this->smartling_utils->isFieldsMethod($this->contentEntityWrapper->getBundle());

    $this->targetFieldLanguage = $this->ifFieldMethod ? $this->drupalTargetLocale : LANGUAGE_NONE;

  }

  /**
   * Wrapper for drupal entity saving.
   */
  private function saveDrupalEntity() {
    $this->contentEntityWrapper->set($this->contentEntity);
    $this->contentEntityWrapper->save();
  }

  /**
   * Contains preparation for entity before smartling processing.
   *
   * Should be overridden for node and term. E.g. before pushing translation we have to fetch data
   * from original node, so swap current node to original translation if necessary.
   */
  protected function prepareDrupalEntity() {
    if (!$this->isOriginalEntityPrepared) {
      $this->isOriginalEntityPrepared = TRUE;

      if ($this->ifFieldMethod) {
        foreach ($this->getTranslatableFields() as $field_name) {
          // Still use entity object itself because entity wrapper hardcodes
          // language and disallow to fetch values from translated fields.
          // However all entities work with entities in the same way.
          if (!empty($this->contentEntity->{$field_name}[$this->drupalOriginalLocale]) && empty($this->contentEntity->{$field_name}[$this->drupalTargetLocale])) {
            $fieldProcessor = $this->fieldProcessorFactory->getProcessor($field_name, $this->contentEntity, $this->drupalEntityType, $this->smartling_submission->getEntity(), $this->targetFieldLanguage);
            $this->contentEntity->{$field_name}[$this->drupalTargetLocale] = $fieldProcessor->prepareBeforeDownload($this->contentEntity->{$field_name}[$this->drupalOriginalLocale]);
          }
        }
      }
    }
  }

  /**
   * Implements entity_translation logic to update translation data in Drupal.
   */
  protected function updateDrupalTranslation() {
    $entity = $this->entity_api_wrapper->entityLoadSingle($this->drupalEntityType, $this->smartling_submission->getRID());
    $handler = $this->getEntityTranslationHandler($this->drupalEntityType, $entity);
    $translations = $handler->getTranslations();

    // Initialize translations if they are empty.
    if (empty($translations->original)) {
      $handler->initTranslations();
      $handler->saveTranslations();
      // Update the wrapped entity.
      $handler->setEntity($entity);
      $handler->smartlingEntityTranslationFieldAttach();
      $translations = $handler->getTranslations();
    }

    $entity_translation = array(
      'entity_type' => $this->drupalEntityType,
      'entity_id' => $this->smartling_submission->getRID(),
      'translate' => '0',
      'status' => !empty($entity->status) ? $entity->status : 1,
      'language' => $this->drupalTargetLocale,
      'uid' => $this->smartling_submission->getSubmitter(),
      'changed' => REQUEST_TIME,
    );

    if (isset($translations->data[$this->drupalTargetLocale])) {
      $handler->setTranslation($entity_translation);
    }
    else {
      // Add the new translation.
      $entity_translation += array(
        'source' => $translations->original,
        'created' => !empty($entity->created) ? $entity->created : REQUEST_TIME,
      );
      $handler->setTranslation($entity_translation);
    }
    $handler->saveTranslations();
    // Update the wrapped entity.
    $handler->setEntity($entity);
    $handler->smartlingEntityTranslationFieldAttach();

    return TRUE;
  }

  /**
   * Updates smartling data entity from given xml parsed object.
   *
   * @param $xml \DOMDocument
   */
  protected function importSmartlingXMLToSmartlingEntity(\DOMDocument $xml) {
    $this->prepareDrupalEntity();

    $xpath = new DomXpath($xml);

    foreach ($this->getTranslatableFields() as $field_name) {
      $fieldProcessor = $this->fieldProcessorFactory->getProcessor($field_name, $this->contentEntity,
        $this->smartling_submission->getEntityType(), $this->smartling_submission->getEntity(), $this->targetFieldLanguage);

      $fieldValue = $fieldProcessor->fetchDataFromXML($xpath);
      $fieldProcessor->setDrupalContentFromXML($fieldValue);
    }

    $this->saveDrupalEntity();
  }

  protected function getFilePath($file_name) {
    return drupal_realpath($this->smartling_utils->cleanFileName($this->smartling_settings->getDir($file_name), TRUE));
  }

  /**
   * Process given xml parsed object using translated_file.
   */
  public function updateEntity($content) {
    libxml_use_internal_errors(TRUE);
    if (FALSE === simplexml_load_string($content)) {
      return FALSE;
    }

    $xml = new \DOMDocument();
    $xml->loadXML($content);

    // Update smartling entity.
    $this->importSmartlingXMLToSmartlingEntity($xml);

    // Update translations information.
    return $this->updateDrupalTranslation();
  }

  public function exportContentToArray() {
    $data = array();
    foreach ($this->getTranslatableFields() as $field_name) {
      $original_entity = $this->getOriginalEntity($this->contentEntity);
      $fieldProcessor = $this->fieldProcessorFactory->getProcessor($field_name, $original_entity,
        $this->smartling_submission->getEntityType(), $this->smartling_submission->getEntity(), $this->targetFieldLanguage);

      if ($fieldProcessor) {
        $data[$field_name] = $fieldProcessor->getSmartlingContent();
      }
    }

    return $data;
  }

  protected function exportFieldsContentToXML($xml, $rid) {
    $localize = $xml->createElement('localize');
    $localize_attr = $xml->createAttribute('title');
    $localize_attr->value = $rid;
    $localize->appendChild($localize_attr);

    foreach ($this->getTranslatableFields() as $field_name) {
      $original_entity = $this->getOriginalEntity($this->contentEntity);
      /* @var $fieldProcessor \Drupal\smartling\FieldProcessors\BaseFieldProcessor */
      $fieldProcessor = $this->fieldProcessorFactory->getProcessor($field_name, $original_entity,
        $this->smartling_submission->getEntityType(), $this->smartling_submission->getEntity(), $this->targetFieldLanguage);
      if ($fieldProcessor) {
        $data = $fieldProcessor->getSmartlingContent();
        $fieldProcessor->putDataToXML($xml, $localize, $data);
      }
    }

    return $localize;
  }

  /**
   * Wrapper for Smartling settings storage.
   *
   * @return array()
   */
  protected function getTranslatableFields() {
    return $this->smartling_settings->getFieldsSettingsByBundle($this->smartling_submission->getEntityType(), $this->smartling_submission->getBundle());
  }


  /**
   * Build xml document and save in file.
   *
   * @param object $processor
   *   Drupal entity processor
   * @param int $rid
   *
   * @return DOMDocument
   *   Returns XML object.
   */
  public function exportContent() {
    $custom_regexp_placeholder = $this->smartling_settings->getCustomRegexpPlaceholder();
    $xml = new \DOMDocument('1.0', 'UTF-8');

    $xml->appendChild($xml->createComment(' smartling.translate_paths = data/localize/string, data/localize/field_collection/string, data/localize/field_collection/field_collection/string, data/localize/field_collection/field_collection/field_collection/string, data/localize/field_collection/field_collection/field_collection/field_collection/string '));
    // @todo remove hardcoded mappping of nested field collections.
    $xml->appendChild($xml->createComment(' smartling.string_format_paths = html : data/localize/string, html : data/localize/field_collection/string, html : data/localize/field_collection/field_collection/string, html : data/localize/field_collection/field_collection/field_collection/string '));
    $xml->appendChild($xml->createComment(' smartling.placeholder_format_custom = ' . $custom_regexp_placeholder . ' '));

    $data = $xml->createElement('data');

    $rid = $this->smartling_submission->getRID();
    $localize = $this->exportFieldsContentToXML($xml, $rid);

    $data->appendChild($localize);
    $xml->appendChild($data);

    // @todo Verify how many child has $data. If zero, then write to log and stop upload
    // This logic was lost in OOP branch
    //  {
    //    smartling_entity_delete_all_by_conditions(array(
    //      'rid' => $rid,
    //      'entity_type' => $entity_type,
    //    ));
    //    $log->setMessage('Entity has no strings to translate for entity_type - @entity_type, id - @rid.')
    //      ->setVariables(array('@entity_type' => $entity_type, '@rid' => $rid))
    //      ->setSeverity(WATCHDOG_WARNING)
    //      ->execute();
    //    $file_name = FALSE;
    //  }

    return $xml->saveXML();
  }

  protected function getOriginalEntity($entity) {
    return $entity;
  }
}