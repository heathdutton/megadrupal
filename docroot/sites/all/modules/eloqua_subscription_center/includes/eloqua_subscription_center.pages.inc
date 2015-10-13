<?php

/**
 * @file
 * Page callbacks and functions for the Eloqua Subscription Center module.
 */


/**
 * Primary page callback for managing subscriptions.
 *
 * @param string $email_domain
 *   The domain for the given contact's e-mail address. This value is used to
 *   verify the authenticity of the contact's subscription management request.
 *
 * @param string $contact_id
 *   The Eloqua contact ID for whom we are loading/editing subscription details.
 *
 * @return
 *   A Drupal form representing this Eloqua contact's subscription settings.
 */
function eloqua_subscription_center_page($email_domain, $contact_id) {
  // Ensure that this never gets indexed.
  drupal_add_html_head(array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'robots',
      'content' => 'noindex, nofollow, noarchive',
    ),
  ), 'eloqua_subscription_center_robots');

  // Ensure that this page isn't cached.
  drupal_page_is_cacheable(FALSE);
  drupal_add_http_header('cache-control', 'no-cache, no-store, must-revalidate');

  // Only respond if we're properly configured to use Eloqua's REST API.
  if ($elqReq = eloqua_rest_api_client('2.0')) {
    $contacts = $elqReq->api('contacts');

    // The contact ID passed by Eloqua
    if (preg_match_all ('/.*?(\\d+)/is', $contact_id, $matches)) {
      // Cast the result to an integer to remove preceding 0s.
      $contact_id = (int) $matches[1][0];
    }

    // Only respond with the form when a valid contact is returned.
    if ($contact = $contacts->show($contact_id)) {
      // Check the provided e-mail domain with the contact's e-mail. If there's
      // no match, do not return the form.
      if (strpos($contact['emailAddress'], '@' . $email_domain) === FALSE) {
        return MENU_ACCESS_DENIED;
      }

      $subscriptions = $contacts->subscriptions($contact_id)->search('*');
      $form_state = array();
      $form_state['build_info']['args']['contact'] = $contact;
      $form_state['build_info']['args']['subscriptions'] = $subscriptions['elements'];

      return drupal_build_form('eloqua_subscription_center_form', $form_state);
    }
    else {
      watchdog('eloqua subscription center', 'Unable to return a contact for the given ID %id.', array(
        '%id' => $contact_id,
      ), WATCHDOG_WARNING);
    }
  }
  else {
    watchdog('eloqua subscription center', 'Unable to instantiate an Eloqua Request object. Make sure Eloqua REST API is properly configured.', WATCHDOG_ERROR);
  }

  // If we're here, either the contact doesn't exist, or we're not configured to
  // use the Eloqua REST API. Throw a 404.
  return MENU_NOT_FOUND;
}

/**
 * Form for managing Eloqua subscriptions.
 */
