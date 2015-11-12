<?php

/**
 * @file
 * Argument plugin for a valid access to card data owner.
 */

/**
 * Validate whether an argument is a valid user.
 */
class commerce_cardonfile_plugin_argument_validate_owner extends views_plugin_argument_validate_user {
  function validate_argument($argument) {
    global $user;

    // call parent validate for user id
    $return = parent::validate_argument($argument);

    // If allowed based on user id alone ...
    if ($return) {
      // Check card data access
      $account_uid = $this->argument->argument;
      $account = user_load($account_uid);

      // DENY if account does not have card access
      if (!commerce_cardonfile_user_access($account)) {
        $return = FALSE;
        $this->argument->argument = NULL;
        $this->argument->validated_title = NULL;
      }
    }

    return $return;
  }
}
