<?php

/**
 * @file
 * Admin page callbacks for the anon_comment module. Largely copied from comment module's comment.admin.inc
 */


//get the functions we need from the main comment module
require_once DRUPAL_ROOT . '/' .  drupal_get_path('module', 'comment') . "/comment.admin.inc";

/**
 * Menu callback; present an administrative comment listing.
 */
function anon_comment_admin($type = 'anon') {
  $edit = $_POST;

  if (isset($edit['operation']) && ($edit['operation'] == 'delete') && isset($edit['comments']) && $edit['comments']) {
    return drupal_get_form('comment_multiple_delete_confirm');
  }
  else {
    return drupal_get_form('anon_comment_admin_overview');
  }
}


/**
 * Form builder; Builds the comment overview form for the admin.
 *
 * @return
 *   The form structure.
 * @ingroup forms
 * @see comment_admin_overview_validate()
 * @see comment_admin_overview_submit()
 * @see theme_comment_admin_overview()
 */
function anon_comment_admin_overview($form, &$form_state) {

  // Build an 'Update options' form.
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update options'),
    '#attributes' => array('class' => array('container-inline')),
  );
  $options = array(
    'unpublish' => t('Unpublish the selected comments'),
    'modify' => t('De-anonymize the selected comments'),
    'delete' => t('Delete the selected comments'),
  );
  $form['options']['operation'] = array(
    '#type' => 'select',
    '#title' => t('Operation'),
    '#title_display' => 'invisible',
    '#options' => $options,
    '#default_value' => 'publish',
  );
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );


  // Load the comments that need to be displayed.
  $status = COMMENT_PUBLISHED;
  $header = array(
    'subject' => array('data' => t('Subject'), 'field' => 'subject'),
    'author' => array('data' => t('Actual Author'), 'field' => 'name'),
    'posted_in' => array('data' => t('Posted in'), 'field' => 'node_title'),
    'changed' => array('data' => t('Updated'), 'field' => 'c.changed', 'sort' => 'desc'),
    'operations' => array('data' => t('Operations')),
  );

  $query = db_select('comment', 'c')->extend('PagerDefault')->extend('TableSort');
  $query->join('node', 'n', 'n.nid = c.nid');
  $query->addField('n', 'title', 'node_title');
  $query->join('anon_comment_authors', 'a', 'c.cid = a.cid');
  $query->join('users', 'u', 'u.uid = a.uid');
  $query->addField('u', 'uid', 'user_id');
  $query->addTag('node_access');
  $result = $query
    ->fields('c', array('cid', 'subject', 'name', 'changed'))
    ->condition('c.status', $status)
    ->condition('c.uid', '0')
    ->limit(50)
    ->orderByHeader($header)
    ->execute();

  $cids = array();

  // We collect a sorted list of node_titles during the query to attach to the
  // comments later.
  foreach ($result as $row) {
    $cids[] = $row->cid;
    $node_titles[] = $row->node_title;
    $user_ids[] = $row->user_id;
  }
  $comments = comment_load_multiple($cids);

  // Build a table listing the appropriate comments.
  $options = array();
  $destination = drupal_get_destination();

  foreach ($comments as $comment) {
    // Remove the first node title from the node_titles array and attach to
    // the comment.
    $comment->node_title = array_shift($node_titles);
    $author = user_load(array_shift($user_ids));
    $options[$comment->cid] = array(
      'subject' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => $comment->subject,
          '#href' => 'comment/' . $comment->cid,
          '#options' => array('attributes' => array('title' => truncate_utf8($comment->comment_body[LANGUAGE_NONE][0]['value'], 128)), 'fragment' => 'comment-' . $comment->cid),
        ),
      ),
      'author' => theme('username', array('account' => $author)), //change me!
      'posted_in' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => $comment->node_title,
          '#href' => 'node/' . $comment->nid,
        ),
      ),
      'changed' => format_date($comment->changed, 'short'),
      'operations' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => t('edit'),
          '#href' => 'comment/' . $comment->cid . '/edit',
          '#options' => array('query' => $destination),
        ),
      ),
    );
  }

  $form['comments'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No published, anonymized comments currently exist.'),
  );

  $form['pager'] = array('#theme' => 'pager');

  return $form;
}


/**
 * Validate anon_comment_admin_overview form submissions.
 */
function anon_comment_admin_overview_validate($form, &$form_state) {
  $form_state['values']['comments'] = array_diff($form_state['values']['comments'], array(0));
  // We can't execute any 'Update options' if no comments were selected.
  if (count($form_state['values']['comments']) == 0) {
    form_set_error('', t('Please select one or more anonymized comments to perform the update on.'));
  }
}

/**
 * Process anon_comment_admin_overview form submissions.
 *
 * Execute the chosen 'Update option' on the selected comments, such as
 * publishing, unpublishing or deleting. Would be nice to add function to make public....
 */
function anon_comment_admin_overview_submit($form, &$form_state) {
  //$operations = comment_operations();

  if (!empty($form_state['values']['operation'])) {
    $operation = $form_state['values']['operation'];
    $cids = $form_state['values']['comments'];

    //perform the operation
    switch ($operation) {
      case "unpublish":
        foreach ($form_state['values']['comments'] as $cid => $value) {
          $comment = comment_load($value);
          $comment->status = COMMENT_NOT_PUBLISHED;
          comment_save($comment);
        }
        break;
      case "modify":
        /* we're de-anonymizing, which is trickier
           but we choose to leave the entry in anon_comment_authors so there's a record of the
           comment having been initally posted anonymously (just in case) */
        foreach ($form_state['values']['comments'] as $cid => $value) {
          if ($value) {
            db_query('UPDATE {comment} c SET
                        c.uid = (SELECT aca.uid FROM {anon_comment_authors} aca WHERE aca.cid = c.cid),
                        c.name = (SELECT u.name FROM {users} u
                            INNER JOIN {anon_comment_authors} aca ON aca.uid = u.uid WHERE aca.cid = c.cid)
                        WHERE EXISTS (SELECT aca.uid FROM {anon_comment_authors} aca WHERE aca.cid = c.cid)
                        AND EXISTS (SELECT u.name FROM {users} u
                            INNER JOIN {anon_comment_authors} aca ON aca.uid = u.uid WHERE aca.cid = c.cid)
                        AND c.cid = :cid', array(":cid" => $cid));
          }
        }
        break;
      case "delete":
        comment_delete_multiple($cids);
        drupal_set_message(t('The selected comment(s) have been deleted.'));
        cache_clear_all();
        return;
        break;
    }
    drupal_set_message(t('The update has been performed.'));
    cache_clear_all();
  }
}