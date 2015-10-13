<?php

/**
 * @file
 * this file include the translatin interface for lang strings
 */

/**
 * menu callback for translating a keyword/coverage.
 *
 * @global object $language
 *   the language
 *
 * @param string $lom_id
 *   the lom id
 *
 * @return string
 *   html
 */
function archibald_translate_keywords($lom_id, $type) {
  $lom = ArchibaldLomSaveHandler::load($lom_id);
  if (!is_object($lom)) {
    return '';
  }

  $output = "";

  $languages = archibald_get_general_language_options();

  // Add main behaviour css and js
  drupal_add_js(drupal_get_path('module', 'archibald') . '/js/archibald.js');
  drupal_add_css(drupal_get_path('module', 'archibald') . '/css/archibald.css');

  if ($type == 'keywords') {
    drupal_set_title(t('Manage keywords'));
    $data_words = $lom->getGeneral()->getKeyword();
    $type_string = t('Keywords');
    $nothing_found_string = t('No keywords found');
  }
  else {
    drupal_set_title(t('Manage coverages'));
    $data_words = $lom->getGeneral()->getCoverage();
    $type_string = t('Coverages');
    $nothing_found_string = t('No coverages found.');
  }

  $keywords = array();

  $metadata_language = $lom->getMetaMetadata()->getLanguage();

  foreach ($data_words AS $langstring_obj) {
    /* @var $langstring_obj ArchibaldLomDataLangString */
    foreach ($langstring_obj->getStrings() AS $lang_key => $string) {
      $keywords[$lang_key][$langstring_obj->getId()] = $string;
    }
  }

  $output .= t("The description uses the following @type in \"!language\":", array(
    "!language" => '<strong>' . $languages[$metadata_language] . '</strong>',
    '@type' => $type_string,
  ));

  if (isset($keywords[$metadata_language]) && !empty($keywords[$metadata_language])) {
    $output .= '<ul>';
    foreach ($keywords[$metadata_language] AS $keyword) {
      $output .= '<li><strong>' . $keyword . '</strong></li>';
    }
    $output .= '</ul>';
    unset($keywords[$metadata_language]);
  }
  else {
    $output .= '<br /><strong>' . $nothing_found_string . '</strong>';
  }

  $f = drupal_get_form('archibald_translate_keywords_form', $lom_id, $type, $keywords, $metadata_language);
  $output .= drupal_render($f);

  return $output;
}

/**
 * Displays the translation interface for keywords/coverages.
 *
 * @param array $form
 *   the form array
 * @param array $form_state
 *   the form state
 * @param string $lom_id
 *   the lom id
 * @param string $type
 *   the type (coverages or keywords)
 * @param array $keywords
 *   the keywords which are currently present
 * @param string $metadata_language
 *   the meta language
 *
 * @return array
 *   the form
 */
