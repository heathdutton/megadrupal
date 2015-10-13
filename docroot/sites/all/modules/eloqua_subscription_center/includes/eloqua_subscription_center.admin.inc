<?php

/**
 * @file
 * Administration callbacks and functions for Eloqua Subscription Center.
 */


/**
 * Tokens used to simplify the admin experience when defining the path alias.
 */
define('ELOQUA_SUBSCRIPTION_CENTER_EMAIL_DOMAIN_TOKEN', '[#email_domain]');
define('ELOQUA_SUBSCRIPTION_CENTER_CONTACT_ID_TOKEN', '[#contact_id]');


/**
 * Administration form for Eloqua Subscription Center
 */
function eloqua_subscription_center_admin($form, &$form_state) {
  $form = array();

  // Configurable path alias where contacts can manage their subscriptions.
  $path = variable_get('eloqua_subscription_center_path', 'subscription-center/%/%');
  $path = _eloqua_subscription_center_translate_path($path, 'to_admin');
  $form['eloqua_subscription_center_path'] = array(
    '#title' => t('Subscription center path'),
    '#description' => t('A valid path at which contacts will manage their subscriptions.') .
      t('This string must contain both the %domain_token token (representing the e-mail domain of the contact) and the %contact_token token (representing the unique ID of an Eloqua contact). These tokens must appear in that order and must appear as distinct parts of the path (e.g. separated by slashes).', array(
        '%domain_token' => ELOQUA_SUBSCRIPTION_CENTER_EMAIL_DOMAIN_TOKEN,
        '%contact_token' => ELOQUA_SUBSCRIPTION_CENTER_CONTACT_ID_TOKEN,
      )) .
      t('Two tokens are used to increase security and privacy and to ensure customers and prospects are not able to simply increment their contact ID in the URL and see/manage the subscription details of another customer or prospect.'),
    '#type' => 'textfield',
    '#default_value' => $path,
    '#required' => TRUE,
    '#element_validate' => array('eloqua_subscription_center_path_validate'),
  );

  // Configurable title for the subscription management page.
  $form['eloqua_subscription_center_title'] = array(
    '#title' => t('Subscription center title'),
    '#description' => t('The page title for the subscription management page.'),
    '#type' => 'textfield',
    '#default_value' => variable_get('eloqua_subscription_center_title', 'Subscriptions'),
    '#element_validate' => array('eloqua_subscription_center_title_validate'),
    '#required' => TRUE,
  );

  // Eloqua form name to which subscription management re-posts should be sent.
  $form['eloqua_subscription_center_form_name'] = array(
    '#title' => t('Eloqua form name'),
    '#description' => t('The elqFormName value used to re-post subscription settings to Eloqua.'),
    '#type' => 'textfield',
    '#default_value' => variable_get('eloqua_subscription_center_form_name', ''),
    '#required' => TRUE,
  );

  // Checkbox for default subscription status.
  $form['eloqua_subscription_center_default_status'] = array(
    '#title' => t('Default subscription status'),
    '#description' => t('Value used when no subscription data yet exists for a given contact and subscription group.'),
    '#type' => 'radios',
    '#options' => array(
      TRUE => t('Subscribed'),
      FALSE => t('Unsubscribed'),
    ),
    '#default_value' => variable_get('eloqua_subscription_center_default_status', TRUE),
  );

  // Whether or not to show a contact's e-mail address on the management page.
  $form['eloqua_subscription_center_hide_email'] = array(
    '#title' => t('Hide e-mail'),
    '#description' => t("Check this to hide the contact's e-mail address on the subscription management page."),
    '#type' => 'checkbox',
    '#default_value' => variable_get('eloqua_subscription_center_hide_email', FALSE),
  );

  // Field group to encapsulate form responses/actions.
  $form['response'] = array(
    '#title' => t('Form actions'),
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
  );

  // Actions to take after successful subscription preference submission.
  $form['response']['eloqua_subscription_center_form_responses'] = array(
    '#title' => t('Actions'),
    '#type' => 'checkboxes',
    '#description' => t('Actions to be taken upon successful subscription preference submission.'),
    '#options' => array(
      'message' => t('Display a confirmation message'),
      'redirect' => t('Redirect to a confirmation page'),
    ),
    '#default_value' => variable_get('eloqua_subscription_center_form_responses', array()),
  );

  // Message to be displayed upon form submission; displayed conditionally.
  $form['response']['eloqua_subscription_center_confirmation_message'] = array(
    '#title' => t('Confirmation message'),
    '#type' => 'textfield',
    '#description' => t('The message to be displayed to the user upon successful preference submission.'),
    '#default_value' => variable_get('eloqua_subscription_center_confirmation_message', ''),
    '#element_validate' => array('eloqua_subscription_center_message_validate'),
    '#states' => array(
      'visible' => array(
        ':input[name="eloqua_subscription_center_form_responses[message]"]' => array('checked' => TRUE),
      ),
      'required' => array(
        ':input[name="eloqua_subscription_center_form_responses[message]"]' => array('checked' => TRUE),
      ),
    ),
  );

  // Page to redirect to upon form submission; displayed conditionally.
  $form['response']['eloqua_subscription_center_confirmation_redirect'] = array(
    '#title' => t('Confirmation redirect'),
    '#type' => 'textfield',
    '#description' => t('An internal Drupal path or URL to which users will be redirected upon successful preference submission.'),
    '#default_value' => variable_get('eloqua_subscription_center_confirmation_redirect', ''),
    '#element_validate' => array('eloqua_subscription_center_redirect_validate'),
    '#states' => array(
      'visible' => array(
        ':input[name="eloqua_subscription_center_form_responses[redirect]"]' => array('checked' => TRUE),
      ),
      'required' => array(
        ':input[name="eloqua_subscription_center_form_responses[redirect]"]' => array('checked' => TRUE),
      ),
    ),
  );

  // Allow configuration of specific e-mail groups and their display.
  if ($email_groups = eloqua_subscription_center_get_group_configs()) {
    // Create a collapsed fieldset that contains e-mail groups.
    $form['eloqua_subscription_center_groups'] = array(
      '#title' => t('E-mail groups'),
      '#tree' => TRUE,
      '#type' => 'vertical_tabs',
    );

    // Iterate through all returned e-mail groups and display settings forms.
    foreach ($email_groups as $id => $group) {

      // Default values.
      $title = isset($group['#title']) ? $group['#title'] : NULL;
      $enabled = isset($group['#enabled']) ? $group['#enabled'] : FALSE;
      $description = isset($group['#description']) ? $group['#description'] : NULL;
      $weight = isset($group['#weight']) ? $group['#weight'] : 0;
      $form_element_id = isset($group['#id']) ? $group['#id'] : $id;

      // Identify this e-mail group.
      $form['eloqua_subscription_center_groups'][$id] = array(
        '#title' => $title,
        '#type' => 'fieldset',
        '#collapsible' => FALSE,
        '#weight' => $weight,
      );

      // Whether or not to show or hide this group.
      $form['eloqua_subscription_center_groups'][$id]['enabled'] = array(
        '#title' => t('Allow subscription management for this e-mail group'),
        '#type' => 'checkbox',
        '#default_value' => $enabled,
      );

      // Provide the ability to override the title.
      $form['eloqua_subscription_center_groups'][$id]['title'] = array(
        '#title' => t('Display title'),
        '#description' => t('The title used to identify this subscription group to end-users.'),
        '#type' => 'textfield',
        '#default_value' => $title,
        '#required' => TRUE,
      );

      // Provide the ability to specify field element names.
      $form['eloqua_subscription_center_groups'][$id]['id'] = array(
        '#title' => t('Form element name'),
        '#description' => t('Used as the parameter name when POSTing subscription preferences to Eloqua.'),
        '#type' => 'textfield',
        '#default_value' => $form_element_id,
        '#required' => TRUE,
      );

      // Provide the ability to override the description.
      $form['eloqua_subscription_center_groups'][$id]['description'] = array(
        '#title' => t('Display description'),
        '#description' => t('The description used to identify this subscription group to end-users.'),
        '#type' => 'textfield',
        '#default_value' => $description,
      );

      // Provide the ability to override the order (just a weight field).
      $form['eloqua_subscription_center_groups'][$id]['weight'] = array(
        '#title' => t('Order'),
        '#description' => t('A number, indicating the sort order in which this group should appear on the page.'),
        '#type' => 'textfield',
        '#default_value' => $weight,
      );
    }

    // Add a validation handler to handle variable/config preparation.
    $form['#validate'][] = 'eloqua_subscription_center_prepare_group_configs';
  }
  else {
    // If no e-mail groups were detected, provide messaging on how to load them
    // from the API.
    $form['eloqua_subscription_center_groups'] = array(
      '#type' => 'fieldset',
      '#title' => t('E-mail groups not yet loaded'),
      '#description' => t('Looks like no e-mail groups from Eloqua have yet been loaded. To get started, click the "!button_text" button below.', array(
        '!button_text' => t('Pull latest groups from Eloqua'),
      )),
    );
  }

  // Add a custom form submit button to load the latest e-mail groups.
  $form['actions']['load_from_elq'] = array(
    '#type' => 'submit',
    '#value' => t('Pull latest groups from Eloqua'),
    '#weight' => 10,
    '#submit' => array('eloqua_subscription_center_refresh_groups_submit'),
  );

  // Add custom validation and submission handlers; ensure ours come last.
  $form = system_settings_form($form);
  $form['#submit'][] = 'eloqua_subscription_center_admin_submit';

  return $form;
}

