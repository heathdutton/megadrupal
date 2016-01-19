<?php

/**
 * @file
 * Hooks provided by the Geo Redirect.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Control redirection.
 *
 * @param $geo_redirect
 *   Geo redirect object for which redirection will be executed.
 * @param $redirect
 *   Boolean indicating whether redirection will take place. Defaults to TRUE.
 */
function hook_geo_redirect_execute_alter(&$geo_redirect, &$redirect) {
  // Disable redirection. This will disable all redirections.
  // You can check your conditions here.
  $redirect = FALSE;
}

/**
 * @} End of "addtogroup hooks".
 */
