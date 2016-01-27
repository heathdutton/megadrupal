<?php

/**
 * Form to submit for an email verification
 */
function email_required_verification_form($form, &$form_state) {
  global $user;
  
  drupal_set_title(t('You must verify your email address to continue'));
  
  $form['warning'] = array(
    '#type' => 'item',
    '#markup' => t('The page you requested requires that you first verify your email address. Clicking the button below will send an email to the address associated with your account. It will contain a link that, when clicked, will verify your email address. You will only have to do this once.'),
  );
  
  $form['email'] = array(
    '#type' => 'item',
    '#markup' => t('Your current email address is %email. !link to change it before verifying.', array('%email' => $user->mail, '!link' => l(t('Click here'), "user/{$user->uid}/edit", array('query' => array('destination' => 'email/verify'))))),
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Send the verification email'),
  );
  
  return $form;
}

/**
 * Submit handler for the email verification form
 */
function email_required_verification_form_submit(&$form, &$form_state) {
  global $user;
  
  // Insert a new hash
  email_required_create_hash($user);
  
  // Send the email
  drupal_mail('email_required', 'verification', $user->mail, user_preferred_language($user));
  
  // Set a message
  drupal_set_message(t('A verification email has been sent to you.'));
  
  // Log the action
  watchdog('email_required', 'Verification email sent to !user (!uid)', array('!user' => $user->name, '!uid' => $user->uid)); 

  // Go home
  $form_state['redirect'] = '<front>';
}

/**
 * Page callback for verification hash links
 * 
 * @param $account
 *   A user object specified in the link
 * @param $string
 *   The hash string on the URL
 */
function email_required_verification_page($account, $string) {
  global $user;
  
  $output = '';

  // See if the user is already validated
  if (email_required_user_is_validated($account)) {
    $output .= t('You have already validated your email address.');
  }
  // Load the hash for this user and see if it's a match
  else if (($hash = email_required_load_hash($account)) && ($hash->hash == $string)) {
    // Verify the email address
    email_required_validate_hash($account);
    // Log the action
    watchdog('email_required', '!user (!uid) has validated their email address.', array('!user' => $account->name, '!uid' => $account->uid));
    // Set a message
    $output = t('Your email address has been verified.');
    // Set the title
    drupal_set_title(t('Verified!'));
  }
  else {
    $output .= t('Invalid verification link. It is possible that this link has expired. !link to have another one emailed to you.', array('!link' => l(t('Click here'), 'email/verify')));
  }

  $output .= "<br/><br/>" . l(t('Return to the homepage'), '<front>');
  
  return $output;
}
