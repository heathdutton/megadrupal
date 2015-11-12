<?php

/**
 * @file
 * UI pages for revisions, copy-paste from node.pages.inc.
 */

/**
 * Generate an overview table of older revisions of a user.
 */
function user_revision_overview($viewed_user) {
  global $user;
  drupal_set_title(t('Revisions for %title', array('%title' => $viewed_user->name)), PASS_THROUGH);

  $header = array(t('Revision'), array('data' => t('Operations'), 'colspan' => 2));

  $revisions = user_revision_list($viewed_user);
  $rows = array();
  $revert_permission = FALSE;
  $delete_permission = FALSE;
  if($user->uid == $viewed_user->uid){
    if (user_access('revert own user revisions') || user_access('revert user revisions')) {
      $revert_permission = TRUE;
    }
    if (user_access('delete own user revisions') || user_access('delete user revisions')) {
      $delete_permission = TRUE;
    }
  }
  else {
    if (user_access('revert user revisions')) {
      $revert_permission = TRUE;
    }
    if (user_access('delete user revisions')) {
      $delete_permission = TRUE;
    }
  }

  foreach ($revisions as $revision) {
    $row = array();
    $operations = array();
    if ($revision->current_vid > 0) {
      $row[] = array('data' => t('!date by !username', array('!date' => l(format_date($revision->timestamp, 'short'), "user/$viewed_user->uid"), '!username' => theme('username', array('account' => $revision))))
                               . (($revision->log != '') ? '<p class="revision-log">' . filter_xss($revision->log) . '</p>' : ''),
                     'class' => array('revision-current'));
      $operations[] = array('data' => drupal_placeholder(t('current revision')), 'class' => array('revision-current'), 'colspan' => 2);
    }
    else {
      $row[] = t('!date by !username', array('!date' => l(format_date($revision->timestamp, 'short'), "user/$viewed_user->uid/revisions/$revision->vid/view"), '!username' => theme('username', array('account' => $revision))))
               . (($revision->log != '') ? '<p class="revision-log">' . filter_xss($revision->log) . '</p>' : '');
      if ($revert_permission) {
        $operations[] = l(t('revert'), "user/$viewed_user->uid/revisions/$revision->vid/revert");
      }
      else {
        //empty row for theming purposes
        $operations[] = "";
      }
      if ($delete_permission) {
        $operations[] = l(t('delete'), "user/$viewed_user->uid/revisions/$revision->vid/delete");
      }
      else {
        //empty row for theming purposes
        $operations[] = "";
      }
    }
    $rows[] = array_merge($row, $operations);
  }

  $build['user_revisions_table'] = array(
    '#theme' => 'table',
    '#rows' => $rows,
    '#header' => $header,
  );

  return $build;
}

/**
 * Revert a revision.
 */
function user_revision_revert_confirm($form, $form_state, $user_revision) {
  $form['#user_revision'] = $user_revision;
  return confirm_form($form, t('Are you sure you want to revert to the revision from %revision-date?', array('%revision-date' => format_date($user_revision->revision_timestamp))), 'user/' . $user_revision->uid . '/revisions', '', t('Revert'), t('Cancel'));
}

function user_revision_revert_confirm_submit($form, &$form_state) {
  $user_revision = $form['#user_revision'];
  $user_revision->revision = 1;
  $user_revision->log = t('Copy of the revision from %date.', array('%date' => format_date($user_revision->revision_timestamp)));
  user_save($user_revision, (array)$user_revision);
  watchdog('user', 'reverted %title revision %revision.', array('%title' => $user_revision->name, '%revision' => $user_revision->vid));
  drupal_set_message(t('%title has been reverted back to the revision from %revision-date.', array('%title' => $user_revision->name, '%revision-date' => format_date($user_revision->revision_timestamp))));
  $form_state['redirect'] = 'user/' . $user_revision->uid . '/revisions';
}

/**
 * Delete a revision.
 */
function user_revision_delete_confirm($form, $form_state, $user_revision) {
  $form['#user_revision'] = $user_revision;
  return confirm_form($form, t('Are you sure you want to delete the revision from %revision-date?', array('%revision-date' => format_date($user_revision->revision_timestamp))), 'user/' . $user_revision->uid . '/revisions', t('This action cannot be undone.'), t('Delete'), t('Cancel'));
}

function user_revision_delete_confirm_submit($form, &$form_state) {
  $user_revision = $form['#user_revision'];
  user_revision_delete($user_revision);

  watchdog('user', 'deleted %title revision %revision.', array('%title' => $user_revision->name, '%revision' => $user_revision->vid));
  drupal_set_message(t('Revision from %revision-date %title has been deleted.', array('%revision-date' => format_date($user_revision->revision_timestamp), '%title' => $user_revision->name)));
  $form_state['redirect'] = 'user/' . $user_revision->uid;
  if (db_select('user_revision', 'ur')->condition('ur.uid', $user_revision->uid)->countQuery()->execute()->fetchField() > 1) {
    $form_state['redirect'] .= '/revisions';
  }
}
