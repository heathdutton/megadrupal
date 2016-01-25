<?php

/**
 * @file
 * Implements hooks for Comment Easy Reply Node module.
 */

/**
 * Implements hook_comment_easy_reply_settings().
 */
function comment_easy_reply_node_comment_easy_reply_settings() {
  $settings = array();
  $default = array(
    'callback' => '_comment_easy_reply_node_settings_value_callback',
    'report callback' => '_comment_easy_reply_node_status_report_callback',
  );
  $names = array(
    'comment_easy_reply_node_reply_node_link_activated' => array(
      'report label' => t('Node Reply link active:'),
    ),
    'comment_easy_reply_node_reply_node_link' => array(
      'report label' => t('Node Reply link:'),
    ),
    'comment_easy_reply_node_reply_subject' => array(
      'report label' => t('Node Reply comment subject:'),
    ),
    'comment_easy_reply_node_node_display' => array(
      'report label' => t('Node Reply active displays:'),
    ),
  );
  foreach ($names as $name => $value) {
    if (is_array($value)) {
      $settings[$name] = array_merge($default, $value);
    }
    else {
      $settings[$value] = $default;
    }
  }
  return $settings;
}

/**
 * Implements hook_comment_easy_reply_text_replace_alter().
 */
function comment_easy_reply_node_comment_easy_reply_text_replace_alter(&$text, $comment_nid, $include_parent, $options, $original_body) {
  if (isset($options['comment']) && _comment_easy_reply_comment_node_type_is_allowed($options['comment'])
    && _comment_easy_reply_node_is_comment_node_type_active($options['comment'])) {
    $node = node_load($comment_nid);
    $account = user_load($node->uid);
    $tag = '@' . format_username($account);
    $text = str_replace('@#0', '<a href="#">' . $tag . '</a>', $text);
  }
}

/**
 * Implements hook_comment_easy_reply_text_matches_alter().
 */
function comment_easy_reply_node_comment_easy_reply_text_matches_alter(&$text, &$result) {
  $matches = array();
  preg_match_all('/@#(0)[^0-9]*/i', $text, $matches);
  if (isset($matches[1]) && !empty($matches[1])) {
    foreach ($matches[1] as $num) {
      $result[$num] = $num;
    }
  }
}
