<?php
/**
 * @file
 * Provides admin settings page to adjust HubSpot API key, debugging settings,
 * JavaScript embedding, and form submission settings.
 */

/**
 * Form constructor for the Hubspot admin settings form.
 *
 * @see hubspot_admin_settings_validate()
 * @see hubspot_admin_settings_submit()
 */
function hubspot_admin_settings() {
  $form = array();

  $form['additional_settings'] = array(
    '#type' => 'vertical_tabs',
  );

  $form['settings'] = array(
    '#title' => t('Connectivity'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#group' => 'additional_settings',
  );

  $form['settings']['hubspot_portalid'] = array(
    '#title' => t('HubSpot Portal ID'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('hubspot_portalid', ''),
    '#description' => t('Enter the Hubspot Portal ID for this site.  It can be found by
      <a href="https://login.hubspot.com/login" target="_blank">logging into HubSpot</a> going to the Dashboard and
      examining the url. Example: "https://app.hubspot.com/dashboard-plus/12345/dash/".  The number after
      "dashboard-plus" is your Portal ID.'),
  );

  if (variable_get('hubspot_portalid', '')) {
    $form['settings']['hubspot_authentication'] = array(
      '#value' => t('Connect Hubspot Account'),
      '#type' => 'submit',
      '#validate' => array(),
      '#submit' => array('hubspot_oauth_submit'),
    );

    if (variable_get('hubspot_refresh_token', '')) {
      $form['settings']['hubspot_authentication']['#suffix'] = t('Your Hubspot account is connected.');
      $form['settings']['hubspot_authentication']['#value'] = t('Disconnect Hubspot Account');
      $form['settings']['hubspot_authentication']['#submit'] = array('hubspot_oauth_disconnect');
    }
  }

  $form['settings']['hubspot_log_code'] = array(
    '#title' => t('HubSpot Traffic Logging Code'),
    '#type' => 'textarea',
    '#default_value' => variable_get('hubspot_log_code', ''),
    '#description' => t('To enable HubSpot traffic logging on your site, paste the External Site Traffic Logging code
      here.'),
  );

  $form['debug'] = array(
    '#title' => t('Debugging'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#group' => 'additional_settings',
  );

  $form['debug']['hubspot_debug_on'] = array(
    '#title' => t('Debugging enabled'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('hubspot_debug_on', 0),
    '#description' => t('If debugging is enabled, HubSpot errors will be emailed to the address below. Otherwise, they
      will be logged to the regular Drupal error log.'),
  );

  $form['debug']['hubspot_debug_email'] = array(
    '#title' => t('Debugging email'),
    '#type' => 'textfield',
    '#default_value' => variable_get('hubspot_debug_email', variable_get('site_mail', '')),
    '#description' => t('Email error reports to this address if debugging is enabled.'),
  );

  $form['webforms'] = array(
    '#title' => t('Webforms'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#group' => 'additional_settings',
    '#description' => 'The following webforms have been detected and can be configured to submit to the HubSpot API.',
    '#tree' => TRUE,
  );

  $webform_nodes = variable_get('webform_node_types', array('webform'));
  $nodes = array();

  $hubspot_forms = _hubspot_get_forms();

  if (isset($hubspot_forms['error'])) {
    $form['webforms']['#description'] = $hubspot_forms['error'];
  }
  else {
    if (empty($hubspot_forms['value'])) {
      $form['webforms']['#description'] = t('No HubSpot forms found. You will need to create a form on HubSpot before you can configure it here.');
    }
    else {
      $hubspot_form_options = array("--donotmap--" => "Do Not Map");
      $hubspot_field_options = array();
      foreach ($hubspot_forms['value'] as $hubspot_form) {
        $hubspot_form_options[$hubspot_form['guid']] = $hubspot_form['name'];
        $hubspot_field_options[$hubspot_form['guid']]['fields']['--donotmap--'] = "Do Not Map";
        foreach ($hubspot_form['fields'] as $hubspot_field) {
          $hubspot_field_options[$hubspot_form['guid']]['fields'][$hubspot_field['name']] = $hubspot_field['label'] . ' (' . $hubspot_field['fieldType'] . ')';
        }
      }

      foreach ($webform_nodes as $node_type) {
        $query = new EntityFieldQuery();

        $query->entityCondition('entity_type', 'node')
          ->entityCondition('bundle', $node_type)
          ->propertyCondition('status', 1);

        $result = $query->execute();

        if (isset($result['node'])) {
          $node_ids = array_keys($result['node']);
          $nodes = array_merge($nodes, entity_load('node', $node_ids));
        }
      }

      foreach ($nodes as $node) {
        $nid = $node->nid;
        $form['webforms']['nid-' . $nid] = array(
          '#title' => $node->title,
          '#type' => 'fieldset',
        );

        $form['webforms']['nid-' . $nid]['hubspot_form'] = array(
          '#title' => t('HubSpot form'),
          '#type' => 'select',
          '#options' => $hubspot_form_options,
          '#default_value' => _hubspot_default_value($nid),
        );

        foreach ($hubspot_form_options as $key => $value) {
          if ($key != '--donotmap--') {
            $form['webforms']['nid-' . $nid][$key] = array(
              '#title' => t('Field mappings for @field', array('@field' => $value)),
              '#type' => 'fieldset',
              '#states' => array(
                'visible' => array(
                  ':input[name="webforms[nid-' . $nid . '][hubspot_form]"]' => array('value' => $key),
                ),
              ),
            );

            foreach ($node->webform['components'] as $component) {
              if ($component['type'] !== 'markup') {
                $form['webforms']['nid-' . $nid][$key][$component['form_key']] = array(
                  '#title' => $component['name'] . ' (' . $component['type'] . ')',
                  '#type' => 'select',
                  '#options' => $hubspot_field_options[$key]['fields'],
                  '#default_value' => _hubspot_default_value($nid, $key, $component['form_key']),
                );
              }
            }
          }
        }
      }
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => ('Save Configuration'),
  );

  return $form;
}

/**
 * Submit handler for hubspot_admin_settings().
 */
function hubspot_admin_settings_submit($form, &$form_state) {
  variable_set('hubspot_portalid', $form_state['values']['hubspot_portalid']);
  variable_set('hubspot_debug_email', $form_state['values']['hubspot_debug_email']);
  variable_set('hubspot_debug_on', $form_state['values']['hubspot_debug_on']);
  variable_set('hubspot_log_code', $form_state['values']['hubspot_log_code']);

  $txn = db_transaction();

  // Check if webform values even exist before continuing.
  if (isset($form_state['values']['webforms'])) {
    foreach ($form_state['values']['webforms'] as $key => $settings) {
      db_delete('hubspot')->condition('nid', str_replace('nid-', '', $key))->execute();

      if ($settings['hubspot_form'] != '--donotmap--') {
        foreach ($settings[$settings['hubspot_form']] as $webform_field => $hubspot_field) {
          $fields = array(
            'nid' => str_replace('nid-', '', $key),
            'hubspot_guid' => $settings['hubspot_form'],
            'webform_field' => $webform_field,
            'hubspot_field' => $hubspot_field,
          );
          db_insert('hubspot')->fields($fields)->execute();
        }
      }
    }
  }

  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Form validation handler for hubspot_admin_settings().
 *
 * Ensures that the debug email address provided is valid if debugging is
 * enabled.
 */
function hubspot_admin_settings_validate($form, &$form_state) {
  if ($form_state['values']['hubspot_debug_on'] &&
      !valid_email_address($form_state['values']['hubspot_debug_email'])) {
    form_set_error('hubspot_debug_email', t('You must provide a valid email address.'));
  }

}

/**
 * Form submission handler for hubspot_admin_settings().
 */
function hubspot_oauth_submit($form, &$form_state) {
  $data = array(
    'client_id' => HUBSPOT_CLIENT_ID,
    'portalId' => $form_state['values']['hubspot_portalid'],
    'redirect_uri' => url('hubspot/oauth', array('query' => drupal_get_destination(), 'absolute' => TRUE)),
    'scope' => HUBSPOT_SCOPE,
  );

  $form_state['redirect'][] = url('https://app.hubspot.com/auth/authenticate', array('query' => $data));
}

/**
 * Page callback.
 *
 * Saves OAuth tokens from HubSpot and redirects user.
 */
function hubspot_oauth_connect() {
  if (!empty($_GET['access_token']) && !empty($_GET['refresh_token']) && !empty($_GET['expires_in'])) {
    drupal_set_message(t('Successfully authenticated with Hubspot.'), 'status', FALSE);

    variable_set('hubspot_access_token', $_GET['access_token']);
    variable_set('hubspot_refresh_token', $_GET['refresh_token']);
    variable_set('hubspot_expires_in', $_GET['expires_in']);
  }

  if (!empty($_GET['error']) && $_GET['error'] == "access_denied") {
    drupal_set_message(t('You denied the request for authentication with Hubspot. Please click the button again and
      choose the AUTHORIZE option.'), 'error', FALSE);
  }

  drupal_goto();
}

/**
 * Form submit handler.
 *
 * Deletes Hubspot OAuth tokens.
 */
function hubspot_oauth_disconnect($form, &$form_state) {
  variable_del('hubspot_access_token');
  variable_del('hubspot_refresh_token');
  variable_del('hubspot_expires_in');
}
