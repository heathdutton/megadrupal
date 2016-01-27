<?php
/**
 * @file Devel Generate batch handling functions using the BatchAPI
 */

/**
 * Functions called from FAPI:
 */
function party_devel_generate_batch_parties($form_state) {
  $operations = array();

  // Setup the batch operations and save the variables.
  $operations[] = array('party_devel_generate_batch_parties_pre_party', array($form_state['values']));

  // add the kill operation
  if ($form_state['values']['kill_parties']) {
    $operations[] = array('party_devel_generate_batch_parties_kill', array());
  }

  // add the operations to create the products
  for ($num = 0; $num < $form_state['values']['num_parties']; $num ++) {
    $operations[] = array('party_devel_generate_batch_parties_add_party', array());
  }

  // start the batch
  $batch = array(
    'title' => t('Generating Parties'),
    'operations' => $operations,
    'finished' => 'party_devel_generate_batch_finished',
    'file' => drupal_get_path('module', 'party_devel') . '/devel_generate_batch.inc',
  );
  batch_set($batch);
}

/**
 * Create Product Batch Functions:
 */

function party_devel_generate_batch_parties_kill(&$context) {
  module_load_include('inc', 'party_devel', 'devel_generate');
  party_devel_generate_parties_kill($context['results']);
}

function party_devel_generate_batch_parties_pre_party($vars, &$context) {
  $context['results'] = $vars;
  $context['results']['num_parties'] = 0;
  module_load_include('inc', 'party_devel', 'devel_generate');
  party_devel_generate_parties_pre_party($context['results']);
}

function party_devel_generate_batch_parties_add_party(&$context) {
  module_load_include('inc', 'party_devel', 'devel_generate');
  module_load_include('inc', 'devel_generate', 'devel_generate');
  party_devel_generate_party_add_party($context['results']);
  $context['results']['num_party_ids'] ++;
}

function party_devel_generate_batch_finished($success, $results, $operations) {
  if ($success) {
    $message = t('Finished @num_party_ids partiess created successfully.', array('@num_party_ids' => $results['num_party_ids']));
  }
  else {
    $message = t('Finished with an error.');
  }
  drupal_set_message($message);
}

