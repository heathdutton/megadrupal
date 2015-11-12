<?php

/**
 * @file
 * Forms for creating, editing, and deleting campaigns.
 */

/**
 * Form callback: create or edit a campaign or campaign page.
 *
 * @array $form
 *   The Form.
 *
 * @array $form_state
 *   The form state.
 *
 * @redhen_campaign $campaign of type $campaign_type
 *   The campaign object to edit or for a create form an empty campaign object
 *     with only a campaign type defined. Also works for campaign pages.
 */
function redhen_campaign_campaign_form($form, &$form_state, $campaign) {
  // Add the default field elements.
  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Campaign title'),
    '#default_value' => $campaign->label,
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -6,
  );

  $form['promote'] = array(
    '#type' => 'checkbox',
    '#title' => 'Promote to Front Page',
    '#default_value' => $campaign->promote,
  );
  $form['sticky'] = array(
    '#type' => 'checkbox',
    '#title' => 'Sticky at top of lists',
    '#default_value' => $campaign->sticky,
  );
  // Add the field related form elements.
  $form_state['campaign_entity'] = $campaign;
  field_attach_form('redhen_campaign', $campaign, $form, $form_state);

  // Set the Donation type as dictated by the campaign type configuration:
  if (!empty($campaign->is_new) && isset($form['redhen_campaign_donation_type'])) {
    $lang = $form['redhen_campaign_donation_type']['#language'];
    $bundle = redhen_campaign_type_load($campaign->type);
    if (isset($bundle->donation_type)) {
      $form['redhen_campaign_donation_type'][$lang][0]['redhen_donation_type']['#default_value'] = $bundle->donation_type;
      // If we have a default, we can hide the field:
      if (!empty($bundle->hide_donation_type) && !user_access('administer redhen campaigns')) {
        $form['redhen_campaign_donation_type'][$lang][0]['redhen_donation_type']['#access'] = FALSE;
      }
    }
  }
  else {
    // Even if the campaign isn't new, we may still want to hide the field.
    if (!empty($bundle->hide_donation_type) && !user_access('administer redhen campaigns')) {
      $form['redhen_campaign_donation_type'][$lang][0]['redhen_donation_type']['#access'] = FALSE;
    }
  }
  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 40,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save campaign'),
  );

  return $form;
}

/**
 * Validation callback for redhen_campaign_campaign_form().
 */
function redhen_campaign_campaign_form_validate($form, &$form_state, $campaign_type = 'redhen_campaign') {
  $campaign = $form_state['campaign_entity'];

  // Notify field widgets to validate their data.
  field_attach_form_validate($campaign_type, $campaign, $form, $form_state);
}

/**
 * Submit callback for redhen_campaign_campaign_form().
 */
function redhen_campaign_campaign_form_submit($form, &$form_state) {
  $campaign = &$form_state['campaign_entity'];

  // Set the campaign's author uid.
  global $user;
  $campaign->author_uid = $user->uid;

  // Save default parameters back into the $campaign object.
  $campaign->label = $form_state['values']['label'];
  $campaign->promote = $form_state['values']['promote'];
  $campaign->sticky = $form_state['values']['sticky'];

  // Notify field widgets.
  field_attach_submit('redhen_campaign', $campaign, $form, $form_state);

  // Save the campaign.
  $campaign->save();

  drupal_set_message(t('Campaign %label saved.', array('%label' => $campaign->label)));
  $form_state['redirect'] = 'redhen_campaign/' . $campaign->internalIdentifier();
}

/**
 * Create/edit form for RedHen Campaign pages.
 */