function eloqua_subscription_center_form($form, &$form_state) {
  $contact = $form_state['build_info']['args']['contact'];
  $subscriptions = $form_state['build_info']['args']['subscriptions'];
  $defaultSub = variable_get('eloqua_subscription_center_default_status', TRUE);

  // Eloqua form name to which this will be posted.
  if ($elqFormName = variable_get('eloqua_subscription_center_form_name', '')) {
    $form['elqFormName'] = array(
      '#type' => 'hidden',
      '#default_value' => $elqFormName,
    );
  }

  // If configured, show the user's e-mail address.
  $type = variable_get('eloqua_subscription_center_hide_email', FALSE) ? 'hidden' : 'textfield';
  $form['email'] = array(
    '#title' => t('E-mail Address'),
    '#default_value' => $contact['emailAddress'],
    '#type' => $type,
    '#weight' => 0,
    '#disabled' => TRUE,
  );

  // A fieldset for all relevant e-mail groups.
  $form['email_groups'] = array(
    '#title' => t('E-mail Subscriptions'),
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#weight' => 1,
  );

  // Iterate through all subscriptions and create radios for relevant groups.
  foreach ($subscriptions as $sub) {
    // Load any config overrides.
    $config = eloqua_subscription_center_get_group_config($sub['emailGroup']['id']);
    unset($config['#default_value'], $config['#options'], $config['#type']);
    if (!array_key_exists('isSubscribed', $sub)) {
      $sub['isSubscribed'] = $defaultSub ? 'true' : 'false';
    }

    // Only display subscription preferences for explicitly enabled groups.
    if (!in_array($sub['emailGroup']['id'], eloqua_subscription_center_get_enabled_groups())) {
      continue;
    }

    // Load values either from Eloqua, or from group config overrides.
    $id = isset($config['#id']) ? $config['#id'] : $sub['emailGroup']['id'];
    $form['email_groups'][$id] = array(
      '#title' => $sub['emailGroup']['name'],
      '#description' => $sub['emailGroup']['description'],
      '#type' => 'radios',
      '#options' => array(TRUE => t('Subscribed'), FALSE => t('Unsubscribed')),
      '#default_value' => strtolower($sub['isSubscribed']) === 'true' ? 1 : 0,
    );
    unset($config['#id']);
    $form['email_groups'][$id] = array_merge($form['email_groups'][$id], $config);
  }

  foreach(element_children($form['email_groups']) as $key) {
    $form['email_groups'][$key]['#title'] = eloqua_subscription_center_translate('form:subscription:' . $key . ':title', $form['email_groups'][$key]['#title']);
    $form['email_groups'][$key]['#description'] = eloqua_subscription_center_translate('form:subscription:' . $key . ':description', $form['email_groups'][$key]['#description']);
  }

  // Submit button.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update subscription'),
    '#weight' => 10,
  );

  $form['#submit'][] = 'eloqua_subscription_center_api_integration';
  $form['#submit'][] = 'eloqua_subscription_center_responses';

  return $form;
}

/**
 * Submit handler to ensure this form integrates with Eloqua API, regardless of
 * how the Eloqua API module is configured.
 */
function eloqua_subscription_center_api_integration($form, &$form_state) {
  if (!isset($form_state['eloqua_values'])) {
    $form_state['eloqua_values'] = array();
  }

  $form_state['eloqua_values'] = array_merge($form_state['eloqua_values'], $form_state['values']);
}

/**
 * Submit handler that handles successful form submission responses and actions.
 */
function eloqua_subscription_center_responses($form, &$form_state) {
  $actions = variable_get('eloqua_subscription_center_form_responses', array());

  // Confirmation message handling.
  if (isset($actions['message']) && $actions['message'] === 'message') {
    if ($message = variable_get('eloqua_subscription_center_confirmation_message', '')) {
      drupal_set_message(filter_xss_admin($message), 'status');
    }
  }

  // Confirmation redirect handling.
  if (isset($actions['redirect']) && $actions['redirect'] === 'redirect') {
    if ($redirect = variable_get('eloqua_subscription_center_confirmation_redirect', '')) {
      $form_state['redirect'] = $redirect;
    }
  }
}

/**
 * Primary page callback for pointing the current user to his/her subscription
 * center page (without having to know e-mail domain or CID.
 */
function eloqua_subscription_center_my_page() {
  // Get the e-mail address with which we'll search all contacts.
  $email = $GLOBALS['user']->mail;
  drupal_alter('eloqua_subscription_center_my_email', $email);

  // Search for contact with the e-mail address from above.
  $client = eloqua_rest_api_client()->api('contacts');
  $contacts = $client->search($email, array('depth' => 'complete'));
  if (isset($contacts['elements']) && count($contacts['elements'])) {
    // Get contact details we need.
    $contact = $contacts['elements'][0];
    $cid = $contact['id'];
    $emailParts = explode('@', $contact['emailAddress']);
    $emailDomain = $emailParts[1];

    // Load the path variable in such a way we can replace the path-part tokens.
    module_load_include('inc', 'eloqua_subscription_center', 'includes/eloqua_subscription_center.admin');
    $path = variable_get('eloqua_subscription_center_path', 'subscription-center/%/%');
    $path = _eloqua_subscription_center_translate_path($path, 'to_admin');

    // Replace path-part tokens with values obtained above, then redirect.
    $replace = array(
      ELOQUA_SUBSCRIPTION_CENTER_EMAIL_DOMAIN_TOKEN,
      ELOQUA_SUBSCRIPTION_CENTER_CONTACT_ID_TOKEN
    );
    $path = str_replace($replace, array($emailDomain, $cid), $path);

    drupal_goto($path);
  }
  else {
    return drupal_access_denied();
  }
}
