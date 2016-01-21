<?php

/**
 * @file
 * Admin page callback file for the poeditor module.
 */

/**
 * User interface for the language overview screen.
 */
function poeditor_configure_form() {
  $poeditor_api_token = poeditor_token();
  $form['poeditor_configuration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configuration'),
    '#weight' => 0,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['poeditor_configuration']['poeditor_token'] = array(
    '#type' => 'textfield',
    '#title' => t('POEditor Token'),
    '#default_value' => $poeditor_api_token,
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
  );
  if (!empty($poeditor_api_token)) {
    $poeditor_projects = poeditor_api_list_projects($poeditor_api_token);
    $projects_available = array();
    $projects_available[0] = t('- No project selected -');
    if (!empty($poeditor_projects)) {
      foreach ($poeditor_projects as $project) {
        $projects_available[$project['id']] = $project['name'];
      };
    }
    $form['poeditor_configuration']['poeditor_project'] = array(
      '#type' => 'select',
      '#title' => t('POEditor Project'),
      '#options' => $projects_available,
      '#default_value' => variable_get('poeditor_project'),
      '#description' => t('Projects available on poeditor.com'),
    );
    $form['poeditor_configuration']['poeditor_send_all_terms_cron'] = array(
      '#type' => 'checkbox',
      '#title' => t('Cron: Send all terms to POEditor'),
      '#default_value' => variable_get('poeditor_send_all_terms_cron'),
    );
    $form['poeditor_configuration']['poeditor_get_all_translations_cron'] = array(
      '#type' => 'checkbox',
      '#title' => t('Cron: Get all translations from POEditor'),
      '#default_value' => variable_get('poeditor_get_all_translations_cron'),
      '#description' => t('Executed once per day'),
    );
  }
  else {
    drupal_set_message(t('Please add your API token.'), 'warning');
  }
  return system_settings_form($form);
}

/**
 * Function that gets the terms from POEditor and saves them inside the Drupal website.
 *
 * @param int $poeditor_api_token
 *   POEditor account key.
 *
 * @param int $project_id
 *   POEditor project ID.
 *
 * @param string $lang
 *   Language ID.
 *
 * @param string $drupal_token
 *   Token based on the user session and private key.
 *
 * @return int
 *   Number of terms from POEditor.
 */
function poeditor_get_terms_from_POEditor($poeditor_api_token, $project_id, $lang, $drupal_token) {
  $i = 0;
  $terms = poeditor_api_view_terms($poeditor_api_token, $project_id, $lang);
  if (!empty($terms)) {
    foreach ($terms as $term) {
      $t = poeditor_save($term['term'], $term['definition']['form'], $lang, $drupal_token);
      if ($t == TRUE) {
        ++$i;
      }
    };
  }
  // Force JavaScript translation file recreation for this language.
  _locale_invalidate_js($lang);
  // Clear locale cache.
  _locale_invalidate_js();
  cache_clear_all('locale:', 'cache', TRUE);

  unset($terms);
  return $i;
}

/**
 * Functions for form and batch generation and processing.
 */
function poeditor_process_form() {
  $action = arg(3);
  $lang = arg(4);
  $drupal_token = arg(5);
  $poeditor_api_token = variable_get('poeditor_token');
  $locales_terms = poeditor_select_terms($lang);
  $project_id = poeditor_project_id();
  $valid_token = drupal_valid_token($drupal_token);
  if ($valid_token == TRUE) {
    switch ($action) {
      case 'get-translations':
        $terms = poeditor_get_terms_from_POEditor($poeditor_api_token, $project_id, $lang, $drupal_token);
        drupal_set_message(t('@i translations have been copied from POEditor.', array('@i' => $terms)));
        drupal_goto('admin/config/regional/poeditor/overview/');
        break;

      case 'send-translations':
        poeditor_api_update_language_to_project($poeditor_api_token, $project_id, $locales_terms, $lang);
        drupal_goto('admin/config/regional/poeditor/overview/');
        break;

      case 'add-language':
        poeditor_api_add_language_to_project($poeditor_api_token, $project_id, $lang);
        drupal_goto('admin/config/regional/poeditor/overview/');
        break;

      case 'delete-language':
        drupal_goto('admin/config/poeditor/overview/translate/delete-confirm/' . $lang . '/' . $drupal_token);
        break;

      case 'delete-all-terms':
        drupal_goto('admin/config/poeditor/overview/translate/delete-all-terms/terms/' . $drupal_token);
        break;
    };
  }
  else {
    drupal_set_message(t('Error validating access token.'), 'error');
  }
}

