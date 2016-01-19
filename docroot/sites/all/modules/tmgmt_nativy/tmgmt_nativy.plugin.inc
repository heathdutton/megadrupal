<?php

/**
 * @file
 * Provides the Nativy Translator plugin controller.
 */

/**
 * Nativy translator plugin controller.
 */
class TMGMTNativyTranslatorPluginController extends TMGMTDefaultTranslatorPluginController implements TMGMTTranslatorRejectDataItem {

  protected $live_site = 'http://nativy.com/';
  protected $test_site = 'http://test.nativy.com/';

  /**
   * Call a REST method from Nativy Connect.
   *
   * @param $translator
   *   Translator object.
   * @param $method
   *   The REST method to call.
   * @param $query
   *   Arguments for the method.
   *
   * @return
   *   The service response.
   */
  protected function get(TMGMTTranslator $translator, $method, array $query = array(), &$status = NULL) {
    return $this->request(url($this->getURL($translator, $method), array('query' => $query, 'absolute' => TRUE)), array(), $status);
  }

  /**
   * Executes and logs a request.
   *
   * @param $url
   *   The URL that should be requested.
   * @param $options
   *   Options to pass to drupal_http_request().
   *
   * @return
   *   The JSON
   */
  protected function request($url, $options = array(), &$status = NULL) {
    $start = microtime(TRUE);
    $request = drupal_http_request($url, $options);

    $headers = '';
    if (!empty($request->headers)) {
      foreach ($request->headers as $name => $value) {
        $headers .= "$name: $value\n";
      }
    }

    db_insert('tmgmt_nativy_log')
      ->fields(array(
        'url' => substr($url, 0, 255),
        'request' => $request->request,
        'response_code' => $request->code,
        'response_data' => isset($request->data) ? $request->data : '',
        'response_headers' => $headers,
        'time' => microtime(TRUE) - $start,
      ))
      ->execute();

    $status = $request->code;

    return isset($request->data) ? json_decode($request->data) : '';
  }

  /**
   * Executes a post request.
   *
   * @param $translator
   *   The translator entity.
   * @param $method
   *   The method to call.
   * @param $query
   *   The POST data to submit.
   *
   * @return
   *   The returned, json decoded object.
   */
  protected function post(TMGMTTranslator $translator, $method, $query = array(), &$status = NULL) {
    $url = url($this->getURL($translator, $method), array($query));
    $options = array(
      'headers' => array(
        'Content-type' => 'application/json',
      ),
      'method' => 'POST',
      'data' => $query,
    );
    return $this->request($url, $options, $status);
  }

  /**
   * Executes a put request.
   *
   * @param $translator
   *   The translator entity.
   * @param $method
   *   The method to call.
   * @param $query
   *   The PUT data to submit.
   *
   * @return
   *   The returned, json decoded object.
   */
  protected function put(TMGMTTranslator $translator, $method, $query = array(), &$status = NULL) {
    $url = url($this->getURL($translator, $method), array('query' => $query));
    $options = array(
      'headers' => array(
        'Content-type' => 'application/json',
      ),
      'method' => 'PUT',
    );
    return $this->request($url, $options, $status);
  }

  /**
   * Returns the URL for a given method.
   *
   * @param $translator
   *   The translator entity.
   * @param $method
   *   The method to call.
   *
   * @return
   *   The absolute URL to call.
   */
  protected function getURL(TMGMTTranslator $translator, $method) {
    $url = $translator->getSetting('use_test') ? $this->test_site : $this->live_site;
    $url .= 'connect/' . $method;
    return $url;
  }

  /**
   * Returns an array of the required authentication query arguments.
   *
   * @param $translator
   *   The translator entity.
   */
  protected function getAuthenticationQuery(TMGMTTranslator $translator) {
    $api_sign = hash_hmac('sha1', REQUEST_TIME, $translator->getSetting('private_key'));
    return array(
      'api_key' => $translator->getSetting('api_key'),
      'api_sign' => $api_sign,
      'timestamp' => REQUEST_TIME,
    );
  }

  /**
   * Verifies the api_sign.
   *
   * @param $translator
   *   The translator entity.
   * @param $timestamp
   *   The timestamp that has been signed.
   * @param $api_sign
   *   The sign.
   *
   * @return
   *   TRUE if the sign is correct.
   */
  public function verify(TMGMTTranslator $translator, $timestamp, $api_sign) {
    return strtolower($api_sign) == strtolower(hash_hmac('sha1', $timestamp, $translator->getSetting('private_key')));
  }