function archibald_translate_keywords_form($form, $form_state, $lom_id, $type, $keywords, $metadata_language) {

  $languages = archibald_get_general_language_options();
  $form = array();

  if ($type == 'keywords') {
    $type_string = t('Keywords');
    $type_string_single = t('Keyword');
  }
  else {
    $type_string = t('Coverages');
    $type_string_single = t('Coverage');
  }

  $form['lom_id'] = array(
    '#type' => 'hidden',
    '#value' => $lom_id,
  );

  $form['type'] = array(
    '#type' => 'hidden',
    '#value' => $type,
  );
  $form['lom_id'] = array(
    '#type' => 'hidden',
    '#value' => $lom_id,
  );

  $form['keywords'] = array(
    '#type' => 'container',
    '#prefix' => '<div class="clearfix" id="archibald_keyword-wrapper">',
    '#suffix' => '</div>',
  );

  if (!empty($form_state['input']) && !empty($form_state['input']['_triggering_element_name'])) {
    if (preg_match("/^submit_(.+)$/", $form_state['input']['_triggering_element_name'], $matches)) {
      $form_state['num_keywords_' . $matches[1]]++;
    }
  }

  if (isset($languages[$metadata_language])) {
    unset($languages[$metadata_language]);
  }

  foreach ($languages AS $lang_key => $language) {
    if( $lang_key == 'none' ) continue;

    if (!isset($form_state['num_keywords_' . $lang_key])) {
      $form_state['num_keywords_' . $lang_key] = 0;
    }

    $form['keywords'][$lang_key] = array(
      '#type' => 'fieldset',
      '#title' => t("@type for: @language", array(
        '@language' => $language,
        '@type' => $type_string,
      )),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
    );

    $remove_num_keywords = 0;

    // Add currently stored keywords/coverages.
    if (isset($keywords[$lang_key])) {
      foreach ($keywords[$lang_key] AS $langstring_id => $keyword) {
        if (empty($_POST[$lang_key]) || (isset($_POST[$lang_key]['keyword_' . $langstring_id]) && $_POST[$lang_key]['keyword_' . $langstring_id] != 'archibald_delete')) {
          archibald_add_keyword_translation($form, $form_state, $type, $lang_key, $keyword, $langstring_id);
        }
      }
    }

    // If we posted a not currently stored new keyword/coverage check if the value is archibald_delete
    // if so we need to remove the field.
    if (!empty($_POST[$lang_key]) && isset($_POST[$lang_key])) {
      foreach ($_POST[$lang_key] AS $k => $v) {
        if (preg_match('/^keyword_([0-9]+)$/', $k, $matches)) {
          if ($v != 'archibald_delete') {
            archibald_add_keyword_translation($form, $form_state, $type, $lang_key, $v, $matches[1]);
            $remove_num_keywords++;
          }
          else {
            $form_state['num_keywords_' . $lang_key]--;
          }
        }
      }
    }

    // Add all empty fields.
    $empty_one = $form_state['num_keywords_' . $lang_key];
    $empty_one -= $remove_num_keywords;
    for ($i = 0; $i <= $empty_one; $i++) {
      archibald_add_keyword_translation($form, $form_state, $type, $lang_key);
    }

    $form['keywords'][$lang_key]['add_more'] = array(
      '#type' => 'submit',
      '#value' => t('Add another @type', array('@type' => $type_string_single)),
      '#weight' => 9999,
      '#name' => 'submit_' . $lang_key,
      '#limit_validation_errors' => array(),
      '#submit' => array(
        'archibald_add_keyword_submit',
      ),
      '#ajax' => array(
        'callback' => 'archibald_add_keyword',
        'wrapper' => 'archibald_keyword-wrapper',
        'method' => 'replace',
      ),
      '#suffix' => '<br />',
    );
  }

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save @type', array('@type' => $type_string)),
  );

  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#weight' => 9999,
  );

  return $form;
}

/**
 * Validates the form.
 *
 * @param array $form
 *   the form array
 * @param array $form_state
 *   the form state
 */
function archibald_translate_keywords_form_validate($form, $form_state) {
  if ($form_state['values']['op'] == $form_state['values']['save']) {
    if (empty($form_state['values']['lom_id'])) {
      form_set_error('', t('Lom id is missing'));
    }
    else {
      $lom = ArchibaldLomSaveHandler::load($form_state['values']['lom_id']);
      if (!is_object($lom)) {
        form_set_error('', t('No such Lom object'));
      }
      else {
        $metadata_language = $lom->getMetaMetadata()->getLanguage();

        $type = $form_state['values']['type'];
        if ($type == 'coverages') {

          $languages = archibald_get_general_language_options();
          foreach ($languages AS $lang_key => $language) {
            if (!isset($form_state['values'][$lang_key])) {
              continue;
            }
            foreach ($form_state['values'][$lang_key] AS $key_id => $keyword_string) {

              // This keywords we do not want because they are empty or not keywords/coverages (add more button).
              if (empty($keyword_string) || !preg_match('/^keyword_/', $key_id)) {
                continue;
              }

              $exist = db_select('archibald_coverage_list', 'cl')
                ->fields('cl')
                ->condition('coverage', $keyword_string)
                ->condition('language', $lang_key)
                ->execute()
                ->fetchAssoc();

              if (empty($exist)) {
                form_set_error($lang_key . '][' . $key_id, t('The coverage "@coverage (@lang)" does not exist within the database.', array(
                  '@coverage' => $keyword_string,
                  '@lang' => $lang_key,
                )));
              }
            }
          }

        }
      }
    }
  }
}

