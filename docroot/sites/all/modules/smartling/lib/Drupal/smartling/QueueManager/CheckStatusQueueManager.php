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

class CheckStatusQueueManager implements QueueManagerInterface {

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
  public function add($eids) {
    //$smartling_entities = smartling_entity_data_load_multiple($eids);
    $smartling_entities = $this->submissions_collection->loadByIDs($eids)
      ->getCollection();

    $smartling_queue = \DrupalQueue::get('smartling_check_status');
    $smartling_queue->createQueue();
    foreach ($smartling_entities as $queue_item) {
      $eid = $queue_item->getEID();
      $file_name = $queue_item->getFileName();
      if (!empty($file_name)) {
        $smartling_queue->createItem($eid);
        $this->log->info('Add item to "smartling_check_status" queue. Smartling entity data id - @eid, related entity id - @rid, entity type - @entity_type',
          array(
            '@eid' => $queue_item->getEID(),
            '@rid' => $queue_item->getRID(),
            '@entity_type' => $queue_item->getEntityType()
          ));
      }
      elseif ($queue_item->status != 0) {
        $this->log->warning('Original file name is empty. Smartling entity data id - @eid, related entity id - @rid, entity type - @entity_type',
          array(
            '@eid' => $queue_item->getEID(),
            '@rid' => $queue_item->getRID(),
            '@entity_type' => $queue_item->getEntityType()
          ));
      }
    }
  }

  /**
   * @inheritdoc
   */
  public function execute($eids) {
    if (!$this->smartling_utils->isConfigured()) {
      throw new \Drupal\smartling\SmartlingExceptions\SmartlingNotConfigured(t('The Smartling module is not configured. Please go to <a href="@link">Smartling settings</a> to finish configuration.', array('@link' => url('admin/config/regional/smartling'))));
    }

    if (!is_array($eids)) {
      $eids = array($eids);
    }

    $result = array();
    foreach ($eids as $eid) {
      $smartling_submission = $this->smartling_submission_wrapper->loadByID($eid)
        ->getEntity();

      if (empty($smartling_submission)) {
        continue;
      }
      $request_result = $this->api_wrapper->getStatus($smartling_submission);

      $result[$eid] = $request_result;
      if (($request_result['response_data']->approvedStringCount == $request_result['response_data']->completedStringCount)
        && ($smartling_submission->entity_type != 'smartling_interface_entity')
      ) {
        $this->queue_download->add($eid);
      }

      //smartling_entity_data_save($result['entity_data']);
      $this->smartling_submission_wrapper->setEntity($request_result['entity_data'])
        ->save();

      $this->drupal_wrapper->rulesInvokeEvent('smartling_after_submission_check_status_event', array($eid));

    }

    return $result;
  }
}
