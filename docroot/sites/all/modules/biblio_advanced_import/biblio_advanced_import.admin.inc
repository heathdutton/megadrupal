<?php

/**
 * @file
 * Administration interface to configureable a duplicate
 * detection strategy for biblio records
 *
 * @see biblio_advanced_import.module
 * @see biblio.module
 *
 * @author Markus Kalkbrenner | Cocomore AG
 *   @see http://drupal.org/user/124705
 */

/**
 * Provides biblio_advanced_import_settings_form
 * using system_settings_form().
 */
function biblio_advanced_import_settings_form($form, &$form_state) {
  $form['biblio_advanced_import_duplicate_merge_fieldset'] = array(
    '#title' => t('Duplicate Detection and Merge Strategy'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $options = array(
    'create duplicate' => t('Create duplicate record.'),
    'skip import' => t('Skip import of a record.'),
    'update latest' => t('Update the latest existing duplicate record. (See "Merge Strategy" below.)'),
    'update oldest' => t('Update the oldest existing duplicate record. (See "Merge Strategy" below.)'),
    'update all' => t('Update all existing duplicate records. (See "Merge Strategy" below.)'),
    'new rev latest' => t('Create a new revision of the latest existing duplicate record. (See "Merge Strategy" below.)'),
    'new rev oldest' => t('Create a new revision of the oldest existing duplicate record. (See "Merge Strategy" below.)'),
    'new rev all' => t('Create a new revisions of all existing duplicate records. (See "Merge Strategy" below.)'),
  );

  $form['biblio_advanced_import_duplicate_merge_fieldset']['biblio_advanced_import_duplicate_strategy'] = array(
    '#title' => t('Duplicate Strategy'),
    '#description' => t('Create a duplicate or skip a single record of an import or update existing records, if the import detects an already exiting biblio record.'),
    '#type' => 'radios',
    '#options' => $options,
    '#default_value' => variable_get('biblio_advanced_import_duplicate_strategy', 'create duplicate'),
    '#required' => TRUE,
  );

  $options = array(
    'md5' => t('Hashing over configurable biblio fields. (See "Hash Settings" below.)'),
    'isbn' => t('ISBN'),
    'issn' => t('ISSN'),
    'doi' => t('DOI'),
  );

  if (module_exists('biblio_pm')) {
    $options['pubmed'] = t('PubMed ID');
  }

  $form['biblio_advanced_import_duplicate_merge_fieldset']['biblio_advanced_import_detect_duplicate_strategy'] = array(
    '#title' => t('Detect Duplicate Strategy'),
    '#description' => t('These fields are used to decide if a new biblio record is a duplicate of an existing one. If you select more than one, only one of the fields has to be identical to declare two records as duplicate. P.e. if you choose "Hash" and "ISBN" duplicates will be found in general using the hash, but if available in both records, ISBN will be used.'),
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => variable_get('biblio_advanced_import_detect_duplicate_strategy', array('md5' => 'md5')),
  );

  $form['biblio_advanced_import_duplicate_merge_fieldset']['biblio_advanced_import_duplicate_criteria_fieldset'] = array(
    '#title' => t('Hash Settings'),
    '#description' => t('Select the criteria to create hashes to detect duplicate biblio records. But think twice about your selection. P.e. the ISBN is a perfect criteria. But if you import the same record from two different sources an one contains the ISBN while the other does not, they will not be considered as duplicates.'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $field_options = array('title' => t('Title'));
  if (module_exists('biblio_pm')) {
    // this key needs to be the name of the node property
    $field_options['biblio_pubmed_id'] = t('Pubmed ID');
  }
  $result = db_query("SELECT name FROM {biblio_fields}");
  while ($row = $result->fetchObject()) {
    $field_options[$row->name] = str_replace('biblio_', '', $row->name);
  }

  $form['biblio_advanced_import_duplicate_merge_fieldset']['biblio_advanced_import_duplicate_criteria_fieldset']['biblio_advanced_import_duplicate_criteria'] = array(
    '#type' => 'checkboxes',
    '#options' => $field_options,
    '#default_value' => variable_get('biblio_advanced_import_duplicate_criteria', array('title' => 'title', 'biblio_year' => 'biblio_year')),
  );

  $options = array(
    'override' => t('Override the existing record.'),
    'override but keep additional' => t('Override the existing record but keep non empty values that do not exist in the record to be imported.'),
    'add new' => t('Only add new values that are empty in the existing record.'),
    'override existing non empty' => t('Only override non empty existing values.'),
    'override existing non empty with non empty' => t('Only override non empty existing values with new non empty existing values.'),
  );

  $form['biblio_advanced_import_duplicate_merge_fieldset']['biblio_advanced_import_merge_strategy'] = array(
    '#title' => t('Merge Strategy'),
    '#description' => t('If you did not choose "Create duplicate record." or "Skip import of a record." as Duplicate Strategy, you have to select a Merge Strategy as well.'),
    '#type' => 'radios',
    '#options' => $options,
    '#default_value' => variable_get('biblio_advanced_import_merge_strategy', 'override'),
  );

  $form['biblio_advanced_import_citekey_creation_fieldset'] = array(
    '#title' => t('Cite ID Creation Strategy'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $options = array(
    'biblio' => t('Use biblio default automatic Cite ID creation.'),
    'fields' => t('Use configurable biblio fields. (See "Cite ID Creation Settings" below.)'),
  );

  $form['biblio_advanced_import_citekey_creation_fieldset']['biblio_advanced_import_citekey_creation_strategy'] = array(
    '#title' => t('Automatic Cite ID Creation Strategy'),
    '#description' => t("Note: Existing Cite IDs won't be changed."),
    '#type' => 'radios',
    '#options' => $options,
    '#default_value' => variable_get('biblio_advanced_import_citekey_creation_strategy', 'biblio'),
  );

  $form['biblio_advanced_import_citekey_creation_fieldset']['biblio_advanced_import_citekey_creation_fieldset'] = array(
    '#title' => t('Cite ID Creation Settings'),
    '#description' => t("Select the fields to automatically create Cite IDs. Note: Existing Cite IDs won't be changed."),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['biblio_advanced_import_citekey_creation_fieldset']['biblio_advanced_import_citekey_creation_fieldset']['biblio_advanced_import_citekey_creation'] = array(
    '#type' => 'checkboxes',
    '#options' => $field_options,
    '#default_value' => variable_get('biblio_advanced_import_citekey_creation', array('title' => 'title', 'biblio_year' => 'biblio_year')),
  );

  $options = array(
    'skip' => t('Skip automatic Cite ID creation.'),
    'append counter' => t('Append counter.'),
  );

  $form['biblio_advanced_import_citekey_creation_fieldset']['biblio_advanced_import_duplicate_citekey_strategy'] = array(
    '#title' => t('Duplicate Cite ID Strategy'),
    '#description' => t("Note: Existing Cite IDs won't be changed."),
    '#type' => 'radios',
    '#options' => $options,
    '#default_value' => variable_get('biblio_advanced_import_duplicate_citekey_strategy', 'skip'),
  );

  $form['biblio_advanced_import_workarounds_fieldset'] = array(
    '#title' => t('Biblio Import Pitfall Workarounds'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $options = array(
    'as is' => t('Store field ISBN as is'),
    'remove' => t('Remove invalid ISBN'),
    'convert 13' => t('Convert ISBN-10 into ISBN-13 and normalize ISBN-13 and GTIN-14'),
  );

  $form['biblio_advanced_import_workarounds_fieldset']['biblio_advanced_import_fix_isbn'] = array(
    '#title' => t('ISBN'),
    '#description' => t("There are three different formats ISBN-10, ISBN-13, GTIN-14."),
    '#type' => 'radios',
    '#options' => $options,
    '#default_value' => variable_get('biblio_advanced_import_fix_isbn', 'as is'),
  );

  $options = array(
    'as is' => t('Store field ISSN as is'),
    'normalize' => t('Normalize (numbers only)'),
    'normalize from isbn' => t('Copy from ISBN if necessary and normalize (numbers only)'),
  );

  $form['biblio_advanced_import_workarounds_fieldset']['biblio_advanced_import_fix_issn'] = array(
    '#title' => t('ISSN'),
    '#description' => t('RIS format does not distinguish between ISBN and ISSN, so biblio imports ISSN as ISBN by default.'),
    '#type' => 'radios',
    '#options' => $options,
    '#default_value' => variable_get('biblio_advanced_import_fix_issn', 'as is'),
  );

  $options = array(
    'as is' => t('Store field DOI as is'),
    'one valid' => t('Only store the first valid DOI if additional text or a list of DOIs is provided'),
  );

  $form['biblio_advanced_import_workarounds_fieldset']['biblio_advanced_import_fix_doi'] = array(
    '#title' => t('DOI'),
    '#description' => t("Some import formats include multiple DOIs which is not supported by biblio."),
    '#type' => 'radios',
    '#options' => $options,
    '#default_value' => variable_get('biblio_advanced_import_fix_doi', 'as is'),
  );

  $options = array(
    'as is' => t('Store field URL as is'),
    'one valid' => t('Only store the first valid URL if additional text or a list of URLs is provided'),
  );

  $form['biblio_advanced_import_workarounds_fieldset']['biblio_advanced_import_fix_url'] = array(
    '#title' => t('URL'),
    '#description' => t("Some import formats include multiple URLs which is not supported by biblio."),
    '#type' => 'radios',
    '#options' => $options,
    '#default_value' => variable_get('biblio_advanced_import_fix_url', 'as is'),
  );

  $options = array(
    'as is' => t('Store field Title as is'),
    'mendeley bibtex' => t('Remove {} from Title exported by Mendeley using BibTex'),
  );

  $form['biblio_advanced_import_workarounds_fieldset']['biblio_advanced_import_fix_title'] = array(
    '#title' => t('Title'),
    '#description' => '',
    '#type' => 'radios',
    '#options' => $options,
    '#default_value' => variable_get('biblio_advanced_import_fix_title', 'as is'),
  );

  $form['biblio_advanced_import_messages'] = array(
    '#title' => t('Message configuration'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $options = array(
    '0'  => t('Off'),
    '1'  => t('On'),
  );
  $form['biblio_advanced_import_messages']['biblio_advanced_import_verbose_messages'] = array(
    '#title' => t('Verbose messages'),
    '#description' => 'This option enables verbose messages that provide information about the decisions taken during import, such as whether creation of a new node is suppressed due to duplicate detection.',
    '#type' => 'radios',
    '#options' => $options,
    '#default_value' => variable_get('biblio_advanced_import_verbose_messages', '0'),
  );

  $form['#submit'] = array('biblio_advanced_import_settings_form_submit');

  return system_settings_form($form);
}


/**
 * Validation of biblio_advanced_import_settings_form().
 */
function biblio_advanced_import_settings_form_validate($form, &$form_state) {
  if ('create duplicate' != $form_state['values']['biblio_advanced_import_duplicate_strategy']) {
    $no_detect_duplicate_strategy = TRUE;
    foreach ($form_state['values']['biblio_advanced_import_detect_duplicate_strategy'] as $value) {
      if ($value) {
        $no_detect_duplicate_strategy = FALSE;
        break;
      }
    }
    if ($no_detect_duplicate_strategy) {
      form_set_error('biblio_advanced_import_detect_duplicate_strategy', t('You need to specifiy at least one criteria.'));
    }

    if (in_array('md5', $form_state['values']['biblio_advanced_import_detect_duplicate_strategy'], TRUE)) {
      $no_duplicate_criteria = TRUE;
      foreach ($form_state['values']['biblio_advanced_import_duplicate_criteria'] as $value) {
        if ($value) {
          $no_duplicate_criteria = FALSE;
          break;
        }
      }
      if ($no_duplicate_criteria) {
        form_set_error('biblio_advanced_import_duplicate_criteria', t('You need to specifiy at least one criteria.'));
      }
    }

    if (empty($form_state['values']['biblio_advanced_import_merge_strategy'])) {
      form_set_error('biblio_advanced_import_merge_strategy', t('You need to specifiy a merge strategy.'));
    }
  }
}


/**
 * Submit handler of biblio_advanced_import_settings_form().
 *
 * Checks if duplicate strategy has been changed which requires
 * an update of all hashes of all existing biblio nodes.
 * If required, a batch process will be started to recalculate
 * all hashes.
 */
function biblio_advanced_import_settings_form_submit($form, &$form_state) {
  if ($form_state['values']['biblio_advanced_import_citekey_creation_strategy'] == 'fields'
     && variable_get('biblio_advanced_import_citekey_creation_strategy', 'biblio') != 'fields') {

    // save existing custom citekey creation php code
    variable_set('biblio_advanced_import_old_citekey_phpcode', variable_get('biblio_citekey_phpcode', ''));
    // insert biblio_advanced_import citekey creation php code
    // @see biblio_advanced_import_create_citekey()
    variable_set('biblio_citekey_phpcode', 'return biblio_advanced_import_create_citekey($node);');
  }
  elseif ($form_state['values']['biblio_advanced_import_citekey_creation_strategy'] != 'fields'
     && variable_get('biblio_advanced_import_citekey_creation_strategy', 'biblio') == 'fields') {

    variable_set('biblio_citekey_phpcode', variable_get('biblio_advanced_import_old_citekey_phpcode', ''));
  }

  if ($form_state['values']['biblio_advanced_import_duplicate_strategy'] != 'create duplicate') {
    $update_hashes = FALSE;
    if (variable_get('biblio_advanced_import_duplicate_strategy', 'create duplicate') == 'create duplicate') {
      $update_hashes = TRUE;
    }
    else {
      $old_values = variable_get('biblio_advanced_import_duplicate_criteria', array('title' => 'title', 'biblio_year' => 'biblio_year'));
      foreach ($form_state['values']['biblio_advanced_import_duplicate_criteria'] as $key => $value) {
        if ($value) {
          if (!array_key_exists($key, $old_values) || ((string) $old_values[$key] != (string) $value)) {
            $update_hashes = TRUE;
            break;
          }
        }
      }
    }

    if ($update_hashes) {
      $batch = array(
        'title' => t('Re-hashing existing biblio nodes'),
        'operations' => array(
          array('biblio_advanced_import_update_hashes_batch', array()),
        ),
        'finished' => 'biblio_advanced_import_update_hashes_batch_finished',
        'file' => 'biblio_advanced_import.batch.inc',
      );
      batch_set($batch);

      // TODO block import and creation of new biblio nodes until batch is finished
    }
  }
}
