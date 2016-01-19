<?php

/**
 * @file
 * Provides REST service translation plugin controller.
 */

/**
 * REST translation plugin controller.
 */
class TMGMTRestTranslatorPluginController extends TMGMTFileTranslatorPluginController {

  /**
   * {@inheritdoc}
   */
  public function requestTranslation(TMGMTJob $job) {
    $export = tmgmt_file_format_controller($job->getSetting('export_format'));

    // Create post data/headers.
    $data = array(
      'timestamp' => time(),
      'jobid' => $job->tjid,
      'xlf' => $export->export($job),
    );
    $auth_string = $data['timestamp'] . '|' . $data['jobid'] . '|' . $data['xlf'];
    $headers = array(
      'Content-Type' => 'application/x-www-form-urlencoded',
      'Authorization' => base64_encode(hash_hmac('sha256', (string) $auth_string, (string) $job->getSetting('app_secret'), TRUE)),
    );
    drupal_alter('tmgmt_rest_post_request', $data, $headers, $job);

    // Do post request.
    $options = array(
      'method' => 'POST',
      'data' => drupal_http_build_query($data),
      'timeout' => 30,
      'headers' => $headers,
    );
    $response = drupal_http_request($job->getSetting('rest_url'), $options);
    if ($response->code == 200) {
      $job->addMessage('Job has been successfully submitted for translation.');
    }
    else {
      $job->addMessage('Failed to submit job for translation. Return error %error', array('%error' => $response->code . ': ' . $response->status_message), 'error');
    }
  }

  /**
   * {@inheritdoc}
   */
  public function defaultSettings() {
    return array(
      'export_format' => 'xlf',
      'allow_override' => FALSE,
      'scheme' => 'public',
      'xliff_processing' => TRUE,
    );
  }

}
