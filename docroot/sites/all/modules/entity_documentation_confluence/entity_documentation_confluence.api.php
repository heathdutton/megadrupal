<?php

/**
 * @file
 * Hooks provided by the Entity Documentation module.
 */

/**
 * Alter http request arguments. It runs before the drupal_http_request.
 *
 * @param array $arguments
 *   The default http request arguments.
 *
 * @return array
 *   Altered arguments.
 */
function hook_edc_http_arguments_alter(array $arguments) {

  $arguments['uri'] = 'Altered uri';

  return $arguments;
}
