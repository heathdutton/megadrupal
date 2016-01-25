<?php

function biblereadings_schedule_form($form, $form_state) {
  $year_options = array();
  for ($i = -1; $i < 4; $i++) {
    $y = date('Y') + $i;
    $year_options[$y] = $y;
  }

  $default = ($form_state['year']) ? $form_state['year'] : date('Y');

  $form['year'] = array(
      '#type' => 'select',
      '#title' => t('Filter by Year'),
      '#options' => $year_options,
      '#default_value' => $default,
  );

  $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Go'),
  );

  return $form;
}

function biblereadings_schedule_form_submit($form, $form_state) {
  drupal_goto('admin/config/biblereadings/schedule/' . $form_state['values']['year']);
}


function biblereadings_subscribe_form($form, $form_state) {
  global $user;
  
  $form['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Your Name'),
      '#size' => 30,
      '#required' => TRUE,
      //'#attributes' => array('placeholder' => t('Your Name')),
  );
  
  $form['email'] = array(
      '#type' => 'textfield',
      '#title' => t('Email Address'),
      '#size' => 30,
      '#required' => TRUE,
      '#default_value' => (user_is_logged_in()) ? $user->mail : '',
  );
  
  
  $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Sign Me Up!'),
  );
  
  
  return $form;
}

function biblereadings_subscribe_form_submit($form, $form_state) {
  
  $exists = biblereadings_subscription_exists($form_state['values']['email']);
  
  if (!$exists) {
    $query = "INSERT INTO {biblereadings_subscriptions} (name, email, joined) VALUES (:name, :email, :joined)";
    //db_query($query, $form_state['values']['name'], $form_state['values']['email'], time());
    db_query($query, array(':name' => $form_state['values']['name'], ':email' => $form_state['values']['email'], ':joined' => time()));
    
    drupal_set_message(t("!name, you have been subscribed to daily Bible readings!", array('!name' => $form_state['values']['name'])));
  }
  else {
    form_set_error('email', t('This email is already subscribed!'));
  }
  
}