/**
 * Validation handler for the Eloqua subscription center path value.
 */
function eloqua_subscription_center_path_validate($element, &$form_state, $form) {
  // Automatically trim unnecessary slashes from the beginning/end.
  form_set_value($element, trim($element['#value'], '/'), $form_state);

  // Before any new values are saved, temporarily store the existing path.
  $form_state['_existing_path'] = variable_get('eloqua_subscription_center_path', 'subscription-center/%/%');
  $new_path_raw = $form_state['values']['eloqua_subscription_center_path'];
  $new_path = _eloqua_subscription_center_translate_path($new_path_raw, 'to_menu');

  // Ensure that the given path is valid.
  if ($error = _eloqua_subscription_center_invalid_token_placement($new_path_raw)) {
    form_set_error($element['#name'], $error);
  }

  // If the given path is new, ensure it does not already exist.
  if ($form_state['_existing_path'] != $new_path && drupal_valid_path($new_path, TRUE)) {
    form_set_error($element['#name'], t('The path you specified is already in use; please provide a unique, unused path.'));
  }

  // Translate the token to its expected form.
  $translated = _eloqua_subscription_center_translate_path($form_state['values']['eloqua_subscription_center_path'], 'to_menu');
  form_set_value($element, $translated, $form_state);
}

/**
 * Validation handler for the Eloqua subscription center title value.
 */
