<?php

/**
 * @file
 * Contains Drupal\smartling\Forms.
 */

namespace Drupal\smartling\QueueManager;

use Drupal\smartling\ApiWrapperInterface;
use Drupal\smartling\Log\LoggerInterface;
use Drupal\smartling\Wrappers\SmartlingEntityDataWrapper;
use Drupal\smartling\Wrappers\SmartlingEntityDataWrapperCollection;
use Drupal\smartling\Wrappers\SmartlingUtils;

class BulkCheckStatusQueueManager {//implements QueueManagerInterface {

  /**
   * @var ApiWrapperInterface
   */
  protected $api_wrapper;
  /** @var  SmartlingEntityDataWrapper */
  protected $smartling_submission_wrapper;
  /** @var  DownloadQueueManager */
  protected $queue_download;
  /** @var  LoggerInterface */
  protected $log;
  /** @var  SmartlingUtils */
  protected $smartling_utils;
  /** @var  SmartlingEntityDataWrapperCollection */
  protected $submissions_collection;

  protected $drupal_wrapper;

  public function __construct($api_wrapper, $smartling_submission_wrapper, $submissions_collection, $queue_download, $log, $smartling_utils, $drupal_wrapper) {
    $this->api_wrapper = $api_wrapper;
    $this->smartling_submission_wrapper = $smartling_submission_wrapper;
    $this->submissions_collection = $submissions_collection;
    $this->queue_download = $queue_download;
    $this->log = $log;
    $this->smartling_utils = $smartling_utils;
    $this->drupal_wrapper = $drupal_wrapper;
  }

  /**
   * @inheritdoc
   */
  public function add($file_name) {
    if (!is_string($file_name)) {
      return;
    }

    $smartling_queue = \DrupalQueue::get('smartling_bulk_check_status');
    $smartling_queue->createQueue();
    $smartling_queue->createItem($file_name);
    $this->log->info('Add item to "smartling_bulk_check_status" queue. File name: @file_name',
      array(
        '@file_name' => $file_name,
      ));
  }

  /**
   * @inheritdoc
   */
  public function execute($file_name) {
    if (!$this->smartling_utils->isConfigured()) {
      throw new \Drupal\smartling\SmartlingExceptions\SmartlingNotConfigured(t('The Smartling module is not configured. Please go to <a href="@link">Smartling settings</a> to finish configuration.', array('@link' => url('admin/config/regional/smartling'))));
    }

    if (!is_string($file_name)) {
      return;
    }

    $submissions   = $this->submissions_collection->loadByCondition(array('file_name' => $file_name))->getCollection();
    $statuses      = $this->api_wrapper->getStatusAllLocales($file_name);
    $last_modified = $this->api_wrapper->getLastModified($file_name);

    if (empty($submissions) || empty($statuses) || empty($last_modified)) {
      return;
    }

    $result = array();
    foreach ($submissions as $submission) {

      if (empty($submission)) {
        continue;
      }

      $target_language = $submission->target_language;
      if (($statuses[$target_language]['authorizedStringCount'] == $statuses[$target_language]['completedStringCount'])
        && ($last_modified[$target_language] > $submission->last_modified)
        && ($submission->entity_type != 'smartling_interface_entity')
      ) {
        $this->queue_download->add($submission->eid);
      }


      $approved = intval($statuses[$target_language]['authorizedStringCount']);
      $completed = intval($statuses[$target_language]['completedStringCount']);
      $submission->progress = ($approved > 0) ? (int) (($completed / $approved) * 100) : 0;

      if ($last_modified[$target_language] > $submission->last_modified) {
        $submission->last_modified = $last_modified[$target_language];
      }

      //smartling_entity_data_save($result['entity_data']);
      $this->smartling_submission_wrapper->setEntity($submission)
        ->save();

      $this->drupal_wrapper->rulesInvokeEvent('smartling_after_submission_check_status_event', array($submission->eid));

    }

    return $result;
  }
}
