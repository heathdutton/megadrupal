<?php
/**
 * @file
 * Administration screens for the Media Translation module.
 */


/**
 * Renders the Translate tab on the media file view/edit page.
 *
 * @param type $file
 * File being translated.
 *
 * @return string
 * HTML for the page.
 */
function media_translation_file_translate_tab($form, $form_state, $file = NULL) {
  $tsid = isset($file->i18n_tsid) ? $file->i18n_tsid : 0;
  $file_type = isset($file->type) ? $file->type : '';
  $media_type = media_get_type($file);

  if ($tsid == 0) {
    // Create an empty translation set structure, but do not save it.
    // Done so we have a consisten object structure for new vs. existing data.
    $translation_set = i18n_translation_set_build('file', array('type' => 'file', 'bundle' => $media_type, 'master_id' => 0, 'translations' => NULL));
  }
  else {
    $translation_set = i18n_translation_set_load($tsid);
  }
  // Initialize the translation objects
  $translations = $translation_set->get_objects();

  // prepopulate the current file into the translation set (mostly for new translation sets).
  if (empty($translations[$file->language])) {
    $translation_set->add_item($file, $file->language);
    $translations = $translation_set->get_objects();
  }

  $form = array();
  $form['fid'] = array(
    '#type' => 'value',
    '#value' => $file->fid,
  );
  $form['tsid'] = array(
    '#type' => 'value',
    '#value' => $tsid,
  );
  $form['file_type'] = array(
    '#type' => 'value',
    '#value' => $file_type,
  );
  $form['media_type'] = array(
    '#type' => 'value',
    '#value' => $media_type,
  );
  $form['translations'] = array(
    '#type' => 'fieldset',
    '#title' => t('Select translations'),
    '#description' => t('Select the existing files that will be displayed for each language.'),
    '#tree' => TRUE
  );
  foreach (i18n_language_list() as $lang => $langname) {
    $form['translations'][$lang] = array(
      '#title' => $langname,
      '#type' => 'textfield',
      '#default_value' => isset($translations[$lang]) ? _media_translation_create_file_tag($translations[$lang]->get_object()) : '',
      '#autocomplete_path' => 'media-translation/autocomplete/' . $lang . '/' . $file->type,
      '#langcode' => $lang,
      '#maxlength' => 1024,
//      '#element_validate' => array('media_translation_autocomplete_validate'),
    );
  }
  $form['actions']['save'] = array(
    '#type' => 'submit',
    '#name'   => 'save',
    '#value' => t('Save'),
  );
  if ($tsid > 0) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#name'   => 'delete',
      '#value' => t('Delete Translation Set'),
    );
  }
  $form['#submit'][] = 'media_translation_file_translate_tab_submit';

  return $form;
}

/**
 * Form submit handler for the media file translate tab.
 */
function media_translation_file_translate_tab_submit($form, &$form_state) {
  $action = $form_state['clicked_button']['#name'];
  $translations = $form_state['values']['translations'];
  $tsid = $form_state['values']['tsid'];
  $media_type = $form_state['values']['media_type'];
  $file_type = $form_state['values']['file_type'];
  $fid = $form_state['values']['fid'];
  $files_in_translation_set = array();

  switch ($action) {
    case 'save':
      // Load/Create the translation set for this file
      /** @var $translation_set media_translation_translation_set */
      $translation_set = $tsid > 0 ? i18n_translation_set_load($tsid) : i18n_translation_set_create('file', $media_type);
      $translation_set->get_objects();

      // Add/Update files in the translation set
      $title = '';
      foreach ($translations as $langcode => $file_tag) {
        $file_data = _media_translation_parse_file_tag($file_tag);
        $files_in_translation_set[$langcode] = $file_data;
        if (!empty($file_data)) {
          $translation_set->add_item($file_data['file'], $langcode);
          if (empty($title)) {
            /** @var $target_object i18n_string_object_wrapper */
            $target_object = $translation_set->get_item($langcode);
            $name = $fid;
            if (!is_null($target_object)) {
              $name = $target_object->get_field('filename', '');
            }
            $title = sprintf('Translations of file %s', $name);
          }
        }
        else {
          // Remote item from set
          $translation_set->remove_language($langcode);
        }
      }
      $translation_set->title = $title;
      $translation_set->save(TRUE);

      drupal_set_message(t('Translation set saved.'));

      break;
    case 'delete':
      $destination = drupal_get_destination();
      $current_path = current_path();
      if ($destination['destination'] == $current_path) {
        // We don't have an explicit return path,
        // so let's return the user to the view
        // page for this media file.
        $destination['destination'] = "media/$fid";
      }

      $path = "media-translation/translation-set/$tsid/delete";

      drupal_goto($path, array('query' => $destination));
      break;
  }

  // Notify other modules that the media file translation has been updated.
  module_invoke_all('media_translation_updated', $fid, $tsid, $translation_set, $files_in_translation_set);
}

