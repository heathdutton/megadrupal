<?php
/**
 * @file
 * Functions related to allow admin's to alter the settings and options.
 */

/**
 * Form constructor for the jabber connection form.
 *
 * @see eve_api_jabber_admin_form_validate()
 * @see eve_api_jabber_admin_form_submit()
 *
 * @ingroup forms
 */
function eve_api_jabber_admin_form($form, &$form_state) {
  $form['jabber'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configure Jabber Connection'),
    '#description' => t('Configure the Jabber Database connection settings.'),
    '#weight' => 0,
  );

  $form['jabber']['url'] = array(
    '#type' => 'textfield',
    '#title' => t('Openfire Admin URL'),
    '#description' => t('The web address to the Openfire admin page. Default http://localhost:9090'),
    '#required' => TRUE,
    '#size' => 60,
    '#maxlength' => 50,
    '#weight' => 40,
    '#default_value' => variable_get('eve_api_jabber_url', 'http://localhost:9090'),
  );

  $form['jabber']['secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Openfire Secret Password'),
    '#description' => t('The Openfire secret password to access the User Service.'),
    '#required' => TRUE,
    '#size' => 60,
    '#maxlength' => 50,
    '#weight' => 50,
    '#default_value' => variable_get('eve_api_jabber_secret', 'Password'),
  );

  $form['jabber']['domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Jabber Domain'),
    '#description' => t('The domain appended to the Jabber login username. (ex. user@yourdomain.com)'),
    '#required' => TRUE,
    '#size' => 60,
    '#maxlength' => 50,
    '#weight' => 60,
    '#default_value' => variable_get('eve_api_jabber_domain', 'yourdomain.com'),
  );

  $form['jabber']['enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Jabber Connection'),
    '#default_value' => variable_get('eve_api_jabber_enable', FALSE),
    '#weight' => 70,
  );

  if (variable_get('eve_api_first_run', FALSE) && !variable_get('eve_api_enable', FALSE)) {
    drupal_set_message(t('Please wait until the cron tasks have pulled the Alliance API Info.'));
    $form['submit_button']['#disabled'] = TRUE;
  }

  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  $form['#validate'] = array('eve_api_jabber_admin_form_validate');
  $form['#submit'] = array('eve_api_jabber_admin_form_submit');

  return $form;
}

/**
 * Form validation handler for eve_api_jabber_admin_form().
 *
 * @see eve_api_jabber_admin_form_submit()
 */
function eve_api_jabber_admin_form_validate($form, &$form_state) {
  variable_set('eve_api_jabber_url', (string) $form_state['values']['url']);
  variable_set('eve_api_jabber_secret', (string) $form_state['values']['secret']);
  variable_set('eve_api_jabber_domain', (string) $form_state['values']['domain']);

  if ((bool) $form_state['values']['enable']) {
    try {
      $result = eve_api_jabber_url_query('check');

      if ($result != 'IllegalArgumentException') {
        form_set_error('enable', t('Please check your Openfire URL settings and try again.'));
        variable_set('eve_api_jabber_enable', FALSE);
      }
    }
    catch (EveApiException $e) {
      form_set_error('enable', t('Please check your Openfire URL settings and try again.'));
      variable_set('eve_api_jabber_enable', FALSE);
    }
  }
}

/**
 * Form submission handler for eve_api_jabber_admin_form().
 *
 * @see eve_api_jabber_admin_form_validate()
 */
function eve_api_jabber_admin_form_submit($form, &$form_state) {
  if ((bool) $form_state['values']['enable']) {
    variable_set('eve_api_jabber_enable', TRUE);
  }
  else {
    variable_set('eve_api_jabber_enable', FALSE);
  }

  menu_rebuild();

  drupal_set_message(t('Jabber Settings Updated.'));
}
