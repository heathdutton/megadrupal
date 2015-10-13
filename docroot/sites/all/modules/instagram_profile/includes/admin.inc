<?php
/**
 * @file
 * Admin settings form for Share your Instagram feed module.
 */


/**
 * Form builder for the admin settings form.
 */
function instagram_profile_settings_form($form, &$form_state) {
  // Check if the current Instagram ID is valid on initial form-render/load.
  if (empty($form_state['input'])) {
    $args = array('%client_id' => 'Instagram client ID');
    if (_instagram_profile_check_client_id()) {
      drupal_set_message(t('The %client_id is valid.', $args));
    }
    else {
      drupal_set_message(t('The %client_id is not valid.', $args), 'warning');
    }
  }

  // Build the help text for the Client ID field.
  $args = array(
    // Instagram is mono-lingual.
    '%client_id' => 'Client ID',
    '%manage'    => 'Manage Clients',
    '!example'   => '<code>f3737296be1642c59c44150e2f7fef05</code>',
  );
  $links = array(
    '!manage'   => 'http://instagram.com/developer/clients/manage/',
    '!register' => 'http://instagram.com/developer/clients/register/',
  );
  foreach ($links as $key => $url) {
    $args[$key] = drupal_attributes(array('href' => $url, 'target' => '_blank'));
  }
  $help = t('The %client_id provided by Instagram on <a !manage>the %manage page</a>.  Your Instagram application/client (your website) <a !register>must be registered</a> if it is not already.  It is a 32-character hexadecimal hash.  E.g. !example.', $args);

  // Form element for the Client ID field.
  $form['instagram_profile_client_id'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Instagram application client ID'),
    '#description'   => $help,
    '#required'      => TRUE,
    '#maxlength'     => 32,
    '#default_value' => instagram_profile_client_id(),
  );

  // Form element for the demo user ID.
  // @todo Use the users/search API endpoint for autocomplete suggestions.
  $form['instagram_profile_demo'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Demo user ID'),
    '#description'   => t('Set the Instagram user ID to use for demonstration purposes along with the call to action to "Share your Instagram feed".  Note this is not a username (alphanumeric).  It is the numeric user ID.  You can use <a href="http://instagram.com/developer/endpoints/users/#get_users_search">Instagram API\'s <code>users/search</code> endpoint</a> to find the user ID from the username.  Leave this blank to disable the demonstration account.'),
    '#maxlength'     => 15,
    '#default_value' => variable_get('instagram_profile_demo'),
  );

  // Let Drupal save the values of a successful form-submission.
  return system_settings_form($form);
}

/**
 * Validate handler for the admin settings form.
 */
function instagram_profile_settings_form_validate($form, &$form_state) {
  instagram_profile_settings_form_validate_client_id($form, $form_state);
  instagram_profile_settings_form_validate_demo_user_id($form, $form_state);
}

function instagram_profile_settings_form_validate_client_id($form, &$form_state) {
  $key  = 'instagram_profile_client_id';
  $id   = $form_state['values'][$key];
  $size = $form[$key]['#maxlength'];
  $args = array('%client_id' => $form[$key]['#title']);

  // Check the length of the ID.
  $length = strlen($id);
  if ($length < $size) {
    form_set_error($key, t('The %client_id is too short.', $args));
  }
  else if ($length > $size) {
    form_set_error($key, t('The %client_id is too long.', $args));
  }
  else {
    // Check the ID is valid.
    if (_instagram_profile_check_client_id($id)) {
      drupal_set_message(t('The %client_id is valid.', $args));
    }
    else {
      form_set_error($key, t('The %client_id is not valid.', $args));
    }
  }
}

function instagram_profile_settings_form_validate_demo_user_id($form, &$form_state) {
  $key  = 'instagram_profile_demo';
  $id   = $form_state['values'][$key];

  // Check the length of the ID.
  if (!empty($id) && !_instagram_profile_validate_user_id($id)) {
    form_set_error($key, t('%demo must be a positive integer.  Note that it is the Instagram user ID.  Not the alphanumeric username.', array('%demo' => $form[$key]['#title'])));
  }
}
