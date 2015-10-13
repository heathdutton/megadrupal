<?php

/**
 * Manage Campaign Attributes in drupal.
 */
function vr_campaign_attributes_form($form, $form_state, $node) {
  $campaign = vr_campaign_retrieve_campaign($node);
  if ($campaign->display_status != 'active') {
    $message = t('This status of this email campaign is %status. Therefore, changes
    made to this content will not be passed to VerticalResponse.',
    array(
      '%status' => ucfirst($campaign->display_status),
    )
    );
    drupal_set_message($message);
    return array();
  }
  $form['campaign'] = array(
    '#type' => 'value',
    '#value' => $campaign,
  );
  $form['node'] = array(
    '#type' => 'value',
    '#value' => $node,
  );
  $form['campaign_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Campaign Name'),
    '#description' => t('Enter a <em>unique</em> name for this email campaign.'),
    '#default_value' => $campaign->name,
    '#required' => TRUE,
  );
  $form['from_label'] = array(
    '#type' => 'textfield',
    '#title' => t('From Label'),
    '#description' => t('This value will be displayed in the newsletter "from" field.'),
    '#default_value' => $campaign->from_label,
    '#required' => TRUE,
  );
  $form['support_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Support email'),
    '#default_value' => $campaign->support_email,
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['submit_cont'] = array(
    '#type' => 'submit',
    '#value' => t('Save and continue'),
  );
  return $form;
}

/**
 * Send updated campaign information to VerticalResponse
 */
function vr_campaign_attributes_form_submit($form, &$form_state) {
  $campaign = $form_state['values']['campaign'];
  $attributes = array(
    'name' => $form_state['values']['campaign_name'],
    'support_email' => $form_state['values']['support_email'],
    'from_label' => $form_state['values']['from_label'],
  );
  vr_campaign_set_campaign_attributes($campaign->id, $attributes);
  if ($form_state['values']['op'] == 'Save and continue') {
    $node = $form_state['values']['node'];
    $form_state['redirect'] = 'node/' . $node->nid . '/enews-export/vr-content';
  }
  drupal_set_message('The campaign attribute have been saved.');
}

/**
 * Manage Campaign Content in drupal.
 */
