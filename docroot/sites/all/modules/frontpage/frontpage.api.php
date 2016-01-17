<?php
// $Id: frontpage.api.php,v 1.1.2.1 2011/01/11 23:06:55 kiam Exp $

/**
 * @file
 * Hooks provided by the Frontpage module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Allows other modules to change how the frontpage is rendered.
 *
 * @param $build
 *   The array required by drupal_render().
 * @param $context
 *   An array containing the index 'logged in', which is TRUE when the
 *   user is logged in.
 */
function hook_frontpage_view_alter(&$build, &$context) {
}

/**
 * Allows other modules to change where users are redirected.
 *
 * @param $redirect
 *   The relative URL where users are redirected. The module uses this value
 *   when the frontpage has not been set; the default value is the value
 *   contained in the Drupal variable 'site_frontpage' before the module is
 *   enabled.
 * @param $context
 *   An array containing the index 'logged in', which is TRUE when the
 *   user is logged in.
 */
function hook_frontpage_redirect_alter(&$redirect, &$context) {
}

/**
 * @} End of "addtogroup hooks".
 */

