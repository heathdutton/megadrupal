<?php

function asaf_example_gui_needed_files($form, &$form_state) {
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Check email'),
  );

  module_load_include('inc', 'asaf_example', 'tests/asaf_example.gui.needed_files.handlers');
  $form['#validate'][] = 'asaf_example_gui_needed_files_validate';
  $form['#submit'][] = 'asaf_example_gui_needed_files_submit';

  return $form;
}