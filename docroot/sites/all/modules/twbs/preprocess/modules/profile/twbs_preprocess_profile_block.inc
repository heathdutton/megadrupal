<?php

/**
 * Process variables for profile-block.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $account
 * - $fields
 *
 * @see profile-block.tpl.php
 */
function twbs_preprocess_profile_block(&$variables) {

  $variables['user_picture'] = theme('user_picture', array('account' => $variables['account']));
  $variables['profile'] = array();
  // Supply filtered version of $fields that have values.
  foreach ($variables['fields'] as $field) {
    if ($field->value) {
      $variables['profile'][$field->name] = new stdClass();
      $variables['profile'][$field->name]->title = check_plain($field->title);
      $variables['profile'][$field->name]->value = $field->value;
      $variables['profile'][$field->name]->type = $field->type;
    }
  }

}
