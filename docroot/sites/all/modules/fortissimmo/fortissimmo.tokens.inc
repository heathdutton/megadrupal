<?php

/**
 * @file
 *
 * Token support for Fortissimmo module
 */

/**
 * Implements hook_token_info().
 */
function fortissimmo_token_info() {
  $info['tokens']['fortissimmo']['title-raw'] = array(
    'name' => t("The Fortissimmo property title (raw)"),
    'description' => t("The Fortissimmo property title (raw)"),
  );
  $info['tokens']['fortissimmo']['title'] = array(
    'name' => t("The Fortissimmo property title"),
    'description' => t("The Fortissimmo property title"),
  );
  $info['tokens']['fortissimmo']['transaction-raw'] = array(
    'name' => t("The Fortissimmo property transaction type (raw)"),
    'description' => t("The Fortissimmo property transaction type (raw)"),
  );
  $info['tokens']['fortissimmo']['transaction'] = array(
    'name' => t("The Fortissimmo property transaction type (raw)"),
    'description' => t("The Fortissimmo property transaction type (raw)"),
  );

  return $info;
}

/**
 * Implements hook_tokens().
 */
function fortissimmo_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();
  if ($type == 'fortissimmo') {
    foreach ($tokens as $name => $original) {

      $object = $data['fortissimmo'];

      if ($name == "title-raw") {
        $replacements[$original] = $object->title;
      }
      if ($name == "title") {
        $replacements[$original] = check_plain($object->title);
      }
      if ($name == "transaction-raw") {
        $replacements[$original] = $object->transaction;
      }
      if ($name == "transaction") {
        $replacements[$original] = check_plain($object->transaction);
      }
    }
  }

  // Return the replacements.
  return $replacements;
}