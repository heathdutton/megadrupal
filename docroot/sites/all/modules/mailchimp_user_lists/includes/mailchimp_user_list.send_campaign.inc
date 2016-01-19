<?php
/**
 * @file
 * Mailchimp user list module send campaign.
 */


/**
 * Form to verify sending campaign.
 *
 * @return array
 *   form array
 */
function mailchimp_user_list_send_campaign_page($lid, $cid) {
  $form = drupal_get_form('mailchimp_user_list_send_campaign_form', $lid, $cid);
  $page = drupal_render($form);
  return $page;
}

/**
 * Form to verify sending campaign.
 *
 * @return array
 *   form array
 */
function mailchimp_user_list_send_campaign_form($form, &$form_state, $lid, $cid) {
  $list_title = mailchimp_user_list_list_title($lid);
  $form['mailchimp_delete_information'] = array(
    '#type' => 'item',
    '#markup' => t(
      '<p><b>Are You sure you wish to send this campaign to everyone on the "@mailing_list" list?</b></p>This cannot be undone.',
      array('@mailing_list' => $list_title)
    ),
  );
  $form['mailchimp_send'] = array(
    '#type' => 'submit',
    '#value' => t('Send'),
    '#submit' => array(
      'mailchimp_user_list_send_campaign',
    ),
  );
  $form['mailchimp_cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#submit' => array('mailchimp_user_list_return_to_list'),
  );
  $form['mailchimp_lid'] = array(
    '#type' => 'hidden',
    '#value' => $lid,
  );
  $form['mailchimp_cid'] = array(
    '#type' => 'hidden',
    '#value' => $cid,
  );
  return $form;
}

/**
 * Send Campaign.
 */
function mailchimp_user_list_send_campaign($form, &$form_state) {
  $q = mailchimp_get_api_object();
  $cid = $form_state['values']['mailchimp_cid'];
  $q->campaignSendNow($cid);
  $title = mailchimp_user_list_campaign_title($cid);
  drupal_set_message(t(
    '@title has been scheduled to be mailed',
    array('@title' => $title)
    )
  );
  mailchimp_user_list_return_to_list($form_state['values']['mailchimp_lid']);
}


/**
 * Test Send Campaign to user drupal email.
 */
function mailchimp_user_list_mailing_form_test_send($lid, $cid) {
  global $user;
  $q = mailchimp_get_api_object();
  $q->campaignSendTest($cid, array($user->mail));
  if ($q->errorCode) {
    drupal_set_message(t('There was a problem sending a test email, please contact your system administrator.'));
    watchdog('mailchimp',
      'Could not send a test email to user @user on Campaign ID @campaign_id  error: @error',
      array(
        '@user' => $user->name,
        '@campaign_id' => $cid,
        '@error' => $q->errorMessage,
      ),
      WATCHDOG_ERROR
    );
  }
  else {
    drupal_set_message(t('A test email has been sent to @user_email', array('@user_email' => $user->mail)));
  }
  mailchimp_user_list_return_to_list($lid);
}