/**
 * Validation handler for the file translate tab fields.
 */
function media_translation_autocomplete_validate($element, &$form_state) {
  // TODO: What is $value supposed to be -- it's undefined
  form_set_value($element, $value, $form_state);
}

/**
 * Handles autocomplete of the file lookup when translating files.
 *
 * @param type $langcode
 * @param type $file_type
 * @param type $fid
 */
function media_translation_autocomplete_language($langcode, $file_type, $search = '') {
  $args = array(':lang' => $langcode, ':type' => $file_type, ':filename' => $search . '%');
  $files = db_query("SELECT fid, filename FROM {file_managed} WHERE type = :type AND language = :lang AND i18n_tsid = 0 AND filename LIKE :filename", $args);

  $results = array();
  foreach ($files as $file) {
    $key = _media_translation_create_file_tag($file);
    $results[$key] = $key;
  }

  drupal_json_output($results);
}

/**
 * Generates the settings form for the File Type Translation settings tab.
 */
function media_translation_media_type_form($form, $form_state, $media_type) {
  // Get current translation settings
  $object = db_query("SELECT * FROM {media_type} WHERE name=:media_type", array(':media_type' => $media_type))->fetchObject();
  $i18n_mode = i18n_object_field($object, 'i18n_mode', I18N_MODE_NONE);;
  $langcode = i18n_object_langcode($object, LANGUAGE_NONE);

  $form = array(
    'media_type' => array(
      '#type' => 'value',
      '#value' => $media_type,
    ),
  );
  $form += i18n_translation_mode_element('media_type', $i18n_mode, $langcode);
  $form['actions']['save'] = array(
    '#type' => 'submit',
    '#name'   => 'save',
    '#value' => t('Save'),
  );
  $form['#submit'][] = 'media_translation_media_type_form_submit';

  return $form;
}

/**
 * Form Submit handler for the File Type Translation tab form.
 */
function media_translation_media_type_form_submit($form, $form_state) {
  $media_type = $form_state['values']['media_type'];
  $i18n_mode = $form_state['values']['i18n_mode'];
  $language = $form_state['values']['language'];

  $update['i18n_mode'] = $i18n_mode;

  // Update language for related terms
  switch ($i18n_mode) {
    case I18N_MODE_LANGUAGE:
      $update['language'] = $language;
      break;
    case I18N_MODE_NONE:
      $update['language'] = LANGUAGE_NONE;
      break;
  }

  if (isset($update)) {
    db_update('media_type')
      ->fields($update)
      ->condition('name', $media_type)
      ->execute();
    drupal_set_message(t('Translation settings saved.'));
  }
}


/**
 * Generates the text tag to display in the translation set field for a given file.
 *
 * @param i18n_string_object_wrapper $file
 *  The file object.
 *
 * @return string
 *  A text string in the format: "[fid:$fid; filename:$filename]"
 */
function _media_translation_create_file_tag($file = NULL) {
  $tag = '';

  if (!empty($file) && is_object($file)) {
    $fid = $file->fid;
    $filename = $file->filename;
    $tag = "[fid:$fid; filename:$filename]";
  }

  return $tag;
}

/**
 * Converts the passed file tag from "[fid:$fid; filename:$filename]"
 * to an associative array of values and a file object instance.
 *
 * @param string $tag
 *  The file tag to parse.
 *
 * @return array
 *  An associative array with the structure:
 *    array(fid => $fid, filename => $filename, $file => $file)
 */
function _media_translation_parse_file_tag($tag) {
  if (empty($tag)) return array();

  // Remove brackets
  $tag = str_replace('[', '', $tag);
  $tag = str_replace(']', '', $tag);

  $result = array();
  $fields = explode(';', $tag);
  foreach ($fields as $field) {
    $data = explode(':', $field);
    $result[$data[0]] = $data[1];
  }
  $result['file'] = file_load($result['fid']);

  return $result;
}