function vr_campaign_content_form($form, $form_state, $node) {
  $campaign = vr_campaign_retrieve_campaign($node);
  if ($campaign->display_status != 'active') {
    $message = t('This status of this email campaign is %status. Therefore, changes
    made to this content will not be passed to VerticalResponse.',
    array(
      '%status' => ucfirst($campaign->display_status),
    )
    );
    drupal_set_message($message);
    return array();
  }
  $content = _vr_campaign_retrieve_campaign_content($campaign);
  $form['campaign'] = array(
    '#type' => 'value',
    '#value' => $campaign,
  );
  $form['node'] = array(
    '#type' => 'value',
    '#value' => $node,
  );
  $form['content'] = array(
    '#type' => 'fieldset',
    '#title' => t('Newsletter Content'),
    '#description' => t('<p>Use the following fields to make adjustments to the HTML and Plain
        text versions of this campaign.</p>'),
  );
  $form['content']['sub'] = array(
    '#type' => 'fieldset',
    '#title' => t('Email Campaign Subject'),
    '#collapsible' => TRUE,
  );
  $form['content']['sub']['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#description' => t('The subject line for this newsletter campaign.'),
    '#default_value' => check_plain($content['subject']),
    '#required' => TRUE,
  );
  $form['content']['plain'] = array(
    '#type' => 'fieldset',
    '#title' => t('Plain Text'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#required' => TRUE,
  );
  $form['content']['plain']['freeform_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Text'),
    '#default_value' => $content['freeform_text'],
    '#required' => TRUE,
    '#description' => t('The plain text version of this email.  Some email clients do not
                         support html.  This field is designed to provide a plain text version
                         of the newsletter for those email clients.'),
  );
  $form['content']['hyper'] = array(
    '#type' => 'fieldset',
    '#title' => t('HTML'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['content']['hyper']['freeform_html'] = array(
    '#title' => t('Markup'),
    '#type' => 'textarea',
    '#default_value' => $content['freeform_html'],
    '#required' => TRUE,
  );
  $form['content']['warning'] = array(
    '#type' => 'item',
    '#value' => t('<strong>Note</strong>: The adjustments made through this
    form only affect the newsletter and will NOT be displayed on the website.  Use the <a href="@url">
    node form</a> to make changes to both the website and the VerticalResponse newsletter campaign.
    Please be aware the changes submitted through the node form will overwrite any changes made here;
    You should only use this form immediately prior to testing/launching the newsletter campaign.',
    array('@url' => url('node/' . $node->nid . '/edit'))),
  );
  $form['unsub'] = array(
    '#title' => t('Unsubscribe Information and Postal Address'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('Administrators can adjust the <a href="@url">default settings</a> for these fields.',
                        array('@url' => url('admin/config/vr'))),
  );
  $form['unsub']['unsub_message'] = array(
    '#title' => t('Unsubscribe Message'),
    '#type' => 'textarea',
    '#description' => t('VerticalResponse will append a link to unsubscribe to this text.'),
    '#default_value' => $content['unsub_message'],
    '#required' => TRUE,
  );
  $form['unsub']['unsub_link_text'] = array(
    '#type' => 'select',
    '#title' => t('Unsubscribe Link Text'),
    '#options' => array(
      'Unsubscribe' => 'Unsubscribe',
      'Leave this list' => 'Leave this list',
      'No More Email' => 'No More Email',
      'Remove Me' => 'Remove Me',
      'Remove' => 'Remove',
      'Opt-Out' => 'Opt-Out',
      'Take me off this list' => 'Take me off this list',
      'Stop receiving mail' => 'Stop receiving mail',
    ),
    '#default_value' => $content['unsub_link_text'],
    '#description' => t('The text used for the unsubscribe link.'),
    '#required' => TRUE,
  );
  $form['unsub']['postal_address'] = array(
    '#type' => 'textarea',
    '#title' => t('Postal Address'),
    '#default_value' => $content['postal_address'],
    '#description' => t('The postal address for your organization.'),
    '#required' => TRUE,
  );
  $form['back'] = array(
    '#type' => 'submit',
    '#value' => t('Back'),
  );
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['save_cont'] = array(
    '#type' => 'submit',
    '#value' => t('Save and continue'),
  );
  return $form;
}

/**
 * vr_campaign_content_form_submit
 */
function vr_campaign_content_form_submit($form, &$form_state) {
  $node = $form_state['values']['node'];
  switch ($form_state['values']['op']) {
    case 'Back':
      $form_state['redirect'] = 'node/' . $node->nid . '/enews-export/vr';
      return;
      break;
    case 'Save and continue':
      $form_state['redirect'] = 'node/' . $node->nid . '/enews-export/vr-preview';
      break;
  }
  $campaign = $form_state['values']['campaign'];
  $content = array(
    'freeform_text' => $form_state['values']['freeform_text'],
    'freeform_html' => $form_state['values']['freeform_html'],
    'subject' => $form_state['values']['subject'],
    'unsub_message' => $form_state['values']['unsub_message'],
    'unsub_link_text' => $form_state['values']['unsub_link_text'],
    'postal_address' => $form_state['values']['postal_address'],
  );
  vr_campaign_set_campaign_content($campaign->id, $content);
  drupal_set_message('The campaign content has been saved.');
}

/**
 * vr_campaign_preview_form
 */
function vr_campaign_preview_form($form, $form_state, $node) {
  $campaign = vr_campaign_retrieve_campaign($node);
  if ($campaign->display_status != 'active') {
    $message = t('This status of this email campaign is %status. Therefore, changes
    made to this content will not be passed to VerticalResponse.',
    array(
      '%status' => ucfirst($campaign->display_status),
    )
    );
    drupal_set_message($message);
    return array();
  }

  $form['campaign'] = array(
    '#type' => 'value',
    '#value' => $campaign,
  );
  $form['node'] = array(
    '#type' => 'value',
    '#value' => $node,
  );
  $form['test'] = array(
    '#type' => 'fieldset',
    '#title' => t('Test Campaign'),
    '#description' => t('Send a test version of this email campaign to your mailbox.'),
  );
  $form['test']['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email address'),
    '#description' => t('The email address to which the test email should be sent.'),
  );
  $form['test']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Send test email'),
  );
  return $form;
}

/**
 * Validate conditionally required form fields on the test/launch
 * campaign form based on the button clicked.
 */
function vr_campaign_preview_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  // Email address is required for test emails.
  if ($values['op'] == t('Send test email') AND empty($values['email'])) {
    form_set_error('email', t('Please enter an email address for the email test recipient.'));
  }
}
/**
 * Send test email or launch campaign based on the button
 * that was clicked.
 */
function vr_campaign_preview_form_submit($form, &$form_state) {
  // Shorten array
  $values = $form_state['values'];
  $param['campaign_id'] = $form_state['values']['campaign']->id;
  $param['recipients'] = array(
    array(
      array(
        'name' => 'email_address',
        'value' => $form_state['values']['email'],
      ),
    ),
  );
  vr_api('sendEmailCampaignTest', $param, TRUE);
  drupal_set_message(t('Test emails have been sent to @email', array('@email' => $form_state['values']['email'])));
}

/**
 * Final review of campaign before it is launched.
 */
function vr_campaign_launch_form($form, $form_state, $node) {
  $campaign = vr_campaign_retrieve_campaign($node);
  // Decipher content objects.
  foreach ($campaign->contents as $content) {
    $contents[$content->type] = $content->copy;
  }
  $count = db_query('SELECT COUNT(*) FROM {vr_campaign_recipients} WHERE vrid = :vrid', array(':vrid' => $campaign->id))->fetchField();
  // Confirm that the campaign has recipients.
  $rows[] = array(
    'class' => array((empty($campaign->name)) ? 'error' : 'ok'),
    'data' => array(
      t('Campaign Name'),
      t('@name', array('@name' => $campaign->name)),
      t('<a href="@url">Edit</a>', array('@url' => url('node/' . $node->nid . '/enews-export'))),
    ),
  );
  $rows[] = array(
    'class' => array((empty($campaign->from_label)) ? 'error' : 'ok'),
    'data' => array(
      t('From Label'),
      t('@from', array('@from' => $campaign->from_label)),
      t('<a href="@url">Edit</a>', array('@url' => url('node/' . $node->nid . '/enews-export'))),
    ),
  );
  $rows[] = array(
    'class' => array(empty($campaign->support_email) ? 'error' : 'ok'),
    'data' => array(
      t('Support Email'),
      t('@email', array('@email' => $campaign->support_email)),
      t('<a href="@url">Edit</a>', array('@url' => url('node/' . $node->nid . '/enews-export'))),
    ),
  );
  $rows[] = array(
    'class' => array((empty($contents['subject'])) ? 'error' : 'ok'),
    'data' => array(
      t('Plain Text Version'),
      'data' => (empty($contents['freeform_text']))
             ? t('Subject is empty.')
             : t('Subject exists: %subject', array('%subject' => $contents['subject'])), t('<a href="@url">Edit</a>', array('@url' => url('node/' . $node->nid . '/enews-export/vr-content'))),
    ),
  );
  $rows[] = array(
    'class' => array((empty($contents['freeform_text'])) ? 'error' : 'ok'),
    'data' => array(
      t('Plain Text Version'),
      'data' => (empty($contents['freeform_text']))
             ? t('Plain text version is empty.')
             : t('Plain text version exists.'), t('<a href="@url">Edit</a>', array('@url' => url('node/' . $node->nid . '/enews-export/vr-content'))),
    ),
  );
  $rows[] = array(
    'class' => array((empty($contents['freeform_html'])) ? 'error' : 'ok'),
    'data' => array(
      t('HTML Version'),
      'data' => (empty($contents['freeform_text']))
             ? t('HTML version is empty.')
             : t('HTML version exists.'), t('<a href="@url">Edit</a>', array('@url' => url('node/' . $node->nid . '/enews-export/vr-content'))),
    ),
  );
  $rows[] = array(
    'class' => array((empty($contents['unsub_message'])) ? 'error' : 'ok'),
    'data' => array(
      t('Unsubscribe Message'),
      'data' => (empty($contents['unsub_message']))
             ? t('Unsubscribe message is empty.')
             : t('Unsubscribe message exists: %message', array('%message' => $contents['unsub_message'])), t('<a href="@url">Edit</a>', array('@url' => url('node/' . $node->nid . '/enews-export/vr-content'))),
    ),
  );
  $rows[] = array(
    'class' => array((empty($contents['unsub_link_text'])) ? 'error' : 'ok'),
    'data' => array(
      t('Unsubscribe Link Text'),
      'data' => (empty($contents['unsub_link_text']))
             ? t('Unsubscribe message is missing.')
             : t('Unsubscribe message exists: %message', array('%message' => $contents['unsub_link_text'])), t('<a href="@url">Edit</a>', array('@url' => url('node/' . $node->nid . '/enews-export/vr-content'))),
    ),
  );
  $rows[] = array(
    'class' => array((empty($contents['postal_address'])) ? 'error' : 'ok'),
    'data' => array(
      t('Postal Address'),
      'data' => (empty($contents['unsub_link_text']))
             ? t('Postal address is missing.')
             : t('Postal address: %address', array('%address' => $contents['postal_address'])), t('<a href="@url">Edit</a>', array('@url' => url('node/' . $node->nid . '/enews-export/vr-content'))),
    ),
  );
  $rows[] = array(
    'class' => array(($count > 0) ? 'ok' : 'error'),
    'data' => array(
      t('Recipients'),
      array(
        'data' => format_plural($count, 'Campaign has @count recipient.', 'Campaign has @count recipients.', array('@count' => $count)),
      ),
      t('<a href="@url">Edit</a>', array('@url' => url('node/' . $node->nid . '/enews-export/vr-recipients'))),
    ),
  );
  $rows[] = array(
    'class' => array(($campaign->tested == TRUE) ? 'ok' : 'error'),
    'data' => array(
      t('Tested'),
      'data' => $campaign->tested == TRUE ? t('Campaign has been tested.') : t('Campaign must be tested.'), t('<a href="@url">Test Campaign</a>', array('@url' => url('node/' . $node->nid . '/enews-export/vr-preview'))),
    ),
  );
  $form['table'] = array(
    '#markup' => theme('table', array(
      'rows' => $rows, 
      'header' => array(t('Requirement'), t('Status'), t('Setting')),
      'attributes' => array('class' => array('system-status-report'))
    )));
  $form['campaign'] = array(
    '#type' => 'value',
    '#value' => $campaign,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Launch Campaign'),
  );
  return $form;
}

/**
 * Launch email campaign.
 */
function vr_campaign_launch_form_submit($form, &$form_state) {
  $campaign = $form_state['values']['campaign'];
  // Create a uniquely-named mailing list for this campaign.
  $param['name'] = $campaign->name . ' - ' . date('Y-m-d h:i:s');
  $param['type'] = 'email';
  $param['custom_field_names'] = array();
  $list_id = vr_api('createList', $param);
  if (empty($list_id)) {
    drupal_set_message(t('The system was unable to create a new mailing list for this campaign.
      Please try again.', 'error'));
    return;
  }
  // Add list to campaign;
  unset($param);
  $param['list_ids'] = array($list_id);
  $param['campaign_id'] = $campaign->id;
  vr_api('setCampaignLists', $param);
  // Add campaign recipients to list.
  $result = db_query('SELECT uid FROM {vr_campaign_recipients} WHERE vrid = :vrid', array(':vrid' => $campaign->id));
  while ($uid = $result->fetchField()) {
    $users[$uid] = $uid;
  }

  $operations[] = array(
    '_vr_list_subscribe',
    array($users, $list_id, 'subscribe', $campaign),
  );
  $batch = array(
    'operations' => $operations,
    'finished' => t('vr_campaign_finish'),
    'title' => t('Updating Mailing List information in VerticalResponse'),
    'progress_message' => t('Processed @current out of @total.'),
  );
  batch_set($batch);
}

/**
 * Return an array of campaign content
 *
 * @param object
 *  A VerticalResponse campaign object that includes content.
 */
function _vr_campaign_retrieve_campaign_content($campaign) {
  if (is_array($campaign->contents)) {
    foreach ($campaign->contents as $object) {
      $content[$object->type] = $object->copy;
    }
    return $content;
  }
}
