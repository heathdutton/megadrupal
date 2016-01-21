<?php

/**
 * @file
 * TODO.
 */

/**
 * Change a membership status.
 */
function gcc_operation_change_status($entity_type, $entity_id, $uid, $params, $config) {

  switch ($config['status']) {

    case GCC_BLOCKED:
      gcc_membership_block($entity_type, $entity_id, $uid);
      break;

    case GCC_ACTIVE:
      gcc_membership_activate($entity_type, $entity_id, $uid);
      break;
  }

  return TRUE;
}

/**
 * Config form for the change status op.
 */
function gcc_operation_change_status_form($entity_type, $entity_id) {

  $form = array();

  $form['status'] = array(

    '#title' => t('New status'),
    '#type' => 'select',
    '#options' => array(

      GCC_ACTIVE => t('Active'),
      GCC_BLOCKED => t('Blocked'),
    ),
    '#required' => TRUE,
    '#default_value' => GCC_ACTIVE,
  );

  return $form;
}

/**
 * Delete a membership.
 */
function gcc_operation_delete($entity_type, $entity_id, $uid, $params) {

  gcc_membership_delete($entity_type, $entity_id, $uid);
  return TRUE;
}

/**
 * Config form for the delete op.
 */
function gcc_operation_delete_form($entity_type, $entity_id) {

  $form = array();

  $form['status'] = array(

    '#markup' => t('You are about to remove members from your group. Are you sure you want to continue ?'),
  );

  return $form;
}

/**
 * Change a membership status.
 */
function gcc_operation_email($entity_type, $entity_id, $uid, $params, $config) {

  $user = user_load($uid);

  if ($user && isset($user->mail)) {
    drupal_mail('gcc', 'operation_email', $user->mail, $GLOBALS['language'], $config);
  }

  return TRUE;
}

/**
 * Config form for the change status op.
 */
function gcc_operation_email_form($entity_type, $entity_id) {

  $form = array();

  $form['subject'] = array(

    '#title' => t('Subject'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );

  $form['body'] = array(

    '#title' => t('Body'),
    '#type' => 'textarea',
    '#required' => TRUE,
  );

  return $form;
}

/**
 * Add a role.
 */
function gcc_operation_add_role($entity_type, $entity_id, $uid, $params) {

  gcc_membership_add_role($entity_type, $entity_id, $uid, $params[0]);
  return TRUE;
}

/**
 * Remove a role.
 */
function gcc_operation_remove_role($entity_type, $entity_id, $uid, $params) {

  gcc_membership_remove_role($entity_type, $entity_id, $uid, $params[0]);
  return TRUE;
}
