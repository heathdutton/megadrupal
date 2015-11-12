<?php

/**
 * @file
 * Form builder, submit, validate and operation functions for managing
 * complaints about comments.
 */

/**
 * Menu callback; administration of listing complaints on comments.
 */
function comment_abuse_admin() {
  return views_embed_view('comment_abuse', 'block_1');
}

function comment_abuse_comment_view_complaints($cid) {
  if ($comment = comment_load($cid)) {
    // Prepare comment.
    $comment_view = comment_view($comment, node_load($comment->nid));
    unset($comment_view['links']);
    $comment_view = render($comment_view);
    // Prepare complaints.
    $query = db_select('comment_abuse', 'ca');
    $query->fields('ca', array('aid', 'uid', 'reason', 'message', 'timestamp', 'ip'));
    $query->fields('u', array('name'));
    $query->leftJoin('users', 'u', 'ca.uid = u.uid');
    $query->condition('ca.cid', $cid);
    $result = $query->execute()->fetchAllAssoc('aid');

    $complaints = array();
    $reasons = comment_abuse_get_complaint_reasons();

    foreach ($result as $item) {
      $complaint = array();
      if (!empty($item->name)) {
        $complaint[] = l($item->name, "user/$item->uid");
      }
      else {
        $complaint[] = t('Annonymous (@ip)', array('@ip' => long2ip($item->ip)));
      }
      $complaint[] = format_date($item->timestamp, 'medium');
      $complaint[] = $reasons[$item->reason];
      $complaint[] = check_plain($item->message);
      $complaints[] = $complaint;
    }

    $header = array(t('Username'), 'Created', 'Reason', 'Message');
    $complaints_table = theme('table', array('header' => $header, 'rows' => $complaints));

    return theme('comment_abuse_comment', array('comment' => $comment_view, 'complaints' => $complaints_table));
  }
}

/**
 * Confirmation form for remove complaints.
 */
function comment_abuse_comment_remove_complaints($form, &$form_state, $cid) {

  $desc = t('All complaints for comment #@cid will be removed.', array('@cid' => $cid));
  $form = array();

  $form['#cid'] = $cid;

  return confirm_form($form,
    t('Are you sure?'),
    drupal_get_destination(),
    $desc,
    t('Continue'),
    t('Cancel')
  );
}

/**
 * Submit handler for remove complaints.
 */
function comment_abuse_comment_remove_complaints_submit($form, $form_state) {
  if (!empty($form['#cid'])) {
    db_delete('comment_abuse')->condition('cid', $form['#cid'])->execute();
  }
}
