<?php

/**
 * Implements hook_tarpit_reaction().
 */
function tarpit_ban_tarpit_reaction() {
  $ip = ip_address();
  db_insert('blocked_ips')
    ->fields(array('ip' => $ip))
    ->execute();
  watchdog('tarpit', 'Banned IP address %ip', array('%ip' => $ip));
}