/**
 * Submits the form.
 *
 * @param array $form
 *   the form array
 * @param array $form_state
 *   the form state
 */
function archibald_translate_keywords_form_submit($form, $form_state) {
  if ($form_state['values']['op'] != $form_state['values']['save']) {
    drupal_goto('archibald/' . $form_state['values']['lom_id'] . '/translate');
    return;
  }
  $lom = ArchibaldLomSaveHandler::load($form_state['values']['lom_id']);

  $type = $form_state['values']['type'];
  $new_keywords = array();

  if ($type == 'keywords') {
    $keywords = $lom->getGeneral()->getKeyword();
  }
  else {
    $keywords = $lom->getGeneral()->getCoverage();
  }
  foreach ($keywords AS $langstring_obj) {
    /* @var $langstring_obj ArchibaldLomDataLangString */
    foreach ($langstring_obj->getStrings() AS $lang_key => $string) {
      $converted_keywords[$lang_key][$string] = $langstring_obj;
    }
  }

  // We need to pre add the current keywords/coverages within the main language
  // Else we would delete the within the new revision.
  $metadata_language = $lom->getMetaMetadata()->getLanguage();

  if (isset($converted_keywords[$metadata_language])) {
    foreach ($converted_keywords[$metadata_language] AS $langstring) {
      $new_keywords[] = $langstring;
    }
    unset($converted_keywords[$metadata_language]);
  }


  $save_lom = FALSE;

  $languages = archibald_get_general_language_options();
  foreach ($languages AS $lang_key => $language) {
    if (!isset($form_state['values'][$lang_key])) {
      continue;
    }
    foreach ($form_state['values'][$lang_key] AS $key_id => $keyword_string) {

      // This keywords we do not want because they are empty or not keywords/coverages (add more button).
      if (empty($keyword_string) || !preg_match('/^keyword_/', $key_id)) {
        continue;
      }

      // We have already this keyword/coverage. Do nothing.
      if (isset($converted_keywords[$lang_key][$keyword_string])) {
        $new_keywords[] = $converted_keywords[$lang_key][$keyword_string];
        unset($converted_keywords[$lang_key][$keyword_string]);
        continue;
      }

      $save_lom = TRUE;

      $converted_keywords[$lang_key][$keyword_string] = new ArchibaldLomDataLangString($keyword_string, $lang_key);

      if ($type == 'keywords') {
        $within_db = db_select('archibald_keyword_list', 'kl')
          ->fields('kl', array('langstring_id'))
          ->condition('keyword', $keyword_string)
          ->condition('language', $lang_key)
          ->execute()
          ->fetchAssoc();
      }
      else {
        $within_db = db_select('archibald_coverage_list', 'cl')
          ->fields('cl', array('langstring_id'))
          ->condition('coverage', $keyword_string)
          ->condition('language', $lang_key)
          ->execute()
          ->fetchAssoc();
      }

      if (!empty($within_db)) {
        $converted_keywords[$lang_key][$keyword_string]->setId($within_db['langstring_id']);
        $new_keywords[] = $converted_keywords[$lang_key][$keyword_string];
      }
      elseif ($type == 'keywords') {
        $lang_string = new ArchibaldLomDataLangString($keyword_string, $lang_key);
        $id = ArchibaldLomSaveHandler::saveOrInsertLangString($lang_string);
        $new_keywords[] = $converted_keywords[$lang_key][$keyword_string] = $lang_string;
      }

      if (empty($within_db) && $type == 'keywords') {
        db_insert('archibald_keyword_list')
          ->fields(array(
            'keyword' => $keyword_string,
            'language' => $lang_key,
            'langstring_id' => $converted_keywords[$lang_key][$keyword_string]->getId(),
          ))
          ->execute();
      }

      unset($converted_keywords[$lang_key][$keyword_string]);
    }
  }

  if ($save_lom == FALSE) {

    // Maybe we deleted a keyword/coverage so we must check if a minimum of one keyword/coverage
    // still exists within the old languages if so we could not proceed this
    // keyword/coverage and therefore it is not present within the posted data and must
    // be removed from the description keywords/coverages.
    // This occures only if the user removed the keyword/coverage string manually (wihout clicking
    // on the delete icon) and press save.
    foreach ($converted_keywords AS $language_keywords) {
      if (!empty($language_keywords)) {
        $save_lom = TRUE;
        break;
      }
    }

  }

  if ($save_lom == TRUE) {
    if ($type == 'keywords') {
      $lom->getGeneral()->setKeyword($new_keywords);
      // echo "<pre>";
      // print_r( $new_keywords );
      // echo "</pre>";
    }
    else {
      $lom->getGeneral()->setCoverage($new_keywords);
    }
    $lom->getLifeCycle()->setStatus(ArchibaldLomDataLifeCycle::LIFECYCLE_STATUS_DRAFT);
    ArchibaldLomSaveHandler::insert($lom);
    if ($type == 'keywords') {
      drupal_set_message(t('Keywords saved'));
    }
    else {
      drupal_set_message(t('Coverages saved'));
    }
  } else {
    drupal_set_message(t('No changes'));
  }

}

