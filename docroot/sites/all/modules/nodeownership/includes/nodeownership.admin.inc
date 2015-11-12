<?php

/**
 * Implementation of admin setting page.
 *
 * @return
 *   array of form elements
 */
function nodeownership_admin_settings($form, &$form_state) {
  $node_types = node_type_get_names();
  $form['nodeownership_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Content types which can be claimed for ownership'),
    '#options' => $node_types,
    '#default_value' => variable_get('nodeownership_node_types', array('page')),
  );

  $form['nodeownership_link_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Claim Text'),
    '#description' => t('The text that will be shown for the claiming the node'),
    '#default_value' => variable_get('nodeownership_link_text', 'Claim'),
  );
  $form['nodeownership_pending_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Claim Pending Text'),
    '#description' => t('The text that will be shown for pending claims'),
    '#default_value' => variable_get('nodeownership_pending_text', 'Claim Pending'),
  );


  $form['email'] = array(
    '#type' => 'fieldset',
    '#title' => t('E-mail settings For Node Ownership'),
    '#description' => t('Drupal sends this email whenever a node would be claimed or any claim would be approved by admin.'),
  );
  module_load_include('inc', 'nodeownership', 'includes/nodeownership');
  $email_token_help = t('Available variables are:') . ' !username, !site, !uri, !uri_brief, !mailto, !date.';

  $form['email']['user_claim'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mail For admin, whenever a node would be claimed.'),
    '#collapsible' => TRUE,
    '#collapsed' => 1,
    '#description' => t('Customize welcome e-mail messages sent to admin.') . ' ' . $email_token_help,
  );
  $form['email']['user_claim']['user_claim_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => _nodeownership_mail_text('user_claim_subject'),
    '#maxlength' => 180,
  );
  $form['email']['user_claim']['user_claim_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => _nodeownership_mail_text('user_claim_body'),
    '#rows' => 15,
  );
  $form['email']['admin_approve_accept'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mail For user, whenever a claim would be accepted by admin.'),
    '#collapsible' => TRUE,
    '#collapsed' => 1,
    '#description' => t('Customize e-mail messages sent to user.') . ' ' . $email_token_help,
  );
  $form['email']['admin_approve_accept']['admin_approve_accept_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => _nodeownership_mail_text('admin_approve_accept_subject'),
    '#maxlength' => 180,
  );
  $form['email']['admin_approve_accept']['admin_approve_accept_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => _nodeownership_mail_text('admin_approve_accept_body'),
    '#rows' => 15,
  );
  $form['email']['admin_approve_decline'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mail For user, whenever a claim would be Declined by admin.'),
    '#collapsible' => TRUE,
    '#collapsed' => 1,
    '#description' => t('Customize e-mail messages sent to user.') . ' ' . $email_token_help,
  );
  $form['email']['admin_approve_decline']['admin_approve_decline_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => _nodeownership_mail_text('admin_approve_decline_subject'),
    '#maxlength' => 180,
  );
  $form['email']['admin_approve_decline']['admin_approve_decline_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => _nodeownership_mail_text('admin_approve_decline_body'),
    '#rows' => 15,
  );

  $form = system_settings_form($form);
  return $form;
}
