<?php

/**
 * @file
 * Hooks provided by the Arte OPA module.
 */

/**
 * Alter the OPA headers before request is sent.
 *
 * It particularly allow drupal websites to specify
 * a custom user agent for identifying themselves to OPA.
 * Note that no modification done to $params will be effective.
 *
 * @param array $headers
 *   Set this to the new desired value.
 * @param array $params
 *   The query parameters, READ ONLY.
 */
function hook_arte_opa_headers_alter(array &$headers, array &$params) {
  $headers['User-Agent'] = 'PLATFORM';
}
