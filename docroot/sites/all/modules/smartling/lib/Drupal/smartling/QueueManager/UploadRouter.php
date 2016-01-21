<?php

/**
 * @file
 * Contains Drupal\smartling\Forms.
 */

namespace Drupal\smartling\QueueManager;

use Drupal\smartling\EntityConversionUtils\EntityConversionFactory;
use Drupal\smartling\Log\LoggerInterface;
use Drupal\smartling\Settings\SmartlingSettingsHandler;
use Drupal\smartling\Wrappers\SmartlingEntityDataWrapperCollection;
use Drupal\smartling\Wrappers\SmartlingUtils;

class UploadRouter {
  /**
   * @var SmartlingEntityDataWrapperCollection
   */
  protected $entity_wrapper_collection;
  /**
   * @var UploadQueueManager
   */
  protected $upload_manager;
  /**
   * @var LoggerInterface
   */
  protected $log;
  /**
   * @var SmartlingSettingsHandler
   */
  protected $settings;
  /**
   * @var EntityConversionFactory
   */
  protected $entity_conversion_factory;
  /**
   * @var SmartlingUtils
   */
  protected $smartling_utils;

  public function __construct($entity_wrapper_collection, $upload_manager, $log, $settings, $entity_conversion_factory, $smartling_utils) {
    $this->entity_wrapper_collection = $entity_wrapper_collection;
    $this->upload_manager = $upload_manager;
    $this->log = $log;
    $this->settings = $settings;
    $this->entity_conversion_factory = $entity_conversion_factory;
    $this->smartling_utils = $smartling_utils;
  }

  public function routeUploadRequest($entity_type, $entity, $languages, $async_mode = NULL) {
    if (entity_language($entity_type, $entity) !== language_default('language')) {
      $this->log->info('Entity is not in the site default language and cannot be submitted for translation. Type - @entity_type, Entity - @entity',
        array(
          '@entity_type' => $entity_type,
          '@entity' => print_r($entity, TRUE)
        ));

      return array(
        'status' => 0,
        'message' => 'Entity in non-default language'
      );
    }
    if ($this->settings->getConvertEntitiesBeforeTranslation()) {
      $this->entity_conversion_factory->getConverter($entity_type)
        ->convert($entity, $entity_type);
    }

    $async_mode = (is_null($async_mode)) ? $this->settings->getAsyncMode() : $async_mode;

    $languages = array_filter($languages);
    $success = $this->entity_wrapper_collection->createForLanguages($entity_type, $entity, $languages);
    if (!$success) {
      return array('status' => 0, 'message' => '');
    }

    if ($async_mode) {
      $this->upload_manager->add($this->entity_wrapper_collection->getIDs());

      $collection = $this->entity_wrapper_collection->getCollection();
      $smartling_wrapper = reset($collection);

      // Create content hash (Fake entity update).
      $this->smartling_utils->hookEntityUpdate($entity, $entity_type);

      $langs = implode(', ', $languages);
      $this->log->info('Smartling queue task was created for entity id - @id, locale - @locale, type - @entity_type',
        array(
          '@id' => $smartling_wrapper->getRID(),
          '@locale' => $langs,
          '@entity_type' => $entity_type
        ));

      $user_message = t('The @entity_type "@title" has been queued for translation in Smartling to "@langs".', array(
        '@entity_type' => $entity_type,
        '@title' => $smartling_wrapper->getTitle(),
        '@langs' => $langs,
      ));
    }
    else {
      $this->upload_manager->execute($this->entity_wrapper_collection->getIDs());

      $collection = $this->entity_wrapper_collection->getCollection();
      $smartling_wrapper = reset($collection);

      $user_message = t('The @entity_type "@title" has been sent to Smartling for translation to "@langs".', array(
        '@entity_type' => $entity_type,
        '@title' => $smartling_wrapper->getTitle(),
        '@langs' => implode(', ', $languages),
      ));
    }
    return array('status' => 1, 'message' => $user_message);
  }

}
