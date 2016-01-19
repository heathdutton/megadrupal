<?php
/**
 * @file
 * Admin interface for adding keywords and links mapping.
 */

/**
 * Page callback.
 *
 * @return bool
 *   Ambigous <string, A, boolean>
 */
function dynamic_internal_linking_map_keywords() {
  // Form for dynamic internal mappings.
  $mapping_form = drupal_get_form('dynamic_internal_linking_mapping_form');
  $mapping_form = drupal_render($mapping_form);
  return $mapping_form;
}

/**
 * This gives an option to add keyword and mapping through UI.
 *
 * Implements hook_form().
 */
function dynamic_internal_linking_mapping_form($form, &$form_state) {

  // We will have many fields with the same name, so we need to be able to
  // access the form hierarchically.
  $form['#tree'] = TRUE;

  if (empty($form_state['num_mappings'])) {
    $form_state['num_mappings'] = 1;
  }
  for ($i = 1; $i <= $form_state['num_mappings']; $i++) {
    $form['map_keywords'][$i] = array(
      '#type' => 'fieldset',
      '#title' => t('Keywords Links Mapping #!number', array('!number' => $i)),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    $form['map_keywords'][$i]['keyword'] = array(
      '#type' => 'textfield',
      '#title' => t('Keyword'),
      '#description' => t('Please enter Keyword.'),
      '#size' => 20,
    );

    $form['map_keywords'][$i]['link'] = array(
      '#type' => 'textfield',
      '#description' => t('Please enter Absoloute URL.'),
      '#title' => t('Link'),
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  // Adds "Add another mapping" button.
  $form['add_mapping'] = array(
    '#type' => 'submit',
    '#value' => t('Add another Mapping'),
    '#submit' => array('dynamic_internal_linking_mapping_form_add_mapping'),
  );

  // If we have more than one mapping, this will remove the last mapping.
  if ($form_state['num_mappings'] > 1) {
    $form['remove_mapping'] = array(
      '#type' => 'submit',
      '#value' => t('Remove latest mapping'),
      '#submit' => array('dynamic_internal_linking_mapping_form_remove_mapping'),

      // Since we are removing a mapping, don't validate until later.
      '#limit_validation_errors' => array(),
    );
  }

  return $form;
}

/**
 * For adding the a new mapping.
 *
 * @param array $form
 *   Form submitted.
 * @param array $form_state
 *   Form State submitted.
 */
function dynamic_internal_linking_mapping_form_add_mapping(array $form, array &$form_state) {
  // Everything in $form_state is persistent, so we'll just use.
  // $form_state['add_mapping']
  $form_state['num_mappings']++;

  // Setting $form_state['rebuild'] = TRUE causes the form to be rebuilt again.
  $form_state['rebuild'] = TRUE;
}

/**
 * To remove the latest mapping fieldset.
 *
 * @param array $form
 *   Form Submitted.
 * @param array $form_state
 *   Form state.
 */
function dynamic_internal_linking_mapping_form_remove_mapping(array $form, array &$form_state) {
  if ($form_state['num_mappings'] > 1) {
    $form_state['num_mappings']--;
  }

  // Setting $form_state['rebuild'] = TRUE causes the form to be rebuilt again.
  $form_state['rebuild'] = TRUE;
}

/**
 * Validation the keyword and the link.
 *
 * @param array $form
 *   Form Submitted.
 * @param array $form_state
 *   Form state.
 */
function dynamic_internal_linking_mapping_form_validate(array $form, array &$form_state) {
  for ($i = 1; $i <= $form_state['num_mappings']; $i++) {
    $link = $form_state['values']['map_keywords'][$i]['link'];
    $keyword = $form_state['values']['map_keywords'][$i]['keyword'];
    if (empty($keyword)) {
      form_set_error("map_keywords][$i][keyword", t('Please Enter a Keyword'));
    }
    if (!valid_url($link, TRUE)) {
      form_set_error("map_keywords][$i][link", t('Please Enter a Valid url'));
    }
  }
}

/**
 * Page Callback function for import keywords.
 */
function dynamic_internal_linking_import_keywords() {
  $import_form = drupal_get_form('dynamic_internal_linking_import_keywords_form');
  $import_form = drupal_render($import_form);
  return $import_form;
}

/**
 * Implements hook_form().
 */
function dynamic_internal_linking_import_keywords_form($form, $form_state) {
  $form = array();
  $form['help_text'] = array(
    '#markup' => '<div style="color:green"><b>' . t('First Column of CSV must be " keyword " & Second Column " link "') . '<b></div>',
  );
  $form['import_csv'] = array(
    '#type' => 'file',
    '#title' => t('Upload a CSV file'),
    '#description' => t('First column should be keyword & second column must be link'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

/**
 * Validates the file wheather it is CSV or not.
 *
 * @param array $form
 *   Form.
 * @param array $form_state
 *   Form state.
 */
function dynamic_internal_linking_import_keywords_form_validate(array $form, array &$form_state) {
  $file = file_save_upload('import_csv', array('file_validate_extensions' => array('csv')), FALSE, FILE_EXISTS_REPLACE);
  if ($file) {
    $file_uri = $file->uri;
    $handle = fopen($file_uri, 'r');
    $row = fgetcsv($handle);
    $columns = array();
    foreach ($row as $i => $header) {
      $columns[$i] = trim($header);
    }
    if ($columns[0] == 'keyword' && $columns[1] == 'link') {
      $form_state['values']['import_csv'] = $file;
    }
    else {
      form_set_error('import_csv', t('Please Correct the CSV columns'));
    }
  }
}

/**
 * Saves the keyword and link mapping into database.
 *
 * @param array $form
 *   CSV Import form.
 * @param array $form_state
 *   CSV Import form state.
 */
function dynamic_internal_linking_import_keywords_form_submit(array $form, array &$form_state) {
  $file_uri = $form_state['values']['import_csv']->uri;
  $handle = fopen($file_uri, 'r');
  $row = fgetcsv($handle);
  $columns = array();
  $failed_items = array();
  foreach ($row as $i => $header) {
    $columns[$i] = trim($header);
  }

  while ($row = fgetcsv($handle)) {
    $record = array();
    foreach ($row as $i => $field) {
      $record[$columns[$i]] = $field;
    }
    $keyword = $record[$columns[0]];
    if (!dynamic_internal_linking_keyword_already_exists($keyword)) {
      drupal_write_record('dil_keywords_links', $record);
    }
    else {
      $failed_items[] = $keyword;
    }
  }

  fclose($handle);

  if (!empty($failed_items)) {
    foreach ($failed_items as $item) {
      drupal_set_message(t('!keyword already exists', array('!keyword' => $item)), 'Error');
    }
  }
  else {
    drupal_set_message(t('All keywords are saved'), 'status');
  }
}


/**
 * Save the mapping for keyword and links.
 *
 * @param array $form
 *   Form Submitted.
 * @param array $form_state
 *   Form state.
 */
function dynamic_internal_linking_mapping_form_submit(array $form, array &$form_state) {
  $output = t('Details :');
  for ($i = 1; $i <= $form_state['num_mappings']; $i++) {
    $keyword = $form_state['values']['map_keywords'][$i]['keyword'];
    $link = $form_state['values']['map_keywords'][$i]['link'];
    $status = t('Not Saved (Keyword already exists)');
    // If keyword not exists save the mapping.
    if (!dynamic_internal_linking_keyword_already_exists($keyword)) {
      dynamic_internal_linking_save_keyword($keyword, $link);
      $status = t('Saved');
    }

    $output .= t('Mapping - @num: Keyword - @keyword , Link - @link is @status', array(
      '@num' => $i,
      '@keyword' => $keyword,
      '@link' => $link,
      '@status' => $status,
    ));
  }

  drupal_set_message($output);
}
