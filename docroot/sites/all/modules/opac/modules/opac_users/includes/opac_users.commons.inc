<?php
/**
 * @file
 * Commons functions for opac users module.
 */

/**
 * Check if a user is connected to a specific server.
 */
function opac_users_connected_to($server) {
  // If $server is not valid.
  if (!$server || $server == 'none') {
    return FALSE;
  }

  // If $_SESSION['opac_user'] is not set,
  // user is not connected to any server.
  if (!isset($_SESSION['opac_user'])) {
    return FALSE;
  }

  if ($_SESSION['opac_user']['opac_server'] != $server) {
    return FALSE;
  }
  return TRUE;
}
