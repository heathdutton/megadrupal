<?php

/**
 * @file
 * Helper functions for mosaik administration.
 */

/**
 * Produces a Mosaik translation form.
 */
function i18n_mosaik_translation_form($form, $form_state, $translation_set = NULL, $source = NULL) {

  $form['translation_set'] = array(
    '#type' => 'value',
    '#value' => $translation_set,
  );

  $translations = $translation_set ? $translation_set->get_translations() : array();

  $form['translations'] = array(
    '#type' => 'fieldset',
    '#title' => t('Select translations'),
    '#description' => t('Select existing mosaiks or type new ones that will be created for each language.'),
    '#tree' => TRUE,
  );

  // List of mosaiks for languages.
  foreach (i18n_language_list() as $lang => $langname) {
    if ($source && ($source['language'] == $lang)) {
      // This is the source mosaik, we don't change it.
      $form['source_mosaik'] = array(
        '#type' => 'value',
        '#value' => $source,
      );
      $form['translations'][$lang] = array(
        '#title' => $langname,
        '#type' => 'item',
        '#markup' => $source['title'],
        '#langcode' => $lang,
      );
    }
    else {
      $form['translations'][$lang] = array(
        '#title' => $langname,
        '#type' => 'textfield',
        '#default_value' => isset($translations[$lang]) ? $translations[$lang]['title'] : '',
        '#autocomplete_path' => 'i18n/mosaik/autocomplete/language/' . $lang,
        '#langcode' => $lang,
        '#maxlength' => 1024,
        '#element_validate' => array('i18n_mosaik_autocomplete_validate'),
      );
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  if ($translation_set) {
    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
    );
  }

  return $form;
}

/**
 * Form element validate handler for mosaik autocomplete element.
 */
function i18n_mosaik_autocomplete_validate($element, &$form_state) {
  // Autocomplete widgets do not send their names in the form, so we must detect
  // them here and process them independently.
  $value = array();
  if ($mosaiks = $element['#value']) {
    // Translate mosaik titles into actual mosaiks.
    $typed_mosaiks = drupal_explode_tags($mosaiks);
    foreach ($typed_mosaiks as $typed_mosaik) {
      // See if the mosaik exists and return the name;
      // otherwise, create a new 'autocreate' mosaik for insert/update.
      $criteria = array(
        'title' => trim($typed_mosaik),
        'language' => $element['#langcode'],
      );

      if ($possibilities = mosaik_load_multiple_by_criteria($criteria)) {
        $mosaik = array_pop($possibilities);
      }
      else {
        if (!empty($form_state['values']['translations'])) {
          foreach ($form_state['values']['translations'] as $lang => $translations) {
            if ($lang != $element['#langcode']) {
              $mosaik = reset($translations);
              $mosaik['path'] .= '-0';
              break;
            }
          }
        }
        else {
          $mosaik = mosaik_default();
        }

        $mosaik['autocreate'] = TRUE;
        $mosaik['name'] = preg_replace("/[^a-z0-9_]+/", "_", strtolower($typed_mosaik));
        $mosaik['title'] = $typed_mosaik;
        $mosaik['language'] = $element['#langcode'];
      }
      $value[] = $mosaik;
    }
  }

  form_set_value($element, $value, $form_state);
}

/**
 * Form callback: Process mosaik translation form.
 */
function i18n_mosaik_translation_form_submit($form, &$form_state) {
  $translation_set = $form_state['values']['translation_set'];

  switch ($form_state['values']['op']) {
    case t('Save'):
      $mosaik_translations = array_filter($form_state['values']['translations']);
      foreach ($mosaik_translations as $lang => $lang_mosaiks) {
        $item = reset($lang_mosaiks);
        if (!empty($item['autocreate'])) {
          unset($item['autocreate']);
          $mosaik = $item;
          mosaik_save($mosaik);

          if (!empty($mosaik['pieces'])) {
            pieces_save_associations($mosaik, $mosaik['pieces']);
          }
        }
        else {
          $mosaik = $item;
        }
        $translations[$lang] = $mosaik;
      }

      if (!empty($form_state['values']['source_mosaik'])) {
        $mosaik = $form_state['values']['source_mosaik'];
        $translations[$mosaik['language']] = $mosaik;
      }

      if (!empty($translations)) {
        $translation_set = $translation_set ? $translation_set : i18n_translation_set_create('mosaik', $mosaik['name']);
        $translation_set
          ->reset_translations($translations)
          ->save(TRUE);
        drupal_set_message(t('Mosaik translations have been updated.'));
      }
      else {
        drupal_set_message(t('There are no translations to be saved.'), 'error');
      }
      break;

    case t('Delete'):
      $translation_set->delete(TRUE);
      drupal_set_message(t('The mosaik translation has been deleted.'));
      break;
  }

  $form_state['redirect'] = MOSAIK_ADMIN_MENU . '/sets';
}

/**
 * Generate a tabular listing of translations for mosaiks.
 */
function i18n_mosaik_translation_sets_overview() {
  module_load_include('admin.inc', 'i18n_translation');
  drupal_set_title(t('Mosaik'));
  $query = db_select('i18n_translation_set', 't');
  return i18n_translation_admin_overview('mosaik', $query);
}

/**
 * Callback for mosaik translation tab.
 */
function i18n_mosaik_translation_mosaik_overview($mosaik) {
  if ($mosaik['i18n_tsid']) {
    // Already part of a set, grab that set.
    $i18n_tsid = $mosaik['i18n_tsid'];
    $translation_set = i18n_translation_set_load($mosaik['i18n_tsid']);
    $translation_set->get_translations();
    $translations = $translation_set->get_translations();
  }
  else {
    // We have no translation source nid, this could be a new set, emulate that.
    $i18n_tsid = $mosaik['name'];
    $translations = array($mosaik['language'] => $mosaik);
  }
  $type = variable_get('translation_language_type', LANGUAGE_TYPE_INTERFACE);
  $header = array(t('Language'), t('Title'), t('Operations'));

  foreach (i18n_language_list() as $langcode => $language_name) {
    $options = array();
    if (isset($translations[$langcode])) {
      // Existing translation in the translation set: display status.
      // We load the full node to check whether the user can edit it.
      $translation_mosaik = mosaik_load($translations[$langcode]['name']);
      $path = $translation_mosaik['path'];
      $title = l($translation_mosaik['title'], $path);

      $options['edit'] = array(
        '#type' => 'link',
        '#title' => t('edit'),
        '#href' => MOSAIK_ADMIN_MENU . '/' . $translation_mosaik['name'] . '/edit',
        '#options' => array(
          'query' => drupal_get_destination(),
        ),
      );

      if ($translation_mosaik['name'] == $i18n_tsid) {
        $language_name = t('<strong>@language_name</strong> (source)', array('@language_name' => $language_name));
      }
    }
    else {
      // No such translation in the set yet: help user to create it.
      $title = t('n/a');
      $options['add'] = array(
        '#type' => 'link',
        '#title' => t('add translation'),
        '#href' => MOSAIK_ADMIN_MENU . '/add',
        '#options' => array(
          'query' => array(
              'translation' => $mosaik['name'],
              'target' => $langcode
            ) + drupal_get_destination(),
        ),
      );
    }
    $rows[$langcode] = array(
      'language' => $language_name,
      'title' => $title,
      'operations' => array('data' => $options),
    );
  }

  drupal_set_title(t('Translations of mosaik %title', array('%title' => $mosaik['title'])), PASS_THROUGH);

  $build['translation_overview'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );

  return $build;
}
