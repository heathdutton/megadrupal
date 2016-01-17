<?php

/**
 * @file vkxp.api.php
 * Hooks provided by the vkxp module.
 */

/**
 * Allows to change http query params.
 *
 * $query contains 3 values:
 *   @param $query['method']: string with vk api method name.
 *   @param $query['params']: array with parameters that will be posted.
 *   @param $query['request_url']: url to request will be sent.
 */
function hook_vkxp_query_alter(&$query) {
  if ($query['method'] == 'wall.post') {
    $query['params']['message'] .= "\n" . t('Attachments disabled');
    unset($query['params']['attachments']);
  }
}
