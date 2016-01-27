<?php
/**
 * @file Devel generate code.
 */

/**
 * The main API function for creating parties
 *
 * See party_devel_generate_parties_form() for the supported keys in
 * $form_state['values']. Other modules may participate by form_alter() on that
 * form and then handling their data during presave or in own submit handler
 * for the form.
 *
 * @param string $form_state
 * @return void
 */
function party_devel_generate_parties($form_state) {
  if (!empty($form_state['values']['kill_parties'])) {
    party_devel_generate_parties_kill($form_state['values']);
  }

  party_devel_generate_parties_pre_party($form_state['values']);
  $start = time();
  for ($i = 1; $i <= $form_state['values']['num_parties']; $i++) {
    // Get list of users we will use.
    party_devel_generate_party_add_party($form_state['values']);
    if (function_exists('drush_log') && $i % drush_get_option('feedback', 1000) == 0) {
      $now = time();
      drush_log(dt('Completed !feedback partiess (!rate party/min)', array('!feedback' => drush_get_option('feedback', 1000), '!rate' => (drush_get_option('feedback', 1000)*60)/($now-$start))), 'ok');
      $start = $now;
    }
  }

  devel_generate_set_message(format_plural($form_state['values']['num_parties'], '1 party created.', 'Finished creating @count parties'));
}

/**
 * Handle the party_devel_generate_parties_form request to kill all of the
 * existing products.
 *
 * This is used by both the batch and non-batch branches of the code.
 *
 * @param $values
 *   array of options obtained from commerce_devel_generate_products_form.
 */
function party_devel_generate_parties_kill($values) {
  $results = db_select('party', 'p')
              ->fields('p', array('pid'))
              ->execute();
  foreach ($results as $result) {
    $party_ids[] = $result->pid;
  }

  if (!empty($party_ids)) {
    party_delete_multiple($party_ids);
    drupal_set_message(t('Deleted %count parties.', array('%count' => count($party_ids))));
  }
}

/**
 * Create one party. Used by both batch and non-batch code branches.
 *
 * @param $results
 *   array of options
 */
function party_devel_generate_party_add_party(&$results) {

  $party = array();

  $party = party_create($party);

  // Insert new data:
  // Save default parameters back into the $product object.
  $party->language = LANGUAGE_NONE;

  // Save party fields
  module_load_include('inc', 'devel_generate', 'devel_generate.fields');
  devel_generate_fields($party, 'party', 'party');

  // save the party
  $party->save();

  $users = $results['users'];

  // If party_user is enabled sometimes attach a user
  if (module_exists('party_user') && rand(0, 2) > 1) {
    $uid = $users[array_rand($users)];
    party_attach_entity($party, $uid, 'user');
  }

  // Attach a selection of data sets
  $data_sets = party_get_party_data_sets($party);

  foreach ($data_sets as $id => $name) {
    if ($name == 'user') {
      continue;
    }

    $entity = $party->getDataSetController($name)->getEntity(0, TRUE);

    // Devel generate requires that a language be set.
    if (!isset($entity->language)) {
      $entity->language = LANGUAGE_NONE;
    }

    module_load_include('inc', 'devel_generate', 'devel_generate.fields');
    devel_generate_fields($entity, $data_set_controller->getDataInfo('entity type'), $data_set_controller->getDataInfo('entity bundle'));

    if (field_info_instance($data_set_controller->getDataInfo('entity type'), 'field_name', $data_set_controller->getDataInfo('entity bundle'))) {
      $entity->field_name[LANGUAGE_NONE][0]['value'] = party_generate_label();
    }

    entity_save($data_set_controller->getDataInfo('entity type'), $entity);
    $party->save();
  }
}

function party_generate_label() {
  $first_name = array("Addie", "Aida", "Allie", "Amanda", "Anita", "Anne", "Audie", "Augusta", "Barb", "Barry", "Bea", "Ben", "Bess", "Celine", "Chris", "Constance", "Dave", "Eileen", "Frank", "Grace", "Harriet", "Hazel", "Hope", "Hugh", "Isabelle", "Ivana", "Jessica", "John", "Josie", "Liz", "Lois", "Luke", "Lynn", "Manny", "Mark", "Marsha", "Martin", "Maureen", "Minnie", "Missy", "Olive", "Paige", "Patty", "Peg", "Phillip", "Randy", "Ray", "Reeve", "Rhoda", "Rita", "Suzie", "Teri", "Tex", "Theresa");

  $second_name = array("Anderson", "Andrews", "Brown", "Campbell", "Clark", "Clarke", "Davies", "De Wet", "Du Toit", "Edwards", "Evans", "Garcia", "Green", "Hall", "Harris", "Hughes", "Jackson", "Johnson", "Jones", "Khan", "Kumar", "Lewis", "Macdonald", "Martin", "Martinez", "Miller", "Mitchell", "Moore", "Morrison", "Mumford", "Murray", "Patak", "Patel", "Patel", "Paterson", "Reid", "Roberts", "Robertson", "Robinson", "Rogers", "Ross", "Scott", "Smith", "Stewart", "Taylor", "Thomas", "Thompson", "Walker", "Watson", "White", "Williams", "Wilson", "Wright", "Young");

  $num_first_name = count($first_name);
  $num_second_name = count($second_name);

  $name = $first_name[mt_rand(0, $num_first_name - 1)] . ' ' . $second_name[mt_rand(0, $num_second_name - 1)];

  return $name;
}
