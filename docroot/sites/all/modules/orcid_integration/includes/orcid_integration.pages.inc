<?php

/**
 * @file orcid.pages.inc
 * ORCID page callbacks.
 */

/**
 * Main routing for orcid view page.
 */
function orcid_integration_view_page($account) {
  if (empty($account->field_orcid_id)) {
    return drupal_get_form('orcid_integration_map_account', $account);
  }
  else {
    return drupal_get_form('orcid_integration_account_unlink', $account);
  }
  return '';
}

/**
 * Displays a creation/mapping account form.
 */
function orcid_integration_map_account($form, &$form_state, $account) {
  $search_account = clone $account;
  if (!empty($form_state['values']['orcid_email'])) {
    $search_account->mail = $form_state['values']['orcid_email'];
  }
  $possible_accounts = orcid_integration_search_account($search_account);

  $form['account'] = array(
    '#type' => 'value',
    '#value' => $account,
  );

  $form['existing'] = array(
    '#type' => 'fieldset',
    '#title' => t('Claim existing account'),
    '#description' => t('Use this to claim an existing ORCID'),
    '#collapsed' => FALSE,
  );
  $form['existing']['orcid'] = array(
    '#type' => 'textfield',
    '#title' => t('ORCID'),
    '#default_value' => '',
  );
  $form['existing']['fetch'] = array(
    '#type' => 'button',
    '#value' => t('Fetch ORCID Record'),
    '#ajax' => array(
      'callback' => 'orcid_integration_ajax_validate_orcid_callback',
      'wrapper' => 'orcid-sync-information',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );
  $form['existing']['info'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="orcid-sync-information">',
    '#suffix' => '</div>',
    '#markup' => t('Please enter a valid orcid.'),
  );
  $form['existing']['claim'] = array(
    '#type' => 'submit',
    '#value' => t('Claim'),
  );

  $form['mapping'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search for Existing Account by Email'),
    '#collapsed' => FALSE,
  );
  $form['mapping']['search_ajax'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search'),
    '#collapsed' => FALSE,
  );
  $form['mapping']['search_ajax']['orcid_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#description' => t('Enter an email to change the search parameters'),
    '#default_value' => $search_account->mail,
  );
  $form['mapping']['search_ajax']['search'] = array(
    '#type' => 'button',
    '#value' => t('Search ORCID Record'),
    '#ajax' => array(
      'callback' => 'orcid_integration_ajax_search_orcid_callback',
      'wrapper' => 'orcid-map-options',
      'method' => 'replaceWith',
      'effect' => 'fade',
    ),
  );

  foreach ($possible_accounts['orcid-search-results']['orcid-search-result'] as $key => $result) {
    foreach ($result['orcid-profile']['orcid-bio']['contact-details']['email'] as $email_key => $email_result) {
      if ($email_result['value'] == $search_account->mail) {
        $options[$result['orcid-profile']['orcid-identifier']['path']] = _orcid_integration_generate_output_from_orcid_array($result);
      }
    }
  }
  if (!empty($options)) {
    $form['mapping']['mapping_options'] = array(
      '#type' => 'radios',
      '#title' => t('Match'),
      '#description' => t('Check the orcid id if it is your match.'),
      '#options' => $options,
      '#prefix' => '<div id="orcid-map-options">',
      '#suffix' => '</div>',
    );
  }
  else {
    $form['mapping']['mapping_options'] = array(
      '#type' => 'markup',
      '#markup' => t('There are no existing orcids that match your email. Use the search email above to see if there is a match with another email address.'),
      '#prefix' => '<div id="orcid-map-options">',
      '#suffix' => '</div>',
    );
  }
  $form['mapping']['map'] = array(
    '#type' => 'submit',
    '#value' => t('Map'),
  );

  if (variable_get('orcid_integration_members_api_url', '') !== '') {
    $form['create'] = array(
      '#type' => 'fieldset',
      '#title' => t('Create'),
      '#description' => t('Use this section to create a new ORCID and associate with your account'),
      '#collapsed' => FALSE,
    );
    $form['create']['first_name'] = array(
      '#title' => t('First Name'),
      '#type' => 'textfield',
      '#description' => t('Please enter your first name.'),
      '#required' => FALSE,
    );
    $form['create']['last_name'] = array(
      '#title' => t('Last Name'),
      '#type' => 'textfield',
      '#description' => t('Please enter your first name.'),
      '#required' => FALSE,
    );
    $form['create']['email'] = array(
      '#type' => 'textfield',
      '#title' => t('EMail'),
      '#default_value' => $account->mail,
      '#attributes' => array(
        // 'disabled' => TRUE,
      ),
    );
    $form['create']['save'] = array(
      '#type' => 'submit',
      '#value' => t('Create'),
    );
  }

  return $form;
}

function orcid_integration_map_account_validate($form, $form_state) {
  $values = $form_state['values'];
  if ($values['op'] == t('Create')) {
    if (empty($values['first_name'])) {
      form_set_error('mapping', t('First name value must be entered if creating new ORCID account.'));
    }
    if (empty($values['last_name'])) {
      form_set_error('mapping', t('Last name value must be entered if creating new ORCID account.'));
    }
    if (empty($values['email'])) {
      form_set_error('mapping', t('EMail value must be entered if creating new ORCID account.'));
    }
  }
  else if ($values['op'] == t('Map')) {
    if (empty($values['mapping_options'])) {
      form_set_error('mapping_options', t('Mapping value must be selected.'));
    }
  }
  else if ($values['op'] == t('Claim')) {
    if (empty($values['orcid'])) {
      form_set_error('orcid', t('Please enter a non-empty orcid.'));
    }
    else {
      $orcid = orcid_integration_search_by_orcid_id($values['orcid']);
      if (empty($orcid->decoded_data)) {
        form_set_error('orcid', t('Please enter a valid orcid.'));
      }
    }
  }
}

function orcid_integration_map_account_submit($form, &$form_state) {
  $values = $form_state['values'];
  if ($values['op'] == t('Create')) {
    $orcid_id = orcid_integration_provision_new_account($values['email'], $values['first_name'], $values['last_name']);
  }
  else if ($values['op'] == t('Map')){
    $orcid_id = $values['mapping_options'];
  }
  else if ($values['op'] == t('Claim')){
    $orcid_id = $values['orcid'];
  }
  if (empty($orcid_id)) {
    drupal_set_message(t('User could not be created - possible duplicate.'));
    return;
  }
  $account = user_load($values['account']->uid);
  $account->field_orcid_id[LANGUAGE_NONE][0]['value'] = $orcid_id;
  user_save($account);
  drupal_set_message(t('Account successfully linked to profile.'));
  $form_state['redirect'] = 'user/' . $account->uid;
}

/**
 * Displays the currently linked ORCID account with an unlink button
 */
function orcid_integration_account_unlink($form, &$form_state, $account) {
  $user_orcid_url = variable_get('orcid_integration_main_url') . $account->field_orcid_id[LANGUAGE_NONE][0]['value'];
  $form['orcid'] = array(
    '#type' => 'item',
    '#title' => t('Your linked ORCID Account'),
    '#markup' => l(
      $user_orcid_url,
      $user_orcid_url,
      $options = array(
        'attributes' => array(
          'target' => '_blank',
        ),
      )
    ),
  );
  $form['unlink'] = array(
    '#type' => 'submit',
    '#value' => t('Unlink ORCID Account'),
  );
  return $form;
}

/**
 * Summit handler for orcid_integration_account_unlink
 */
function orcid_integration_account_unlink_submit($form, &$form_state) {
  $account = user_load($form_state['build_info']['args'][0]->uid);
  $account->field_orcid_id = array();
  user_save($account);
  drupal_set_message(t('ORCID Account successfully unlinked.'));
}
