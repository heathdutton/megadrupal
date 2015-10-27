<?php

/**
 * @file
 * Test plugin for success.
 */

class UserAuthAPITestSuccessPlugin implements UserAuthAPIPlugin {

  function isExternal() {
    return TRUE;
  }

  function validate($form_values) {
    return TRUE;
  }

  function validateError($form, $form_values) {
    return form_error($form, t('Error has occurred for the Test case!'));
  }

  function validateSuccess($account, $form_values) {
    return drupal_set_message(t('Success with Test case!'));
  }

  function defaultWeight() {
    return -20;
  }

} // UserAuthAPITestUserAuthValidatePlugin.
