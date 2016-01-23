<?php

/**
 * Implements hook_drush_command().
 */
function cointools_daemon_drush_command() {
  $items = array();

  $items['bitcoin-walletnotify'] = array(
    'description' => "Respond when a wallet transaction changes.",
    'arguments' => array(
      'txid' => 'Transaction ID',
    ),
  );

  return $items;
}
