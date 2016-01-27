<?php

/**
 * @file
 * Mailchimp user list module edit campaign form.
 */


/**
 * Wrapper for campaign form.
 *
 * @return string
 *   A table of users with user ID, name, and lists.
 */
function mailchimp_user_list_edit_campaign($lid, $cid) {
  $form = drupal_get_form('mailchimp_user_list_edit_campaign_form', $lid, $cid);
  $page = drupal_render($form);
  return $page;
}


/**
 * Creates a form to edit an existing or create a new campaign.
 *
 * @return string
 *   A table of users with user ID, name, and lists.
 */
function mailchimp_user_list_edit_campaign_form($form, &$form_state, $lid, $cid = FALSE) {
  $templates = mailchimp_user_list_retrieve_users_templates();;
  if ($cid) {
    $previous_entry = mailchimp_user_list_retrieve_campaigns($cid);
    $sendstatus = $previous_entry['data'][0]['status'];
  }
  else {
    $sendstatus = 'new';
  }
  if ($sendstatus != 'sent') {
    $groups = mailchimp_user_list_retrieve_list_groups($lid);
    $mergetags = mailchimp_user_list_listemergevars($lid);
    $form['mailchimp_lid'] = array(
      '#title' => t('Assigned to Mailing List'),
      '#type' => 'hidden',
      '#value' => $lid,
      '#weight' => 1,
    );
    if ($groups) {
      $form['mailchimp_segment'] = array(
        '#title' => t('Group'),
        '#type' => 'checkboxes',
        '#required' => FALSE,
        '#weight' => 2,
        '#options' => $groups,
        '#description' => t('If no boxes are checked everyone on the list will receive this email'),
      );
    }
    $form['mailchimp_from_email'] = array(
      '#title' => t('From Email'),
      '#type' => 'textfield',
      '#required' => TRUE,
      '#size' => 60,
      '#weight' => 3,
    );
    $form['mailchimp_from_name'] = array(
      '#title' => t('From Name'),
      '#type' => 'textfield',
      '#required' => TRUE,
      '#size' => 60,
      '#weight' => 4,
    );
    $form['mailchimp_subject'] = array(
      '#title' => t('Subject'),
      '#type' => 'textfield',
      '#required' => TRUE,
      '#size' => 60,
      '#weight' => 5,
    );
    $form['mailchimp_tag_instructions'] = array(
      '#markup' => t('<p>Use tags to load user data from the mailing list, available tags are:') . $mergetags,
      '#weight' => 6,
    );
    $form['mailchimp_content'] = array(
      '#title' => t('HTML Content'),
      '#type' => 'text_format',
      '#required' => FALSE,
      '#format' => 'full_html',
      '#rows' => '20',
      '#weight' => 7,
    );

    $form['mailchimp_save'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#submit' => array('mailchimp_user_list_campaign_save'),
      '#weight' => 8,
    );

    $form['mailchimp_cid'] = array(
      '#type' => 'hidden',
      '#value' => $cid,
    );
    $form['mailchimp_campaign_status'] = array(
      '#type' => 'hidden',
    );
    // Check to see if they have a campaign id, if so, then this is an update.
    if ($sendstatus != 'new') {
      $form['mailchimp_campaign_status']['#value'] = 'edit';
      $previous_content = mailchimp_user_list_retrieve_campaign_template_content($cid);
      if (isset($previous_entry['data'][0]['segment_opts']['conditions'])) {
        $groups_selected = mailchimp_user_list_retrieve_list_group_names($previous_entry['data'][0]['segment_opts']);
        $form['mailchimp_segment']['#default_value'] = $groups_selected;
      }
      $form['mailchimp_lid']['#value'] = $previous_entry['data'][0]['list_id'];
      $form['mailchimp_from_email']['#default_value'] = $previous_entry['data'][0]['from_email'];
      $form['mailchimp_from_name']['#default_value'] = $previous_entry['data'][0]['from_name'];
      $form['mailchimp_subject']['#default_value'] = $previous_entry['data'][0]['subject'];
      $form['mailchimp_content']['#default_value'] = $previous_content;
      $form['mailchimp_test'] = array(
        '#type' => 'submit',
        '#value' => t('Test'),
        '#submit' => array('mailchimp_user_list_test_send_wrapper'),
        '#weight' => 9,
      );
      $form['mailchimp_preview'] = array(
        '#markup' => t('<p><label>Preview</label></p>') . $previous_entry['content']['html'],
        '#weight' => 50,
      );
    }
    // This is a new campaign.
    else {
      $templates = mailchimp_user_list_retrieve_users_templates();
      if (count($templates) > 0) {
        $form['mailchimp_campaign_status']['#value'] = 'new';
        $form['mailchimp_template'] = array(
          '#title' => t('Use email template'),
          '#type' => 'select',
          '#required' => TRUE,
          '#options' => mailchimp_user_list_retrieve_users_templates(),
          '#weight' => 2,
        );
      }
      else {
        drupal_set_message('You do not have ant templates available, you cannot create any campaigns.',
          'warning',
          FALSE);
        drupal_goto('admin/mailchimp/user_lists/my_mailing_lists');
      }
    }
  }
  else {
    $form['mailchimp_preview'] = array(
      '#markup' => '<p><label>Previously sent email</label></p>' . $previous_entry['content']['html'],
      '#weight' => 50,
    );
  }
  return $form;
}

