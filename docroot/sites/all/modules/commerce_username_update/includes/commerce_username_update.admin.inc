<?php

/**
 * @file
 * Defines administrative forms for Commerce Username Update.
 */


/**
 * Builds the administrative form to process the username update.
 */
function commerce_username_update_admin_form($form, &$form_state) {
  $count = commerce_username_update_email_username_count();

  $form['status'] = array(
    '#type' => 'fieldset',
    '#title' => t('Username status'),
  );

  if ($count > 0) {
    $message = '<p>' . format_plural($count, 'There is 1 username in your database that looks like an e-mail address.', 'There are @count usernames in your database that look like e-mail addresses.') . '</p>';
    $message .= '<div>' . t('Updating usernames now will batch process your usernames 100 at a time, removing the host name from the address and appending a numeral if necessary to ensure all generated usernames are unique.');
    $message .= ' ' . t('This module does not currently provide any notification to users that their usernames have changed. It is up to you to notify your users or provide an alternate log in mechanism, such as log in via e-mail address as implemented by the <a href="!url">E-mail Registration</a> module.', array('!url' => 'https://www.drupal.org/project/email_registration')) . '</div>';
  }
  else {
    $message = t("There aren't any usernames in your database that look like an e-mail address. You do not need to run this update and can safely disable this module asusming you have updated to Commerce 1.10 or later.");
  }

  $form['status']['count'] = array(
    '#type' => 'markup',
    '#markup' => '<div>' . $message . '</div>',
  );

  if ($count > 0) {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Update usernames now'),
      '#disabled' => $count == 0,
    );
  }

  return $form;
}

/**
 * Form submit handler: processes the username update.
 */
function commerce_username_update_admin_form_submit($form, &$form_state) {
  // Ensure there are still usernames to be updated.
  $count = commerce_username_update_email_username_count();

  if ($count > 0) {
    commerce_username_update_batch_set();
  }
}