/**
 * Implements hook_form().
 *
 * Confirm message before deleting a language.
 */
function poeditor_delete_lang_form($form_state, $args_from_url = NULL) {
  $lang = arg(6);
  $drupal_token = arg(7);
  $valid_token = drupal_valid_token($drupal_token);
  if ($valid_token == TRUE) {
    $form['lang'] = array('#type' => 'hidden', '#value' => $lang);
    if (!isset($form_state['storage']['confirm'])) {
      return confirm_form($form, t('POEditor'), 'admin/config/regional/poeditor/overview/translate', t('Do you want to remove this language?'), t('Yes'), t('No'));
    }
    return $form;
  }
  else {
    drupal_set_message(t('Error validating access token'), 'error');
  }
}

/**
 * Processing the language deletion form.
 */
function poeditor_delete_lang_form_submit($form, &$form_state) {
  $lang = $form_state['values']['lang'];
  if (!isset($form_state['storage']['confirm'])) {
    poeditor_api_delete_language_to_project(poeditor_token(), poeditor_project_id(), $lang);
    drupal_goto('admin/config/regional/poeditor/overview/translate');
  }
}
/**
 * Implements hook_form().
 *
 * Confirm message before deleting all terms.
 */
function poeditor_delete_all_terms($form_state, $args_from_url = NULL) {
  $drupal_token = arg(7);
  $valid_token = drupal_valid_token($drupal_token);
  if ($valid_token == TRUE) {
    if (!isset($form_state['storage']['confirm'])) {
      return confirm_form($form_state, t('POEditor'), 'admin/config/regional/poeditor/overview/translate', t('Do you want to remove all terms?'), t('Yes'), t('No'));
    }
    return $form;
  }
  else {
    drupal_set_message(t('Error validating access token'), 'error');
  }
}
/**
 * Processing the deletion form for all terms.
 */
function poeditor_delete_all_terms_submit($form, &$form_state) {
  $poeditor_api_token = poeditor_token();
  $project_id = poeditor_project_id();
  $locales = poeditor_select_terms_original('en');
    foreach (poeditor_api_list_project_languages($poeditor_api_token, $project_id) as $lang) {
      $result = poeditor_api_delete_terms($poeditor_api_token, $project_id, $locales);
    }
    if (isset($result->data)) {
      $response = drupal_json_decode($result->data);
      if ($response['response']['status'] == 'success') {
        drupal_set_message(t('The terms have been deleted succesfully from POEditor.'));
      }
      else{
        drupal_set_message(t('An error occured when we tried to delete the terms.'), 'error');
        drupal_set_message(t('The POEditor website error  was: %message', array('%message' => $response['response']['message'])), 'error');
      }
    }
    else{
      drupal_set_message(t('The POEditor website error  was: %message', array('%message' => $result->error)), 'error');
    }
  drupal_goto('admin/config/regional/poeditor/overview/translate');
}
/**
 * Implements hook_form().
 *
 * POEditor translation interface.
 * Return the form for managing POEditor translations from your own Drupal website.
 */
