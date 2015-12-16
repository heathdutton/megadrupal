<?php

/**
 * @file
 * Provides the menu callback for the Formstack administration page.
 */

/**
* Page callback: Formstack settings
*
* @see formstack_menu()
*/
function formstack_settings_form($form, &$form_state) {

  $form = array();

  $form['application'] = array(
    '#title' => t('Step 1: Register Application'),
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#group' => 'settings',
  );
  $form['application']['instructions'] = array(
    '#markup' => formstack_application_instructions_markup(),
  );

  $form['authorization'] = array(
    '#title' => t('Step 2: Authorization'),
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#group' => 'settings',
  );
  $form['authorization']['instructions'] = array(
    '#markup' => formstack_authorization_instructions_markup(),
  );
  $form['authorization']['formstack_access_token'] = array(
    '#title' => t('Access Token'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('formstack_access_token', ''),
  );

  return system_settings_form($form);
}

/**
 * Implementation of hook_form_validate().
 */
function formstack_settings_form_validate($form, &$form_state) {
  $forms = Formstack::form($form_state['values']['formstack_access_token']);
  if (is_numeric($forms)) {
    form_set_error('formstack', t('Formstack Error: @errorcode @errormessage', array('@errorcode' => $forms, '@errormessage' => _formstack_error_message($forms))));
  }
}

function formstack_application_instructions_markup() {
  global $base_url;
  $site_name = variable_get('site_name', '');
  $redirect_url = $base_url . '/' . current_path();
  $output =  <<<FAP
<h2>You must first register a new application in your Formstack account.</h2>
<p>Login to your Formstack account and proceed to <a href="https://www.formstack.com/developers/applications/add" target="_blank" >https://www.formstack.com/developers/applications/add</a></p>

<p>In the fields enter the following values:</p>
<ul>
  <li><strong>Application Name:</strong> $site_name Website</li>
  <li><strong>Redirect URI:</strong> $redirect_url</li>
  <li><strong>Description:</strong> <em>(anything you want)</em></li>
  <li><strong>Logo:</strong> <em>(optional)</em></li>
  <li><strong>Platform:</strong> Website</li>
</ul>
<p>Click "Create Application" button</p>
FAP;

  return $output;
}

function formstack_authorization_instructions_markup() {

  $output = <<<FAU
<h3>Enter the your Access Token below from the application you created in Step 1.</h3>
<p>From the <a href="https://www.formstack.com/developers/applications/" target"_blank">Applications page on Formstack</a>, click on the application icon. You will find the details in a box on the right side of the window</p>
FAU;

  return $output;
}