/**
 * ajax menu callback
 *
 * @param array $form
 *   the form array
 * @param array $form_state
 *   the form state
 *
 * @return array
 *   the keywords
 */
function archibald_add_keyword($form, $form_state) {
  return $form['keywords'];
}

/**
 * add a new empty object
 *
 * @param array $form
 *   the form array
 * @param array $form_state
 *   the form state
 * @param string $type
 *   the type (keywords or coverages)
 * @param string $language
 *   the language
 * @param string $keyword
 *   the keyword default value (optional, default = "")
 *   the language
 * @param string $id
 *   the keyword id (optional, default = "")
 */
function archibald_add_keyword_translation(&$form, &$form_state, $type, $language, $keyword = "", $id = "") {
  if (empty($id)) {
    $id = 1;
    foreach (array_keys($form['keywords'][$language]) as $k) {
      if (preg_match('/^keyword_([0-9]+)$/', $k, $matches)) {
        $matches[1] = intval($matches[1]);
        if ($id < $matches[1]) {
          $id = $matches[1];
        }
      }
    }
    $id++;
  }

  $ac_path = 'keyword';
  $key_string = t('Keyword');
  if ($type != 'keywords') {
    $key_string = t('Coverage');
    $ac_path = 'coverage';
  }
  $form['keywords'][$language]['keyword_' . $id] = array(
    '#type' => 'textfield',
    '#title' => $key_string,
    '#autocomplete_path' => 'archibald/autocomplete/' . $ac_path . '/' . $language,
    '#attributes' => array('class' => array('general_keyword', 'keyword_' . $language)),
  );

  if (!empty($keyword)) {
    $form['keywords'][$language]['keyword_' . $id]['#default_value'] = $keyword;
  }
  else {
    $form['keywords'][$language]['keyword_' . $id]['#default_value'] = '';
  }
}

/**
 * Submit handler for the "Add more" button.
 *
 * @param array $form
 *   the form array
 * @param array $form_state
 *   the form state
 */
function archibald_add_keyword_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

/**
 * menu callback for translating a ressource.
 *
 * @global object $language
 *
 * @param string $lom_id
 *
 * @return string
 *   html
 */
