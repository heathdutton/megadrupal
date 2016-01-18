<?php

/**
 * @file
 * Administration functions.
 */

/**
 * Settings form.
 * @ingroup forms
 */
function twitter_queue_settings() {
  $form = array();

  $form['basic']['twitter_queue_schedule'] = array(
    '#type'          => 'select',
    '#title'         => 'time',
    '#description'   => t('time to send a tweet from the twitter queue. the time is in minutes.'),
    '#default_value' => variable_get('twitter_queue_schedule', 10),
    '#options'        => array(
      '5'  => 5,
      '10' => 10,
      '15' => 15,
      '20' => 20,
      '30' => 30,
      '45' => 45,
      '60' => 60,
    ),
  );

  $form['basic']['twitter_queue_twitter_account'] = array(
    '#type'          => 'textfield',
    '#title'         => 'URL twitter tweets',
    '#description'   => 'URL tread line for twitter page.',
    '#default_value' => variable_get('twitter_queue_twitter_account', 'https://twitter.com/#!/example'),
  );

  $form['accounts'] = array(
    '#type'        => 'fieldset',
    '#title'       => 'accounts',
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
  );
  $form['accounts']['twitter_queue_test'] = array(
    '#type'          => 'checkbox',
    '#title'         => 'test',
    '#description'   => t('use testing data'),
    '#default_value' => variable_get('twitter_queue_test', 0),
  );
  $form['accounts']['real'] = array(
    '#type'        => 'fieldset',
    '#title'       => 'real account',
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
  );
  $form['accounts']['real']['twitter_queue_consumer_key'] = array(
    '#type'          => 'textfield',
    '#title'         => 'consumer_key',
    '#description'   => '',
    '#default_value' => variable_get('twitter_queue_consumer_key', ''),
  );
  $form['accounts']['real']['twitter_queue_consumer_secret'] = array(
    '#type'          => 'textfield',
    '#title'         => 'consumer_secret',
    '#description'   => '',
    '#default_value' => variable_get('twitter_queue_consumer_secret', ''),
  );
  $form['accounts']['real']['twitter_queue_user_token'] = array(
    '#type'          => 'textfield',
    '#title'         => 'user_token',
    '#description'   => '',
    '#default_value' => variable_get('twitter_queue_user_token', ''),
  );
  $form['accounts']['real']['twitter_queue_user_secret'] = array(
    '#type'          => 'textfield',
    '#title'         => 'user_secret',
    '#description'   => '',
    '#default_value' => variable_get('twitter_queue_user_secret', ''),
  );
  $form['accounts']['test'] = array(
    '#type'        => 'fieldset',
    '#title'       => 'test account',
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
  );
  $form['accounts']['test']['twitter_queue_consumer_key_test'] = array(
    '#type'          => 'textfield',
    '#title'         => 'consumer_key',
    '#description'   => '',
    '#default_value' => variable_get('twitter_queue_consumer_key_test', ''),
  );
  $form['accounts']['test']['twitter_queue_consumer_secret_test'] = array(
    '#type'          => 'textfield',
    '#title'         => 'consumer_secret',
    '#description'   => '',
    '#default_value' => variable_get('twitter_queue_consumer_secret_test', ''),
  );
  $form['accounts']['test']['twitter_queue_user_token_test'] = array(
    '#type'          => 'textfield',
    '#title'         => 'user_token',
    '#description'   => '',
    '#default_value' => variable_get('twitter_queue_user_token_test', ''),
  );
  $form['accounts']['test']['twitter_queue_user_secret_test'] = array(
    '#type'          => 'textfield',
    '#title'         => 'user_secret',
    '#description'   => '',
    '#default_value' => variable_get('twitter_queue_user_secret_test', ''),
  );

  return system_settings_form($form);
}

/**
 * Content Type you want to handle.
 */
function twitter_queue_types() {
  $form = array();

  $types = _node_types_build()->types;
  $names = _node_types_build()->names;

  if (empty($types)) {
    $form['nocontenttype'] = array(
      '#prefix' => '<div>',
      '#markup' => t("There's not any kind of content type. Please !link first.", array('!link' => l(t("add content type"), "admin/structure/types/add"))),
      '#suffix' => '</div>',
    );

    return $form;
  }

  foreach ($types as $key => $value) {
    $types_raw[] = $value->type;
  }

  $content_types = array_combine($types_raw, $names);

  $form['twitter_queue_types'] = array(
    '#type'          => 'checkboxes',
    '#title'         => t('Content Types'),
    '#default_value' => variable_get('twitter_queue_types', array()),
    '#options'       => $content_types,
    '#description'   => t('Select the content type do you want to allow automatically be added to the twitter queue.'),
  );

  return system_settings_form($form);

}
