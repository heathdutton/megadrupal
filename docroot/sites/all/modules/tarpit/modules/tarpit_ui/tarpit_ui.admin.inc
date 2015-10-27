<?php

function tarpit_ui_admin_settings($form, $form_state) {
  $form['paths'] = array(
    '#type' => 'textarea',
    '#title' => 'Path(s)',
    '#default_value' => implode(\Drupal\tarpit\Config::get('tarpit.paths'), "\r\n"),
    '#description' => 'Path on which to enable the trap. One per line.',
  );

  $form['paths_documentation'] = array(
    '#type' => 'fieldset',
    '#title' => 'Content of robots.txt',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    'robots' => array(
      '#type' => 'textarea',
      '#rows' => 15,
      '#default_value' => file_get_contents(DRUPAL_ROOT . '/robots.txt')
    )
  );

  $form['depth'] = array(
    '#type' => 'textfield',
    '#title' => 'Depth',
    '#default_value' => \Drupal\tarpit\Config::get('tarpit.depth'),
    '#description' => 'Depth\'s limit before doing an action. If you use the Rules module, this can be also overridden.',
  );

  $form['size'] = array(
    '#type' => 'textfield',
    '#title' => 'Content size',
    '#default_value' => \Drupal\tarpit\Config::get('tarpit.size'),
    '#description' => 'Number of words in the content.',
  );

  $form['links'] = array(
    '#type' => 'textfield',
    '#title' => 'Links',
    '#default_value' => \Drupal\tarpit\Config::get('tarpit.links'),
    '#description' => 'Number of links in the content.',
  );

  $form['page_title'] = array(
    '#type' => 'textfield',
    '#title' => 'Page title',
    '#default_value' => \Drupal\tarpit\Config::get('tarpit.page_title'),
    '#description' => 'Page title set on each page.',
  );

  $form['wordlist'] = array(
    '#type' => 'textfield',
    '#title' => 'Wordlist',
    '#default_value' => \Drupal\tarpit\Config::get('tarpit.wordlist'),
    '#description' => 'Filepath to a text file. Each words must be on separated lines.',
  );

  $form['buttons'] = array(
    'submit' => array(
      '#type' => 'submit',
      '#value' => 'Save configuration'
    )
  );

  return $form;
}

function tarpit_ui_admin_settings_submit($form, $form_state) {
  $values = $form_state['values'];

  unset($values['submit']);
  unset($values['form_build_id']);
  unset($values['form_token']);
  unset($values['form_id']);
  unset($values['op']);

  foreach($values as $key => $value) {
    \Drupal\tarpit\Config::set('tarpit.' . $key, $value);
  }

  $values['paths'] = explode("\r\n", $values['paths']);
  \Drupal\tarpit\Config::set('tarpit.paths', $values['paths']);

  menu_rebuild();
  drupal_set_message(t('The configuration options have been saved.'));
}
