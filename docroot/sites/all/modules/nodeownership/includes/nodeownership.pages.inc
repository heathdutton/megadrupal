<?php

/**
 * Implementation of logic for user claim.
 *
 * @param object $node
 *   Claimed node.
 *
 * @return array
 */
function nodeownership_get_claim($node) {
  global $user;
  $result_claimed  = "SELECT uid FROM {nodeownership} WHERE nid = :nid AND status = 1";
  $result_exist    = "SELECT nid FROM {nodeownership} WHERE nid = :nid AND uid = :uid AND status = 0";
  $result_declined = "SELECT nid FROM {nodeownership} WHERE uid = :uid AND nid= :nid AND status = 2";

  if (db_query($result_claimed, array(':nid' => $node->nid))->fetchField()) {
    drupal_set_message(t('Someone else has claimed this node.'));
    drupal_goto('node/' . $node->nid);
  }
  elseif (db_query($result_exist, array(':nid' => $node->nid, ':uid' => $user->uid))->fetchField()) {
    drupal_set_message(t('Your claim request is being reviewed.'));
    drupal_goto('node/' . $node->nid);
  }
  elseif (db_query($result_declined, array(':nid' => $node->nid, ':uid' => $user->uid))->fetchField()) {
    drupal_set_message(t('Your claim request for this node has already been declined.'));
    drupal_goto('node/' . $node->nid);
  }
  else {
    $content = drupal_get_form('nodeownership_claim_form', $node->nid);
    return $content;
  }
}

/**
 * Implementation of the use claim form.
 *
 * @param $form_state
 *   The form state
 * @param $nid
 *   Node Id of the claimed node.
 *
 * @return array
 *   return form element.
 */
function nodeownership_claim_form($form, &$form_state, $nid) {
  $form          = array();
  $node_title    = db_query("SELECT title FROM {node} WHERE nid = :nid", array(':nid' => $nid))->fetchField();
  $form['claim'] = array(
    '#type' => 'fieldset',
    '#title' => t('Claim Details'),
    '#value' => '<br/>' . t('NOTE: False claims may lead to termination of your account on our site.'),
    '#weight' => -9,
  );
  $form['claim']['nid'] = array(
    '#type' => 'value',
    '#value' => $nid,
  );
  $form['claim']['contact'] = array(
    '#type' => 'textarea',
    '#title' => t('Contact information'),
    '#description' => t('Please provide your contact details here so that we can contact you and verify your identity'),
    '#required' => TRUE,
  );
  $form['claim']['notes'] = array(
    '#type' => 'textarea',
    '#title' => t('Additional Details'),
    '#description' => t('Any other details that you might want to provide which may help us verify your identity.'),
  );
  $form['claim']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit Claim'),
  );
  return $form;
}

/**
 * Implementation for user claim submission.
 *
 * @param $form
 *   form filled by user
 * @param $form_state
 *   state of the form submitted.
 */
function nodeownership_claim_form_submit($form, &$form_state) {
  global $user;

  $claim = (object) array(
    'uid' => $user->uid,
    'nid' => $form_state['values']['nid'],
    'contact' => $form_state['values']['contact'],
    'notes' => $form_state['values']['notes'],
    'status' => 0,
    'timestamp' => REQUEST_TIME,
  );
  entity_save('claim', $claim);

  return drupal_goto('node/' . $form_state['values']['nid']);
}

/*
 * Implementation of logic for claim confirmation by admin(approve/reject).
 *
 * @param $type
 *   confirmation type approve/decline.
 * @param $nid
 *   nid of the node.
 * @param $uid
 *   uid of the claimed user.
 */
function _node_claim_process($type, $id) {
  module_load_include('inc', 'nodeownership', 'includes/nodeownership');
  $claim = entity_load_single('claim', $id);

  if ($type == 'approve') {
    nodeownership_claim_approve($claim);
  }
  elseif ($type == 'decline') {
    nodeownership_claim_decline($claim);
  }
  else {
    drupal_set_message(t("Invalid Url!!"));
    drupal_goto('admin/content/claims');
  }
}
