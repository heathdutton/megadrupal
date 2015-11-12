<?php

require_once dirname(__FILE__) . '/pages.php';
use \Drupal\share_light\Loader;

/**
 * Menu/form callback: settings form.
 */
function share_light_settings_form() {

  $form['share_light_tracking_enabled'] = array(
    '#title' => t('Enable tracking of share link clicks.'),
    '#type' => 'checkbox',
    '#access' => module_exists('googleanalytics'),
    '#description' => t('If set to "1" and tracking modules are enabled, then tracking events will be sent to Google Analytics.'),
    '#default_value' => variable_get('share_light_tracking_enabled', '1'),
  );

  $options = Loader::instance()->channelOptions();
  $form['share_light_channels_enabled'] = array(
    '#title' => t('Enable social media channels for sharing.'),
    '#type' => 'checkboxes',
    '#options' => $options,
    '#description' => t('The social media channels available for sharing.'),
    '#default_value' => variable_get('share_light_channels_enabled', array_keys($options)),
  );

  return system_settings_form($form);
}

/**
 * Menu/form callback: email settings form.
 */
function share_light_email_admin_form($form, &$form_state) {
  $defaults = share_light_email_defaults();
  $prefix = 'share_light_email_';
  $form = _share_light_email_settings_form($defaults, $prefix);

  $form['page'][$prefix . 'page_noindex'] = array(
    '#type' => 'checkbox',
    '#title' => t('Generate a noindex meta-tag on the share via email page'),
    '#default_value' => $defaults['share_light_email_page_noindex'],
  );

  // Flood Control
  $form['flood_control'] = array(
    '#type' => 'fieldset',
    '#title' => t('Flood Control'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['flood_control']['share_light_email_flood_control'] = array(
    '#type' => 'textfield',
    '#title' => t('Flood control limit'),
    '#default_value' => $defaults['share_light_email_flood_control'],
    '#description' => t('How many times a user can use the form in a one hour period. This will help prevent the email form from being used for spamming.'),
  );
  $form['flood_control']['share_light_email_flood_error'] = array(
    '#type' => 'textarea',
    '#title' => t('Flood control error'),
    '#default_value' => $defaults['share_light_email_flood_error'],
    '#cols' => 40,
    '#rows' => 10,
    '#description' => t('This text appears if a user exceeds the flood control limit.  The value of the flood control limit setting will appear in place of !number in the message presented to users'),
  );

  return system_settings_form($form);
}

function share_light_node_email_settings($form, &$form_state, $node) {
  $form_state['node'] = $node;
  $defaults = share_light_email_settings($node);
  $form = _share_light_email_settings_form($defaults);

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));
  $form['#submit'][] = 'share_light_node_email_settings_submit';
  return $form;
}

function share_light_node_email_settings_submit($form, &$form_state) {
  $data['nid'] = $form_state['node']->nid;

  $keys = array('page_title', 'page_instructions', 'page_redirect', 'message_edit', 'message_subject', 'message_message', 'message_footer');
  foreach ($keys as $key) {
    $data[$key] = $form_state['values'][$key];
  }
  db_merge('share_light_email_settings')
    ->key(array('nid' => $data['nid']))
    ->fields($data)
    ->execute();
  drupal_set_message(t('The configuration options have been saved.'));
}

function _share_light_email_settings_form($defaults, $prefix = '') {
  // General Options
  $form['page'] = array(
    '#type' => 'fieldset',
    '#title' => t('Share page settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['page'][$prefix . 'page_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $defaults['share_light_email_page_title'],
    '#size' => 40,
    '#maxlength' => 256,
    '#description' => t('Title to display above the Forward page form'),
  );
  $form['page'][$prefix . 'page_instructions'] = array(
    '#type' => 'textarea',
    '#title' => t('Forward Instructions'),
    '#default_value' => $defaults['share_light_email_page_instructions'],
    '#cols' => 40,
    '#rows' => 10,
    '#description' => t('This message will be displayed above the form.  The token [site:name] will be replaced with the site name.'),
  );
  $form['page'][$prefix . 'page_redirect'] = array(
    '#type' => 'textfield',
    '#title' => t('Redirect path'),
    '#description' => t('Path to the page that to user is redirected to after submitting the share form.'),
    '#default_value' => $defaults['share_light_email_page_redirect'],
  );

  $form['message'] = array(
    '#type' => 'fieldset',
    '#title' => t('Email settings'),
  );
  $form['message'][$prefix . 'message_edit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Users can edit the message.'),
    '#default_value' => $defaults['share_light_email_message_edit'],
    '#description' => t('Choose whether users can edit the share message.'),
  );
  $form['message'][$prefix . 'message_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => $defaults['share_light_email_message_subject'],
    '#size' => 40,
    '#maxlength' => 256,
    '#description' => t('Email subject line. Replacement tokens, as found below, may be used'),
  );
  $form['message'][$prefix . 'message_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message Body'),
    '#default_value' => $defaults['share_light_email_message_message'],
    '#cols' => 40,
    '#rows' => 10,
    '#description' => t('Email message body. Replacement tokens, as found below, may be used. The sender will be able to add their own message after this.'),
  );
  $form['message'][$prefix . 'message_footer'] = array(
    '#type' => 'textarea',
    '#title' => t('Footer'),
    '#default_value' => $defaults['share_light_email_message_footer'],
    '#cols' => 40,
    '#rows' => 10,
    '#description' => t("This part of the message is rendered below the user's message and is not editable by the user."),
  );

  // Replacement tokens
  $form['token_help'] = array(
    '#title' => t('Replacement patterns'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['token_help']['help'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array('node', 'user', 'share'),
  );

  return $form;
}
