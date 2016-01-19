<?php

/**
 * @file
 * Contains Drupal\smartling\Forms.
 */

namespace Drupal\smartling\QueueManager;

class DownloadQueueManager implements QueueManagerInterface {

  protected $smartling_submission_wrapper;
  protected $field_api_wrapper;
  protected $entity_processor_factory;
  protected $smartling_utils;
  protected $drupal_wrapper;
  protected $file_transport;


  public function __construct($smartling_submission_wrapper, $field_api_wrapper, $entity_processor_factory, $smartling_utils, $drupal_wrapper, $file_transport) {
    $this->smartling_submission_wrapper = $smartling_submission_wrapper;
    $this->field_api_wrapper = $field_api_wrapper;
    $this->entity_processor_factory = $entity_processor_factory;
    $this->smartling_utils = $smartling_utils;
    $this->drupal_wrapper = $drupal_wrapper;
    $this->file_transport = $file_transport;
  }

  /**
   * @inheritdoc
   */
  public function add($eids) {
    if (empty($eids)) {
      return;
    }
    $smartling_queue = \DrupalQueue::get('smartling_download');
    $smartling_queue->createQueue();
    $smartling_queue->createItem($eids);
  }

  /**
   * @inheritdoc
   */
  public function execute($eids) {
    if ($this->drupal_wrapper->getDefaultLanguage() != $this->field_api_wrapper->fieldValidLanguage(NULL, FALSE)) {
      throw new \Drupal\smartling\SmartlingExceptions\WrongSiteSettingsException('The download failed. Please switch to the site\'s default language: ' . $this->drupal_wrapper->getDefaultLanguage());
    }

    if (!$this->smartling_utils->isConfigured()) {
      throw new \Drupal\smartling\SmartlingExceptions\SmartlingNotConfigured(t('Smartling module is not configured. Please follow the page <a href="@link">"Smartling settings"</a> to setup Smartling configuration.', array('@link' => url('admin/config/regional/smartling'))));
    }

    if (!is_array($eids)) {
      $eids = array($eids);
    }

    $global_status = TRUE;
    foreach ($eids as $eid) {
      $status = FALSE;

      $smartling_submission_wrapper = $this->smartling_submission_wrapper->loadByID($eid);
      $smartling_submission = $smartling_submission_wrapper->getEntity();
      if ($smartling_submission) {
        $downloaded_content = $this->file_transport->download($smartling_submission_wrapper);
        if ($downloaded_content) {
          $processor = $this->entity_processor_factory->getProcessor($smartling_submission);
          $status = $processor->updateEntity($downloaded_content);
        }
        $this->drupal_wrapper->rulesInvokeEvent('smartling_after_submission_download_event', array($eid));
      }
      $global_status = $global_status & $status;
    }

    return $global_status;
  }
}
