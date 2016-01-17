<?php

/**
 * @file
 * Provides Gengo translation plugin controller.
 */

/**
 * Gengo translation plugin controller.
 */
class TMGMTMyGengoTranslatorPluginController extends TMGMTDefaultTranslatorPluginController {

  /**
   * {@inheritdoc}
   */
  protected $escapeStart = '[[[';

  /**
   * {@inheritdoc}
   */
  protected $escapeEnd = ']]]';

  /**
   * If set it will be sent by job post action as a comment.
   *
   * @var string
   */
  protected $serviceComment;

  /**
   * Implements TMGMTDefaultTranslatorPluginController::getDefaultRemoteLanguagesMappings().
   */
  public function getDefaultRemoteLanguagesMappings() {
    return array(
      'zh-hans' => 'zh',
      'zh-hant' => 'zh-tw',
    );
  }

  /**
   * Sets comment to be sent to gengo service with job post request.
   *
   * @param string $comment
   */
  public function setServiceComment($comment) {
    $this->serviceComment = check_plain(trim($comment));
  }

  /**
   * Implements TMGMTTranslatorRejectDataItem::rejectForm()
   */
  public function rejectForm($form, &$form_state) {
    $form['message'] = array(
      '#markup' => '<div class="messages warning">' .
          t('By rejecting this item you will submit a new translate job to the Gengo translate service which will result in additional costs.') . '</div>',
    );
    $form['comment'] = array(
      '#type' => 'textarea',
      '#title' => t('Rejection comment'),
      '#description' => t('Provide a brief explanation that you actually rejected previous translation and state your reasons.'),
    );
    return $form;
  }

