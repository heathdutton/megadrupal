<?php

/**
 * @file
 * Test plugin for fail.
 */

class UserAuthAPITestFailPlugin implements UserAuthAPIPlugin {

  function isExternal() {
    return FALSE;
  }

  function validate($form_values) {
    return FALSE;
  }

  function validateError($form, $form_values) {
    return form_error($form, t('Error has occurred for the Test case!'));
  }

  function validateSuccess($form, $form_values) {
    return drupal_set_message(t('Success with Test case!'));
  }

  function defaultWeight() {
    return -10;
  }

} // UserAuthAPITestUserAuthValidatePlugin.
