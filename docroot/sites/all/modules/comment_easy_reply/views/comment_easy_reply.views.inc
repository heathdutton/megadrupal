<?php

/**
 * @file
 * Contains Views module hooks used by Comment Easy Reply module.
 */

/**
 * Implements hook_views_data().
 */
function comment_easy_reply_views_data() {
  // Link to reply to a comment.
  $data['comment']['comment_easy_reply_replyto_comment'] = array(
    'field' => array(
      'title' => t('Comment Easy Reply Reply-to link'),
      'help' => t('Provide a Comment Easy Reply link to reply to the comment.'),
      'handler' => 'views_handler_field_comment_easy_reply_link_reply',
      'label' => t('Reply-to link'),
    ),
  );
  if (module_exists('quote')) {
    // Link to quote a comment.
    $data['comment']['comment_easy_reply_quote_comment'] = array(
      'field' => array(
        'title' => t('Comment Easy Reply Quote link'),
        'help' => t('Provide a link to quote to the comment.'),
        'handler' => 'views_handler_field_comment_easy_reply_link_quote',
        'label' => t('Quote link'),
      ),
    );
  }
  // Comment number.
  $data['comment']['comment_easy_reply_comment_number'] = array(
    'field' => array(
      'title' => t('Comment Easy Reply Comment number'),
      'help' => t('Provide a Comment Easy Reply number for every comment.'),
      'handler' => 'views_handler_field_comment_easy_reply_comment_number',
      'label' => t('Comment number'),
    ),
    'sort' => array(
      'title' => t('Comment Easy Reply Comment number'),
      'help' => t('Sort by the comment number order.'),
      'handler' => 'views_handler_sort_comment_easy_reply_comment_number',
    ),
  );
  return $data;
}

/**
 * Implements hook_views_data_alter().
 */
function comment_easy_reply_views_data_alter(&$data) {
  if (isset($data['field_data_comment_body']['comment_body']['field']['handler'])) {
    $data['field_data_comment_body']['comment_body']['field']['handler'] = 'views_handler_field_comment_easy_reply_comment_body';
  }
  if (isset($data['comment']['subject']['field']['handler'])) {
    $data['comment']['subject']['field']['handler'] = 'views_handler_field_comment_easy_reply_comment_title';
  }
}
