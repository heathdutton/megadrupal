<?php

/**
 * @file
 * Common functions for the aul_views module.
 */

/**
 * Helper function. Get the user context from aul_grants_field.
 * 
 * @global object $user
 *   Current user.
 * @param views_handler_aul_grants_field $grants_field
 *   aul_grants_field object.
 *
 * @return array
 *   User id and name.
 */
function _aul_views_get_user_context(views_handler_aul_user_grants_field $grants_field) {
  // Get the user context.
  if (empty($grants_field->options['aul_user'])) {

    // Check 'aul_user_context' filter value.
    $filter_value = NULL;
    if (!empty($grants_field->view->filter)) {
      foreach ($grants_field->view->filter as $filter) {
        if ($filter->field == 'aul_user_context' && !empty($filter->value)) {
          $filter_value = $filter->value[0];
          break;
        }
      }
    }

    // Get 'aul_user_context' filter value.
    if (!empty($filter_value['entity_id'])) {
      $uid = $filter_value['entity_id'];
      $username = $filter_value['entity_label'];
    }

    // Get current user.
    else {
      global $user;
      $uid = $user->uid;
      if ($uid) {
        $username = $user->name;
      }
      else {
        $username = t('Anonnymous');
      }
    }
  }
  // Get user from field settings.
  else {
    $uid = $grants_field->options['aul_user']['entity_id'];
    $username = $grants_field->options['aul_user']['entity_label'];
  }

  return array(
    'uid' => $uid,
    'username' => $username,
  );
}
