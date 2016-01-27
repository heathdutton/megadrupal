<?php

/**
 * Initialize IP address.
 */
function cryptolog_initialize() {
  // If no supported backends are found then return.
  foreach (array('dmemcache_set' => 'dmemcache_get', 'apc_store' => 'apc_fetch', 'xcache_set' => 'xcache_get') as $set => $get) {
    if (function_exists($set)) {
      $cryptolog = $get('cryptolog');
      $day = gmdate('zY');
      if (!$cryptolog || $cryptolog['day'] !== $day) {
        $cryptolog = array('day' => $day, 'salt' => drupal_random_bytes(32));
        $set('cryptolog', $cryptolog, 86400);
      }
      // Generate a keyed hash in IPv6 address notation.
      $_SERVER['REMOTE_ADDR'] = inet_ntop(hash_hmac('md5', ip_address(), $cryptolog['salt'], TRUE));
      drupal_static_reset('ip_address');
      break;
    }
  }
}

cryptolog_initialize();
