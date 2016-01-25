<?php

/**
 * @file
 * Hooks provided by the VirusTotal API module.
 *
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 *
 * See virustotal.class.php or virustotal_example module for more information.
 */

/**
 * @addtogroup hooks
 * @{
 * Hooks that can be implemented by other modules to extend VirusTotal API.
 */

/**
 * The API request is about to be send.
 *
 * Modules may make changes to the query data before it is sent to VirusTotal.
 *
 * @param array &$data
 *   An array containing request data as name/value pairs.
 * @param string $function
 *   The API function name.
 */
function hook_virustotal_query_alter(&$data, $function) {
  // Print a status message containing the requested report ressource ID.
  if ($function == VIRUSTOTAL_FUNC_GET_URL_REPORT || $function == VIRUSTOTAL_FUNC_GET_FILE_REPORT) {
    drupal_set_message(t('Requested resource: @resource', array('@resource' => $data['resource'])));
  }
}

/**
 * The API response data is about to be returned.
 *
 * Modules may make changes to the response data before it is returned.
 *
 * @param array &$data
 *   An array containing decoded json data.
 *   At least the following keys:
 *   - response_code: Response status code.
 *   - verbose_msg: Versbose message regarding response code.
 * @param string $function
 *   The API function name.
 */
function hook_virustotal_result_alter(&$data, $function) {
  // Print a result status message.
  drupal_set_message(t('Response code: @response_code, Verbose message: @verbose_msg', array(
    '@response_code' => $data['response_code'],
    '@verbose_msg' => $data['verbose_msg'])
  ));
}

/**
 * @} End of "addtogroup hooks".
 */
