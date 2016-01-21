<?php

/**
 * @file
 * Hooks provided by SMS Gateway Hub module.
 */

/**
 * Do something before sending SMS.
 *
 * This hook is invoked after hook_smsgatewayhub_sms_alter().
 *
 * @param string $numbers
 *   Single number or comma separated numbers.
 * @param string $message
 *   SMS message.
 * @param int $flash
 *   0 or 1 indicating whether this SMS should be sent as flash message.
 * @param int $transactional
 *   0 or 1 indicating whether this SMS should be sent as transactional message.
 * @param int $scheduled_time
 *   Timestamp indicating when the message will be sent. If 0 then scheduled
 *   time is disabled.
 */
function hook_smsgatewayhub_before_send_sms($numbers, $message, $flash, $transactional, $scheduled_time) {
  // Do something here.
}

/**
 * Do something after SMS is sent.
 *
 * @param string $numbers
 *   Single number or comma separated numbers.
 * @param string $message
 *   SMS message.
 * @param int $flash
 *   0 or 1 indicating whether this SMS should be sent as flash message.
 * @param int $transactional
 *   0 or 1 indicating whether this SMS should be sent as transactional message.
 * @param int $scheduled_time
 *   Timestamp indicating when the message will be sent. If 0 then scheduled
 *   time is disabled.
 * @param object $response
 *   Response object as returned by drupal_http_request().
 */
function hook_smsgatewayhub_after_send_sms($numbers, $message, $flash, $transactional, $scheduled_time, $response) {
  // Do something here.
}

/**
 * Alter message or number here.
 *
 * This hook is invoked before hook_smsgatewayhub_before_send_sms().
 * You can perform alter operations in this hook.
 *
 * @param array $data
 *   Array with alterable data. Attributes of data are:
 *   - numbers: A string containing numbers, single or multiple separated with
 *     comma.
 *   - message: SMS message.
 *   - flash: Indicating whether this SMS to be sent as flash message.
 *   - transactional: Indicating whether this SMS to be sent as transactional
 *     SMS.
 *   - scheduled_time: Timestamp indicating when the message will be sent.
 */
function hook_smsgatewayhub_sms_alter(array &$data) {
  // Do something here, like changing message text.
}
