<?php
/**
 * @file
 * Cicero Contact drush command.
 */

/**
 * Implements hook_drush_command().
 *
 * @return
 *   An associative array describing your command(s).
 */
function civicrm_cicero_drush_command() {
  $items = array();

  $items['civicrm-cicero-update-contact'] = array(
    'description' => "Update Cicero data for the given contact.",
    'arguments' => array(
      'contact_id' => 'The Civi contact ID for the record you are looking up',
    ),
    // taken from example drush file, saving for later use  --ncm
    /* 'options' => array( */
    /*   'spreads' => array( */
    /*     'description' => 'Comma delimited list of spreads.', */
    /*     'example-value' => 'mayonnaise,mustard', */
    /*   ), */
    /* ), */
    /* 'examples' => array( */
    /*   'drush mmas turkey --spreads=ketchup,mustard' => 'Make a terrible-tasting sandwich that is lacking in pickles.', */
    /* ), */
    'aliases' => array('ciciuc'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );
  $items['civicrm-cicero-show-contact'] = array(
    'description' => "Show all available Cicero data for the given contact.",
    'arguments' => array(
      'contact_id' => 'The Civi contact ID for the record you are looking up',
    ),
    // taken from example drush file, saving for later use  --ncm
    /* 'options' => array( */
    /*   'spreads' => array( */
    /*     'description' => 'Comma delimited list of spreads.', */
    /*     'example-value' => 'mayonnaise,mustard', */
    /*   ), */
    /* ), */
    /* 'examples' => array( */
    /*   'drush mmas turkey --spreads=ketchup,mustard' => 'Make a terrible-tasting sandwich that is lacking in pickles.', */
    /* ), */
    'aliases' => array('cicisc'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );
  // Commandfiles may also add topics.  These will appear in
  // the list of topics when `drush topic` is executed.
  // To view this topic, run `drush --include=/full/path/to/examples topic`
  // save this for later --ncm.
  /* $items['contact-exposition'] = array( */
  /*   'description' => 'Ruminations on the true meaning and philosophy of sandwiches.', */
  /*   'hidden' => TRUE, */
  /*   'topic' => TRUE, */
  /*   'bootstrap' => DRUSH_BOOTSTRAP_DRUSH, */
  /*   'callback' => 'drush_print_file', */
  /*   'callback arguments' => array(dirname(__FILE__) . '/sandwich-topic.txt'), */
  /* ); */

  return $items;
}

/**
 * Implements hook_drush_help().
 *
 * This function is called whenever a drush user calls
 * 'drush help <name-of-your-command>'. This hook is optional. If a command
 * does not implement this hook, the command's description is used instead.
 * This hook is also used to look up help metadata, such as help
 * category title and summary.  See the comments below for a description.
 *
 * @param $section
 *   A string with the help section (prepend with 'drush:')
 *
 * @return
 *   A string with the help text for your command.
 */
function civicrm_cicero_drush_help($section) {
  switch ($section) {
    case 'drush:civicrm-cicero-contact':
      return dt("This command will search Cicero for information from a given CiviCRM contact.");
      // The 'title' meta item is used to name a group of
      // commands in `drush help`.  If a title is not defined,
      // the default is "All commands in ___", with the
      // specific name of the commandfile (e.g. sandwich).
      // Command files with less than four commands will
      // be placed in the "Other commands" section, _unless_
      // they define a title.  It is therefore preferable
      // to not define a title unless the file defines a lot
      // of commands.
      /* case 'meta:sandwich:title': */
      /*   return dt("Sandwich commands"); */
      // The 'summary' meta item is displayed in `drush help --filter`,
      // and is used to give a general idea what the commands in this
      // command file do, and what they have in common.
    case 'meta:contact:summary':
      return dt("Is part of in integration between CiviCRM and the Cicero API.");
  }
}

/**
 * Implements hook_COMMAND_validate().
 *
 * The validate command should exit with
 * `return drush_set_error(...)` to stop execution of
 * the command.  In practice, calling drush_set_error
 * OR returning FALSE is sufficient.  See drush.api.php
 * for more details.
 */
function drush_civicrm_cicero_update_contact_validate($contact_id) {
  if (!is_numeric($contact_id)) {
    drush_set_error('CIVICRM_CICERO_DRUSH_LOOKUP_INVALID_ID', dt('The contact ID must be a valid number.'));
  }
}

/**
 * Validate the show contact command.
 */
function drush_civicrm_cicero_show_contact_validate($contact_id) {
  if (!is_numeric($contact_id)) {
    drush_set_error('CIVICRM_CICERO_DRUSH_LOOKUP_INVALID_ID', dt('The contact ID must be a valid number.'));
  }
}

/**
 * Call back function for updating a contact.
 */
function drush_civicrm_cicero_update_contact($contact_id = '') {
  civicrm_cicero_use_civi_api();
  $contact_info = civicrm_api("Contact", "get", array('version' => '3', 'contact_id' => $contact_id));
  if ($contact_info['is_error'] == 0) {
    // Updating contact.
    $updates = civicrm_cicero_contact_update($contact_id);

    if(FALSE === $updates) {
      drush_print(dt("Something went wrong with the lookup."));
      return;
    }
    if(count($updates) == 0) {
      drush_print(dt("No info updated, perhaps already filled in."));
    }
    // Get info about the contact
    $name = $contact_info['values'][$contact_id]['display_name'];
    $addr = "\n Using this address for Cicero search: \n";
    $address = civicrm_cicero_get_contact_address($contact_id);
    $addr .= '   ' . $address['street_address'] . "\n";
    $addr .= '   ' . $address['city'] . ', ';
    $addr .= $address['state_province_name'] . ' ';

    $msg = dt(' Updating contact: !contact with information from Cicero...', array('!contact' => $name));
    $msg .= "\n" . $addr;
    drush_print("\n" . $msg . "\n");

    $field_map = array_flip(variable_get('civicrm_cicero_contact_field_map', array()));
    $msg = " " . dt('Values from Cicero:') . "\n";
    foreach ($updates as $civi_field => $value) {
      $field = array_key_exists($civi_field, $field_map) ? $field_map[$civi_field] : $civi_field;
      $msg .= "   " . $field . ": " . $value . "\n";
    }
    drush_print($msg);
  }
}

/**
 * Call back function for showing all cicero data for a given contact.
 */
function drush_civicrm_cicero_show_contact($contact_id = '') {
  civicrm_cicero_use_civi_api();
  $contact_info = civicrm_api("Contact", "get", array('version' => '3', 'contact_id' => $contact_id));
  if ($contact_info['is_error'] == 0) {
    $addr_resps = civicrm_cicero_search_addr_by_contact($contact_id, $all_fields = TRUE, $force = TRUE);
    if(empty($addr_resps)) {
      return;
    }
    $name = $contact_info['values'][$contact_id]['display_name'];
    $addr = "\n Using this address for Cicero search: \n";
    $address = civicrm_cicero_get_contact_address($contact_id);
    $addr .= '   ' . $address['street_address'] . "\n";
    $addr .= '   ' . $address['city'] . ', ';
    $addr .= $address['state_province_name'] . ' ';
    $addr .= $address['postal_code'];

    $msg = dt(' Fetching Cicero information for !contact ...', array('!contact' => $name));
    $msg .= "\n" . $addr;
    drush_print("\n" . $msg . "\n");

    $query_string = civicrm_cicero_query_string_for_address($address);
    $legislative_query = CIVICRM_CICERO_LEGISLATIVE_QUERY_URL . $query_string;
    $nonlegislative_query = CIVICRM_CICERO_NONLEGISLATIVE_QUERY_URL . $query_string;

    $msg = dt(' Using the following query addresses: ');
    $msg .= "\n  " . $legislative_query . "\n  " . $nonlegislative_query . "\n\n";

    drush_print($msg);

    $msg = " " . dt('Values from Cicero:') . "\n";
    $values = civicrm_cicero_get_values($addr_resps);
    foreach ($values as $field => $value) {
      $msg .= '   ' . $field . ": " . $value . "\n";
    }
    drush_print($msg);
  }
}
