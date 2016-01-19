<?php

/**
 * @file
 * User Import module hooks. Allows for importing FedIDs from a csv file.
 */

/**
 * Implements hook_field_user_import_supported_alter().
 *
 * This hook will only work with user_import versions 2.2 and higher.
 */
function beidmellon_fedid_field_user_import_supported_alter(&$supported) {
  $supported['beidmellon_fedid'] = array(
    'save' => 'beidmellon_fedid_user_import_field_processor',
  );
}

/**
 * Copied from field_user_import_default_field_processor.
 */
function beidmellon_fedid_user_import_field_processor($user_fields, $field_name, $values) {
  try {
    $field = $user_fields->$field_name;

    // Validate.
    $fedid = $values[0];
    // Check for non allowed characters (ie. numbers, spaces, dots or dashes).
    $allowed_characters_pattern = "/^[0-9A-z]$/";

    if (preg_match($allowed_characters_pattern, $fedid) != 1) {
      throw new Exception(t("Non allowed character in FedID."));
    }
    else {
      // Length must be 40 characters.
      if (strlen($fedid) > 40) {
        throw new Exception(t("Length of the FedID is not 40 characters or less."));
      }
    }
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'user')
    ->fieldCondition($field_name, 'beidmellon_fedid', _beidmellon_fedid_hash($fedid))
    ->propertyCondition('uid', $user_fields->uid, '!=')
    ->count();
    $count = $query->execute();
    if ($count > 0) {
      throw new Exception(t('The submitted FedID is already being used by another account. It is not possible to have more than one account with the same FedID:'));
    }
    // Save.
    $field[LANGUAGE_NONE][0]['beidmellon_fedid'] = _beidmellon_fedid_hash($fedid);
  }
  catch (Exception $e) {
    drupal_set_message(t('The user %uid has been saved but the FedID for this user has not been saved', array('%uid' => $user_fields->uid)) . ': ' . $e->getMessage(), 'warning');
  }

  return $field;
}