function eloqua_subscription_center_title_validate($element, &$form_state, $form) {
  // Before any new values are saved, temporarily store the existing title.
  $form_state['_existing_title'] = variable_get('eloqua_subscription_center_title', 'Subscriptions');
}

/**
 * Validation handler for the Eloqua subscription center confirmation message.
 */
function eloqua_subscription_center_message_validate($element, &$form_state, $form) {
  // This should only fire if confirmation message is a chosen response.
  if (isset($form_state['values']['eloqua_subscription_center_form_responses']['message']) && $form_state['values']['eloqua_subscription_center_form_responses']['message'] === 'message') {
    // Because #required is only set via #states, manually check for emptiness.
    if (empty($element['#value'])) {
      form_set_error($element['#name'], t('!name field is required.', array('!name' => $element['#title'])));
    }
  }
  // If it's not a chosen response, wipe the value.
  else {
    form_set_value($element, '', $form_state);
  }
}

/**
 * Validation handler for the Eloqua subscription center form redirect value.
 */
function eloqua_subscription_center_redirect_validate($element, &$form_state, $form) {
  // This should only fire if confirmation redirection is a chosen response.
  if (isset($form_state['values']['eloqua_subscription_center_form_responses']['redirect']) && $form_state['values']['eloqua_subscription_center_form_responses']['redirect'] === 'redirect') {
    // Because #required is only set via #states, manually check for emptiness.
    if (empty($element['#value'])) {
      form_set_error($element['#name'], t('!name field is required.', array('!name' => $element['#title'])));
    }
  }
  // If it's not a chosen response, wipe the value.
  else {
    form_set_value($element, '', $form_state);
  }
}

