<?php

/**
 * Plugin controller for remote translation sources.
 */
class TMGMTRemoteSourcePluginController extends TMGMTDefaultSourcePluginController {

  /**
   * Implements TMGMTSourcePluginControllerInterface::getData().
   */
  public function getData(TMGMTJobItem $item) {
    $source = entity_load_single('tmgmt_server_remote_source', $item->item_id);

    // We simply have to return the data property.
    return $source->data;
  }

  /**
   * Implements TMGMTSourcePluginControllerInterface::saveTranslation().
   */
  public function saveTranslation(TMGMTJobItem $item) {
    /**
     * @var TMGMTRemoteSource $source
     */
    $source = entity_load_single('tmgmt_server_remote_source', $item->item_id);
    $source->data = $item->getData();
    $source->state = TMGMT_SERVER_REMOTE_SOURCE_TRANSLATED;
    $source->save();

    // The job item was accepted.
    $item->accepted();

    // Notify the remote client about the finished translation unless the
    // translation happened instantly (machine-translation) in which case the
    // service should output the translated data in the response of the
    // request.
    if (!empty($source->callback) && $source->created != REQUEST_TIME) {
      $query = array(
        'id' => $source->rsid,
      );

      if (!empty($source->reference)) {
        $query['reference'] = $source->reference;
      }

      // Send job info only in case it has been mediated by a DS.
      if (!empty($source->ds_url)) {
        /**
         * @var TMGMTClient $client
         */
        $client = entity_load_single('tmgmt_server_client', $source->cid);
        TMGMTServerDSConnector::sendJobInfo($item, TMGMT_DS_JOB_ACTION_TRANSLATION_SENT_TO_TC, $client->uuid);
      }

      $url = url($source->callback, array('query' => $query, 'absolute' => TRUE));

      // Notify the remote server about the finished translation. Normally this
      // will issue a pull request on the client.
      drupal_http_request($url);
    }
  }

  /**
   * Overrides TMGMTDefaultSourcePluginController::getLabel().
   */
  public function getLabel(TMGMTJobItem $job_item) {
    $data = $job_item->getData();
    $children_keys = element_children($data);
    $children_keys = array_reverse($children_keys);
    $key = array_shift($children_keys);
    $elements = $job_item->getData(tmgmt_ensure_keys_array($key), 0);

    if (isset($elements[0])) {
      $element = $elements[0]['value'];
    }
    else {
      $element = $elements;
    }

    if (!empty($element['#text'])) {
      return truncate_utf8($element['#text'], 70, TRUE, TRUE);
    }

    return parent::getLabel($job_item);
  }

}
