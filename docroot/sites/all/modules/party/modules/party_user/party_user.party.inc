<?php
/**
 * @file
 * Party General hook include.
 */

/**
 * Implements hook_party_access()
 */
function party_user_party_access($op, $party, $data_set, $account) {
  // If we haven't been passed a party, it's outside of the scope of these
  // checks.
  if (!isset($party)) {
    return NULL;
  }

  // Prepare the data set controller.
  $controller = $party->getDataSetController('user');

  // Deny access for party deletions if the configuration says so.
  if (empty($data_set) && $op == 'delete' && count($controller->getEntityIds())
    && variable_get('party_user_party_delete_action', 'disallow') == 'disallow') {
    return FALSE;
  }

  // Check if this is their own party by getting the controller and checking
  // that $account is one of the attached entities for this party.
  if (!in_array($account->uid, $controller->getEntityIds())) {
    return NULL;
  }

  // Set the data_set_name
  if (isset($data_set)) {
    $data_set_name = $data_set['set_name'];
  }

  // If we're looking at permission for a particular data set we check these.
  if (isset($data_set_name)) {
    // Determine which permission we need to check.
    switch ($op) {
      case 'view':
        $permission_string = 'view own party attached ' . $data_set_name;
        break;
      case 'edit':
        $permission_string = 'edit own party attached ' . $data_set_name;
        break;
      case 'detach':
        $permission_string = 'detach own party attached ' . $data_set_name;
        break;
      case 'attach':
      case 'add':
        $permission_string = 'attach own party ' . $data_set_name;
        break;
    }
  }
  else {
    // If we're not being asked about attachments, just use plain permissions.
    switch ($op) {
      case 'view':
        $permission_string = 'view own party';
        break;
      case 'edit':
        $permission_string = 'edit own party';
        break;
    }
  }


  // If we have a valid permission, check it. In this hook we only grant access
  // with the permissions, not deny it. In other words, the permission
  // 'view party attached foo' is sufficient but not neccessary, it is
  // advisable to use these blanket permissions sparingly.
  // @todo: add explanation of this to the party_access() docblock.
  if (isset($permission_string) && user_access($permission_string, $account)) {
    return TRUE;
  }

  // If we get here - say nothing.
  return NULL;
}

/**
 * Primary field callback for converting UID to username.
 *
 * @see party_user_party_primary_fields_sources_alter()
 */
function party_user_primary_fields_callback_uid_to_format_username($value) {
  if (isset($value)) {
    $account = user_load($value);
    return format_username($account);
  }
}