  /**
   * Implements TMGMTTranslatorPluginControllerInterface::isAvailable().
   */
  public function isAvailable(TMGMTTranslator $translator) {
    if ($translator->getSetting('api_public_key') && $translator->getSetting('api_private_key')) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Implements TMGMTTranslatorPluginControllerInterface::requestTranslation().
   *
   * Here we will actually query source and get translations.
   */
  public function requestTranslation(TMGMTJob $job) {

    try {

      // Check if we have comment from user input and if yes, set it to be sent.
      if (!empty($job->settings['comment'])) {
        $this->setServiceComment($job->settings['comment']);
      }

      $this->sendJob($job);

      $job->submitted(t('Job has been submitted.'));
    }
    catch (TMGMTException $e) {
      watchdog_exception('tmgmt_mygengo', $e);
      $job->rejected('Job has been rejected with following error: @error',
        array('@error' => $e->getMessage()), 'error');
    }
  }

  /**
   * Implements TMGMTDefaultTranslatorPluginController::getSupportedRemoteLanguages().
   */
  function getSupportedRemoteLanguages(TMGMTTranslator $translator) {
    if (!empty($this->supportedRemoteLanguages)) {
      return $this->supportedRemoteLanguages;
    }

    try {
      $connector = new TMGMTGengoConnector($translator);
      foreach($connector->getLanguages() as $gengo_language) {
        $this->supportedRemoteLanguages[$gengo_language->lc] = $gengo_language->lc;
      }
    }
    catch (TMGMTException $e) {
      watchdog_exception('tmgmt', $e);
      drupal_set_message($e->getMessage(), 'error');
    }

    return $this->supportedRemoteLanguages;
  }

  /**
   * Implements TMGMTTranslatorPluginControllerInterface::getSupportedTargetLanguages().
   */
  public function getSupportedTargetLanguages(TMGMTTranslator $translator, $source_language) {
    $results = array();

    $connector = new TMGMTGengoConnector($translator);
    $response = $connector->getLanguages($translator->mapToRemoteLanguage($source_language));
    foreach ($response as $target) {
      $results[$translator->mapToLocalLanguage($target->lc)] = $translator->mapToLocalLanguage($target->lc);
    }

    return $results;
  }

  /**
   * Will build and send a job to gengo service.
   *
   * @param TMGMTJob $job
   *   Job to be submitted for translation.
   * @param boolean $quote_only
   *   (Optional) Set to TRUE to only get a quote for the given job.
   *
   * @return mixed
   *   - Array of job objects returned from gengo.
   *   - Status object with order info.
   */
  public function sendJob(TMGMTJob $job, $quote_only = FALSE) {
    $data = tmgmt_flatten_data($job->getData());

    $translator = $job->getTranslator();
    $translations = array();
    $sources = array();
    $position = 0;
    $duplicates = array();

    foreach ($data as $key => $value) {
      if (isset($value['#translate']) && $value['#translate'] === FALSE) {
        continue;
      }

      if (!$quote_only) {
        // Detect duplicate source strings and add a mapping for them.
        if ($duplicate_key = array_search($value['#text'], $sources)) {
          $duplicates[$duplicate_key][] = $key;
          continue;
        }
      }

      // Keep track of source texts for easy lookup.
      $sources[$key] = $value['#text'];

      $translations[$job->tjid . '][' . $key] = array(
        'type' => 'text',
        // Labels that are longer than 56 characters are cut off in the Gengo
        // dashboard, limit them to that size.
        'slug' => tmgmt_data_item_label($value, 56),
        'body_src' => $this->escapeText($value),
        'lc_src' => $translator->mapToRemoteLanguage($job->source_language),
        'lc_tgt' => $translator->mapToRemoteLanguage($job->target_language),
        'tier' => $job->settings['quality'],
        'callback_url' => url('tmgmt_mygengo_callback', array('absolute' => TRUE)),
        'custom_data' => $job->tjid . '][' . $key,
        'position' => $position++,
        'auto_approve' => (int) $translator->getSetting('mygengo_auto_approve'),
        'use_preferred' => (int) $translator->getSetting('use_preferred'),
      );

      if (!empty($this->serviceComment)) {
        $translations[$job->tjid . '][' . $key]['comment'] = $this->serviceComment;
      }
    }

    $connector = new TMGMTGengoConnector($job->getTranslator());
    if ($quote_only) {
      return $connector->getQuote($translations);
    }
    else {
      $response = $connector->submitJob($translations);
      // If we already receive jobs, process them.
      if (!empty($response->jobs)) {
        $this->processGengoJobsUponTranslationRequest($job, $response->jobs, $duplicates);
      }
      elseif (isset($response->order_id)) {
        $this->initGengoMapping($job, $response->order_id, $translations, $duplicates);
      }
      return $response;
    }
  }

  /**
   * Receives and stores a translation returned by Gengo.
   *
   * @param TMGMTJob $job
   *   Job for which to receive translations.
   * @param string $key
   *   Data keys for data item which will be updated with translation.
   * @param array $data
   *   Translated data received from gengo.
   * @param TMGMTRemote $remote
   *   (optional) A remote mapping that should be considered for duplicates.
   */
  public function saveTranslation(TMGMTJob $job, $key, $data) {
    if ($data->status == 'approved' || $data->status == 'reviewable') {
      // If the status is approved or reviewable, we expect a body_tgt property,
      // abort and log if it doesn't exist.
      if (!isset($data->body_tgt)) {
        $job->addMessage('Callback called for @key and status @status without translation.', array('@key' => $data->custom_data, '@status' => $data->status));
        return;
      }
      $text = $this->unescapeText($data->body_tgt);
      $job->addTranslatedData(array('#text' => $text), $key);

      // Look for duplicated strings that were saved with a mapping to this key.
      // @todo: Refactor this method to accept the remote instead of $key?
      list($tjiid, $data_item_key) = explode('][', $key, 2);
      $remotes = entity_get_controller('tmgmt_remote')->loadByLocalData($job->tjid, $tjiid, $data_item_key);
      $remote = reset($remotes);
      if ($remote && !empty($remote->remote_data['duplicates'])) {
        // If we found any mappings, also add the translation for those.
        foreach ($remote->remote_data['duplicates'] as $duplicate_key) {
          $job->addTranslatedData(array('#text' => $text), $duplicate_key);
        }
      }
    }
  }

  /**
   * Will process remote jobs upon translation request.
   *
   * This deals with following:
   *   - Creates mappings of local data items to remote gengo jobs.
   *   - Saves translation in case it has been already received.
   *   - Deals with duplicate translations.
   *
   * @param TMGMTJob $job
   *   Local job that is going to be processed.
   * @param array $response_jobs
   *   List of gengo jobs received.
   * @param array $duplicates
   *   Array of duplicate mappings.
   */
  protected function processGengoJobsUponTranslationRequest(TMGMTJob $job, $response_jobs, array $duplicates) {
    foreach ($response_jobs as $key => $response_job) {

      // Duplicate content has been submitted.
      if (isset($response_job->duplicate)) {
        // @todo Currently handled manually, so this should never occur.
        continue;
      }

      // For machine translations the job is not wrapped in another object
      // however for human translations it is. So try to cope with this
      // gengo system variety.
      if (!isset($response_job->custom_data)) {
        $response_job = reset($response_job);
      }

      // Not sure what this status means - I was unable to find its purpose in
      // the gengo documentation. But if we have this status we will receive
      // no data at all and therefore we will not be able to create mapping.
      // @todo - this is just a quick fix so that we can finish job submission
      // in such case. But not a solution as we end up with not mapped jobs
      // at gengo. This should be fixed in #2022147.
      if ($response_job->status == 'held') {
        continue;
      }

      // In case we receive an existing translation the array offset IS also
      // the data kay, and we ought to use it as the object custom data is not
      // updated. However this is not always the case and in some case we
      // receive numeric keys, so try to set some custom data to increase the
      // chance of matching the job.
      if (is_numeric($key)) {
        $key = $response_job->custom_data;
      }

      // Extract job item id and data item key.
      list(, $tjiid, $data_item_key) = explode('][', $key, 3);

      $item_id_data_key = $tjiid . '][' . $data_item_key;
      $item = tmgmt_job_item_load($tjiid);
      // Create the mapping.
      $item->addRemoteMapping($data_item_key, NULL, array(
        // Yes, this is not a joke, they really return string value "NULL" in
        // case of a machine translation.
        'remote_identifier_2' => $response_job->job_id == 'NULL' ? 0 : $response_job->job_id,
        'word_count' => $response_job->unit_count,
        'remote_data' => array(
          'credits' => $response_job->credits,
          'tier' => $response_job->tier,
          'duplicates' => isset($duplicates[$item_id_data_key]) ? $duplicates[$item_id_data_key] : array(),
        ),
      ));
      // Update the translation. This needs to be after the mapping as it
      // depends on it for the duplicates handling.
      $this->saveTranslation($job, $item_id_data_key, $response_job);
    }
  }

  /**
   * Creates placeholder records in the mapping table.
   *
   * The idea here is not to introduce additional storage to temporarily store
   * gegngo order id before we get gengo job ids.
   *
   * @param TMGMTJob $job
   *   Job for which to initiate mappings with remote jobs.
   * @param int $gorder_id
   *   Gengo job id.
   * @param array $translations
   *   Array of requested translations.
   * @param array $duplicates
   *   Array of duplicate mappings.
   */
  protected function initGengoMapping(TMGMTJob $job, $gorder_id, array $translations, array $duplicates) {
    $items = $job->getItems();
    foreach ($translations as $key => $translation) {
      // Extract job item id and data item key.
      list(, $tjiid, $data_item_key) = explode('][', $key, 3);
      $mapping_data = array();
      $item_id_data_key = $tjiid . '][' . $data_item_key;
      if (isset($duplicates[$item_id_data_key])) {
        $mapping_data['remote_data']['duplicates'] = $duplicates[$item_id_data_key];
      }
      $items[$tjiid]->addRemoteMapping($data_item_key, $gorder_id, $mapping_data);
    }
  }

  /**
   * Maps TMGMT job data items to gengo jobs.
   *
   * @param TMGMTJob $job
   *   Job that will be mapped to remote jobs.
   */
  public function fetchGengoJobs(TMGMTJob $job) {
    // Search for placeholder item.
    $remotes = entity_get_controller('tmgmt_remote')->loadByLocalData($job->tjid);

    $connector = new TMGMTGengoConnector($job->getTranslator());

    // Collect unique order id's and job ids.
    $order_ids = array();
    $job_ids = array();
    $new_job_ids = array();
    foreach ($remotes as $remote) {
      if (!empty($remote->remote_identifier_1) && !isset($order_ids[$remote->remote_identifier_1])) {
        $order_ids[$remote->remote_identifier_1] = $remote->remote_identifier_1;
      }
      if (!empty($remote->remote_identifier_2)) {
        $job_ids[$remote->remote_identifier_2] = $remote->remote_identifier_2;
      }
    }

    // If we have orders we want to check for new job ids.
    foreach ($order_ids as $gorder_id) {
      try {
        $response = $connector->getOrder($gorder_id);
        // No jobs yet created at Gengo side. Nothing to do.
        if ($response->order->jobs_queued == $response->order->total_jobs) {
          return;
        }

        // Collect job ids of that job. Use array_merge() as we have numerical
        // array keys.
        $order_job_ids = array();
        if (!empty($response->order->jobs_available)) {
          $order_job_ids = array_merge($order_job_ids, $response->order->jobs_available);
        }
        if (!empty($response->order->jobs_pending)) {
          $order_job_ids = array_merge($order_job_ids, $response->order->jobs_pending);
        }
        if (!empty($response->order->jobs_reviewable)) {
          $order_job_ids = array_merge($order_job_ids, $response->order->jobs_reviewable);
        }
        if (!empty($response->order->jobs_approved)) {
          $order_job_ids = array_merge($order_job_ids, $response->order->jobs_approved);
        }
        if (!empty($response->order->jobs_revising)) {
          $order_job_ids = array_merge($order_job_ids, $response->order->jobs_revising);
        }

        // Keep a record of new job ids and their mappings and add them to the
        // job id list.
        foreach (array_diff($order_job_ids, $job_ids) as $new_job_id) {
          $job_ids[$new_job_id] = $new_job_id;
          $new_job_ids[$new_job_id] = $gorder_id;
        }

      }
      catch (TMGMTException $e) {
        watchdog_exception('tmgmt_mygengo', $e);
        drupal_set_message($e->getMessage(), 'error');
        continue;
      }
    }

    // Get gengo jobs for existing gengo job ids.
    $response = $connector->getJobs($job_ids);

    // Gengo did not provide any mapping data, do nothing.
    // This only happens in the case that Gengo is unreachable.
    if (empty($response->jobs)) {
      return;
    }

    // Update mappings and save any translations.
    foreach ($response->jobs as $key => $response_job) {
      // Extract job item id and data item key.
      list(, $tjiid, $data_item_key) = explode('][', $response_job->custom_data, 3);

      // If this is a new job, look for a remote mapping based on the data item
      // key and update it.
      if (isset($new_job_ids[$response_job->job_id])) {

        $matching_remote = NULL;
        foreach ($remotes as $remote) {
          if ($remote->data_item_key == $data_item_key && $remote->tjiid == $tjiid) {
            $matching_remote = $remote;
            break;
          }
        }

        // We don't have a remote mapping yet, create one.
        if (!$matching_remote) {
          $item = tmgmt_job_item_load($tjiid);
          $item->addRemoteMapping($data_item_key, $new_job_ids[$response_job->job_id], array(
            'remote_identifier_2' => $response_job->job_id,
            'word_count' => $response_job->unit_count,
            'remote_data' => array(
              'credits' => $response_job->credits,
              'tier' => $response_job->tier,
            ),
            // @todo: Add remote_url.
          ));
        }
        // We have a mapping, update it.
        else {
          $matching_remote->remote_identifier_2 = $response_job->job_id;
          $matching_remote->word_count = $response_job->unit_count;
          $matching_remote->remote_data = array(
            'credits' => $response_job->credits,
            'tier' => $response_job->tier,
          );
          $matching_remote->save();
        }
      }
      $this->saveTranslation($job, $tjiid . '][' . $data_item_key, $response_job);
    }
  }

}
