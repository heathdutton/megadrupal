<?php
/**
 * @file
 * Manipulations to have the abillity to lock wanted taxonomies to prevent changes.
 */


/**
 * Implements hook_form_alter().
 *
 * Adds additional submit function to the i18n string translation forms:
 *  - i18n_string_locale_translate_edit_form
 *  - i18n_string_translate_page_form
 *
 * @param array $form
 *   Nested array of form elements that comprise the form
 * @param array $form_state
 *   A keyed array containing the current state of the form
 * @param string $form_id
 *   String representing the name of the form itself
 */
function archibald_form_alter(&$form, &$form_state, $form_id) {

  if ($form_id == 'i18n_string_locale_translate_edit_form' && isset($form['context'])) {
    $parts = explode(':', $form['context']['#markup']);
    $category = $parts[0];
    $tid = $parts[1];
    if ($category == 'term' && is_numeric($tid)) {
      $term = taxonomy_term_load($tid);
      if ($term->vocabulary_machine_name == 'rights_licenses') {
        $form['#submit'][1] = $form['#submit'][0];
        $form['#submit'][0] = 'archibald_i18n_string_locale_translate_edit_form_submit';
        $form['tid'] = array(
          '#type' => 'hidden',
          '#value' => $tid
        );
      }
    }
  }

  if ($form_id == 'i18n_string_translate_page_form') {
    $parts = explode('/', request_path());
    $tid = $parts[3];

    if (drupal_strlen($parts[0]) != 2) {
      $tid = $parts[2];
    }

    $term = taxonomy_term_load($tid);

    if ($term->vocabulary_machine_name == 'rights_licenses') {
      $form['tid'] = array(
        '#type' => 'hidden',
        '#value' => $tid
      );
      $form['#submit'][1] = $form['#submit'][0];
      $form['#submit'][0] = 'archibald_i18n_string_translate_page_form_submit';
    }
  }
}

/**
 * This form submit function is called when a term of the vocabulary 'rights_licenses' is saved.
 * In case a new translation for the licence text is saved, mark all descriptions using this licence for republication.
 *
 * @param array $form
 *   Nested array of form elements that comprise the form
 * @param array $form_state
 *   A keyed array containing the current state of the form
 */
function archibald_i18n_string_locale_translate_edit_form_submit($form, &$form_state) {
  foreach ($form_state['values']['translations'] as $langcode => $new_name) {
    $old_name = $form_state['complete form']['translations'][$langcode]['#default_value'];

    if ($old_name != $new_name) {
      $term = taxonomy_term_load($form_state['values']['tid']);
      archibald_check_descriptions_for_republication($term, $old_name, $new_name, $langcode);
    }
  }
}

/**
 * This form submit function is called when a translation of term in the vocabulary 'rights_licenses' is saved.
 * In case a new translation for the licence text is saved, mark all descriptions using this licence for republication.
 *
 * @param array $form
 *   Nested array of form elements that comprise the form
 * @param array $form_state
 *   A keyed array containing the current state of the form
 */
function archibald_i18n_string_translate_page_form_submit($form, &$form_state) {

  $tid = $form_state['values']['tid'];
  $langcode = $form_state['values']['langcode'];
  $old_name = $form_state['build_info']['args'][0]['taxonomy:term:' . $tid . ':name']->translations[$langcode];
  $new_name = $form_state['input']['strings']['taxonomy:term:' . $tid . ':name'];

  if ($old_name != $new_name) {
    $term = taxonomy_term_load($tid);
    if ($term->vocabulary_machine_name == 'rights_licenses') {
      archibald_check_descriptions_for_republication($term, $old_name, $new_name, $langcode);
    }
  }
}

/**
 * Implements hook_taxonomy_term_presave().
 * In case a new name for the licence is saved, mark all descriptions using this licence for republication.
 *
 * @param object $term
 *   the term object which is saved.
 */
function archibald_taxonomy_term_presave($term) {
  if (!empty($term->tid)) {
    $old_name = taxonomy_term_load($term->tid)->name;
    $new_name = $term->name;

    if ($old_name != $new_name && $term->vocabulary_machine_name == 'rights_licenses') {
      archibald_check_descriptions_for_republication($term, $old_name, $new_name);
    }
  }
}

/**
 *
 * This function checks if any national published descriptions are using the changed vocabulary term as copyright text.
 * All descriptions using the changed term are marked for republication and the description change log is updated.
 *
 * @param object $term
 *  the term object which was changed
 * @param string $old_name
 *   the old name of the term
 * @param string $new_name
 *   the new name of the term
 * @param string $language
 *   the language in which the term was changed
 */
