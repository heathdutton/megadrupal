<?php
/**
 * @file
 * Enables taxonomy terms to be exported and imported.
 *
 * When exporting, all if the vocabularies details will be exported
 * along with all the terms.
 *
 * When importing, all of the vocabularies on the receiving instance
 * will be checked first to check that they match the vocabularies
 * in the dataset. If there are any differences in the vocabularies
 * then they import routine will raise an error then abort.
 *
 * After updating or inserting a taxonomy term there will be a check
 * to see if the i18ntaxonomy module is installed.  If it is installed
 * then the value of the 'language' column of the {term_data} table
 * will be updated with the imported value.
 *
 * It is better that this is called after the term is updated or
 * inserted with the API functions as the hook_taxonomy() function of
 * i18ntaxonomy.module incorrectly sets the value to a default value
 * if it can't find a value in a couple of forms.
 */

/**
 * Callback function to export taxonomy terms.
 */
function data_export_import_callback_export_taxonomy_terms() {

  return drupal_get_form('data_export_import_export_taxonomy_terms_form');
}

/**
 * Function to create form to export taxonomy terms.
 */
function data_export_import_export_taxonomy_terms_form($form_state) {

  $form['export_taxonomy_terms'] = array(
    '#type' => 'fieldset',
    '#title' => t('Export taxonomy terms'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $form['export_taxonomy_terms']['description'] = array(
    '#type' => 'item',
    '#title' => t('Export all taxonomy terms to a dataset file'),
  );

  // Adds a simple submit button that refreshes the form and clears its
  // contents. This is the default behavior for forms.
  $form['export_taxonomy_terms']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create dataset file'),
  );

  return $form;
}

/**
 * Function to process form to export taxonomy terms.
 */
function data_export_import_export_taxonomy_terms_form_submit($form, &$form_state) {

  // This module has not yet been extended to correctly handle data
  // which has internationalization (i18n) enabled.
  if (module_exists('i18n')) {

    drupal_set_message(t("The data export functionality is not currently compatible with internationalization (i18n)."), 'error');
    return TRUE;
  }

  // This is the function which will carry out the exporting of the
  // taxonomy terms to a dataset file.
  $dataset_file_created = data_export_import_export_taxonomy_terms_to_file();

  drupal_set_message(t("The following dataset file was created:"));
  drupal_set_message(check_plain($dataset_file_created));

  return TRUE;
}

/**
 * Exports detailed data about the vocabularies and its terms.
 *
 * This function is going to use the API to extract the relevant
 * data. This data is then going to be output to a data file.
 *
 * The first element in the array will be an array of the
 * vocabularies and the vocabulary details. This will be used
 * during the import to check that there are the correct
 * vocabularies in place which are ready to receive the terms being
 * imported.
 *
 * The second element will contain details of all the terms which
 * are in each vocabulary. These terms will be used to ensure that
 * the receiving instance ends up with a matching set of terms.
 *
 * @return string
 *   The filename of the datafile which was created.
 */
function data_export_import_export_taxonomy_terms_to_file() {

  // This will be the main array which will hold the data which will
  // be output to the dataset file.
  $dataset = array();

  // Add the vocabularies data the the array which will exported.
  $vocabularies_array = taxonomy_get_vocabularies();
  $dataset['vocabularies'] = $vocabularies_array;

  // Add the vocabulary terms to the main array. We will loop through
  // the vocabularies array and use the vid to then get the list of
  // terms in each vocabulary.
  foreach ($vocabularies_array as $vocabulary) {

    $vocabulary_id = $vocabulary->vid;

    // Here we will get the terms related to the vocabulary - then
    // we'll add on the synonyms and add on the related terms - then
    // we'll add the array on terms to the main array.
    $terms_array = data_export_import_taxonomy_get_tree_with_reset($vocabulary->vid, 0, NULL, TRUE, TRUE);

    // The $list_of_terms will be used to dedupe the terms returned by
    // data_export_import_taxonomy_get_tree_with_reset() which outputs
    // terms twice if they have two parents.
    $list_of_terms = array();
    foreach ($terms_array as $array_key => $term) {

      // If the term has been exported already then we don't want to
      // export it again. So we'll remove the term from the array.
      // This is probably to deal with the related terms - i.e. terms
      // will be exported twice if they are related.
      if (in_array($term->tid, $list_of_terms)) {
        unset($terms_array[$array_key]);
      }
      else {

        // Here we are going to add all the term id to an array to
        // keep a list of all terms currently output. This is because
        // when terms have more than one parent they are output twice.
        // What we are going to do is to check to see if the term has
        // been output already - and if so then not re-output it
        // again.
        $list_of_terms[] = $term->tid;
      }
    }

    // Before we add the terms to the data being output we are going
    // to set the array key value to be the term id. Currently the
    // array of terms have a sequential key: 0,1,2,3 etc - these will
    // be changed to be the term id value. As the terms id's are
    // unique then there will be no conflict - and this makes the
    // array more useful later on.
    $terms_array_with_tid_as_key = array();
    foreach ($terms_array as $term) {
      $terms_array_with_tid_as_key[$term->tid] = $term;
    }

    // Now add the array of terms to the dataset array and use the vid
    // as the key. Using the vid as key makes it easier to search this
    // array during the import.
    $dataset['vocabulary_terms'][$vocabulary_id] = $terms_array_with_tid_as_key;
  }

  // Here we will serialize the array to convert it to a string which
  // can then be output to a file.
  $dataset_serialized = serialize($dataset);

  // Create the default directory to hold the datasets.
  $dataset_directory_parent_directory = variable_get('file_public_path', conf_path() . '/files') . "/data_export_import";
  file_prepare_directory($dataset_directory_parent_directory, $mode = FILE_CREATE_DIRECTORY);

  $dataset_directory = variable_get('file_public_path', conf_path() . '/files') . "/data_export_import/taxonomy_terms/";
  file_prepare_directory($dataset_directory, $mode = FILE_CREATE_DIRECTORY);

  // Save the string as a file. This is the dataset data file.
  $file_name = format_date(REQUEST_TIME, 'custom', 'Ymd_His') . "_taxonomy_terms.dataset";
  $file_path_and_name = $dataset_directory . "/" . $file_name;

  file_unmanaged_save_data($dataset_serialized, $file_path_and_name, FILE_EXISTS_REPLACE);

  return $file_name;
}

/**
 * Callback function to import taxonomy terms.
 */
function data_export_import_callback_import_taxonomy_terms() {

  return drupal_get_form('data_export_import_import_taxonomy_terms_form');
}

/**
 * Function to create form to import taxonomy terms.
 */
function data_export_import_import_taxonomy_terms_form($form_state) {

  $form = array();

  $form['import_taxonomy_terms'] = array(
    '#type' => 'fieldset',
    '#title' => t('Import taxonomy terms'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  // Get the contents of the dataset directory and create a list of
  // links to dataset files.
  $directory = variable_get('file_public_path', conf_path() . '/files') . "/data_export_import/taxonomy_terms";

  $mask = '/.dataset/';

  $files = file_scan_directory($directory, $mask);

  // Sort them by the filename which is used as the key.  Since the
  // files are named using datetime stamps they will be listed in
  // date/time order.
  ksort($files);

  $options = array();
  $options['none'] = "None";

  foreach ($files as $file) {

    $options[$file->filename] = check_plain($file->filename);
  }

  $form['import_taxonomy_terms']['dataset_file'] = array(
    '#type' => 'radios',
    '#title' => t('Select file to import.'),
    '#default_value' => 'none',
    '#options' => $options,
  );

  $form['import_taxonomy_terms']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import dataset files'),
  );

  return $form;
}

/**
 * Function to process form to import terms.
 */
function data_export_import_import_taxonomy_terms_form_submit($form, &$form_state) {

  if ($form_state['values']['dataset_file'] != 'none') {

    $result = data_export_import_import_taxonomy_terms($form_state['values']['dataset_file']);

    if ($result) {
      drupal_set_message(t('The Taxonomy Terms dataset file %dataset_file was imported.', array('%dataset_file' => $form_state['values']['dataset_file'])));
    }
    else {
      drupal_set_message(t('The Taxonomy Terms dataset file %dataset_file was not imported.', array('%dataset_file' => $form_state['values']['dataset_file'])), 'error');
    }
  }
}

/**
 * Make taxonomy terms match the terms from a dataset file.
 *
 * This function is to effectively 'import' the vocabularies and all
 * the terms in those vocabularies.
 *
 * As we are expecting the vocabularies to exist already then we are
 * not going to import them but check that they exist and are the same
 * as in the imported dataset.
 *
 * NB - Later we may extend this to create any new vocabularies if
 * necessary.
 *
 * Then we are going to bring in the terms from the dataset file.
 *
 * NB - When this import has finished the terms in the receiving
 * instance should be an exact match with the terms in the imported
 * dataset file.  Think in terms of rsync with the '--delete'
 * option. This means that as well as importing new terms and updating
 * existing terms we need to delete terms from the reciving instance
 * which are not in the imported dataset.
 *
 * This synchronisation will be carried out by two passes.
 *
 * First we will loop through the terms in the receiving instance  and
 * check against the the imported dataset. If the term exists in the
 * dataset then it will be updated in the receiving instance.  If it
 * doesn't exist in the dataset then it will be deleted from the
 * receiving Drupal instance.
 *
 * The second pass will be a loop through the dataset - any terms
 * which are in the dataset but are not in the receiving Drupal
 * instance will be created.
 *
 * This will effectively mean that the terms have been sychronised
 * completely.
 *
 * NB - There is some deeper logic here we need to be aware of.  The
 * term ID's need to match exactly otherwise the related terms will
 * not match.  So when the new terms are created they will need to
 * have their old ID's set to match exactly - again due to the related
 * terms.
 *
 * Also, this should deal with the scenario where a new term has been
 * created in the receiving instance which has the same tid as one
 * being imported but it is in a different vocabulary.
 *
 * @param string $file
 *   The dataset file which is being imported.
 *
 * @return bool
 *   TRUE if the import process ran without errors.
 */
function data_export_import_import_taxonomy_terms($file) {

  // Load the dataset file into a variable.
  $file_content = file_get_contents(variable_get('file_public_path', conf_path() . '/files')  . "/data_export_import/taxonomy_terms/" . $file);

  // Decode the serialized data and store it in an array of objects.
  $file_content_as_array = unserialize($file_content);

  // This will check that the vocabularies in the dataset file all
  // exist in the receiving instance and are an exact match.
  // NB - The receiving instance may have extra vocabularies which
  // have been created.  This is OK because even though there is not
  // an exact match at least the vocabularies which are needed to
  // enable the terms to be imported all exist and are an exact match
  // to the vocabularies exported from.
  foreach ($file_content_as_array['vocabularies'] as $vocabulary_from_file) {

    // Get the existing vocabulary as an object.
    // Second parameter is set to TRUE to reset the cache - needed as
    // we call this function after we've unset some values in a
    // previous usage.
    $existing_vocabulary = taxonomy_vocabulary_load($vocabulary_from_file->vid, TRUE);

    // Here we need to carry out a little fix to make the formats
    // match - this probably should not be needed and has been
    // reported as http://drupal.org/node/1286320.
    unset($vocabulary_from_file->type);

    // Here we need to unset the hierarchy value for both objects.
    // This is for two reasons.
    // 1. The hierarchy value is different dependent on the terms.
    // I.e. If any term has two parents then it should be set to a
    // value of '2'.  So we can't expect the values to be the same
    // because the imported terms may have different numbers of
    // parents etc.
    // 2. The hierarchy value does not appear to be being set properly
    // all the time - this has been registered as a bug at:
    // http://drupal.org/node/1309990.
    unset($vocabulary_from_file->hierarchy);
    unset($existing_vocabulary->hierarchy);

    // We will also unset the nodes properties.  The receiving instance may
    // have different content types - and as part of a new setup the
    // vocabularies may be linked to different content types.
    unset($vocabulary_from_file->nodes);
    unset($existing_vocabulary->nodes);

    // Check the vocabularies actually match.
    if ($vocabulary_from_file != $existing_vocabulary) {

      drupal_set_message(t('There is a mismatch between the vocabularies in the dataset file and the current vocabularies - vocabulary name: %vocabular_name', array('%vocabular_name' => $vocabulary_from_file->name)), 'error');

      return FALSE;
    }
  }

  // Here we will loop through the vocabulary terms in the receiving
  // instance.  For each term we will look into the dataset being
  // imported.  If the term exists in the imported dataset then we
  // will update it - if it does not exist then we will delete it.
  // *
  // We will use the API to get the list of vocabularies - and then
  // loop through each vocabulary and get all the terms. This should
  // deal with the issue where a new term has been created in the
  // receiving instance which has the same tid as one being imported
  // but is in a different vocabulary.
  // *
  // NB - Effectively all new terms in the receiving instance will be
  // deleted - which is fine as we don't want them.
  // *
  // In this case we will first need to delete this term completely in
  // the receiving instance.  It will then be created again when we
  // loop through the dataset as it will be created and assigned to
  // the correct vocabulary.
  // *
  // This should mean that the terms for a vocabulary in the receiving
  // instance match exactly the terms in the imported dataset.
  $existing_vocabularies_array = taxonomy_get_vocabularies();

  // We will loop through the vocabularies array and use the vid to
  // then get the list of terms in that vocabulary.
  foreach ($existing_vocabularies_array as $existing_vocabulary) {

    // Get the vocabulary ID so we can use it to get the terms in that
    // vocabulary.
    $existing_vocabulary_id = $existing_vocabulary->vid;

    // Here we are looping through all of the terms in the vocabulary.
    // NB - These are terms which are in the receiving instance and we
    // are going to check them against the terms in the dataset being
    // imported.
    $existing_terms_array = data_export_import_taxonomy_get_tree_with_reset($existing_vocabulary_id, 0, NULL, TRUE, TRUE);

    // We will use this array to hold all the terms id's which have
    // been found.  The function taxonomy_get_tree_with_reset outputs
    // terms more than once if they have multiple parents. This will
    // mean we can ignore extra copies of the terms.
    $list_of_terms = array();
    foreach ($existing_terms_array as $existing_term) {

      // If the term has been exported already then we don't want to
      // export it again so we'll go to the next term in the foreach loop.
      if (in_array($existing_term->tid, $list_of_terms)) {

        continue;
      }

      // Add the term id to the list we've worked on.
      $list_of_terms[] = $existing_term->tid;

      // Look for a term in the dataset being imported which matches
      // the existing term.
      // *
      // If one doesn't exist then we'll delete it from the existing
      // vocabulary.
      // *
      // If we find a matching term then we will check if the existing
      // term is an exact match or not - and if it is not then we'll
      // update the existing term with the term data from the dataset.
      // *
      // Check to see if the term can be found in the dataset file
      // with a matching vocabulary id and term id. If it does not
      // exist then is should be deleted from the receiving instance.
      if (!isset($file_content_as_array['vocabulary_terms'][$existing_vocabulary_id][$existing_term->tid])) {

        $return_status = taxonomy_term_delete($existing_term->tid);
      }

      if (isset($file_content_as_array['vocabulary_terms'][$existing_vocabulary_id][$existing_term->tid])) {

        // We are going to check that the terms match exactly.  First
        // we need to add some data to the existing terms and unset
        // some fields.
        // *
        // The the existing term does not match the term in the
        // dataset exactly then it will be updated to make it match
        // the term being imported.
        // *
        // We will sort the synonyms to put them in the same order as
        // they are in the dataset file.
        // *
        // We unset the depth as this is calculated and can be
        // different when terms have two parents which are at
        // different depths.
        unset($existing_term->depth);
        unset($file_content_as_array['vocabulary_terms'][$existing_vocabulary_id][$existing_term->tid]->depth);

        // If the terms don't match then update the existing term.
        // *
        // Get the object which is the existing term in the receiving
        // instance.
        $existing_term_object = $existing_term;

        // Get the term object from the dataset array.
        $term_object_from_dataset = $file_content_as_array['vocabulary_terms'][$existing_vocabulary_id][$existing_term->tid];

        if ($existing_term_object == $term_object_from_dataset) {

          // Don't update the term - maybe carry out a count later.
          $place_holder = 1;
        }

        // Update the term with the data from the dataset.
        if ($existing_term_object != $term_object_from_dataset) {

          // Update the term with data from the dataset.
          $term_data_array = array(
            'tid' => $existing_term->tid,
            'vid' => $existing_vocabulary_id,
            'name' => $file_content_as_array['vocabulary_terms'][$existing_vocabulary_id][$existing_term->tid]->name,
            'description' => $file_content_as_array['vocabulary_terms'][$existing_vocabulary_id][$existing_term->tid]->description,
            'weight' => $file_content_as_array['vocabulary_terms'][$existing_vocabulary_id][$existing_term->tid]->weight,
            'parent' => $file_content_as_array['vocabulary_terms'][$existing_vocabulary_id][$existing_term->tid]->parents,
          );

          // Here we are using the standard taxonomy_save_term() API
          // function as it handles terms being updated OK.
          // $save_term_status = taxonomy_save_term($term_data_array);
          $save_term_status = taxonomy_term_save((object) $term_data_array);

          if ($save_term_status != 2) {
            drupal_set_message(t("Term not saved correctly.  Term id:") . $existing_term->tid . "\n", 'error');
          }

          // Here we need to update the language field if the translation
          // module has been enabled.  This also picks up the fault of
          // the hook_taxonomy() function in i18ntaxonomy.module from
          // setting the language to a default setting if it can't find
          // a value in a couple of forms.  hook_taxonomy() is called
          // when using the
          // data_export_import_taxonomy_save_term_with_defined_tid()
          // function.
          if (module_exists('i18ntaxonomy')) {
            db_update('taxonomy_term_data')
              ->fields(array('language' => $file_content_as_array['vocabulary_terms'][$existing_vocabulary_id][$existing_term->tid]->language))
              ->condition('tid', $existing_term->tid)
              ->execute();
          }
        }
      }
    }
  }

  // Here we will pick up if the file contains no taxonomy terms.
  // if (count($file_content_as_array['vocabulary_terms']) == 0) {
  if (empty($file_content_as_array['vocabulary_terms'])) {
    return TRUE;
  }

  // Loop through the terms in the dataset being imported and if they
  // don't exist in the receiving instance then we will create them.
  foreach ($file_content_as_array['vocabulary_terms'] as $vid => $vocabulary_from_dataset) {

    foreach ($vocabulary_from_dataset as $term_from_dataset) {

      $get_term_result = entity_load('taxonomy_term', array($term_from_dataset->tid), array(), TRUE);

      if (empty($get_term_result)) {

        // Update the term with data from the dataset.
        $term_data_array = array(
          'tid' => $term_from_dataset->tid,
          'vid' => $term_from_dataset->vid,
          'name' => $term_from_dataset->name,
          'description' => $term_from_dataset->description,
          'weight' => $term_from_dataset->weight,
          'parent' => $term_from_dataset->parents,
        );

        $save_term_status = data_export_import_taxonomy_save_term_with_defined_tid($term_data_array);

        if ($save_term_status != 1) {
          drupal_set_message(t("Term not saved correctly.  Term id:") . $existing_term->tid . "\n", 'error');
        }

        // Here we need to update the language field if the translation
        // module has been enabled.  This also picks up the fault of
        // the hook_taxonomy() function in i18ntaxonomy.module from
        // setting the language to a default setting if it can't find
        // a value in a couple of forms.  hook_taxonomy() is called
        // when using the
        // data_export_import_taxonomy_save_term_with_defined_tid()
        // function.
        if (module_exists('i18ntaxonomy')) {

          db_update('taxonomy_term_data')
            ->fields(array('language' => $term_from_dataset->language))
            ->condition('tid', $term_from_dataset->tid)
            ->execute();

          if ($query_result == FALSE) {
            drupal_set_message(t("Insertion into term_data table failed."), 'error');
          }
        }
      }
    }
  }

  // Loop through all the vocabularies and reset the hierarchy
  // value. This value is a calculated value which is stored in the
  // vocabularies table and will need to be calculated dependent on
  // the structure of the terms in the vocabulary.
  // Value will be either: 0 - Flat list of terms. 1 - One or more
  // terms have a parent. 2 - One or more terms have more than one
  // parent.
  $vocabularies_array = taxonomy_get_vocabularies();
  foreach ($vocabularies_array as $vocabulary) {
    // Apparently the new reset API function should work OK. We need
    // to set the array with a fake tid field value to enable the
    // function to run.
    taxonomy_check_vocabulary_hierarchy($vocabulary, array("tid" => 0));
  }

  return TRUE;
}

/**
 * This function overrides the taxonomy_get_tree() API function.
 *
 * The extra functionality is to provide a reset flag as a
 * parameter. This enables the static attributes to be unset. This is
 * important as this function is used multiple times during the course
 * of this module. If it was not possible to unset the statics the
 * values would stay the same between the function calls resulting in
 * stale data being returned.
 *
 * @param int $vid
 *   Which vocabulary to generate the tree for.
 *
 * @param int $parent
 *   The term ID under which to generate the tree. If 0, generate the
 *   tree for the entire vocabulary.
 *
 * @param int $max_depth
 *   The number of levels of the tree to return. Leave NULL to return
 *   all levels.
 *
 * @param int $load_entities
 *   If TRUE, a full entity load will occur on the term
 *   objects. Otherwise they are partial objects queried directly from
 *   the {taxonomy_term_data} table to save execution time and memory
 *   consumption when listing large numbers of terms. Defaults to
 *   FALSE.
 *
 * @param bool $reset
 *   Whether or not to clear the static variables to force this
 *   function to get fresh data
 *
 * @return array
 *   An array of all term objects in the tree. Each term object is
 *   extended to  have "depth" and "parents" attributes in addition to
 *   its normal ones. Results are statically cached.
 */
function data_export_import_taxonomy_get_tree_with_reset($vid, $parent = 0, $max_depth = NULL, $load_entities = FALSE, $reset = FALSE) {

  $children = &drupal_static(__FUNCTION__, array());
  $parents = &drupal_static(__FUNCTION__ . ':parents', array());
  $terms = &drupal_static(__FUNCTION__ . ':terms', array());

  // Here we will unset the static values to force this function to
  // get fresh data.
  if ($reset) {
    unset($children[$vid]);
    unset($parents[$vid]);
    unset($terms[$vid]);
  }

  // We cache trees, so it's not CPU-intensive to call
  // taxonomy_get_tree() on a term and its children, too.
  if (!isset($children[$vid])) {
    $children[$vid] = array();
    $parents[$vid] = array();
    $terms[$vid] = array();

    $query = db_select('taxonomy_term_data', 't');
    $query->join('taxonomy_term_hierarchy', 'h', 'h.tid = t.tid');
    $result = $query
      ->addTag('translatable')
      ->addTag('term_access')
      ->fields('t')
      ->fields('h', array('parent'))
      ->condition('t.vid', $vid)
      ->orderBy('t.weight')
      ->orderBy('t.name')
      ->execute();

    foreach ($result as $term) {
      $children[$vid][$term->parent][] = $term->tid;
      $parents[$vid][$term->tid][] = $term->parent;
      $terms[$vid][$term->tid] = $term;
    }
  }

  // Load full entities, if necessary. The entity controller statically
  // caches the results.
  if ($load_entities) {
    $term_entities = taxonomy_term_load_multiple(array_keys($terms[$vid]));
  }

  $max_depth = (!isset($max_depth)) ? count($children[$vid]) : $max_depth;
  $tree = array();

  // Keeps track of the parents we have to process, the last entry is used
  // for the next processing step.
  $process_parents = array();
  $process_parents[] = $parent;

  // Loops over the parent terms and adds its children to the tree array.
  // Uses a loop instead of a recursion, because it's more efficient.
  while (count($process_parents)) {
    $parent = array_pop($process_parents);

    // The number of parents determines the current depth.
    $depth = count($process_parents);
    if ($max_depth > $depth && !empty($children[$vid][$parent])) {
      $has_children = FALSE;
      $child = current($children[$vid][$parent]);
      do {
        if (empty($child)) {
          break;
        }
        $term = $load_entities ? $term_entities[$child] : $terms[$vid][$child];
        if (isset($parents[$vid][$term->tid])) {
          // Clone the term so that the depth attribute remains correct
          // in the event of multiple parents.
          $term = clone $term;
        }
        $term->depth = $depth;
        unset($term->parent);
        $term->parents = $parents[$vid][$term->tid];
        $tree[] = $term;
        if (!empty($children[$vid][$term->tid])) {
          $has_children = TRUE;

          // We have to continue with this parent later.
          $process_parents[] = $parent;
          // Use the current term as parent for the next iteration.
          $process_parents[] = $term->tid;

          // Reset pointers for child lists because we step in there more often
          // with multi parents.
          reset($children[$vid][$term->tid]);
          // Move pointer so that we get the correct term the next time.
          next($children[$vid][$parent]);
          break;
        }
      } while ($child = next($children[$vid][$parent]));

      if (!$has_children) {
        // We processed all terms in this hierarchy-level, reset pointer
        // so that this function works the next time it gets called.
        reset($children[$vid][$parent]);
      }
    }
  }

  return $tree;
}

/**
 * This function is a modified version of the taxonomy_save_term function.
 *
 * @param array &$form_values
 *   Array of term data which is to be saved.
 *
 * @return int
 *   $status value showing if the term was saved OK.
 */
function data_export_import_taxonomy_save_term_with_defined_tid(&$form_values) {

  // I think this has the effect of making sure these values exist on
  // the array but not overwrite any existing values.
  $form_values += array(
    'description' => '',
    'weight' => 0,
  );

  // Insert the record into the term_data table.
  // We're using a direct insertion as the drupal_write_record
  // function does not allow us to insert a new row with a defined id
  // value.
  $id = db_insert('taxonomy_term_data')
    ->fields(array(
        'tid' => $form_values['tid'],
        'vid' => $form_values['vid'],
        'name' => $form_values['name'],
        'description' => $form_values['description'],
        'weight' => $form_values['weight'],
      ))
    ->execute();

  // Set up parameters used later when calling hook_taxonomy().
  $hook = 'insert';
  $status = SAVED_NEW;

  db_delete('taxonomy_term_hierarchy')
    ->condition('tid', $form_values['tid'])
    ->execute();

  if (!isset($form_values['parent']) || empty($form_values['parent'])) {
    $form_values['parent'] = array(0);
  }

  if (is_array($form_values['parent'])) {
    foreach ($form_values['parent'] as $parent) {
      if (is_array($parent)) {
        foreach ($parent as $tid) {
          $id = db_insert('taxonomy_term_hierarchy')
            ->fields(array(
                'tid' => $form_values['tid'],
                'parent' => $tid,
              ))
            ->execute();
        }
      }
      else {
        $id = db_insert('taxonomy_term_hierarchy')
          ->fields(array(
              'tid' => $form_values['tid'],
              'parent' => $parent,
            ))
          ->execute();
      }
    }
  }
  else {

    $id = db_insert('taxonomy_term_hierarchy')
      ->fields(array(
          'tid' => $form_values['tid'],
          'parent' => $form_values['parent'],
        ))
      ->execute();
  }

  // We're OK to run any hooks which are called. If we used
  // drupal_write_record() to insert the record then it would have
  // filled in the tid field with the newly created tid and passed it
  // back out via 'pass by reference'. Since we've already got the
  // tid field filled in as we're creating a new record but with the
  // defined (i.e. specified tid) then then field in $form_values is
  // already filled in.
  if (isset($hook)) {
    module_invoke_all('taxonomy', $hook, 'term', $form_values);
  }

  cache_clear_all();

  return $status;
}