function archibald_translate_wizard($lom_id) {
  global $language;
  $_SESSION['langStringOrder'] = array();
  $_SESSION['langStringOrder_fieldname'] = array();
  $lom = ArchibaldLomSaveHandler::load($lom_id);
  if (!is_object($lom)) {
    return '';
  }
  $output = "";
  drupal_set_title(t('Translate'));

  $translations = array(
    'required' => array(),
    'optional' => array(),
  );


  $translations['required']['general.title'] = $lom->getGeneral()->getTitle();

  $translations['required']['general.description'] = $lom->getGeneral()->getDescription();

  $x = 0;
  foreach ($lom->getGeneral()->getIdentifier() as $i => $identifier) {
    if (empty($identifier)) {
      continue;
    }

    $lang_string_obj = $identifier->getTitle();
    if (empty($lang_string_obj)) {
      continue;
    }
    $translations['optional']['general.identifier.title ' . ++$x] = $lang_string_obj;
  }

  $translations['optional']['lifecycle.version'] = $lom->getLifeCycle()->getVersion();

  $translations['optional']['education.description'] = $lom->getEducation()->getDescription();

  $translations['optional']['technical.otherPlattformRequirements'] = $lom->getTechnical()->getOtherPlattformRequirements();

  $x = 0;
  foreach ($lom->getRelation() as $i => $relation) {
    if (empty($relation)) {
      continue;
    }

    $lang_string_obj = $relation->getDescription();
    if (empty($lang_string_obj)) {
      continue;
    }
    $translations['optional']['relation.relation.description ' . ++$x] = $lang_string_obj;
  }


  $pref = '';
  foreach ($translations as $trans_group => $trans) {

    if ($trans_group == 'required') {
      $title = t("Required fields");
    }
    elseif ($trans_group == 'optional') {
      $title = t("Optional fields");
    }
    else {
      $title = t("Relations");
    }

    $table = array(
      'header' => array(
        $title,
        t('Translate'),
      ),
      'rows' => array(),
    );

    foreach ($trans as $k => $lom_langstring) {
      if (strpos($k, ' ') !== FALSE) {
        $tmp = explode(' ', $k);
        $field_name = archibald_get_field_label($tmp[0], TRUE);
      }
      else {
        $field_name = archibald_get_field_label($k, TRUE);
      }
      $_SESSION['langStringOrder_fieldname'][$lom_langstring->id] = $field_name;

      $_SESSION['langStringOrder'][$pref] = $lom_langstring->id;

      $pref = $lom_langstring->id;

      // print_r( $lom_langstring->strings );

      $row = array(
        "<span style='font-weight:bold;'>" . $field_name . "</span><br />" . current($lom_langstring->strings),
        array(
          'style' => 'width:100px;',
          'data' => count($lom_langstring->strings) ? l(t('Translate'), 'admin/archibald/translate_term/' . $lom_id . '/' . $pref) : 'Field not set',
        ),
      );
      $table['rows'][] = $row;
    }

    $output .= theme('table', $table) . '<br/>';
  }

  $output .= l(t('Manage keywords'), 'archibald/' . $lom_id . '/translate/keywords') . '<br />';
  $output .= l(t('Manage coverages'), 'archibald/' . $lom_id . '/translate/coverages');

  return $output;
}

/**
 * Generate the translation form.
 *
 * @param array $form
 *   the form array
 * @param array $form_state
 *   the form state
 * @param string $langstring_terms_id
 *   the langstring id
 *
 * @return array
 *   the form array
 */
