<?php

/**
 * @file
 *   voipscripts for voipnode.
 */

/**
 * Basic default script to read voipnode contents
 */
function _voipnode_get_basic_script($node) {
  $say = '';
  if ($file = field_get_items('node', $node, 'field_voipnode_audio')) {
    $file = reset($file);
    $say = file_create_url($file['uri']);
  }
  if (empty($say)) {
    $body = field_get_items('node', $node, 'body');
    $body = reset($body);
    $say = strip_tags($body['value']);
  }

  $script = new VoipScript('voipnode_basic');
  $script->addSay($say);
  $script->addReturn();

  return $script;
}

