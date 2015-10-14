<?php

function signup_share_track_settings() {
  $form['sst_url_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sign Up Share Track Url settings'),
    '#weight' => 5,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => 0,
  );
  $form['sst_url_settings']['sst_allowed_char'] = array(
    '#type' => 'textfield',
    '#title' => t('Allows character for generate user url'),
    '#default_value' => variable_get('sst_allowed_char', 'QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm'),
    '#required' => TRUE,
  );
  $form['sst_url_settings']['sst_link_length'] = array(
    '#type' => 'textfield',
    '#title' => t('Num character for url'),
    '#default_value' => variable_get('sst_link_length', '3'),
    '#required' => TRUE,
  );
  $form['sst_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sign Up Share Track settings'),
    '#weight' => 5,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => 1,
  );
  $form['sst_settings']['sst_title'] = array(
    '#title' => t('Title'),
    '#type' => 'textfield',
    '#weight' => '-6',
    '#default_value' => variable_get('sst_title', 'Join The Revolution'),
    '#required' => '1',
    '#description' => t('Display title to other users.'),
  );
  $form['sst_settings']['sst_title_user'] = array(
    '#title' => t('User Title'),
    '#type' => 'textfield',
    '#weight' => '-5',
    '#default_value' => variable_get('sst_title_user', 'Your Contribution to Leading Revolution'),
    '#required' => '1',
    '#description' => t('Display title when loged in user at his share page.'), 
  );
  $form['sst_settings']['sst_text'] = array(
    '#title' => t('Text'),
    '#type' => 'textarea',
    '#row' => 2,
    '#col' => 5,
    '#weight' => '-4',
    '#default_value' => variable_get('sst_text', '<username> Recommends You Join The Revolution, Sign Up Now'),
    '#description' => t('You can use variable &lt;username&gt; for display name of page owner.'),
    '#required' => '1',
  );
  $form['sst_settings']['sst_button_text'] = array(
    '#title' => t('Button text'),
    '#type' => 'textfield',
    '#weight' => '-3',
    '#default_value' => variable_get('sst_button_text', 'Signup Now'),
    '#required' => '1',
  );
  $form['sst_settings']['sst_skip_signup'] = array(
    '#type' => 'checkboxes', 
    '#title' => t('Skip Registration Page'),
    '#default_value' => array(
      '1' => variable_get('sst_skip_signup', '1')
    ),
    '#options' => array(
      '1' => t('Skip Signup'),
    ),
  );
  $form['sst_settings']['sst_share_checkboxes'] = array(
    '#multiple' => '1',
    '#required' => '0',
    '#weight' => '1',
    '#default_value' => array(
      '1' => variable_get('sst_addthis', 1),
      '2' => variable_get('sst_jiathis', 2),
    ),
    '#type' => 'checkboxes',
    '#title' => t('Share buttons'),
    '#options' => array(
      '1' => t('Addthis'),
      '2' => t('JiaThis'),
    ),
  );  
  $form['sst_settings']['sst_form_text'] = array(
  '#title' => t('Block Form Text'),
  '#type' => 'textarea',
  '#row' => 2,
  '#col' => 5,
  '#weight' => '2',
  '#default_value' => variable_get('sst_form_text', ''),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Configuration'),
    '#weight' => 3,
  );
  return $form;
}

function signup_share_track_settings_validate($form, $form_state) {
  $values = $form_state['values'];
  if (!is_numeric($values['sst_link_length'])) {
    form_set_error('sst_link_length', t('You can use only numbers'));
  }
  elseif ($values['sst_link_length'] > 200) {
    form_set_error('sst_link_length', t('Max number 200'));
  }
}

function signup_share_track_settings_submit($form, $form_state) {
  $values = $form_state['values'];
  variable_set('sst_allowed_char', $values['sst_allowed_char']);
  variable_set('sst_link_length', $values['sst_link_length']);
  variable_set('sst_addthis', $values['sst_share_checkboxes']['1']);
  variable_set('sst_jiathis', $values['sst_share_checkboxes']['2']);
  variable_set('sst_title', $values['sst_title']);
  variable_set('sst_title_user', $values['sst_title_user']);
  variable_set('sst_text', $values['sst_text']);
  variable_set('sst_button_text', $values['sst_button_text']);
  variable_set('sst_skip_signup', $values['sst_skip_signup']['1']);
  variable_set('sst_form_text', $values['sst_form_text']);
  drupal_set_message('Configuration has been saved.');
  
}

