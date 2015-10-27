<?php

/**
 * @file
 * Contains Drupal\smartling\EntityProcessorFactory.
 */

namespace Drupal\smartling;

use Drupal\smartling\Log\SmartlingLog;
use Drupal\smartling\Processors\GenericEntityProcessor;

/**
 * Factory that creates entity processor instances and caches it statically.
 *
 * @package Drupal\smartling
 */
class EntityProcessorFactory {

  /**
   * @var array
   *   entity_type => ProcessorClass
   */
  protected $processorMapping;

  protected $drupal_api_wrapper;

  /**
   * @param array $processor_mapping
   * @param $drupal_api_wrapper
   */
  public function __construct($processor_mapping, $drupal_api_wrapper) {
    $this->drupal_api_wrapper = $drupal_api_wrapper;

    $this->drupal_api_wrapper->alter('smartling_entity_processor_mapping_info', $processor_mapping);
    $this->processorMapping = $processor_mapping;
  }

  public function getContainer() {
    return drupal_container();
  }

  /**
   * Creates GenericEntityProcessor instance based on entity type.
   *
   * Also caches instances statically to work with nested usages.
   *
   * @param $smartling_submission \stdClass|\SmartlingEntityData
   *
   * @return GenericEntityProcessor
   */
  public function getProcessor($smartling_submission) {
    //We can't use a static storage here, cause the entity can be cached before some languages are applied.
    //And so only the last one will be saved (this all is aplicable for "by fields" translation only.

    // @Todo avoid hardcoding 'generic' key.
    $processor_class = isset($this->processorMapping[$smartling_submission->entity_type]) ? $this->processorMapping[$smartling_submission->entity_type] : $this->processorMapping['generic'];

    $container = $this->getContainer();
    $smartling_submission = $container->get('smartling.wrappers.smartling_submission_wrapper')->setEntity($smartling_submission);
    $fieldProcessorFactory = $container->get('smartling.field_processor_factory');
    $smartling_settings = $container->get('smartling.settings');
    $logger = $container->get('smartling.log');
    $entity_api_wrapper = $container->get('smartling.wrappers.entity_api_wrapper');
    $smartling_utils = $container->get('smartling.wrappers.smartling_utils');
    $field_api_wrapper = $container->get('smartling.wrappers.field_api_wrapper');
    $i18n_wrapper = $container->get('smartling.wrappers.i18n_wrapper');

    $entity_type = $smartling_submission->getEntityType();
    switch ($entity_type) {
      case 'node':
        $entity_processor = new $processor_class($smartling_submission, $fieldProcessorFactory, $smartling_settings, $logger, $entity_api_wrapper, $smartling_utils, $field_api_wrapper);
        break;
      case 'taxonomy_term':
        $entity_processor = new $processor_class($smartling_submission, $fieldProcessorFactory, $smartling_settings, $logger, $entity_api_wrapper, $smartling_utils, $i18n_wrapper);
        break;

      case 'smartling_interface_entity':
        $entity_processor = new $processor_class($smartling_submission, $smartling_settings, $logger, $entity_api_wrapper, $smartling_utils);
        break;

      default :
        if ($entity_api_wrapper->entityTypeIsFieldable($entity_type)) {
          $entity_processor = new $processor_class($smartling_submission, $fieldProcessorFactory, $smartling_settings, $logger, $entity_api_wrapper, $smartling_utils);
        }
        else {
          $entity_processor = new $processor_class($smartling_submission, $smartling_settings, $logger, $entity_api_wrapper, $smartling_utils);
        }
        break;
    }

    return $entity_processor;
  }

}