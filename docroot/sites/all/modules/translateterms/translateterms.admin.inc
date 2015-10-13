<?php
/**
 * @file
 * Admin page callbacks for the Translate Terms module.
 */

/**
 * Menu callback for the Translate Terms module to display its administration.
 */
function translateterms_admin($form, $form_state, $vocabulary) {
  global $language;
  $globallang = $language->language;
  $languages = array(LANGUAGE_NONE => 'Undefined');
  $languages = array_merge($languages, locale_language_list());

  $form['vocabulary'] = array('#type' => 'value', '#value' => $vocabulary);
  $form['term'] = array(
    '#tree' => TRUE,
    '#theme' => 'translateterms_terms',
  );
  // Get all terms for current vocabulary.
  $terms = taxonomy_get_tree($vocabulary->vid);
  // Sort these by translation id of i18n.
  usort($terms, 'translateterms_admin_terms_sort');
  $terms_length = count($terms);
  foreach ($terms as $key => $term) {
    if (!isset($row)) {
      foreach ($languages as $lang => $langname) {
        $row[$lang] = $lang == LANGUAGE_NONE ? translateterms_textbox_combo('', $lang, '') : translateterms_textbox_combo('autocreate', $lang, '');
      }
    }
    $row[$term->language] = translateterms_textbox_combo($term->tid, $term->language, $term->name);
    if ($term->language == LANGUAGE_NONE) {
      $row[$globallang] = translateterms_textbox_combo($term->tid, $globallang, $term->name);
    }

    // Set next_term to false if at end of $terms.
    $next_term = (++$key) > $terms_length - 1 ? FALSE : $terms[$key];

    // If at end of $terms, next translation id will change or
    // translation id indicates no translation set exists,
    // add a new row to $form['term'].
    if ($next_term == FALSE || $term->i18n_tsid < $next_term->i18n_tsid || $term->i18n_tsid == 0) {
      $form['term'][] = $row;
      // Clear the row for next entry.
      unset($row);
    }
  }
  if (count(element_children($form['term'])) == 0) {
    drupal_set_message(t("The table is empty! Check your !link or add some terms", array('!link' => l(t('vocabulary multilingual options'), 'admin/structure/taxonomy'))), "warning");
  }
  else {
    $form['actions'] = array(
      '#type' => 'container',
      '#attributes' => array('class' => array('form-actions', 'container-inline')),
    );
    $form['actions']['save'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#weight' => 3,
    );
  }
  return $form;
}
/**
 * Function to return a textbox combo form.
 */
function translateterms_textbox_combo($tid, $lang, $value) {
  $form = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('term-container', 'container-inline')),
  );
  $form['tid'] = array(
    '#type' => 'value',
    '#value' => $tid,
  );
  if ($lang == LANGUAGE_NONE) {
    $form['value'] = array(
      '#type' => 'item',
      '#markup' => check_plain($value),
    );
  }
  else {
    $form['value'] = array(
      '#type' => 'textfield',
      '#default_value' => $value,
    );
  }
  return $form;
}

/**
 * Trigger when the user submits the page.
 */
function translateterms_admin_submit($form, &$form_state) {
  if (!in_array($form_state['clicked_button']['#id'], array('edit-save'))) {
    // Submit the form only for save button.
    return;
  }
  $vocabulary = $form_state['values']['vocabulary'];
  if (!empty($form_state['values']['term'])) {
    $terms = $form_state['values']['term'];
    foreach ($terms as $key => $term_translations) {
      // Remove any termtranslations that have language set to LANGUAGE_NONE.
      unset($term_translations[LANGUAGE_NONE]);
      foreach ($term_translations as $lang => $term_translation) {
        if ($term_translation['tid'] == 'autocreate' && isset($term_translation['value'])) {
          $values = array(
            'tid' => NULL,
            'vid' => $vocabulary->vid,
            'name' => $term_translation['value'],
            'language' => $lang,
          );
          $term = (object) $values;
          if (!empty($term->name) && !empty($term->vid)) {
            taxonomy_term_save($term);
            $term = taxonomy_term_load($term->tid);
          }
          else {
            unset($term);
          }
        }
        else {
          $term = taxonomy_term_load($term_translation['tid']);
          $term->name = $term_translation['value'];
          $term->language = $lang;
          taxonomy_term_save($term);
        }
        if (isset($term)) {
          $translations[$lang] = $term;
        }
        unset($term);
      }
      if (!empty($translations)) {
        $i18n_tsid = 0;
        foreach ($translations as $key => $term) {
          if (isset($term->i18n_tsid) && $term->i18n_tsid > $i18n_tsid) {
            $i18n_tsid = $term->i18n_tsid;
          }
        }
        if ($i18n_tsid != 0) {
          $translation_set = i18n_translation_set_load($i18n_tsid);
        }
        else {
          $type = variable_get('translation_language_type', LANGUAGE_TYPE_INTERFACE);
          $translation_set = i18n_translation_set_create('taxonomy_term', $vocabulary->machine_name);
        }
        if (isset($translation_set)) {
          $translation_set->reset_translations($translations)->save(TRUE);
        }
        unset($translation_set);
        unset($i18n_tsid);
      }
      unset($translations);
    }
    drupal_set_message(t('Your changes are saved'));
  }
}

/**
 * Theme the form so it will display as a table.
 */
function theme_translateterms_terms($variables) {
  $form = $variables['form'];
  $rows = array();
  foreach (element_children($form) as $key) {
    $rows[$key] = array('data' => array());
    foreach (element_children($form[$key]) as $lang) {
      $rows[$key]['data'][] = array('data' => drupal_render($form[$key][$lang]), 'class' => 'translateterms-cell');
    }
  }
  $header = array(
    array('data' => t('Undefined'), 'title' => t('Undefined terms')),
  );
  foreach (locale_language_list() as $languagename) {
    $header[] = array(
      'data' => t($languagename),
      'title' => t('@language terms', array('@language' => $languagename)),
    );
  }
  $output = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => 'translateterms-wrapper'),
    ));
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Function to sort terms by translation id.
 */
function translateterms_admin_terms_sort($term1, $term2) {
  return ($term1->i18n_tsid > $term2->i18n_tsid);
}
