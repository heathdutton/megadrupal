<?php

/**
 * @file
 * Token callbacks for the Gentle Reminder module.
 */

/**
 * Implements hook_token_info().
 */
function gentle_reminder_token_info() {
  return array(
    'types' => array(
      'gentle_reminder' => array(
        'name' => t('Gentle reminder'),
        'description' => t('The recipient of the message.'),
        'needs-data' => 'gentle_reminder',
      ),
    ),
    'tokens' => array(
      'gentle_reminder' => array(
        'recipient' => array(
          'name' => t('Recipient'),
          'description' => t('The recipient of the messages.'),
          'type' => 'user',
        ),
        'items' => array(
          'name' => t('Items'),
          'description' => t('The array of formatted items to send.'),
          'type' => 'array',
        ),
      ),
    ),
  );
}

/**
 * Implements hook_tokens().
 */
function gentle_reminder_tokens($type, array $tokens, array $data = array(), array $options = array()) {
  $return = array();

  if ($type == 'gentle_reminder' && !empty($data['gentle_reminder'])) {
    $recipient = $data['gentle_reminder']['recipient'];
    $items = $data['gentle_reminder']['items'];
    $rendered_items = $data['gentle_reminder']['rendered_items'];

    // Elaboration replacements.
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'items:first':
          $return[$original] = reset($rendered_items);
          break;

        case 'items:last':
          $return[$original] = end($rendered_items);
          break;

        case 'items:count':
          $return[$original] = count($items);
          break;

        case 'items:keys':
          $return[$original] = token_render_array(array_keys($items), $options);
          break;

        case 'items:reversed':
          $reversed = array_reverse(array_keys($items), TRUE);
          $return[$original] = token_render_array($reversed, $options);
          break;

        case 'items':
        case 'items:join':
          $return[$original] = token_render_array($rendered_items, array('join' => '', 'sanitize' => FALSE) + $options);
          break;
      }
    }

    if ($recipient_tokens = token_find_with_prefix($tokens, 'recipient')) {
      $return += token_generate('user', $recipient_tokens, array('user' => $recipient), $options);
    }
  }

  return $return;
}
