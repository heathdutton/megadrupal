<?php

/**
 * Returns HTML for a results report of an operation run by authorize.php.
 *
 * @param $variables
 *   An associative array containing:
 *   - messages: An array of result messages.
 *
 * @ingroup themeable
 */
function twbs_authorize_report($variables) {
  $messages = $variables['messages'];
  $output = '';
  if (!empty($messages)) {
    $output .= '<div id="authorize-results">';
    foreach ($messages as $heading => $logs) {
      $items = array();
      foreach ($logs as $number => $log_message) {
        if ($number === '#abort') {
          continue;
        }
        $items[] = theme('authorize_message', array('message' => $log_message['message'], 'success' => $log_message['success']));
      }
      $output .= theme('item_list', array('items' => $items, 'title' => $heading));
    }
    $output .= '</div>';
  }
  return $output;
}
