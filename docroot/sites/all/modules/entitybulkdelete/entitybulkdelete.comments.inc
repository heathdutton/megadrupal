<?php

/**
 * @file
 * Bulk deletion of comments.
 */

/**
 * Form builder; Return form for deletion of comments.
 */
function entitybulkdelete_comments($form, &$form_state) {
  // Display page 2 if $form_state['page_num'] == 2
  if (!empty($form_state['page_num']) && $form_state['page_num'] == 2) {
    return entitybulkdelete_comments_confirm_form($form, $form_state);
  }
  $form_state['page_num'] = 1;
  $form['cid'] = array(
    '#title' => 'Comments cid',
    '#type' => 'textarea',
    '#rows' => 6,
    '#cols' => 40,
    '#description' => t("Enter comments's cid separate by ','."),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Delete',
    '#submit' => array('entitybulkdelete_comments_submit'),
  );
  return $form;
}

/**
 * Submit handler for the comments deletion form.
 */
function entitybulkdelete_comments_submit($form, &$form_state) {
  $find = array(' ', '-', '+');
  $selcids = check_plain($form_state['values']['cid']);
  if (!empty($selcids)) {
    $selcids = str_replace($find, '', $selcids);
    $selcids_array = explode(',', $selcids);
    if (!empty($selcids_array)) {
      foreach ($selcids_array as $key => $value) {
        if (!is_numeric($value)) {
          unset($selcids_array[$key]);
        }
        else {
          $temp_cid = (int) $value;
          $cids[$temp_cid] = $temp_cid;
        }
      }
      // Get list of comments's cid and comments.
      $query = db_select('comment', 'c');
      $query->fields('c', array('cid', 'subject'));
      $query->condition('cid', $cids, 'IN');
      $comments_list = $query->execute()->fetchAllKeyed(0, 1);
      foreach ($comments_list as $key => $value) {
        $valid_rows[$key] = array('cid' => $key, 'comments' => check_plain($value));
      }
    }
  }
  if (!empty($valid_rows)) {
    $form_state['valid_rows'] = $valid_rows;
    $form_state['page_num'] = 2;
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Form builder; Return confirmation form for comments deletion.
 */
function entitybulkdelete_comments_confirm_form($form, &$form_state) {
  $header['cid'] = 'Cid';
  $header['comments'] = 'Comments Subject';
  $valid_row = $form_state['valid_rows'];
  $form['msg'] = array(
    '#type' => 'markup',
    '#markup' => t('Are you sure you want to deletes these comments ?') .
    '<br>' . t('This action cannot be undone.') . '<br>',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Confirm',
    '#submit' => array('entitybulkdelete_comments_confirm_form_submit'),
  );
  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => 'Cancel',
    '#submit' => array('entitybulkdelete_comments_cancel_form_submit'),
  );
  $form['table'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $valid_row,
    '#empty' => t('No Valid Row(s).'),
  );
  return $form;
}

/**
 * Submit handler(Cancel) of confirmation form for comments deletion.
 */
function entitybulkdelete_comments_cancel_form_submit($form, &$form_state) {
  $form_state['page_num'] = 1;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler(Confirm) of confirmation form for comments deletion.
 */
function entitybulkdelete_comments_confirm_form_submit($form, &$form_state) {
  $table = $form_state['values']['table'];
  $table = array_flip($table);
  if (array_key_exists('0', $table)) {
    unset($table[0]);
  }
  if (!empty($table)) {
    $count = count($table);
    $operations = array();
    for ($i = 0; $i < $count; $i = $i + 5) {
      // Set up an operations array with 5 elements, each doing function
      // entitybulkdelete_comments_delete_batch.
      $batch_tbl = array_slice($table, $i, 5);
      if (!empty($batch_tbl)) {
        $operations[] = array('entitybulkdelete_comments_delete_batch',
          array($batch_tbl, &$form_state));
      }
    }
    // Execute the batch.
    $batch = array(
      'operations' => $operations,
      'title' => t('Comments Delete Batch'),
      'init_message' => t('Comments(s) deleting is in process...'),
      'progress_message' => t('Processed @current*5 out of @total*5.'),
      'finished' => 'entitybulkdelete_comments_delete_batch_finished',
      'file' => drupal_get_path('module', 'entitybulkdelete') . '/entitybulkdelete.comments.inc',
    );
    batch_set($batch);
  }
  $form_state['page_num'] = 1;
  $form_state['rebuild'] = TRUE;
}

/**
 * Batch operation entitybulkdelete_comments_delete_batch.
 *
 * Delete comments, 5 by five after each group of 5 control
 * is returned to the batch API for later continuation.
 */
function entitybulkdelete_comments_delete_batch($table, $form_state, &$context) {
  if (!empty($table)) {
    foreach ($table as $value) {
      $cids[$value] = $value;
      watchdog('entitybulkdelete', 'Comments %cid marked as deleted.', array('%cid' => $value), WATCHDOG_NOTICE);
    }
    // Deletes comments.
    comment_delete_multiple($cids);
  }
}

/**
 * Batch 'finished' callback used by entitybulkdelete_comments_delete_batch.
 */
function entitybulkdelete_comments_delete_batch_finished($success, $results, $operations) {
  if ($success) {
    $msg = t('Comments(s) %success has been deleted.');
    watchdog('entitybulkdelete', $msg, array('%success' => 'successfuly'), WATCHDOG_NOTICE);
  }
  else {
    $error_operation = reset($operations);
    drupal_set_message(t('An error occurred while processing
     @operation with arguments : @args', array(
       '@operation' => $error_operation[0],
       '@args' => print_r($error_operation[0], TRUE))));
  }
}
