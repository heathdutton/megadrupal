<?php

/**
 * @file
 *
 * Page callbacks for the unpublish_own_comment module.
 *
 * @author: alvar0hurtad0
 * @special Thanks: keopx
 */

/**
 * Returns a form to unpublish an own comment.
 */
function unpublish_own_comment_form($form, $form_state, $node, $comment) {
  $form['nid'] = array('#type' => 'value', '#value' => $node->nid);
  $form['cid'] = array('#type' => 'value', '#value' => $comment->cid);

  $dest = array('path' => 'node/' . $node->nid, 'fragment' => 'comment-' . $comment->cid);
  if (isset($_GET['destination'])) {
    $dest = $_GET['destination'];
  }

  $form = confirm_form(
    $form,
    t('Are you sure you want to unpublish this comment?'),
    $dest,
    t('Submitting this form will unpublish this comment.'),
    t('Unpublish')
  );

  return $form;
}

/**
 * Form unpublish submit actions hook.
 */
function unpublish_own_comment_form_submit($form, &$form_state) {
  $comment = comment_load($form_state['values']['cid']);
  $comment->status = COMMENT_NOT_PUBLISHED;
  comment_save($comment);

  if (!isset($_GET['destination'])) {
    $form_state['redirect'] = 'node/' . $form_state['values']['nid'];
  }
}