/**
 * Validation handler that prepares e-mail group configs.
 */
function eloqua_subscription_center_prepare_group_configs($form, &$form_state) {
  // Retrieve the input and unset it from the actual form state.
  $input = $form_state['values']['eloqua_subscription_center_groups'];
  unset ($form_state['values']['eloqua_subscription_center_groups']);

  $writethrough = array();
  foreach ($input as $id => $config) {
    if (is_array($config)) {
      $writethrough[$id] = array();
      foreach ($config as $key => $value) {
        $writethrough[$id]['#' . $key] = $value;
      }
    }
  }

  $form_state['values']['eloqua_subscription_center_groups'] = $writethrough;
}

/**
 * Submit handler for the Eloqua subscription center configuration form.
 */
function eloqua_subscription_center_admin_submit($form, &$form_state) {
  $existing_path = $form_state['_existing_path'];
  $new_path = $form_state['values']['eloqua_subscription_center_path'];
  $existing_title = $form_state['_existing_title'];
  $new_title = $form_state['values']['eloqua_subscription_center_title'];

  // Only rebuild the menu if the path has been modified.
  if ($existing_path != $new_path || $existing_title != $new_title) {
    menu_rebuild();

    // Set an HTTP header for debugging / testing purposes.
    drupal_add_http_header('X-Menu-Rebuilt', TRUE);
  }
}

/**
 * Submit handler, seperate from normal submit handler execution, which loads
 * the latest Eloqua e-mail groups from Eloqua. Values are merged in such that
 * existing configurations are not overridden.
 */
function eloqua_subscription_center_refresh_groups_submit($form, &$form_state) {
  if ($client = eloqua_rest_api_client('2.0')) {
    $existing = eloqua_subscription_center_get_group_configs();

    // Load e-mail groups from Eloqua and iterate through them and mold them to
    // match the form expected by the group configuration.
    $from_eloqua = array();
    $groups = $client->api('email')->groups()->search('*');
    $groups = isset($groups['elements']) ? $groups['elements'] : array();
    foreach ($groups as $group) {
      $from_eloqua[$group['id']] = array(
        '#enabled' => FALSE,
        '#title' => $group['name'],
        '#id' => $group['id'],
        '#description' => $group['description'],
        '#weight' => 0,
      );
    }

    // To ensure we do not overwrite any data, only find and save new groups.
    $new_groups = array_diff_key($from_eloqua, $existing);
    eloqua_subscription_center_set_group_configs($existing + $new_groups);

    // Provide a confirmation message.
    drupal_set_message(t('Successfully retrieved %num new groups from Eloqua.', array(
      '%num' => count($new_groups),
    )), 'status');
  }
  else {
    // Link to the Eloqua REST API configuration page for configuration.
    drupal_set_message(t('Unable to load the Eloqua API client. Make sure Eloqua REST API !is_installed_and_configured_properly.', array(
      '!is_installed_and_configured_properly' => l(t('is_installed and configured properly'), 'admin/config/services/eloqua'),
    )), 'warning');
  }
}

/**
 * Takes a given path string and returns the appropriate string translation.
 *
 * @param string $path
 *   A Drupal path alias containing either a wildcard suitable for use in a
 *   hook_menu() implementation or an admin-facing token.
 *
 * @param string $direction
 *   One of "to_menu" or "to_admin", indicating the direction of translation. If
 *   "to_menu" is supplied, $path is expected to contain an human-friendly token
 *   otherwise if "to_admin" is supplied, $path is expected to contain a wild-
 *   card suitable for use in hook_menu().
 *
 * @return string
 *   The $path string translated as specified.
 */
