<?php
/**
 * @file
 * Administrative pages for configuring signed nodes.
 */

/**
 * Form builder. Create the form for adding a signed node agreement.
 *
 * @return
 *   HTML form.
 */
function signed_nodes_add_form($form, &$form_state) {
  //$form = array();
  $form['nid'] = array(
    '#type' => 'textfield',
    '#title' => t('Node ID'),
    '#maxlength' => '254',
    '#required' => TRUE,
    '#default_value' => '',
  );
  $form['year'] = array(
    '#type' => 'textfield',
    '#title' => t('Sign for Year'),
    '#maxlength' => '4',
    '#required' => TRUE,
    '#default_value' => date('Y'),
  );

  $form['purge_year'] = array(
    '#type' => 'textfield',
    '#title' => t('Years to Purge User signed nodes'),
    '#maxlength' => '2',
    '#required' => TRUE,
    '#default_value' => 0,
  );

  $form['agreement_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Node agreement to sign'),
    '#required' => TRUE,
    '#default_value' => '',
  );

  $form['reporting_profile_fields'] = array(
    '#type' => 'textarea',
    '#title' => t('Profile fields in reporting'),
    '#description' => t('Add comma separated list of Title|profilefields e.g. Title|profile_title,Extension|profile_extension'),
    '#default_value' => variable_get('reporting_profile_fields'),
  );


  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add Node Aggreement')
  );
  return $form;
}

/**
 * Validate the signed nodes add form.
 *
 * @see signed_nodes_add_form()
 */
function signed_nodes_add_form_validate($form, &$form_state) {
  $nid = $form_state['values']['nid'];
  $year = $form_state['values']['year'];
  $agreement_message = $form_state['values']['agreement_message'];
  $snid_years = signed_nodes_get_year($nid);
  // Make sure a nonblank node id is provided.
  if ($nid == '') {
    form_set_error('nid', t('Please provide a nonblank nid for the new node agreement.'));
  }
  // Make sure a nonblank year value is provided
  if ($year == '') {
    form_set_error('year', t('Please provide a year e.g 2012 for the new node agreement.'));
  }
  // Make sure a nonblank agreement message is provided
  if ($agreement_message == '') {
    form_set_error('agreement_message', t('Please provide a nonblank node agreement.'));
  }

  // Make sure nid and year are unique.

  if (in_array($year, $snid_years)) {
    form_set_error('year', t('Agreement for node id = %name for the year = %year already exists. Please enter another year.',
      array('%name' => $nid, '%year' => $year)));
  }
}

/**
 * Submit handler for the signed nodes add form.
 *
 * @see signed_nodes_add_form()
 */
function signed_nodes_add_form_submit($form, &$form_state) {
  $record = array();
  $record['nid'] = $form_state['values']['nid'];
  $record['year'] = $form_state['values']['year'];
  $record['purge_year'] = $form_state['values']['purge_year'];
  $record['agreement_message'] = $form_state['values']['agreement_message'];
  drupal_write_record('signed_nodes', $record);

  variable_set('reporting_profile_fields', $form_state['values']['reporting_profile_fields']);

  watchdog('signed_nodes', 'Created signed node agreement for Node ID =  %name', array('%name' => $record['nid']));
  drupal_set_message(t('The node agreement for Node ID = %name was created.',
    array('%name' => $record['nid'])), 'status');
  //$form_state['wid'] = $wid;
  $form_state['redirect'] = 'admin/config/content/signed_nodes';
}

/**
 * Form builder. Create the form for editing a signed node agreement.
 *
 * @return
 *   HTML form.
 */
function signed_nodes_edit_form($form, &$form_state, $snid) {
  $snid_load = array();
  if ($snid) {
    //$form['update'] = TRUE;
    $form['snid'] = array(
      '#type' => 'hidden',
      '#value' => $snid,
    );
    $snid_load = signed_nodes_load_all($snid);
  }
  $form['nid'] = array(
    '#type' => 'textfield',
    '#title' => t('Node ID'),
    '#maxlength' => '254',
    '#required' => TRUE,
    '#default_value' => (($snid) ? $snid_load->nid : ''),
  );
  $form['year'] = array(
    '#type' => 'textfield',
    '#title' => t('Sign for Year'),
    '#maxlength' => '4',
    '#required' => TRUE,
    '#default_value' => (($snid) ? $snid_load->year : date('Y')),
  );

  $form['purge_year'] = array(
    '#type' => 'textfield',
    '#title' => t('Years to Purge User signed nodes'),
    '#maxlength' => '2',
    '#required' => TRUE,
    '#default_value' => (($snid) ? $snid_load->purge_year : '0'),
  );

  $form['agreement_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Node agreement to sign'),
    '#required' => TRUE,
    '#default_value' => (($snid) ? $snid_load->agreement_message : ''),
  );

  $form['reporting_profile_fields'] = array(
    '#type' => 'textarea',
    '#title' => t('Profile fields in reporting'),
    '#description' => t('Add comma separated list of Title|profilefields e.g. Title|profile_title,Extension|profile_extension'),
    '#default_value' => variable_get('reporting_profile_fields'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Edit Node Aggreement')
  );
  return $form;

}

