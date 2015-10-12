<?php

/**
 * @file
 * ORCID administrative functions, forms.
 */

function orcid_integration_services_configuration($form, &$form_state) {
  global $base_url;
  
  $form['orcid_integration_main_url'] = array(
    '#title' => t('ORCID Main URL'),
    '#type' => 'textfield',
    '#description' => t('Please enter the main orcid url, such as http://orcid.org/ or http://sandbox.orcid.org/'),
    '#required' => TRUE,
    '#default_value' => variable_get('orcid_integration_main_url', 'http://sandbox.orcid.org/'),
  );
  $form['orcid_integration_public_api_url'] = array(
    '#title' => t('ORCID API URL'),
    '#type' => 'textfield',
    '#description' => t('Please enter the public orcid api url as per !link. If a members api search is not possible, we will use the public query api instead.', array('!link' => 'http://support.orcid.org/knowledgebase/articles/116874-orcid-api-guide')),
    '#required' => TRUE,
    '#default_value' => variable_get('orcid_integration_public_api_url', 'http://api.sandbox.orcid.org/'),
  );
  $form['oauth'] = array(
    '#type' => 'fieldset',
    '#title' => 'OAUTH Credentials',
    '#description' => 'If OAUTH credentials are provided, this will allow for actions beyond basic search.',
    '#collapsed' => FALSE,
    '#collapsible' => FALSE,
  );
  $form['oauth']['orcid_integration_members_api_url'] = array(
    '#title' => t('ORCID Members API URL'),
    '#type' => 'textfield',
    '#description' => t('Please enter the members orcid api url as per !link. If Member tokens are provided, this will be preferred over the public api.', array('!link' => 'http://support.orcid.org/knowledgebase/articles/116874-orcid-api-guide')),
    '#default_value' => variable_get('orcid_integration_members_api_url', ''),
  );
  $form['oauth']['orcid_integration_api_client_id'] = array(
    '#title' => t('ORCID API client ID'),
    '#type' => 'textfield',
    '#description' => t('If you are using OAUTH for any major integration (more orcid member information, ability to create/edit member information), please enter the ORCID api Client ID'),
    '#default_value' => variable_get('orcid_integration_api_client_id', ''),
  );
  $form['oauth']['orcid_integration_api_client_secret'] = array(
    '#title' => t('ORCID API client secret'),
    '#type' => 'textfield',
    '#description' => t('If you are using OAUTH (for reasons above), please also enter the client secret'),
    '#default_value' => variable_get('orcid_integration_api_client_secret', ''),
  );
  $form['oauth']['orcid_integration_api_redirect_url'] = array(
    '#title' => t('ORCID API redirect url'),
    '#type' => 'textfield',
    '#description' => t('If it is different from the site, please enter the redirect url provided to ORCID'),
    '#default_value' => variable_get('orcid_integration_api_redirect_url', $base_url),
  );
  return system_settings_form($form);
}

function orcid_integration_services_configuration_validate($form, &$form_state) {
  // Test public search uri
  $values = $form_state['values'];
  $api_url = orcid_integration_generate_public_search_url($values['orcid_integration_public_api_url']);
  $url_options = array(
    'query' => array(
      'q' => 'test'
    ),
  );
  $response = orcid_integration_search_request($api_url, $url_options, array());
  if ($response->code !== "200") {
    form_set_error('orcid_integration_api_url', t('Public API URL is not correct. No results found on "test" search.'));
  }

  if (!empty($values['orcid_integration_members_api_url'])) {
    if (empty($values['orcid_integration_api_client_id'])) {
      form_set_error('orcid_integration_api_client_id', t('Please provide a client ID.'));
    }
    else if (empty($values['orcid_integration_api_client_secret'])) {
      form_set_error('orcid_integration_api_client_id', t('Please provide a client secret.'));
    }
    else {
      // Test member search uri
      $request = orcid_integration_generate_member_search_url($values['orcid_integration_api_client_id'], $values['orcid_integration_api_client_secret'], $values['orcid_integration_members_api_url'], $values['orcid_integration_api_redirect_url']);
      if (!$request) {
        form_set_error(NULL, t('Please recheck your cliend ID/secret.'));
      } else {
        $url_options = array(
          'query' => array(
            'q' => 'test'
          ),
        );
        orcid_integration_search_request($request['api_url'], $url_options, $request);
        $response = orcid_integration_search_request($request['api_url'], $url_options, $request);
        if ($response->code !== "200") {
          form_set_error('orcid_integration_members_api_url', t('Members API URL is not correct. No results found on "test" search.'));
        }
      }
    }
  }
}
