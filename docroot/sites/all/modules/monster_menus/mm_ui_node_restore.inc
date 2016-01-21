<?php

/**
 * @file
 * User interface routines for restoring nodes from the recycle bin
 */

/**
 * Confirm that the user wants to restore a node from the recycle bin
 *
 * @param $mmtid
 *   MM tree ID of the page containing the node to restore
 * @param $node
 *   Node to restore
 * @param $return
 *   Optional tree ID of the entry to return the user to when done
 * @return
 *   The HTML code for the results
 */
function mm_ui_node_restore_confirm($form, &$form_state, $mmtid, $node) {
  $from = mm_content_get_parent($mmtid);
  if (!_mm_ui_recycle_page_list($node, array($from), $names, $msg, TRUE)) {
    $form['error'] = array('#markup' => $msg);
    return $form;
  }
  $form['nid'] = array('#type' => 'value', '#value' => $node->nid);
  $form['bin_mmtid'] = array('#type' => 'value', '#value' => $mmtid);
  $form['return'] = array('#type' => 'value', '#value' => $from);

  return confirm_form($form,
      t('Are you sure you want to restore this content?'),
      !empty($_GET['destination']) ? $_GET['destination'] : mm_content_get_mmtid_url($mmtid),
      $msg, t('Restore'), t('Cancel'));
}

/**
 * Execute node restoration
 */
function mm_ui_node_restore_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm'] && ($node = node_load($form_state['values']['nid'])) && $node->nid) {
    $err = mm_content_move_from_bin(NULL, $node, $form_state['values']['bin_mmtid']);
    if (is_string($err)) {
      drupal_set_message($err);
    }
    if ($form_state['values']['return']) {
      mm_redirect_to_mmtid($form_state['values']['return']);
    }
  }
}