/**
 * Validate the signed nodes edit form.
 *
 * @see signed_nodes_edit_form()
 */
function signed_nodes_edit_form_validate($form, &$form_state) {
  $nid = $form_state['values']['nid'];
  $year = $form_state['values']['year'];
  $snid = $form_state['values']['snid'];
  $agreement_message = $form_state['values']['agreement_message'];
  $snid_years = signed_nodes_get_year($nid);
  unset($snid_years[$snid]);
  // Make sure a nonblank node id is provided.
  if ($nid == '') {
    form_set_error('nid', t('Please provide a nonblank nid for the new node agreement.'));
  }
  // Make sure a nonblank year value is provided
  if ($year == '') {
    form_set_error('year', t('Please provide a year e.g 2012 for the new node agreement.'));
  }
  // Make sure a nonblank agreement message is provided
  if ($agreement_message == '') {
    form_set_error('agreement_message', t('Please provide a nonblank node agreement.'));
  }

  // Make sure nid and year are unique.

  if (in_array($year, $snid_years)) {
    form_set_error('year', t('Agreement for node id = %name for the year = %year already exists. Please enter another year.',
      array('%name' => $nid, '%year' => $year)));
  }
}

/**
 * Submit handler for the signed nodes edit form.
 *
 * @see signed_nodes_edit_form()
 */
function signed_nodes_edit_form_submit($form, &$form_state) {
  $record = array();
  $record['snid'] = $form_state['values']['snid'];
  $record['nid'] = $form_state['values']['nid'];
  $record['year'] = $form_state['values']['year'];
  $record['purge_year'] = $form_state['values']['purge_year'];
  $record['agreement_message'] = $form_state['values']['agreement_message'];
  drupal_write_record( 'signed_nodes', $record, 'snid' );
  variable_set('reporting_profile_fields', $form_state['values']['reporting_profile_fields']);
  watchdog('signed_nodes', 'Updated signed node agreement for Node ID =  %name for Year = %year', array('%name' => $record['nid'], '%year' => $record['year']));
  drupal_set_message(t('The node agreement for Node ID = %name for Year = %year was updated.',
    array('%name' => $record['nid'], '%year' => $record['year'])), 'status');
  $form_state['redirect'] = 'admin/config/content/signed_nodes';
}


/**
 * Form builder. Create form for confirmation of signed node agreement deletion.
 *
 * @param $snid
 *   The ID of the signed node agreement to delete.
 *
 * @return
 *   Form definition array.
 *
 */
function signed_nodes_delete_form($form, &$form_state, $snid) {
  $form['snid'] = array('#type' => 'value', '#value' => $snid);
  return confirm_form(
    $form,
    t('Are you sure you want to delete node agreement for Node ID = %title ? All nodes signed by users will be removed.', array('%title' => signed_node_get_nid($snid))),
    !empty($_GET['destination']) ? $_GET['destination'] : 'admin/config/content/signed_nodes',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Submit handler for signed nodes deletion form.
 *
 * @see signed_nodes_delete_form()
 */
function signed_nodes_delete_form_submit($form, &$form_state) {
  if ($form_state['values']['confirm'] == 1) {
    $snid = $form_state['values']['snid'];
    $nid = signed_node_get_nid($snid);
    signed_nodes_delete_agreement($snid);
    watchdog('signed_nodes', 'Deleted signed node agreemet for Node ID = %name with all its user signed agreements', array('%name' => $nid));
    drupal_set_message(t('The node agreement with all its signed agreements from users for Node ID = %name was deleted.', array('%name' => $nid)));
    $form_state['redirect'] = 'admin/config/content/signed_nodes';
  }
}

/**
 * Menu callback. Create the main signed nodes agreement page,
 * which gives an administrative overview of signed nodes agreements.
 *
 * @return
 *   All agreement in tabular form with edit/delete operations attached.
 */
function signed_nodes_overview() {
  $signed_nodes = signed_nodes_get_all();
  $row = array();
  foreach ($signed_nodes as $snid => $snid_array) {
    $links = array(
      'signed_nodes_overview_edit' => array(
        'title' => t('Edit'),
        'href' => "admin/config/content/signed_nodes/edit/$snid"),
      'signed_nodes_overview_delete' => array(
        'title' => t('Delete'),
        'href' => "admin/config/content/signed_nodes/delete/$snid"),
    );
    // krumo($links);
    $row[] = array(check_plain(t($snid_array->nid . " (" . $snid_array->year . ")")), theme('links', array('links' => $links, 'attributes' => array('class' => array('links', 'inline')))));
  }

  if ($row) {
    $header = array(t('Node Agreement'), t('Operations'));
    $output = theme('table', array(
      'header' => $header,
      'rows' => $row,
    ));
  }
  else {
    $output = '<p>' . t('No signed nodes agreement have been added. Would you like to <a href="@link">add a new node agreement</a>?', array('@link' => url('admin/config/content/signed_nodes/add'))) . '</p>';
  }
  return $output;
}