/**
 * Saves the campaign to mailchimp.
 *
 * @param array $form
 *   form
 * @param array $form_state
 *   form state
 */
function mailchimp_user_list_campaign_save($form, &$form_state) {
  $q = mailchimp_get_api_object();
  $type = 'regular';
  $options = array();
  $content = array();
  $options['list_id'] = $form_state['values']['mailchimp_lid'];
  $options['subject'] = $form_state['values']['mailchimp_subject'];
  $options['title'] = $form_state['values']['mailchimp_subject'];
  $options['from_email'] = $form_state['values']['mailchimp_from_email'];
  $options['from_name'] = $form_state['values']['mailchimp_from_name'];
  $content['html_std_content00'] = $form_state['values']['mailchimp_content']['value'];
  $filters = array();
  foreach ($form_state['values']['mailchimp_segment'] as $key => $value) {
    if ($value != 0) {
      $value_explode = explode('|', $key);
      $filters['interests-' . $value_explode[0]][] = $value_explode[1];
    }
  }
  $conditions = array();
  foreach ($filters as $key => $value) {
    $conditions[] = array('field' => $key, 'op' => 'all', 'value' => $value);
  }
  $options['segment_opts'] = array('match' => 'any', 'conditions' => $conditions);
  if ($form_state['values']['mailchimp_campaign_status'] == 'edit') {
    foreach ($options as $key => $value) {
      $q->campaignUpdate($form_state['values']['mailchimp_cid'], $key, $value);
    }
    $q->campaignUpdate($form_state['values']['mailchimp_cid'], 'content', $content);
    if ($q->errorCode) {
      drupal_set_message(t('There was a problem saving your
        changes please contact your system administrator.'));
      watchdog('mailchimp',
        'Could not update campaign @title id number @id error @error',
        array(
          '@title' => $form_state['values']['mailchimp_subject'],
          '@id' => $form_state['values']['mailchimp_cid'],
          '@error' => $q->errorMessage),
          WATCHDOG_ERROR
      );
    }
    else {
      drupal_set_message(t('Your campaign has been updated'));
    }
    mailchimp_user_list_return_to_list($form_state['values']['mailchimp_lid']);
  }
  else {
    $options['template_id'] = $form_state['values']['mailchimp_template'];
    $q->campaignCreate($type, $options, $content);
    if ($q->errorCode) {
      drupal_set_message(t('There was a problem creating your campaign
        please contact your system administrator'));
      watchdog('mailchimp',
        'Could not create campaign error message @error',
        array('@error' => $q->errorMessage),
        WATCHDOG_ERROR
      );
    }
    else {
      drupal_set_message(t('You have created your campaign'));
      mailchimp_user_list_return_to_list($form_state['values']['mailchimp_lid']);
    }
  }
}
