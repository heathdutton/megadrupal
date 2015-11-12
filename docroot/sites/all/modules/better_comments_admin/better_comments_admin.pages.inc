<?php
/**
 * @file
 */

/**
 * Menu callback; approve specified comment.
 *
 * @param $type
 *
 * @param $cid
 *   A comment identifier.
 */
function better_comments_admin_comment_approve($type = 'ajax', $cid) {
  if (!isset($_GET['token']) || !drupal_valid_token($_GET['token'], "better_comments_admin/ajax/nojs/$cid/approve")) {
    return MENU_ACCESS_DENIED;
  }
  if ($comment = comment_load($cid)) {
    $comment->status = COMMENT_PUBLISHED;
    comment_save($comment);

    drupal_set_message(t('Comment approved.'));

    if ('ajax' == $type) {
      $element = array(
        '#type' => 'link',
        '#title' => t('reject'),
        '#href' => "better_comments_admin/ajax/nojs/" . $cid . "/reject",
        '#options' => array(
          'query' => drupal_get_destination() + array('token' => drupal_get_token("better_comments_admin/ajax/nojs/$cid/reject")),
          'attributes' => array(
            'class' => array(
              'use-ajax'
            ),
          ),
        ),
      );

      $selector = 'a[href="' . str_replace('/ajax/ajax/', '/ajax/nojs/', request_uri()) . '"]';
      $commands = array();
      $commands[] = ajax_command_replace($selector, render($element));

      $element = array(
        '#type' => 'ajax',
        '#commands' => $commands,
      );
      ajax_deliver($element);
    }

    else {
      drupal_goto('node/' . $comment->nid);
    }
  }
}

/**
 * Menu callback; preview specified comment.
 *
 * @param $type
 *
 * @param $comment
 *   A comment object.
 */
function better_comments_admin_comment_preview($type = 'ajax', $comment) {
  $settings = better_comments_admin_settings_defaults();

  $node = node_load($comment->nid);
  $element = comment_view($comment, $node, $settings['view_mode']);

  if ('ajax' == $type) {
    $commands = array();
    $commands[] = ajax_command_html('#better_comments_admin-preview', render($element));

    $element = array(
      '#type' => 'ajax',
      '#commands' => $commands,
    );

    ajax_deliver($element);
  }

  else {
    return render($element);
  }
}

/**
 * Menu callback; unpublish specified comment.
 *
 * @param $type
 *
 * @param $cid
 *   A comment identifier.
 */
function better_comments_admin_comment_reject($type = 'ajax', $cid) {
  if (!isset($_GET['token']) || !drupal_valid_token($_GET['token'], "better_comments_admin/ajax/nojs/$cid/reject")) {
    return MENU_ACCESS_DENIED;
  }
  if ($comment = comment_load($cid)) {
    $comment->status = COMMENT_NOT_PUBLISHED;
    comment_save($comment);

    drupal_set_message(t('Comment rejected.'));

    if ('ajax' == $type) {
      $element = array(
        '#type' => 'link',
        '#title' => t('approve'),
        '#href' => "better_comments_admin/ajax/nojs/{$cid}/approve",
        '#options' => array(
          'query' => drupal_get_destination() + array('token' => drupal_get_token("better_comments_admin/ajax/nojs/$cid/approve")),
          'attributes' => array(
            'class' => array(
              'use-ajax'
            ),
          ),
        ),
      );

      $selector = 'a[href="' . str_replace('/ajax/ajax/', '/ajax/nojs/', request_uri()) . '"]';
      $commands = array();
      $commands[] = ajax_command_replace($selector, render($element));

      $element = array(
        '#type' => 'ajax',
        '#commands' => $commands,
      );
      ajax_deliver($element);
    }

    else {
      drupal_goto('node/' . $comment->nid);
    }
  }
}
