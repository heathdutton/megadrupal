<?php

/**
 * @file
 */

/**
 * Internal action that handle the service to shorten the URL
 */
function _tq_shorten_url_action(&$ret = array(), $node) {
  $service_selected = variable_get('tq_shorten_url_service_select', 'none');

  $node_path = url('node/' . $node->nid, array('absolute' => TRUE));

  if ($service_selected === "none") {
    return $ret['error'] = TRUE;
  }

  if ($service_selected === "bitly") {
    $address = 'https://api-ssl.bitly.com';
    $get     = '/v3/shorten?login=' . variable_get('tq_shorten_url_bitly_user', '') . '&apiKey=' . variable_get('tq_shorten_url_bitly_token_api', '') . '&longUrl=' . urlencode($node_path);
    $domain  = variable_get('tq_shorten_url_bitly_domain', 'bit.ly');

    if ($domain !== 'custom') {
      $get .= '&domain=' . $domain;
    }

    $response = drupal_http_request($address . $get);

    if ((int)$response->code === 200) {
      $data = json_decode($response->data);

      $ret['error'] = FALSE;
      $ret['url']   = urldecode($data->data->url);
    }
    else {
      $ret['error'] = TRUE;
    }
  }
}