function archibald_check_descriptions_for_republication($term, $old_name, $new_name, $language = NULL) {

  if (empty($language)) {
    $language = language_default()->language;
  }

  $query = db_select('archibald_langstring_terms', 'alt')->distinct();
  $query->join('archibald_lom', 'al', 'al.rights_description = alt.langstring_terms_id');
  $query->join('archibald_lom_stats', 'als', 'als.publication_version = al.version');
  $query->fields('al', array('lom_id'));
  $query->condition('alt.term', $term->field_taxon_key['und'][0]['value']);

  $result = $query->execute();
  while ($lom = $result->fetchAssoc()) {
    archibald_need_republication($lom['lom_id']);
    archibald_insert_change_log(
      $lom['lom_id'],
      $language,
      'copyright',
      $old_name,
      $new_name
    );
  }
}

/**
 * This function checks if a term was used by at least one description for the copyright licence.
 * Mind that also older versions of descriptions are considered!
 *
 * @param object $term
 *   the taxonomy term object
 *
 * @return boolean
 *   TRUE if term is/was used by at least one description, FALSE if term was never used
 */
function archibald_licence_term_in_use($term) {

  $query = db_select('archibald_langstring_terms', 'alt')->distinct();
  $query->join('archibald_lom', 'al', 'al.rights_description = alt.langstring_terms_id');
  $query->fields('alt', array('term'));
  $query->condition('alt.term', $term->field_taxon_key['und'][0]['value']);
  $result = $query->execute();

  if ($result->rowCount() > 0) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}


/**
 *
 * This function returns an array containing machine names of all vocabularies which are required by the archibald module
 * and which must not be edited or deleted by the user.
 *
 * @return array
 *   list with vocabulary machine names
 */
function archibald_get_vocabulary_names() {
  return array(
    'aggregation_level' => 'aggregation_level',
    'difficulty' => 'difficulty',
    'education_context' => 'education_context',
    'educa_school_levels' => 'educa_school_levels',
    'educa_school_subjects' => 'educa_school_subjects',
    'general_catalog' => 'general_catalog',
    'intended_enduserrole' => 'intended_enduserrole',
    'lc_cont_role' => 'lc_cont_role',
    'mmd_cont_role' => 'mmd_cont_role',
    'learning_resourcetype' => 'learning_resourcetype',
    'rel_catalog' => 'rel_catalog',
    'rel_kind' => 'rel_kind',
    'rights_cost' => 'rights_cost',
    'technical_format' => 'technical_format',
    'typical_learning_time' => 'typical_learning_time'
  );
}


/**
 * Implements hook_form_FORM_ID_alter().
 * Do not allow users to edit or delete vocabularies used by archibald module.
 *
 * @param array $form
 *   Nested array of form elements that comprise the form
 * @param array $form_state
 *   A keyed array containing the current state of the form
 * @param string $form_id
 *   String representing the name of the form itself
 */
function archibald_form_taxonomy_form_vocabulary_alter(&$form, &$form_state, $form_id) {

  $taxonomy_names = archibald_get_vocabulary_names();

  if (isset($taxonomy_names[$form['#vocabulary']->machine_name])) {
    drupal_set_message(t('You are not allowed to edit or delete this vocabulary because it is required by the archibald module.'), 'warning');
    $form['actions']['#disabled'] = TRUE;
  }
}


/**
 * Implements hook_form_FORM_ID_alter().
 * Do not allow users to edit or delete terms of vocabularies used by archibald module.
 *
 * @param array $form
 *   Nested array of form elements that comprise the form
 * @param array $form_state
 *   A keyed array containing the current state of the form
 * @param string $form_id
 *   String representing the name of the form itself
 */
function archibald_form_taxonomy_form_term_alter(&$form, &$form_state, $form_id) {

  $taxonomy_names = archibald_get_vocabulary_names();

  if (is_array($form['#term'])) {
    $vocabulary_machine_name = $form['#term']['vocabulary_machine_name'];
    $tid = $form['#term']['tid'];
  }
  else {
    $vocabulary_machine_name = $form['#term']->vocabulary_machine_name;
    $tid = $form['#term']->tid;
  }

  if (isset($taxonomy_names[$vocabulary_machine_name])) {
    if ($vocabulary_machine_name == 'rights_licenses') {
      if (!empty($tid)) {
        $form['field_taxon_key']['#disabled'] = TRUE;

        if (archibald_licence_term_in_use(taxonomy_term_load($tid)) == TRUE) {
          drupal_set_message(t('You are not allowed to delete this term because its used by a description.'), 'warning');
          $form['actions']['delete']['#disabled'] = TRUE;
        }
      }
    }
    else{
      drupal_set_message(t('You are not allowed to add, edit or delete terms because they are managed by the archibald module.'), 'warning');
      $form['actions']['#disabled'] = TRUE;
    }
  }
}