function redhen_campaign_campaign_page_form($form, &$form_state, $page) {
  $type_entity = redhen_campaign_page_type_load($page->type);
  // Add the default field elements.
  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => $type_entity->page_type == REDHEN_CAMPAIGN_PAGE_TEAM ? t('Team Name') : t('Fundraiser Name'),
    '#default_value' => isset($page->label) ? $page->label : '',
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -6,
  );
  $campaigns = entity_load('redhen_campaign');
  $campaign_options = array('' => '-- select --');
  foreach ($campaigns as $id => $campaign) {
    $campaign_options[$id] = $campaign->label;
  }
  $form['campaign'] = array(
    '#type' => 'select',
    '#title' => 'Campaign',
    '#default_value' => isset($page->campaign) ? $page->campaign : (isset($form_state['campaign']) ? $form_state['campaign'] : NULL),
    '#options' => $campaign_options,
    '#required' => TRUE,
    '#disabled' => !user_access('administer redhen campaign pages') && !(isset($page->is_new) && $page->is_new),
  );
  if ($type_entity->page_type == REDHEN_CAMPAIGN_PAGE_INDIVIDUAL) {
    $team_query = new EntityFieldQuery();
    $team_query->entityCondition('entity_type', 'redhen_campaign_page')
      ->propertyCondition('campaign_page_type', REDHEN_CAMPAIGN_PAGE_TEAM);
    if (isset($page->campaign)) {
      $team_query->propertyCondition('campaign', $page->campaign);
    }
    elseif (isset($form_state['campaign'])) {
      $team_query->propertyCondition('campaign', $form_state['campaign']);
    }
    $results = $team_query->execute();
    if (isset($results['redhen_campaign_page'])) {
      $valid_teams = redhen_campaign_page_load_multiple(array_keys($results['redhen_campaign_page']));
      $team_options = array('' => '-- Select --');
      foreach ($valid_teams as $team) {
        $team_options[$team->page_id] = $team->label;
      }
      if (isset($page->page_id)) {
        unset($team_options[$page->page_id]);
      }
      $default_team = isset($page->team) ? $page->team : (isset($form_state['team']) ? $form_state['team'] : NULL);
      $form['team'] = array(
        '#type' => 'select',
        '#title' => 'Are you fundraising as part of a team?',
        '#default_value' => $default_team,
        '#options' => $team_options,
        '#disabled' => $default_team && !user_access('administer redhen campaign pages'),
      );
    }
  }
  // Add the field related form elements.
  $form_state['page_entity'] = $page;
  field_attach_form('redhen_campaign_page', $page, $form, $form_state);

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 40,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save campaign page'),
  );
  return $form;
}

/**
 * Validation callback for redhen_campaign_campaign_page_form().
 */
function redhen_campaign_campaign_page_form_validate($form, &$form_state) {
  $page = $form_state['page_entity'];

  // Notify field widgets to validate their data.
  field_attach_form_validate('redhen_campaign_page', $page, $form, $form_state);
}

/**
 * Submit callback for redhen_campaign_campaign_page_form().
 */
function redhen_campaign_campaign_page_form_submit($form, &$form_state) {
  $page = &$form_state['page_entity'];

  // Set the campaign's author uid.
  global $user;
  $page->author_uid = $user->uid;

  // Save parameters back into the $page object.
  $page->label = $form_state['values']['label'];
  $page->campaign = $form_state['values']['campaign'];
  $bundle = redhen_campaign_page_type_load($page->type);
  $page->campaign_page_type = $bundle->page_type;
  if (!empty($form_state['values']['team'])) {
    $page->team = $form_state['values']['team'];
  }

  // Notify field widgets.
  field_attach_submit('redhen_campaign_page', $page, $form, $form_state);

  // If this is a new team page, point the creator to the invitation tab.
  if (isset($page->is_new) && $page->is_new && $page->campaign_page_type == REDHEN_CAMPAIGN_PAGE_TEAM) {
    // Save the campaign after the is_new check.
    $page->save();
    drupal_set_message(t('Your team fundraiser %label has been created. Now visit the <a href=@link>Invite tab</a> to recruit team members!',
      array(
        '%label' => $page->label,
        '@link' => "/redhen_campaign/page/" . $page->internalIdentifier() . "/invite",
        )));
  }
  else {
    $page->save();
    drupal_set_message(t('Fundraiser %label saved.', array('%label' => $page->label)));
  }

  $form_state['redirect'] = 'redhen_campaign/page/' . $page->internalIdentifier();
}

