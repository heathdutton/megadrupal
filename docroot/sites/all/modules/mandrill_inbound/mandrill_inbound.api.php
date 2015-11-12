<?php

/**
 * @file
 * Describe hooks provided by the Mandrill Inbound module.
 */

/**
 * Alter argument that sends to Rules.
 *
 * @see mandrill_inbound_callback()
 */
function hook_mandrill_inbound_alter(&$args) {
  $args['text']['value'] = 'hello';
}