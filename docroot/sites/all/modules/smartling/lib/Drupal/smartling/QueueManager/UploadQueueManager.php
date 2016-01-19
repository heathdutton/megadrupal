<?php

/**
 * @file
 * Contains Drupal\smartling\Forms.
 */

namespace Drupal\smartling\QueueManager;

class UploadQueueManager implements QueueManagerInterface {

  protected $smartling_utils;
  protected $entity_processor_factory;
  protected $smartling_submission_wrapper;
  protected $drupal_wrapper;
  protected $settings;
  protected $file_transport;

  public function __construct($smartling_submission_wrapper, $entity_processor_factory, $settings, $smartling_utils, $drupal_wrapper, $file_transport) {
    $this->smartling_submission_wrapper = $smartling_submission_wrapper;
    $this->entity_processor_factory = $entity_processor_factory;
    $this->smartling_utils = $smartling_utils;
    $this->drupal_wrapper = $drupal_wrapper;
    $this->settings = $settings;
    $this->file_transport = $file_transport;
  }

  /**
   * @inheritdoc
   */
  public function add($eids) {
    if (empty($eids)) {
      return FALSE;
    }
    $smartling_queue = \DrupalQueue::get('smartling_upload');
    $smartling_queue->createQueue();
    return $smartling_queue->createItem($eids);
  }

  /**
   * @inheritdoc
   */
  public function execute($eids) {
    if (!$this->smartling_utils->isConfigured()) {
      throw new \Drupal\smartling\SmartlingExceptions\SmartlingNotConfigured(t('Smartling module is not configured. Please follow the page <a href="@link">"Smartling settings"</a> to setup Smartling configuration.', array('@link' => url('admin/config/regional/smartling'))));
    }

    if (!is_array($eids)) {
      $eids = array($eids);
    }
    $eids = array_unique($eids);

    $target_locales = array();
    $entity_data_array = array();

    foreach ($eids as $eid) {
      $this->smartling_submission_wrapper->loadByID($eid);
      if ($this->smartling_submission_wrapper->isEmpty()) {
        continue;
      }
      $file_name = $this->smartling_submission_wrapper->getFileName();
      $target_locales[$file_name][] = $this->smartling_submission_wrapper->getTargetLanguage();
      $entity_data_array[$file_name][] = clone $this->smartling_submission_wrapper;
    }


    foreach ($entity_data_array as $file_name => $entity_array) {
      $submission = reset($entity_array);
      $processor = $this->entity_processor_factory->getProcessor($submission->getEntity());
      $content = $processor->exportContent();

      $event = $this->file_transport->upload($content, $submission, $target_locales[$file_name]);

      foreach ($entity_array as $submission) {
        $submission->setStatusByEvent($event)->save();

        $this->drupal_wrapper->rulesInvokeEvent('smartling_after_submission_upload_event', array($submission->getEID()));
      }
    }
  }
}
