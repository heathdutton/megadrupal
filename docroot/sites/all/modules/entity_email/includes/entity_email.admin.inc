<?php

/**
 * @file
 * Contains pages for creating, editing, and deleting Entity Mail Type.
 */

/**
 * Generate the Entity Email Type editing form.
 */
function entity_email_type_form($form, &$form_state, $entity_email_type, $op = 'edit') {
  $field_language = NULL;

  // Clone.
  if ($op == 'clone') {
    $entity_email_type->name .= '';
    $entity_email_type->label .= ' (cloned)';
  }

  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#default_value' => $entity_email_type->label,
    '#description' => t('A short, descriptive label for this email template.'),
    '#maxlength' => 255,
    '#required' => TRUE,
  );
  $form['name'] = array(
    '#type' => 'machine_name',
    '#title' => t('Name'),
    '#default_value' => $entity_email_type->name,
    '#description' => t('The machine-name for this email template. It may only contain lowercase letters, underscores, and numbers. It will be used in URLs and in all API calls.'),
    '#maxlength' => 64,
    '#machine_name' => array(
      'exists' => 'entity_email_type_load',
      'source' => array('label'),
    ),
    '#required' => TRUE,
    '#disabled' => !(isset($entity_email_type->is_new) && $entity_email_type->is_new),
  );
  $form['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => $entity_email_type->subject,
    '#description' => t('The subject line of the email template. May include tokens of any token type specified below.'),
    '#maxlength' => 255,
    '#required' => TRUE,
  );
  $form['mail'] = array(
    '#prefix' => '<div id="mail-type-wrapper">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
    '#parents' => array('mail'),
  );

  // Attach extra fields.
  field_attach_form('entity_email_type', $entity_email_type, $form['mail'], $form_state, $field_language);

  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Additional options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['advanced']['tags'] = array(
    '#type' => 'textfield',
    '#title' => t('Tags'),
    '#default_value' => $entity_email_type->tags,
    '#autocomplete_path' => 'admin/structure/entity_email_type/autocomplete_tags',
    '#description' => t('Tags associated with this configuration, used for sorting in the admin interface. Separate multiple tags with commas.'),
  );
  $form['advanced']['cc'] = array(
    '#type' => 'textarea',
    '#title' => t('CC default'),
    '#rows' => 3,
    '#default_value' => $entity_email_type->cc,
    '#description' => t('Emails to be copied by default for each mail sent to recipient. Enter emails separated by lines or commas. May include tokens of any token type specified below.'),
    '#required' => FALSE,
  );
  $form['advanced']['bcc'] = array(
    '#type' => 'textarea',
    '#title' => t('BCC default'),
    '#rows' => 3,
    '#default_value' => $entity_email_type->bcc,
    '#description' => t('Emails to be blind copied by default for each mail sent to recipient. Enter emails separated by lines or commas. May include tokens of any token type specified below.'),
    '#required' => FALSE,
  );

  $form['tokens'] = entity_email_type_token_help();

  $form['send_email'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send a test email.'),
  );

  $form['test_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Test Email'),
    '#default_value' => variable_get('site_mail', ini_get('sendmail_from')),
    '#description' => t('The email address to send the test email.'),
    '#maxlength' => 255,
    '#required' => TRUE,
    '#states' => array(
      'visible' => array(
        ':input[name="send_email"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save template'),
  );

  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'admin/structure/entity_email_type',
  );

  return $form;
}


/**
 * Form API submit callback for the type form.
 */
function entity_email_type_form_validate(&$form, &$form_state) {

  $cc = $form_state['values']['cc'];
  $bcc = $form_state['values']['bcc'];

  $cc = entity_email_valid_email_address($cc);
  $bcc = entity_email_valid_email_address($bcc);

  $compt = 0;
  if (isset($cc['error'])) {
    foreach ($cc['error'] as $email) {
      form_set_error('cc' . $compt++, t('Please enter a valid email addresses for ') . $email);
    }
  }

  $compt = 0;
  if (isset($bcc['error'])) {
    foreach ($bcc['error'] as $email) {
      form_set_error('bcc' . $compt++, t('Please enter a valid email addresses for ') . $email);
    }
  }

}

/**
 * Form API submit callback for the type form.
 */
function entity_email_type_form_submit(&$form, &$form_state) {

  $entity_email_type = entity_ui_form_submit_build_entity($form, $form_state);

  $entity_email_type->save();

  // Send a test email.
  if($form_state['values']['send_email'] === 1) {
    global $user;
    $params = array(
      'cuid' => $user,
      'fuid' => $user,
      'mail_to' => $form_state['values']['test_email'],
      'mail_cc' => $form_state['values']['cc'],
      'mail_bcc' => $form_state['values']['bcc'],
    );

    entity_email_send($form_state['values']['name'], $params);
  }

  $form_state['redirect'] = 'admin/structure/entity_email_type';
}


/**
 * Helper function to provide token help to template construction.
 */
function entity_email_type_token_help() {
  if (module_exists('token')) {
    $tokens = array(
      '#title' => t('Replacement patterns'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#description' => t('Make sure that the tokens you choose are available.'),
    );
    $tokens['token_tree'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('site', 'current-user', 'entity-email-from', 'entity-email-to', 'entity-email-dynamic', 'entity-email-node',),
      '#global_types' => FALSE,
    );
  }
  else {
    $tokens = array(
      '#markup' => '<p>' . t('Enable the <a href="@drupal-token">Token module</a> to view the available token browser.', array('@drupal-token' => 'http://drupal.org/project/token')) . '</p>',
    );
  }
  return $tokens;
}


/**
 * Helper.
 *
 * @param string $string
 * @return array $form_state
 *
 * @see entity_email_type_form_validate().
 */
function entity_email_valid_email_address($email) {
  $emails_array = array();

  // Split on , ; or space.
  $emails = preg_split('(,|;|\s+)', $email );

  foreach ($emails as $key => $email) {
    $email = trim($email);
    if ($email != '') {

      if (module_exists('token')) {
        preg_match_all('/
        \[             # [ - pattern start
        ([^\s\[\]:]*)  # match $type not containing whitespace : [ or ]
        :              # : - separator
        ([^\[\]]*)     # match $name not containing [ or ]
        \]             # ] - pattern end
        /x', $email, $matches
        );
        // If token is pass.
        if (!empty($email) && isset($matches[0][0]) && $email == $matches[0][0]) {
          return;
        }
      }

      // If email is pass.
      elseif (!empty($email) && !valid_email_address($email)) {
        $emails_array['error'][$email] = $email;
      }
      else {
        $emails_array['valid'][$email] = $email;
      }

    }
  }
  return $emails_array;
}
