<?php

function asaf_example_api_needed_files_validate($form, &$form_state) {
  if (!valid_email_address($form_state['values']['email'])) {
    form_set_error('email', t('Email is not valid'));
  }
}

function asaf_example_api_needed_files_submit($form, &$form_state) {
  drupal_set_message(t('Email is valid'));
}