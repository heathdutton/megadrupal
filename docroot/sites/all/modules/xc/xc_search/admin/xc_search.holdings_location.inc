<?php
/**
 * @file
 * Search holdings location admin functions
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_holdings_location_form($form, &$form_state) {
  $form['#attributes'] = array('enctype' => "multipart/form-data");

  $form['contact_information'] = array(
    '#value' => t('You can leave us a message using the contact form below.'),
  );

  $form['description'] = array(
    '#type' => 'item',
    '#markup' => t('Upload a CSV (Comma Separated Values) file containing the location data. Each record contains a code and a label. The first line of the file should contain the name of fields (code and label).'),
  );

  $form['upload'] = array(
    '#type' => 'file',
    '#title' => t('Upload CSV file'),
    '#size' => 40,
    '#description' => t('Upload a CSV (Comma Separated Values) file containing the location data. Each record contains a code and a label.'),
  );

  $form['separator'] = array(
    '#type' => 'radios',
    '#title' => t('Select field separator'),
    '#options' => array(
      1 => t('comma (,)'),
      2 => t('semicolon (;)'),
      3 => t('tabulator (\t)'),
    ),
    '#default_value' => 2,
  );

  $form['first_line'] = array(
    '#type' => 'radios',
    '#title' => t('Does first line contain field names?'),
    '#options' => array(
      1 => t('Yes'),
      0 => t('No'),
    ),
    '#default_value' => 1,
    '#description' => t("The first line of the CSV file may contain the name of fields (code and label). If that\'s the case, select Yes, otherwise select No."),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Upload and set locations'),
  );
  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_holdings_location_form_validate() {
  /*
   if (!file_check_upload('upload')) {
   // If you want to require it, you'll want to do it here... something like this:
   form_set_error('upload', t('File missing for upload.'));
   }
   */
}

/**
 * Handler for location submission.
 *
 * It deletes the locations table, then populate it with the records coming from the CSV file.
 *
 * @param unknown_type $form
 * @param unknown_type $form_state
 */
function xc_search_holdings_location_form_submit($form, $form_state) {
  global $user;

  switch ($form_state['values']['separator']) {
    case 1:
      $separator = ',';
      break;
    case 3:
      $separator = "\t";
      break;
    case 2:
    default:
      $separator = ';';
      break;
  }

  if ($form_state['values']['first_line']) {
    $field_names = array();
  }
  else {
    $field_names = array('code', 'label');
  }

  $limits = _upload_file_limits($user);
  $validators = array(
    'file_validate_extensions' => array($limits['extensions'] . ' csv'),
    'file_validate_image_resolution' => array($limits['resolution']),
    'file_validate_size' => array($limits['file_size'], $limits['user_size']),
  );

  if (user_access('upload files')
       && ($file = file_save_upload('upload', $validators, file_directory_path()))) {
    $count = db_query('SELECT count(*) AS count FROM {xc_search_location}')->fetchField();
    drupal_set_message(t('Removed %count locations', array('%count' => $count)));

    // TODO Please review the conversion of this statement to the D7 database API syntax.
    /* db_query('DELETE FROM {xc_search_location}') */
    db_delete('xc_search_location')
      ->execute();

    xc_util_bulk_insert('xc_search_location', xc_util_csv2objects($file->filepath, $separator, $field_names));
    variable_set('xc_search_location_defaults_installed', XC_LOADED);

    $count = db_query('SELECT count(*) AS count FROM {xc_search_location}')->fetchField();
    drupal_set_message(t('Created %count locations', array('%count' => $count)));
  }
  //handle the file, using file_save_upload, or something similar
}

/**
 * List all locations
 */
function xc_search_holdings_location_list() {
  $rows = array();
  $result = db_query('SELECT * FROM {xc_search_location} ORDER BY id');
  while ($data = $result->fetchObject()) {
    $rows[] = array($data->code, $data->label);
  }
  return theme('table', array('header' => array(t('Abbreviation'), t('Display name')), 'rows' => $rows));
}