/**
 * Form callback: confirmation form for deleting a campaign.
 *
 * @param RedhenCampaign $campaign
 *   The campaign object to be deleted.
 *
 * @see confirm_form()
 */
function redhen_campaign_campaign_delete_form($form, &$form_state, $campaign) {
  $form_state['redhen_campaign'] = $campaign;

  $form['#submit'][] = 'redhen_campaign_campaign_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete %label?', array('%label' => $campaign->label)),
    entity_uri('redhen_campaign', $campaign),
    '<p>' . t('Deleting this campaign cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit callback for redhen_campaign_campaign_delete_form().
 */
function redhen_campaign_campaign_delete_form_submit($form, &$form_state) {
  $campaign = $form_state['redhen_campaign'];

  redhen_campaign_delete_multiple(array($campaign->campaign_id));
  drupal_set_message(t('%label has been deleted.', array('%label' => $campaign->label)));
  watchdog('redhen_campaign', 'Deleted campaign %label.', array('%label' => $campaign->label), WATCHDOG_NOTICE);
  $form_state['redirect'] = 'admin/content/redhen_campaign';
}
/**
 * Form callback: confirmation form for deleting a campaign.
 *
 * @param RedhenCampaignPage $page
 *   The campaign object to be deleted.
 *
 * @see confirm_form()
 */
function redhen_campaign_campaign_page_delete_form($form, &$form_state, RedhenCampaignPage $page) {
  $form_state['redhen_campaign_page'] = $page;
  $form['#submit'][] = 'redhen_campaign_campaign_page_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete %label?', array('%label' => $page->label)),
    entity_uri('redhen_campaign_page', $page),
    '<p>' . t('Deleting this campaign page cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit callback for redhen_campaign_campaign_delete_form().
 */
function redhen_campaign_campaign_page_delete_form_submit($form, &$form_state) {
  $page = $form_state['redhen_campaign_page'];
  $dest = drupal_get_destination();
  $uri = entity_uri('redhen_campaign_page', $page);
  if (strpos($dest['destination'], $uri['path']) !== FALSE) {
    $form_state['redirect'] = 'home';
  }
  redhen_campaign_page_delete_multiple(array($page->page_id));
  drupal_set_message(t('%label has been deleted.', array('%label' => $page->label)));
  watchdog('redhen_campaign', 'Deleted campaign_page %label.', array('%label' => $page->label), WATCHDOG_NOTICE);
}

/**
 * Returns a form for adding a campaign page where you can select the type.
 */
function redhen_campaign_page_add_form($form, &$form_state) {
  $info = entity_get_info('redhen_campaign_page');
  $fundraiser_types = array();
  foreach ($info['bundles'] as $key => $bundle) {
    $fundraiser_types[$key] = $bundle['label'];
  }

  $form['fundraiser_type'] = array(
    '#type' => 'radios',
    '#title' => t('Select the type of fundraiser you want to create:'),
    '#default_value' => !empty($form_state['values']['fundraiser_type']) ? $form_state['values']['fundraiser_type'] : FALSE,
    '#empty_option' => t('- Select -'),
    '#options' => $fundraiser_types,
    '#ajax' => array(
      'callback' => 'redhen_campaign_page_add_form_callback',
      'wrapper' => 'redhen-campaign-page-campaign',
    ),
  );

  // Grab the campaign from the query string on initial load.
  if (!empty($_GET['campaign'])) {
    $form_state['campaign'] = $_GET['campaign'];
  }
  // Grab the team from the query string on initial load.
  if (!empty($_GET['team'])) {
    $form_state['team'] = $_GET['team'];
    $form['fundraiser_type']['#default_value'] = 'individual';
    $form['fundraiser_type']['#access'] = user_access('administer redhen campaign pages');
  }

  $form['campaign_page_wrapper'] = array(
    '#type' => 'item',
    '#id' => 'redhen-campaign-page-campaign',
  );

  if (!empty($form_state['values']['fundraiser_type']) || !empty($form['fundraiser_type']['#default_value'])) {
    $selection = !empty($form_state['values']['fundraiser_type']) ? $form_state['values']['fundraiser_type'] : $form['fundraiser_type']['#default_value'];
    $page = entity_get_controller('redhen_campaign_page')->create(array('type' => $selection));
    $form['campaign_page_wrapper']['campaign_page'] = redhen_campaign_campaign_page_form($form['campaign_page_wrapper'], $form_state, $page);
    $form['campaign_page_wrapper']['campaign_page']['team']['#states'] = array(
      'visible' => array(
        ':input[name="fundraiser_type"]' => array('value' => 'individual'),
      ),
    );
    // Manually call alter hooks for the form we are embedding:
    $form_id = 'redhen_campaign_campaign_page_form';
    $hooks = array('form', 'form_' . $form_id);
    drupal_alter($hooks, $form['campaign_page_wrapper']['campaign_page'], $form_state, $form_id);
  }

  return $form;
}

/**
 * Submission function for custom campaign page creation form.
 */
function redhen_campaign_page_add_form_submit($form, &$form_state) {
  redhen_campaign_campaign_page_form_submit($form, $form_state);
}

/**
 * Ajax callback for redhen_campaign_page_add_form().
 */
function redhen_campaign_page_add_form_callback($form, $form_state) {
  return $form['campaign_page_wrapper']['campaign_page'];
}

/**
 * Return a campaign invitation form.
 */
function redhen_campaign_invite_form($form, &$form_state, RedhenCampaignPage $campaign_page) {
  $form_state['redhen_campaign_page'] = $campaign_page;
  $wrapper = entity_metadata_wrapper('redhen_campaign_page', $campaign_page);
  $form['emails'] = array(
    '#type' => 'textarea',
    '#required' => TRUE,
    '#title' => t('Emails'),
    '#description' => t('Enter a comma delimited list of emails to invite to your team.'),
    '#weight' => 10,
    '#element_validate' => array('redhen_campaign_invite_emails_field_validate'),
  );

  $default_message = t('Hi you! @team has invited you to be a team member of their fundraiser to support the @campaign_name campaign.', array('@team' => $wrapper->label(), '@campaign_name' => $wrapper->campaign->label()));

  $form['message'] = array(
    '#type' => 'textarea',
    '#required' => TRUE,
    '#title' => t('Message'),
    '#description' => t('Customize your invitation. The invite link will be appended to the end of the message.'),
    '#weight' =>11,
    '#default_value' => $default_message,
  );

  $form['actions'] = array(
    '#type' => 'actions',
    '#weight' => 15,
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Invite'),
    ),
  );

  return $form;
}

/**
 * Validation function for email invitation form.
 */
function redhen_campaign_invite_emails_field_validate($element, &$form_state, $form) {
  $clean_emails = preg_replace('/\s+/', '', $element['#value']);
  if (empty($clean_emails)) {
    return;
  }
  $emails = explode(',', $clean_emails);
  $bad_emails = array();
  foreach ($emails as $email) {
    if (!valid_email_address($email)) {
      $bad_emails[] = $email;
    }
  }
  if (!empty($bad_emails)) {
    form_error($element, t('The following emails are invalid: !emails.', array('!emails' => implode(', ', $bad_emails))));
  }
}

/**
 * Submit handler for redhen_campaign_invite_form().
 */
function redhen_campaign_invite_form_submit($form, &$form_state) {
  $campaign_page = $form_state['redhen_campaign_page'];
  $result = redhen_campaign_send_invite($campaign_page, $form_state['values']['emails'], $form_state['values']['message']);

  if ($result) {
    $redirect_url = $campaign_page->uri();
    $form_state['redirect'] = $redirect_url['path'];
  }
}
