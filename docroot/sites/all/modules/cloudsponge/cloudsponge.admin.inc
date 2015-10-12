<?php

function cloudsponge_admin_form() {
  $form = array();

  $form['cloudsponge_domain_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Cloudsponge Domain Key'),
    '#required' => TRUE,
    '#default_value' => variable_get('cloudsponge_domain_key', ''),
    '#description' => t("The domain API key from the CloudSponge website. !link if you don't have one.", array(
      '!link' => l('Create new account', "http://www.cloudsponge.com/signup?utm_source=drupal&utm_medium=partner&utm_campaign=integrator"
    )))
  );

  $form['cloudsponge_providers'] = array(
    '#title' => t('Providers'),
    '#type' => 'checkboxes',
    '#options' => array(
      'facebook' => t('Facebook'),
      'linkedin' => t('LinkedIn'),
      'yahoo' => t('Yahoo'),
      'windowslive' => t('Windows Live'),
      'gmail' => t('Gmail'),
      'aol' => t('Aol'),
      'plaxo' => t('Plaxo'),
      'addressbook' => t('Address Book'),
      'outlook' => t('Outlook'),
    ),
    '#default_value' => variable_get('cloudsponge_providers', array()),
  );

  $form['cloudsponge_skip_choosing'] = array(
    '#type' => 'checkbox',
    '#title' => t("Skip 'Choose Your Contacts' stage"),
    '#default_value' => variable_get('cloudsponge_skip_choosing', FALSE),
  );

  $form['redirect'] = array(
    '#type' => 'fieldset',
    '#title' => t('Redirect'),
    '#description' => t('After the user clicks the Next button on the Choose Your Contacts page.'),
    '#collapsible' => TRUE,
    '#collapsed' => !variable_get('cloudsponge_redirect', FALSE),
  );

  $form['redirect']['cloudsponge_redirect'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable'),
    '#default_value' => variable_get('cloudsponge_redirect', FALSE),
  );

  $form['redirect']['cloudsponge_redirect_url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL'),
    '#default_value' => variable_get('cloudsponge_redirect_url', ''),
    '#states' => array(
      'invisible' => array(
        ':input[name="cloudsponge_redirect"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['redirect']['tokens'] = array(
    '#theme' => 'token_tree',
    '#global_types' => TRUE,
    '#click_insert' => TRUE,
  );

  return system_settings_form($form);
}
