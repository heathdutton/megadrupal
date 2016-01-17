<?php
/**
 * @file
 * Mailchimp delete campaign.
 */

/**
 * Form Wrapper.
 *
 * @return array
 *   form elements.
 */
function mailchimp_user_list_delete_campaign($lid, $cid) {
  $form = drupal_get_form('mailchimp_user_list_delete_campaign_form', $lid, $cid);
  $page = drupal_render($form);
  return $page;
}

/**
 * Creates a form to verify node deletion.
 *
 * @return array
 *   form elements.
 */
function mailchimp_user_list_delete_campaign_form($form, &$form_state, $lid, $cid) {
  $form['mailchimp_delete_information'] = array(
    '#type' => 'item',
    '#markup' => t('<p><b>Are You sure you wish to delete this campaign?</b></p>This cannot be undone.'),
  );
  $form['mailchimp_delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#submit' => array('mailchimp_user_list_delete_campaign_delete'),
  );
  $form['mailchimp_cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#submit' => array('mailchimp_user_list_delete_campaign_delete'),
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
 * Delete a campaign.
 *
 * @param array $form
 *   form
 * @param array $form_state
 *   form state
 */
function mailchimp_user_list_delete_campaign_delete($form, &$form_state) {
  if ($form_state['values']['op'] == 'Cancel') {
    mailchimp_user_list_return_to_list($form_state['values']['mailchimp_lid']);
  }
  $q = mailchimp_get_api_object();
  $q->campaignDelete($form_state['values']['mailchimp_cid']);
  if ($q->errorCode) {
    drupal_set_message(t('There was a problem deleting this campaign.'));
    watchdog('mailchimp',
      'Could not delete campaign @campaign_name ID number @campaign_id, error @error',
      array(
        '@campaign_name' => mailchimp_user_list_campaign_title($form_state['values']['mailchimp_cid']),
        '@campaign_id' => $form_state['values']['mailchimp_cid'],
        '@error' => $q->errorMessage,
      ),
      WATCHDOG_ERROR
    );
  }
  else {
    drupal_set_message(t('Your campaign has successfully been deleted.'));
    mailchimp_user_list_return_to_list($form_state['values']['mailchimp_lid']);
  }
}
