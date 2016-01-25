<?php

/**
 * @file
 * Private message with node.js API Documentation
 */

/**
 * One can use powerful API from the private message module.
 */

/**
 * One can change some settings for message/alert which are going to node.js.
 *
 * @param object $data
 *   Object with settings which is going to node.js channel.
 *
 * @param array $message
 *   Privatemsg message array. If the $type is equal to the status, then the
 *   empty array is given.
 *
 * @param string $type
 *   as string, can be:
 *   - 'message'
 *   - 'alert'
 *   - 'status'
 */
function hook_privatemsg_nodejs_channel_alter(&$data, $message, $type) {

  // For example one can change js callback for alert type.
  if ($type == 'alert') {
    $data->callback = 'mymoduleNewCallback';
  }

  // For example one can add new information for js from $message for message
  // type.
  if ($type == 'message') {
    $data->newInfo = $message['new_info'];
    $data->callback = 'mymoduleNewCallback';
  }

}
