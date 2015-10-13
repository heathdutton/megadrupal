<?php

/**
 * @file
 * This file contains configuration forms for opac suggestions module.
 */

/**
 * Configuration form for suggestions.
 */
function opac_suggestions_settings_form($form, $form_state) {

  $vocabularies = taxonomy_vocabulary_get_names();
  $vocabulary_options = array('' => t('None'));
  // We save vid but need to keep
  // machine names to retrieve fields.
  $vid_name = array('' => t('None'));
  foreach ($vocabularies as $vocabulary) {
    $vocabulary_options[$vocabulary->vid] = $vocabulary->name;
    $vid_name[$vocabulary->vid] = $vocabulary->machine_name;
  }

  $form['library_form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Library form element'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['library_form']['suggestions_branch_vocabulary'] = array(
    '#type' => 'select',
    '#title' => t('Vocabulary'),
    '#options' => $vocabulary_options,
    '#default_value' => variable_get('suggestions_branch_vocabulary', ''),
    '#description' => t('Select the vocabulary used for the library dropdown list in suggestion form.'),
    '#ajax' => array(
      'callback' => 'opac_suggestions_branch_field_callback',
      'wrapper' => 'branch_field_ajax_wrapper',
      'method' => 'replace',
    ),
  );

  $selected_branch = isset($form_state['values']['suggestions_branch_vocabulary']) ?
    $vid_name[$form_state['values']['suggestions_branch_vocabulary']] :
    $vid_name[variable_get('suggestions_branch_vocabulary', '')];

  $form['library_form']['suggestions_branch_field'] = array(
    '#type' => 'select',
    '#title' => t('Code field'),
    '#options' => isset($selected_branch) ? _opac_suggestions_vocabulary_fields($selected_branch) : array(),
    '#default_value' => isset($selected_branch) ? variable_get('suggestions_branch_field', '') : '',
    '#description' => t('Select the vocabulary field which contains the code of the branch.'),
    '#prefix' => '<div id="branch_field_ajax_wrapper">',
    '#suffix' => '</div>',
  );

  $user_fields = array('' => t('No'));
  foreach (field_info_instances('user', 'user') as $field_name => $field) {
    $field_info = field_info_field($field_name);
    if ($field_info['type'] == 'taxonomy_term_reference') {
      $user_fields[$field_name] = $field['label'];
    }
  }

  $form['library_form']['suggestions_user_branch_field'] = array(
    '#type' => 'select',
    '#title' => t('Auto select library using a user field'),
    '#options' => $user_fields,
    '#default_value' => variable_get('suggestions_user_branch_field', ''),
    '#description' => t('The user field used to select default library in new suggestion form.'),
  );

  $form['doctype_form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Doctype form element'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['doctype_form']['suggestions_doctype_vocabulary'] = array(
    '#type' => 'select',
    '#title' => t('Document type vocabulary'),
    '#options' => $vocabulary_options,
    '#default_value' => variable_get('suggestions_doctype_vocabulary', ''),
    '#description' => t('Select the vocabulary used for the document type dropdown list in suggestion form.'),
    '#ajax' => array(
      'callback' => 'opac_suggestions_doctype_field_callback',
      'wrapper' => 'doctype_field_ajax_wrapper',
      'method' => 'replace',
    ),
  );

  $selected_doctype = isset($form_state['values']['suggestions_doctype_vocabulary']) ?
    $vid_name[$form_state['values']['suggestions_doctype_vocabulary']] :
    $vid_name[variable_get('suggestions_doctype_vocabulary', '')];

  $form['doctype_form']['suggestions_doctype_field'] = array(
    '#type' => 'select',
    '#title' => t('Code field'),
    '#options' => isset($selected_doctype) ? _opac_suggestions_vocabulary_fields($selected_doctype) : array(),
    '#default_value' => isset($selected_doctype) ? variable_get('suggestions_doctype_field', '') : '',
    '#description' => t('Select the vocabulary field which contains the code of the document type.'),
    '#prefix' => '<div id="doctype_field_ajax_wrapper">',
    '#suffix' => '</div>',
  );

  // Email options.
  $form['email'] = array(
    '#type' => 'fieldset',
    '#title' => t('Email options'),
    '#description' => t('Edit the e-mail sent when a user creates a new suggestion'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['email']['opac_suggestions_send_email'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send an email when a suggestion is created.'),
    '#default_value' => variable_get('opac_suggestions_send_email', FALSE),
  );

  $form['email']['opac_suggestions_email_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Email address'),
    '#default_value' => variable_get('opac_suggestions_email_address', ''),
    '#size' => 60,
    '#states' => array(
      'disabled' => array(
        ':input[name="opac_suggestions_use_address_field"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['email']['opac_suggestions_use_address_field'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use library vocabulary field for email address.'),
    '#default_value' => variable_get('opac_suggestions_use_address_field', FALSE),
  );

  $form['email']['opac_suggestions_address_field'] = array(
    '#states' => array(
      'visible' => array(
        ':input[name="opac_suggestions_use_address_field"]' => array('checked' => TRUE),
      ),
    ),
  );
  if (variable_get('suggestions_branch_vocabulary', '')) {
    $form['email']['opac_suggestions_address_field'] = array(
      '#type' => 'select',
      '#title' => t('Email field'),
      '#options' => _opac_suggestions_vocabulary_fields($selected_branch),
      '#default_value' => variable_get('opac_suggestions_address_field', ''),
    );
  }
  else {
    $markup = t('No Library branch selected. Please choose a vocabulary and save the form.');
    $form['email']['error'] = array(
      '#type' => 'item',
      '#markup' => "<strong>$markup</strong>",
    );
  }

  $form['email']['opac_suggestions_mail_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => variable_get('opac_suggestions_mail_subject', OPAC_SUGGESTIONS_SUBJECT_MAIL),
    '#size' => 100,
    '#required' => TRUE,
  );

  $form['email']['opac_suggestions_mail_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => variable_get('opac_suggestions_mail_body', OPAC_SUGGESTIONS_BODY_MAIL),
    '#rows' => 10,
    '#required' => TRUE,
  );

  // Display the user documentation of placeholders supported by
  // this module, as a description on the last pattern.
  $form['email']['token_help'] = array(
    '#title' => t('Replacement patterns'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['email']['token_help']['help'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array('suggestion'),
  );

  // Email options.
  $form['user_email'] = array(
    '#type' => 'fieldset',
    '#title' => t('User email options'),
    '#description' => t('Options for the mail that notify the user who created the suggestion.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['user_email']['opac_suggestions_send_user_email'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send an email to the suggetion\'s creator.'),
    '#default_value' => variable_get('opac_suggestions_send_user_email', FALSE),
  );

  $form['user_email']['opac_suggestions_user_mail_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => variable_get('opac_suggestions_user_mail_subject', OPAC_SUGGESTIONS_SUBJECT_USER_MAIL),
    '#size' => 100,
    '#required' => TRUE,
  );

  $form['user_email']['opac_suggestions_user_mail_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => variable_get('opac_suggestions_user_mail_body', OPAC_SUGGESTIONS_BODY_USER_MAIL),
    '#rows' => 10,
    '#required' => TRUE,
  );

  // Display the user documentation of placeholders supported by
  // this module, as a description on the last pattern.
  $form['user_email']['token_help'] = array(
    '#title' => t('Replacement patterns'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['user_email']['token_help']['help'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array('suggestion'),
  );

  return system_settings_form($form);
}

/**
 * Returns an array of a vocabulary's fields, ready to use in '#options'.
 */
function _opac_suggestions_vocabulary_fields($vocabulary_name) {
  $fields = field_info_instances('taxonomy_term', $vocabulary_name);

  $fields_options = array('' => t('None'));
  foreach ($fields as $machine_name => $field) {
    $fields_options[$machine_name] = $field['label'];
  }
  return $fields_options;
}

/**
 * AJAX callback called when user select a vocabulary for branch.
 */
function opac_suggestions_branch_field_callback($form, $form_state) {
  return $form['library_form']['suggestions_branch_field'];
}

/**
 * AJAX callback called when user select a vocabulary for document type.
 */
function opac_suggestions_doctype_field_callback($form, $form_state) {
  return $form['doctype_form']['suggestions_doctype_field'];
}
