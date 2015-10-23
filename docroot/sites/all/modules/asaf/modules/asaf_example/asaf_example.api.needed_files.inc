<?php

function asaf_example_api_needed_files($form, &$form_state) {
  $form['up'] = array(
    '#markup' => l('← Go to the examples list', 'examples/asaf_example'),
  );

  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Check email'),
  );

  module_load_include('inc', 'asaf_example', 'asaf_example.api.needed_files.handlers');
  $form['#validate'][] = 'asaf_example_api_needed_files_validate';
  $form['#submit'][] = 'asaf_example_api_needed_files_submit';


  // In cases when some part of form handlers located in the another place you can specify paths to files which should
  // be loaded for the correct form handling
  $options = array(
    'needed_files' => array(
      // You can specify file dirrectly...
      drupal_get_path('module', 'asaf_example') . '/asaf_example.api.needed_files.handlers.inc'

      // ...or specify module name. In this case all INC-files from module directory will be loaded
      //'asaf_example'
    )
  );

  asaf_prepare_form($form, $form_state, array(), $options);
  return $form;
}