function poeditor_translate_form($form, &$form_state) {
  $drupal_token = drupal_get_token();
  $poeditor_api_token = poeditor_token();
  $project_id = poeditor_project_id();
  // Checking POEditor website status.
  $status = poeditor_api_status($poeditor_api_token, $project_id);
  if ((!empty($poeditor_api_token)) && (!empty($project_id)) && ($project_id != 0) && ($status != 'fail')) {
    drupal_static_reset('language');
    $languages = language_list('language');
    foreach ($languages as $lang) {
      if (($lang->language != 'en') && (!empty($lang->language))) {
        $display = 'TRUE';
      }
    }
    if (!empty($display)) {
      $rows = array();
      $terms_in_drupal = (poeditor_count_terms('en'));
      $terms_in_drupal = $terms_in_drupal - 1;
      $terms_in_poeditor = poeditor_api_count_terms($poeditor_api_token, $project_id);
      foreach ($languages as $lang) {
        if ($lang->language != 'en') {
          $row = array();
          $x = poeditor_compare_lang_get_definitions($poeditor_api_token, $project_id, $lang->language, $drupal_token);
          if (empty($x)) {
            $row[] = '<span class="inactivelanguage">' . $lang->name . '</span>';
            $row[] = '';
            $row[] = '';
            $row[] = '';
          }
          else {
            $row[] = "<span class = " . t('@lang', array('@lang' => $lang->name)) . ">" . t('@lang', array('@lang' => $lang->name)) . "</span>";
            $row[] = poeditor_count_terms($lang->language) . ' ' . t('of') . ' ' . $terms_in_drupal;
            $language_procent = '(' . poeditor_lang_percentage($poeditor_api_token, $project_id, $lang->language) . ' ' . '%)';
            $row[] = poeditor_api_count_definition($poeditor_api_token, $project_id, $lang->language) . ' ' . t('of') . ' ' . $terms_in_poeditor . ' ' . $language_procent;
            $row[] = poeditor_compare_lang_update($poeditor_api_token, $project_id, $lang->language, $drupal_token) . ' | ' . poeditor_compare_lang_get_definitions($poeditor_api_token, $project_id, $lang->language, $drupal_token) . ' | ' . poeditor_compare_lang_export($poeditor_api_token, $project_id, $lang->language, $drupal_token);
          }
          $row[] = poeditor_compare_lang_add($poeditor_api_token, $project_id, $lang->language, $drupal_token) . poeditor_compare_lang_delete($poeditor_api_token, $project_id, $lang->language, $drupal_token);
          $rows[] = $row;
        }
      };
      $header = array(t('Language'),
        t('Translations in Drupal'),
        t('Translations in POEditor'),
        t('POEditor'),
        t('Actions'),
      );
      $table = theme('table', array('header' => $header, 'rows' => $rows));
      $form['#prefix'] = '</br><span class="title">' . t('Translate interface with POEditor') . '</span></br></br>';
      $form['languages'] = array('#markup' => $table);
      $form['languages']['#suffix'] = '<span class="drupal">' . t('Terms on your Drupal website:') . '</span> <span class = "terms">' . $terms_in_drupal . ' </span>' . '<span class="poeditor">/ ' . t('Terms on POEditor website:') . '</span> <span class = "terms">' . $terms_in_poeditor . '</span></br></br>';
      $form['get_all_translation_from_POEditor'] = array('#type' => 'submit', '#value' => t('Get all translations from POEditor'));
      $form['send_all_term_to_poeditor'] = array('#type' => 'submit', '#value' => t('Send all terms to POEditor'));
      $form['send_all_term_to_poeditor']['#suffix'] = poeditor_confirm_delete_all_terms($drupal_token);
      return $form;
    }
    else {
      drupal_set_message(t('Please <a href="/admin/config/regional/language/add/">add language</a> to project.'), 'warning');
    }
  }
  else {
    drupal_set_message(t('Please check the configuration tab and add your token or select a project.'), 'warning');
  }
}

/**
 * Processing the POEditor translation interface form.
 */
function poeditor_translate_form_submit($form, &$form_state) {
  $drupal_token = drupal_get_token();
  $poeditor_api_token = poeditor_token();
  $project_id = poeditor_project_id();
  $locales = poeditor_select_terms_original('en');
  if ($form_state['values']['op'] == $form_state['values']['get_all_translation_from_POEditor']) {
    foreach (poeditor_api_list_project_languages($poeditor_api_token, $project_id) as $lang) {
      $language = $lang['language_code'];
      if ($language != 'en') {
        $terms = poeditor_api_view_terms($poeditor_api_token, $project_id, $language);
        if (!empty($terms)) {
          foreach ($terms as $term) {
            // Saving or updating terms on your Drupal website.
            poeditor_save($term['term'], $term['definition']['form'], $language, $drupal_token);
          };
        }
      }
    }
  }
  elseif ($form_state['values']['op'] == $form_state['values']['send_all_term_to_poeditor']) {
    // Sending all terms from your Drupal website to the POEditor website.
    poeditor_api_add_terms($poeditor_api_token, $project_id, $locales);
  }
}
