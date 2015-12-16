<?php

/**
 * @file
 * Hooks provided by the module.
 */

/**
 * Alter rest request data/headers.
 *
 * @param $data
 *   The post data sent to the external REST api.
 * @param $headers
 *   The request headers sent to the external REST api.
 * @param $job
 *   The translation job being sent.
 */
function hook_tmgmt_rest_post_request_alter(&$data, &$headers, &$job) {
  // Add a custom field to the post data.
  $data['custom_field'] = 'Custom post value';
  // Unset authorization header.
  unset($headers['Authorization']);
}

/**
 * Alter the rest API input data/signature.
 *
 * @param $post_data
 *   The post data sent to our REST api.
 * @param $internal_signature
 *   The internal signature calculated from the post data.
 * @param $job
 *   The translation job being sent.
 */
function hook_tmgmt_rest_api_request_alter(&$data, &$signature, &$job) {
  // Change the way we calculate the signature to validate.
  $auth_string = $data['custom_field'] . '|' . $_POST['timestamp'] . '|' . $job->tjid . '|' . $_POST['xlf'];
  $signature = base64_encode(hash_hmac('sha256', (string) $auth_string, (string) $job->getSetting('app_secret'), TRUE));
}
