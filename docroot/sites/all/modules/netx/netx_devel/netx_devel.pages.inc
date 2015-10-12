<?php

/**
 * @file
 * Page callbacks for the NetX module.
 */

/**
 * Page callback: debug/profile NetX repository functions.
 */
function netx_devel_page() {
  timer_start('json-rpc');
  $repo = new NetXRepository();
  $data = $repo->getAssetBean(18);
  $t1 = timer_stop('json-rpc');

  return array(
    '#theme' => 'table',
    '#header' => array(
      'rpc' => format_string('RPC @time', array('@time' => $t1['time'])),
    ),
    '#rows' => array(
      array(
        'rpc' => array('data' => '<pre>' . var_export($data, TRUE) . '</pre>'),
      ),
    ),
  );
}
