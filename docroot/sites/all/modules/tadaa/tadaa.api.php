<?php

/**
 * @file
 * API documentation for Tadaa!.
 */

/**
 * Get the environment switched from and the new environment.
 *
 * This hook is invoked before the alterations is being performed by tadaa.
 *
 * @param string $old
 *   The environment swtiched from.
 * @param string $new
 *   The environment switched to.
 */
function hook_tadaa_pre_switch($old, $new) {

}


/**
 * Get the environment switched from and the new environment.
 *
 * This hook is invoked after the alterations is performed by tadaa.
 *
 * @param string $old
 *   The environment swtiched from.
 * @param string $new
 *   The environment switched to.
 */
function hook_tadaa_post_switch($old, $new) {

}
