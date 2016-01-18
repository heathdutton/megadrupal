<?php
/**
 * @file
 * Form functions for the Election Export module.
 */

/**
 * Form for exporting the results of an election.
 */
function election_export_form($form, &$form_state, stdClass $election) {

  $form['#election'] = $election;

  drupal_set_title(t('Export: %election', array('%election' => $election->title)), PASS_THROUGH);
  election_set_breadcrumb($election);

  $default_filename = $election->title . '-' . $election->election_id;
  $default_filename = truncate_utf8(preg_replace('/[^A-Z0-9\-_]+/i', '-', $default_filename), 60, FALSE, FALSE);
  $default_filename .= '_' . date('YmdHis', REQUEST_TIME);

  $post_name_plural = election_get_post_name($election->type, TRUE);

  $post_options = db_select('election_post')
    ->fields('election_post', array('post_id', 'title'))
    ->condition('election_id', $election->election_id)
    ->orderBy('title')
    ->execute()
    ->fetchAllKeyed();

  $form['help'] = array(
    '#markup' => t(
      'You can use this form to export a ZIP archive (.zip) containing ballot files (.blt) for %election.',
      array(
        '%election' => $election->title,
      )
    ),
  );

  if (!empty($post_options)) {
    $form['post_ids'] = array(
      '#type' => 'select',
      '#title' => t('@Posts', array('@Posts' => drupal_ucfirst($post_name_plural))),
      '#description' => t('Select the @posts you want to include in the export.', array('@posts' => $post_name_plural)),
      '#multiple' => TRUE,
      '#options' => $post_options,
      '#default_value' => array_keys($post_options),
      '#required' => TRUE,
    );
  }
  else {
    $form['#disabled'] = TRUE;
    drupal_set_message(t('There are no @posts yet to export.', array('@posts' => $post_name_plural)), 'warning');
  }

  $form['filename'] = array(
    '#type' => 'textfield',
    '#title' => t('Filename'),
    '#default_value' => $default_filename,
    '#field_suffix' => '.zip',
    '#required' => TRUE,
  );

  $form['include_ballot_ids'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include ballot IDs'),
    '#description' => t('Ballot IDs could help to identify ballots uniquely.'),
    '#default_value' => FALSE,
  );

  $form['buttons'] = array(
    '#type' => 'actions',
  );
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Export'),
  );

  return $form;

}

/**
 * Submit callback for election_export_form().
 *
 * Export a ZIP archive of ballot files, per election, and provide for download.
 */
function election_export_form_submit($form, &$form_state) {

  $election = $form['#election'];

  $filename = $form_state['values']['filename'];

  $zipfile = election_export_election($election, NULL, array(
    'post ids' => $form_state['values']['post_ids'],
    'include ballot ids' => $form_state['values']['include_ballot_ids'],
  ));

  header('Content-Type:application/zip;charset=UTF-8');
  header('Content-Disposition:attachment;filename=' . $filename . '.zip');
  header('Cache-Control:max-age=0; must-revalidate');
  readfile($zipfile);
  drupal_unlink($zipfile);

  drupal_exit();

}
