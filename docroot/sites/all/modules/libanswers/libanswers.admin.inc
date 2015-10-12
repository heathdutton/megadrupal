<?php

/**
 * @file
 * Admin page callbacks.
 */


/**
 * Admin settings form
 */
function libanswers_settings() {
  $form = array();
  
  $form['libanswers_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('LibAnswers Domain Name'),
    '#description' => t('Include the domain name for your LibAnswers instance.<br />e.g., http://yourlibrary.libanswers.com'),
    '#default_value' => variable_get('libanswers_domain'),
    '#size' => 100,
    '#required' => TRUE
  );
  $form['libanswers_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Libanswers ID'),
    '#description' => t('Include the ID number for your LibAnswers instance.<br />e.g., 1234'),
    '#default_value' => variable_get('libanswers_id'),
    '#size' => 100,
    '#required' => TRUE
  );
  $form['libanswers_description'] = array(
    '#title' => t('Description'),
    '#type' => 'textarea',
    '#description' => t('This is an optional space to add some descriptive/explanatory text or HTML to the main knowledgebase page that appears at /ask-us.'),
    '#default_value' => variable_get('libanswers_description'),
  );
  $form['libanswers_chat_code'] = array(
    '#title' => t('LibChat Code'),
    '#type' => 'textarea',
    '#description' => t('If you\'ve built a custom LibChat widget that you want to embed as a block, please paste the code for that here.'),
    '#default_value' => variable_get('libanswers_chat_code'),
  );

  $form = system_settings_form($form);
  return $form;
}