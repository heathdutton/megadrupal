<?php

/**
 * Implements hook_tokens_alter().
 */
function anonymous_posting_tokens_alter(array &$replacements, array $context) {
  // Replace token [*:node:author:*}] value with anonymous_posting author values
  // if the node author is anonymous.
  if ($context['type'] == 'node'
    && !empty($context['data']['node'])
    && !$context['data']['node']->uid // true if node author is anonymous
    && $field_items = field_get_items('node', $context['data']['node'], ANONYMOUS_POSTING_FIELD_NAME)) {
    foreach ($replacements as $token => $replacement) {
      $field = null;
      if (preg_match('/^\[(?:[^\s:]+:)*node:author(?:\:([^\s\:]+))?\]$/', $token, $matches)) {
        if (count($matches) > 1) { // token specify one of :
          switch ($matches[1]) {
            case 'name':
            case 'mail':
              $field = $matches[1];
              break;
          }
        }
        else {
          $field = 'name';
        }
      }
      if ($field) {
        $field_items = current($field_items);
        if ($field_items[$field]) {
          $replacements[$token] = check_plain($field_items[$field]);
        }
      }
    }
  }
}