function _eloqua_subscription_center_translate_path($path, $direction) {
  $wildcards = array('%', '%');
  $tokens = array(
    ELOQUA_SUBSCRIPTION_CENTER_EMAIL_DOMAIN_TOKEN,
    ELOQUA_SUBSCRIPTION_CENTER_CONTACT_ID_TOKEN,
  );

  switch ($direction) {
    case 'to_menu':
      $path = str_replace($tokens, $wildcards, $path);
      break;

    case 'to_admin':
      $path_parts = explode('/', $path);
      $keys = array_keys($path_parts, '%');
      foreach ($keys as $n => $key) {
        $path_parts[$key] = $tokens[$n];
      }
      $path = implode('/', $path_parts);
      break;
  }

  return $path;
}

/**
 * Returns whether or not a given path's token's placement is invalid.
 *
 * @param string $path
 *   The Drupal path alias containing the contact ID token.
 *
 * @return string
 *   If the token placement in the given path alias is valid, an empty value
 *   will be returned, otherwise an error message will be returned.
 */
function _eloqua_subscription_center_invalid_token_placement($path) {
  $domain_pos = strpos($path, ELOQUA_SUBSCRIPTION_CENTER_EMAIL_DOMAIN_TOKEN);
  $domain_len = strlen(ELOQUA_SUBSCRIPTION_CENTER_EMAIL_DOMAIN_TOKEN);
  $contact_pos = strpos($path, ELOQUA_SUBSCRIPTION_CENTER_CONTACT_ID_TOKEN);
  $contact_len = strlen(ELOQUA_SUBSCRIPTION_CENTER_CONTACT_ID_TOKEN);

  // Ensure that the given path contains the contact ID token.
  if ($domain_pos === FALSE || $contact_pos === FALSE) {
    return t('The path you specified did not include one or both of the required tokens: %domain_token, %contact_token.', array(
      '%domain_token' => ELOQUA_SUBSCRIPTION_CENTER_EMAIL_DOMAIN_TOKEN,
      '%contact_token' => ELOQUA_SUBSCRIPTION_CENTER_CONTACT_ID_TOKEN,
    ));
  }

  // Ensure that the domain token always precedes the contact token.
  if ($domain_pos > $contact_pos) {
    return t('The domain token %domain_token must always come before the %contact_token token in the path.', array(
      '%domain_token' => ELOQUA_SUBSCRIPTION_CENTER_EMAIL_DOMAIN_TOKEN,
      '%contact_token' => ELOQUA_SUBSCRIPTION_CENTER_CONTACT_ID_TOKEN,
    ));
  }

  // Tokens must never be the first item in the path part.
  if ($domain_pos === 0 || $contact_pos === 0) {
    return t('The path must not begin with a token.');
  }

  // The character immediately preceding the token must be a slash.
  if ($path[$domain_pos - 1] !== '/' || $path[$contact_pos - 1] !== '/') {
    return t('Tokens must be distinct parts of the path. Make sure that they are placed between slashes and/or are placed at the end of the path.');
  }

  // The token must be at the path's end, or be immediately followed by a slash.
  if (($domain_pos + $domain_len != strlen($path) && $path[$domain_pos + $domain_len] !== '/') || ($contact_pos + $contact_len != strlen($path) && $path[$contact_pos + $contact_len] !== '/')) {
    return t('Tokens must be distinct parts of the path. Make sure that they are placed between slashes and/or are placed at the end of the path.');
  }

  // The path must not contain the token more than once.
  if (substr_count($path, ELOQUA_SUBSCRIPTION_CENTER_EMAIL_DOMAIN_TOKEN) > 1 || substr_count($path, ELOQUA_SUBSCRIPTION_CENTER_CONTACT_ID_TOKEN) > 1) {
    return t('Tokens must not occur more than once in the specified path.');
  }

  return FALSE;
}