  /**
   * Implements TMGMTTranslatorPluginControllerInterface::isAvailable().
   */
  public function isAvailable(TMGMTTranslator $translator) {
    if ($translator->getSetting('api_key') && $translator->getSetting('private_key')) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Implements TMGMTTranslatorPluginControllerInterface::requestTranslation().
   *
   * Here we will acutally query source and get translations.
   */
  public function requestTranslation(TMGMTJob $job) {
    $rank_details = $job->getSetting('rank_details');
    $redirect_path = current_path();
    // Attempt to find a better redirect path if tmgmt_ui module is available.
    if (module_exists('tmgmt_ui')) {
      $queue_destination = tmgmt_ui_redirect_queue_destination();
      $redirect_path = $queue_destination ? $queue_destination : $redirect_path;
    }
    $payment_link = $rank_details[$job->getSetting('rank')]['payment_link'] . '&returnlink=' . urlencode(url($redirect_path, array('absolute' => TRUE)));
    $job->reference = $rank_details[$job->getSetting('rank')]['orderId'];
    $job->submitted('Proceed to the <a href="!url">payment page</a> to submit this job.', array('!url' => $payment_link));
  }

  /**
   * Returns an order from a job
   */
  public function getOrder(TMGMTJob $job) {
    $order = $this->getAuthenticationQuery($job->getTranslator());
    $order['callbacklink'] = url('tmgmt/nativy/callback/' . $job->tjid, array('absolute' => TRUE));
    $order['language_from'] = $job->source_language;
    $order['language_to'] = $job->target_language;
    $order['nativy_partner_name'] = 'Drupal TMGMT';

    // Add selected options.
    foreach (array('short_description', 'long_description', 'correction_required') as $option) {
      $order[$option] = $job->getSetting($option);
    }
    $order += $job->getSetting('weighting');

    $data = array_filter(tmgmt_flatten_data($job->getData()), '_tmgmt_filter_data');
    $texts = array();
    foreach ($data as $key => $text) {
      $texts[] = array(
        'Key' => $key,
        'Value' => $text['#text'],
      );
    }
    $order['text_from'] = $texts;

    $response = $this->post($job->getTranslator(), 'order', json_encode($order));
    return $response;
  }

  /**
   * Retrieves an order object from the Nativy service.
   *
   * @param TMGMTJob $job
   *   The job for which the order should be retrieved.
   *
   * @return
   *   The nativy order object.
   */
  public function nativyRetrieveOrder(TMGMTJob $job) {
    $query = $this->getAuthenticationQuery($job->getTranslator());
    return $this->get($job->getTranslator(), 'order/' . $job->reference, $query);
  }

  /**
   * Retrieves an order object from the Nativy service.
   *
   * @param TMGMTJob $job
   *   The job for which the order should be retrieved.
   * @param $key
   *   The key of the reviewd data item.
   *   The key is an array containing the keys of a nested array hierarchy path.
   *
   * @return
   *   The nativy order object.
   */
  public function nativyRetrieveSingleTranslation(TMGMTJob $job, array $key) {
    $query = $this->getAuthenticationQuery($job->getTranslator());
    return $this->get($job->getTranslator(), $this->reviewMethod($job, $key), $query);
  }

  /**
   * Retrieve job when ready (we got an email with a link?)
   */
  public function retrieveTranslation(TMGMTJob $job) {
    if ($document = $this->nativyRetrieveOrder($job)) {
      $translation = array();
      foreach ($document->text_to as $text) {
        $key = $text->Key;
        $text = $text->Value;
        $translation[$key] = array(
          '#text' => $text,
        );
      }
      $job->addTranslatedData(tmgmt_unflatten_data($translation));
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Retrieve review when ready
   */
  public function retrieveSingleTranslation(TMGMTJob $job, array $key) {
    if ($document = $this->nativyRetrieveSingleTranslation($job, $key)) {
      $translation = array();
      foreach ($document->text_to as $text) {
        $key = $text->Key;
        $text = $text->Value;
        $translation[$key] = array(
          '#text' => $text,
        );
      }
      $job->addTranslatedData($translation);
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Implements TMGMTTranslatorPluginControllerInterface::getSupportedTargetLanguages().
   */
  public function getSupportedTargetLanguages(TMGMTTranslator $translator, $source_language) {
    global $language;
    $response = $this->get($translator, 'languages', array('language_short' => $language->language));
    $languages = array();
    foreach ($response as $language_row) {
      $languages[$language_row->language_short] = $language_row->language_short;
    }
    return $languages;
  }

  /**
   * Implements TMGMTTranslatorRejectDataItem::rejectDataItem().
   */
  public function rejectDataItem(TMGMTJobItem $job_item, array $key, array $values = NULL) {
    array_unshift($key, $job_item->tjiid);
    $query = $this->getAuthenticationQuery($job_item->getTranslator());
    $query['comment'] = $values['comment'];
    $this->put($job_item->getTranslator(), $this->reviewMethod($job_item->getJob(), $key), $query, $status);
    if ($status != 200) {
      $job_item->addMessage('Reject failed', array(), 'error');
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Implements TMGMTTranslatorRejectDataItem::rejectForm().
   */
  public function rejectForm($form, &$form_state) {
    $form['comment'] = array(
      '#type' => 'textarea',
      '#title' => t('Comment'),
      '#description' => t('Provide a reason for your rejection.'),
      '#required' => TRUE,
    );
    return $form;
  }

  /**
   * Build a method string to acces a review.
   *
   * @param TMGMTJob $job
   *   The job associated to the review.
   * @param $key
   *   The key of the reviewd data item.
   *   The key is an array containing the keys of a nested array hierarchy path.
   *
   * @return
   *   Method string to be used in a API request.
   */
  private function reviewMethod(TMGMTJob $job, array $key) {
    return 'order/' . $job->reference . '/review/' . urlencode(tmgmt_ensure_keys_string($key));
  }
}