function archibald_translate_form($form, $form_state, $lom_id, $langstring_terms_id) {

  $languages = archibald_get_general_language_options();


  $lom = ArchibaldLomSaveHandler::load($lom_id);
  $lang_string_obj = new ArchibaldLomDataLangString();
  $lang_string_obj->strings = array();
  $lang_string_obj->setId($langstring_terms_id);
  $res = db_select('archibald_langstring_terms', 'archibaldls')
    ->fields('archibaldls')
    ->condition('langstring_terms_id', $langstring_terms_id)
    ->execute();

  while ($row = $res->fetchAssoc()) {
    $lang_string_obj->setString($row['term'], $row['language']);
  }
  if (is_null($lang_string_obj->strings)) {
    $lang_string_obj->strings = array();
  }

  // print_r( $lang_string_obj->strings );
  // print_r($langstring_terms_id);
  // die();

  $meta_lang = $lom->getMetaMetadata()->getLanguage();

  $strings = $lang_string_obj->getStrings();

  if (isset($_SESSION['langStringOrder_fieldname'][$langstring_terms_id])) {
    $title = t('Translate field: !field', array('!field' => $_SESSION['langStringOrder_fieldname'][$langstring_terms_id]));
    drupal_set_title($title);
    $form['title'] = array(
      '#markup' => $title,
    );


    $lid = $lom->lom_id;
    $title = ArchibaldLom::determinTitle($lid);
    if (isset($strings[$meta_lang])) {
      $form['original_text'] = array(
        '#markup' => '<br /><br /><h1>' . $title . '</h1><strong>' . t('Original text') . ' (' . $languages[$meta_lang] . ')' . ':</strong><br />' . $strings[$meta_lang],
      );
    }
  }



  $type = 'textfield';
  foreach ($strings as $ls) {
    if (strpos($ls, "\n") !== FALSE || drupal_strlen($ls) > 170) {
      $type = 'textarea';
    }
  }

  $form['#prefix'] = "";
  $form['langstring_terms_id'] = array(
    '#type' => 'hidden',
    '#default_value' => $langstring_terms_id,
  );
  $form['lom_id'] = array(
    '#type' => 'hidden',
    '#default_value' => $lom_id,
  );


  if (isset($languages[$meta_lang])) {
    unset($languages[$meta_lang]);
  }

  foreach ($languages as $lang_code => $language) {
    if (strlen($lang_code) > 2) continue;
    $default_value = '';
    if (!empty($strings[$lang_code])) {
      $default_value = $strings[$lang_code];
    }

    $form['language_' . $lang_code] = array(
      '#type' => $type,
      '#title' => filter_xss($language),
      '#default_value' => $default_value,
      '#size' => 130,
      '#maxlength' => ($type == 'textfield') ? 700 : 3500,
      '#rows' => 8,
    );
  }
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 9999,
  );
  $form['save_and_next'] = array(
    '#type' => 'submit',
    '#value' => t('Save and next field'),
    '#weight' => 9999,
  );
  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#weight' => 9999,
  );
  return $form;
}

/**
 * Submit handler for translating form.
 *
 * @param array $form
 *   the form array
 * @param array $form_state
 *   the form state
 */
function archibald_translate_form_submit($form, &$form_state) {
  $version = ArchibaldLomSaveHandler::getActualVersion($form['lom_id']['#value']);
  $cid = 'lom:' . $form['lom_id']['#value'] . '__version:' . $version;
  cache_set($cid, NULL, 'cache', CACHE_TEMPORARY);

  switch ($form_state['clicked_button']['#id']) {
    case 'edit-cancel':
      drupal_goto('archibald/' . $form['lom_id']['#value'] . '/translate');
      break;

    case 'edit-save-and-next':
      $langstring_id = archibald_translate_form_submit_save($form, $form_state);
      if (!empty($_SESSION['langStringOrder'][$langstring_id])) {
        drupal_goto(
          'admin/archibald/translate_term/' . $form['lom_id']['#value'] . '/' .
          $_SESSION['langStringOrder'][$langstring_id]
        );
      }
      else {
        drupal_goto('archibald/' . $form['lom_id']['#value'] . '/translate');
      }
      break;

    case 'edit-save':
      archibald_translate_form_submit_save($form, $form_state);
      drupal_goto('archibald/' . $form['lom_id']['#value'] . '/translate');
      break;
  }
}

/**
 * save a lang string.
 *
 * @param array $form
 *   the form array
 * @param array $form_state
 *   the form state
 *
 * @return string
 *    the langstring id
 */
function archibald_translate_form_submit_save(&$form, &$form_state) {


  $lang_string = new ArchibaldLomDataLangString();
  $lang_string->id = $form['langstring_terms_id']['#value'];
  unset($form['langstring_terms_id']);
  foreach ($form_state['values'] as $k => $tmp_arr) {
    if (!preg_match("/^language_([a-z][a-z])$/", $k, $matches)) {
      continue;
    }
    if (!empty($matches[1]) && trim($matches[1]) != '') {
      if (empty($tmp_arr)) {
        $lang_string->strings[$matches[1]] = '';
      }
      else {
        $lang_string->setString($tmp_arr, $matches[1]);
      }
    }
  }

  $return = ArchibaldLomSaveHandler::saveOrInsertLangString($lang_string);

  $lom = ArchibaldLomSaveHandler::load($form['lom_id']['#value'], '', FALSE);
  $lom->getLifeCycle()->setStatus(ArchibaldLomDataLifeCycle::LIFECYCLE_STATUS_DRAFT);

  ArchibaldLomSaveHandler::insert($lom);
  return $return;
}
