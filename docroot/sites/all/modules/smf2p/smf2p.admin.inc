<?php

/**
 * @file
 * smf2p module administration and module settings UI.
 */

/*
 * Common Settings form.
*/
function smf2p_common_settings() {
  $form = array();

  $form['smf2p_url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL to SMF2 site'),
    '#description' => t('Absolute URL to SMF2 forum. For example, http://forum.example.com'),
    '#default_value' => variable_get('smf2p_url'),
  );
  $form['smf2p_url_api'] = array(
    '#type' => 'textfield',
    '#title' => t('URL to SMF2 REST API site'),
    '#description' => t('Absolute URL to SMF2 REST API. For example, http://forum.example.com/api'),
    '#default_value' => variable_get('smf2p_url_api'),
  );
  $form['smf2p_secretkey'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret key to SMF2'),
    '#description' => t('You should get it from SmfRestServer.php file.'),
    '#default_value' => variable_get('smf2p_secretkey'),
  );
  $form['smf2p_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Mail of user'),
    '#description' => t('Mail of user, who will post to SMF2 site.'),
    '#default_value' => variable_get('smf2p_email'),
  );
  
  $connected = FALSE;
  
  if (variable_get('smf2p_secretkey')) {
    $api = smf2p_connect2forum();
    if ($api) {
      drupal_set_message(t('SMF2 REST API Successfully connected.'));
      $smf2_user = $api->login_user(variable_get('smf2p_email'));
      if (isset($smf2_user->data->id_member) && $smf2_user->data->id_member) {
        drupal_set_message(t('User !smf2user successfully connected.', array('!smf2user' => variable_get('smf2p_email'))));
        $connected = TRUE;
      }
      else {
        drupal_set_message(t('User !smf2user connection error.', array('!smf2user' => variable_get('smf2p_email'))), 'error');
      }
    }
    else {
      drupal_set_message(t('SMF2 REST API connection error.'), 'error');
    }
  }
  
  if ($connected) {
    $opts = array();
    foreach (node_type_get_types() as $type) {
      if (isset($type->type)) {
        $opts[$type->type] = $type->name;
      }
    }

    $form['templates'] = array(
      '#type' => 'fieldset',
      '#title' => t('Node settings and Forum message templates'),
      '#collapsible' => true,
      '#collapsed' => false,
    );
    
    $form['templates']['smf2p_nodetypes'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Node types'),
      '#description' => t('Set allowed node types that will posted to forum. If no one selected, all will chosen.'),
      '#default_value' => variable_get('smf2p_nodetypes', array()),
      '#options' => $opts,
    );
    $form['templates']['smf2p_link2forum_field'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show link to forum topic at node view as field'),
      '#description' => t('Otherwise it will shown in links.'),
      '#default_value' => variable_get('smf2p_link2forum_field'),
    );
    
    $form['templates']['smf2p_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Message title'),
      '#default_value' => variable_get('smf2p_title'),
    );

    $form['templates']['smf2p_body'] = array(
      '#type' => 'textarea',
      '#title' => t('Message body'),
      '#default_value' => variable_get('smf2p_body'),
      '#description' => t('For tokens that generating only after submitting or generated incorrectly, replace [ ] to {{ }}.'),
    );

    $form['templates']['token_tree'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('node'),
    );
    
  }

  // Store that API is connected.
  variable_set('smf2p_connected', $connected);
    
  return system_settings_form($form);
}

