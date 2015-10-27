<?php

namespace Drupal\smartling\Utils;
use Drupal\smartling\ApiWrapperInterface;
use Drupal\smartling\Settings\SmartlingSettingsHandler;

/**
 * Class SmartlingCancelSubmission.
 */
class SmartlingCancelSubmission {


  /** @var  SmartlingSettingsHandler */
  protected $settings;
  /** @var  ApiWrapperInterface */
  protected $api_wrapper;
  protected $upload_queue;
  protected $log;
  protected $submission_wrapper;

  public function __construct($log, $settings, $api_wrapper, $upload_queue, $submission_wrapper) {
    $this->log = $log;
    $this->settings = $settings;
    $this->api_wrapper = $api_wrapper;
    $this->upload_queue = $upload_queue;
    $this->submission_wrapper = $submission_wrapper;
  }

  protected function getPreCanceledSubmissions($limit) {
    return db_select('smartling_entity_data', 'sm')
      ->distinct()
      ->fields('sm', array('rid', 'entity_type'))
      ->condition('status', SMARTLING_STATUS_PENDING_CANCEL, '=')
      ->range(0, $limit)
      ->execute()
      ->fetchAll();
  }

  protected function getSubmissionsByCondition($conditions) {
    return smartling_entity_load_all_by_conditions($conditions);
  }

  public function cancel($limit) {
    $subm_ids = $this->getPreCanceledSubmissions($limit);

    if (empty($subm_ids)) {
      return;
    }

    foreach ($subm_ids as $subm_id) {
      $smartling_submissions = $this->getSubmissionsByCondition(array(
        'rid' => $subm_id->rid,
        'entity_type' => $subm_id->entity_type,
      ));

      $file_name = '';
      $reupload_eids = array();
      $submissions_to_cancel = array();
      foreach($smartling_submissions as $subm) {
        if (!in_array((int) $subm->status, array(SMARTLING_STATUS_PENDING_CANCEL, SMARTLING_STATUS_CANCELED))) {
          $reupload_eids[] = $subm->eid;
        } elseif ((int) $subm->status === SMARTLING_STATUS_PENDING_CANCEL) {
          $submissions_to_cancel[] = $subm;
        }
        $file_name = (empty($file_name)) ? $subm->file_name : $file_name;
      }

      if (empty($file_name)) {
        continue;
      }

      if (count($reupload_eids) == 0) {
        //@todo: add some logs
        $this->api_wrapper->deleteFile($file_name);
      } else {
        try {
          $this->settings->setOverwriteApprovedLocales(TRUE);
          $this->upload_queue->execute($reupload_eids);
        }
        catch (\Drupal\smartling\SmartlingExceptions\SmartlingGenericException $e) {
          $this->log->error($e->getMessage() . '   ' . print_r($e, TRUE));
        }
      }


      foreach ($submissions_to_cancel as $subm) {
        $this->submission_wrapper->setEntity($subm)->setStatus(SMARTLING_STATUS_CANCELED)->save();
      }
    }
  }
}
