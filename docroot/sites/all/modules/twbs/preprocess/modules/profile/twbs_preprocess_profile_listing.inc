<?php

/**
 * Process variables for profile-listing.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $account
 * - $fields
 *
 * @see profile-listing.tpl.php
 */
function twbs_preprocess_profile_listing(&$variables) {

  $variables['user_picture'] = theme('user_picture', array('account' => $variables['account']));
  $variables['name'] = theme('username', array('account' => $variables['account']));
  $variables['profile'] = array();
  // Supply filtered version of $fields that have values.
  foreach ($variables['fields'] as $field) {
    if ($field->value) {
      $variables['profile'][$field->name]->title = $field->title;
      $variables['profile'][$field->name]->value = $field->value;
      $variables['profile'][$field->name]->type = $field->type;
    }
  }

}
