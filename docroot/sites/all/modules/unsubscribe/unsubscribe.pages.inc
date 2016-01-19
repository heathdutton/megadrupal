<?php
/**
 * @file
 * Defines form and page callbacks for unsubscribe module.
 */

/**
 * Form constructor for unsubscribe settings page.
 */
function unsubscribe_settings_form($form, &$form_state) {
  $form['unsubscribe_append'] = array(
    '#type' => 'checkbox',
    '#title' => t('Append unsubscribe link to all outgoing emails'),
    '#default_value' => variable_get('unsubscribe_append', 1),
  );
  $form['exemptions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Module Exemptions'),
    '#description' => t("These modules will be exempt from the unsubscribe module's blocking mechanism. Emails sent by select modules will still be sent to unsubscribed users."),
  );
  $form['exemptions']['unsubscribe_exempt_modules'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Exempt modules'),
    '#options' => drupal_map_assoc(module_implements('mail')),
    '#default_value' => variable_get('unsubscribe_exempt_modules', drupal_map_assoc(array('system', 'user'))),
    '#description' => t('Developers: you can programatically alter this list using hook_unsubscribe_exemptions_alter().'),
  );
  return system_settings_form($form);
}

/**
 * Form constructor for unsubscribe form.
 */
function unsubscribe_unsubscribe_form($form, &$form_state) {
  $form['mail'] = array(
    '#type' => 'textfield',
    '#title' => t('Your email address'),
    '#description' => t('Enter your email address to unsubscribe from email communications.'),
    '#default_value' => !empty($_GET['mail']) ? $_GET['mail'] : '',
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Unsubscribe'),
  );
  return $form;
}

/**
 * Form validation handler for unsubscribe_unsubscribe_form().
 */
function unsubscribe_unsubscribe_form_validate($form, &$form_state) {
  $mail = $form_state['values']['mail'];

  // Validate email address.
  if (!valid_email_address($mail)) {
    form_set_error('mail', t('The e-mail address you specified is not valid.'));
  }
  else {
    // Check to see that email address belongs to an existing user.
    if ($account = user_load_by_mail($mail)) {
      // Check to see if this user has already unsubscribed.
      if (unsubscribe_user_check(array('mail' => $mail))) {
        form_set_error('mail', t('The e-mail address that you specified has already been unsubscribed.'));
      }
    }
    else {
      form_set_error('mail', t('The e-mail address that you specified is not in our database.'));
    }
  }
}

/**
 * Form submission handler for unsubscribe_unsubscribe_form().
 */
function unsubscribe_unsubscribe_form_submit($form, &$form_state) {
  $account = user_load_by_mail($mail);
  unsubscribe_user_add($account->uid);
}