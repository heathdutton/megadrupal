<?php

/**
 * @file
 * Administrative functions for the Translate set module.
 */

/**
 * Page callback: configure the settings.
 */
function translate_set_settings_form($form, $form_state) {

  $form['tracking_container'] = array(
    '#type' => 'fieldset',
    '#title' => t('Track strings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $form['tracking_container']['translate_set_tracking_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable tracking of new strings'),
    '#default_value' => variable_get('translate_set_tracking_enabled', FALSE),
    '#description' => t('Tracking will be done when visiting the translate set screen.'),
  );

  $form['tracking_container']['translate_set_tracking_contexts'] = array(
    '#type' => 'textarea',
    '#title' => t('Contexts'),
    '#default_value' => variable_get('translate_set_tracking_contexts', ''),
    '#states' => array(
      'visible' => array(
        ':input[name="translate_set_tracking_enabled"]' => array('checked' => TRUE),
      ),
    ),
    '#description' => t('Enter contexts per line which will be used to evaluate whether a string will be added or not. Wildcards are possible. e.g.<br />custom<br />rules:rules_config:*:subject<br />taxonomy:*:name'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Validate callback: validates the translate set settings form.
 */
function translate_set_settings_form_validate($form, $form_state) {
  if ($form_state['values']['translate_set_tracking_enabled'] && empty($form_state['values']['translate_set_tracking_contexts'])) {
    form_set_error('translate_set_tracking_contexts', t('Tracking contexts is required.'));
  }
}

/**
 * Submit callback: saves the translate set settings form.
 */
function translate_set_settings_form_submit($form, $form_state) {

  if ($form_state['values']['translate_set_tracking_enabled']) {
    variable_set('translate_set_tracking_enabled', TRUE);
    variable_set('translate_set_tracking_contexts', $form_state['values']['translate_set_tracking_contexts']);
  }
  else {
    variable_del('translate_set_tracking_enabled');
    variable_del('translate_set_tracking_contexts');
  }

  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Page callback: configure the languages per user for Translate set.
 */
function translate_set_configure_languages_per_user($form, $form_state, $account) {
  $languages = language_list();
  unset($languages['en']);

  if (empty($languages)) {
    $form['info'] = array(
      '#type' => 'markup',
      '#markup' => '<p>' . t('No languages found to translate to.') . '</p>',
    );
  }
  else {

    $language_options = array();
    foreach ($languages as $key => $info) {
      $language_options[$key] = $info->name . ' (' . $info->language . ')';
    }

    $user_languages = db_query('SELECT language FROM {locales_translate_set_user_languages} WHERE uid = :uid', array(':uid' => $account->uid))->fetchCol();

    $form['languages'] = array(
      '#type' => 'checkboxes',
      '#options' => $language_options,
      '#default_value' => $user_languages,
      '#description' => t('Select languages this user can translate on the Translate set interface. Toggle nothing if you want to allow all.')
    );

    $form['uid'] = array(
      '#type' => 'value',
      '#value' => $account->uid,
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );

  }

  return $form;
}

/**
 * Submit callback: saves the languages per user for Translate set.
 */
function translate_set_configure_languages_per_user_submit($form, $form_state) {
  db_truncate('locales_translate_set_user_languages')->execute();

  foreach ($form_state['values']['languages'] as $key => $value) {
    if ($key === $value) {
      db_query('INSERT INTO {locales_translate_set_user_languages} (uid, language) VALUES (:uid, :language)', array(':uid' => $form_state['values']['uid'], ':language' => $value));
    }
  }

  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Page callback: add or remove a string from the translate set.
 *
 * @param string $action
 *   The action to perform
 * @param int $lid
 *   The locale id.
 */
function translate_set_add_or_remove($action = '', $lid = 0) {
  $message = t('Invalid action');

  if (!empty($action) && !empty($lid)) {

    // Make sure this source exists.
    $exists = db_query('SELECT lid FROM {locales_source} WHERE lid = :lid', array(':lid' => $lid))->fetchField();

    if ($exists) {
      switch ($action) {
        case 'remove':
          $message = t('The string has been removed from the translate set.');
          db_query("DELETE FROM {locales_translate_set} WHERE lid = :lid", array(':lid' => $lid));
          break;
        case 'add':
          $message = t('The string has been added to the translate set.');
          db_query("INSERT INTO {locales_translate_set} VALUES (:lid)", array(':lid' => $lid));
          break;
      }
    }
  }

  drupal_set_message($message);
  drupal_goto('admin/config/regional/translate/translate');
}

/**
 * Page callback: String search screen.
 *
 * This function resembles locale_translate_seek_screen(), but we've replaced
 * _locale_translate_seek() by _translate_set_translate_seek() so we can
 * inject our own operation to add or remove a string to the translate set.
 */
function translate_set_translate_seek_screen() {
  // Add CSS.
  drupal_add_css(drupal_get_path('module', 'locale') . '/locale.css');

  // Load locale.admin.inc file for the filter form.
  module_load_include('inc', 'locale', 'locale.admin');

  $elements = drupal_get_form('locale_translation_filter_form');
  $output = drupal_render($elements);

  // Call our own translate seek function.
  $output .= translate_set_translate_seek();
  return $output;
}

/**
 * Perform a string search and display results in a table.
 *
 * @see translate_set_translate_seek_screen().
 */
function translate_set_translate_seek() {
  $output = '';

  // We have at least one criterion to match
  if (!($query = _locale_translate_seek_query())) {
    $query = array(
      'translation' => 'all',
      'group' => 'all',
      'language' => 'all',
      'string' => '',
    );
  }

  $sql_query = db_select('locales_source', 's');

  $limit_language = NULL;
  if ($query['language'] != 'en' && $query['language'] != 'all') {
    $sql_query->leftJoin('locales_target', 't', "t.lid = s.lid AND t.language = :langcode", array(':langcode' => $query['language']));
    $limit_language = $query['language'];
  }
  else {
    $sql_query->leftJoin('locales_target', 't', 't.lid = s.lid');
  }

  $sql_query->fields('s', array('source', 'location', 'context', 'lid', 'textgroup'));
  $sql_query->fields('t', array('translation', 'language'));

  // Compute LIKE section.
  switch ($query['translation']) {
    case 'translated':
      $sql_query->condition('t.translation', '%' . db_like($query['string']) . '%', 'LIKE');
      $sql_query->orderBy('t.translation', 'DESC');
      break;
    case 'untranslated':
      $sql_query->condition(db_and()
          ->condition('s.source', '%' . db_like($query['string']) . '%', 'LIKE')
          ->isNull('t.translation')
      );
      $sql_query->orderBy('s.source');
      break;
    case 'all' :
    default:
      $condition = db_or()
        ->condition('s.source', '%' . db_like($query['string']) . '%', 'LIKE');
      if ($query['language'] != 'en') {
        // Only search in translations if the language is not forced to English.
        $condition->condition('t.translation', '%' . db_like($query['string']) . '%', 'LIKE');
      }
      $sql_query->condition($condition);
      break;
  }

  // Add a condition on the text group.
  if (!empty($query['group']) && $query['group'] != 'all') {
    $sql_query->condition('s.textgroup', $query['group']);
  }

  $sql_query = $sql_query->extend('PagerDefault')->limit(50);
  $locales = $sql_query->execute();

  $groups = module_invoke_all('locale', 'groups');
  $header = array(t('Text group'), t('String'), t('Context'), ($limit_language) ? t('Language') : t('Languages'), array('data' => t('Operations'), 'colspan' => '3'));

  $strings = array();
  foreach ($locales as $locale) {
    if (!isset($strings[$locale->lid])) {
      $strings[$locale->lid] = array(
        'group' => $locale->textgroup,
        'languages' => array(),
        'location' => $locale->location,
        'source' => $locale->source,
        'context' => $locale->context,
      );
    }
    if (isset($locale->language)) {
      $strings[$locale->lid]['languages'][$locale->language] = $locale->translation;
    }
  }

  $rows = array();
  foreach ($strings as $lid => $string) {

    // Determine action for the Translate set link.
    $exists = db_query('SELECT lid FROM {locales_translate_set} WHERE lid = :lid', array(':lid' => $lid))->fetchField();
    $translate_set_action = $exists ? 'remove' : 'add';
    $translate_set_action_label = $exists ? t('remove from translate set') : t('add to translate set');

    $rows[] = array(
      $groups[$string['group']],
      array('data' => check_plain(truncate_utf8($string['source'], 150, FALSE, TRUE)) . '<br /><small>' . $string['location'] . '</small>'),
      $string['context'],
      array('data' => _locale_translate_language_list($string['languages'], $limit_language), 'align' => 'center'),
      array('data' => l(t('edit'), "admin/config/regional/translate/edit/$lid", array('query' => drupal_get_destination())), 'class' => array('nowrap')),
      array('data' => l(t('delete'), "admin/config/regional/translate/delete/$lid", array('query' => drupal_get_destination())), 'class' => array('nowrap')),
      array('data' => l($translate_set_action_label, "admin/config/regional/translate/translate-set-action/$translate_set_action/$lid"), 'class' => array('nowrap')),
    );
  }

  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('No strings available.')));
  $output .= theme('pager');

  return $output;
}

/**
 * Page callback: Translate set screen.
 */
function translate_set_translate_form($form, $form_state) {
  $form = array();
  $rows = array();
  $languages = array();
  $number_of_strings = 0;
  $untranslated = array();
  $header = array(t('Source'));

  // Track new strings - do not run on a POST request.
  if (empty($_POST) && variable_get('translate_set_tracking_enabled', FALSE)) {
    translate_set_tracking_find_strings();
  }

  // Show all, untranslated or translated strings.
  $type = isset($_GET['type']) ? $_GET['type'] : 'all';

  // Filter on textgroup.
  $textgroup = isset($_GET['textgroup']) ? $_GET['textgroup'] : 'all';

  // Check user language settings.
  $restricted_languages = db_query('SELECT language FROM {locales_translate_set_user_languages} WHERE uid = :uid', array(':uid' => $GLOBALS['user']->uid))->fetchCol();

  // Determine the language.
  $language_list = language_list();
  $language_options = array();
  foreach ($language_list as $key => $info) {

    // Ignore built-in English.
    if ($key == 'en') {
      continue;
    }

    // Check restricted languages.
    if (!empty($restricted_languages)) {
      if (!in_array($info->language, $restricted_languages)) {
        continue;
      }
    }

    // Add to options.
    $language_options[$key] = $info->name . ' (' . $info->language . ')';
  }

  // If there are no options for the language, stop here.
  if (empty($language_options)) {
    $form['info'] = array(
      '#type' => 'markup',
      '#markup' => '<p>' . t('No languages found to translate to. Either add new languages or contact an administrator.') . '</p>',
    );
    return $form;
  }

  // Check selected language.
  $selected_language = isset($_GET['language']) ? (isset($language_options[$_GET['language']]) ? $_GET['language'] : $GLOBALS['language']->language) : $GLOBALS['language']->language;

  // Selected language can not be 'en'.
  if ($selected_language == 'en') {
    $selected_language = key($language_options);
  }

  // Add to header and languages.
  // Languages is already an array since in the future we might allow
  // multiple columns as a feature.
  $header[] = $language_list[$selected_language]->name;
  $languages[] = $selected_language;

  // Create the query. We limit to around 500 strings so we don't hit the limit
  // of maximum post values in PHP. And also, 500 strings on a page is fine
  // enough.
  $select = db_select('locales_translate_set', 'lts')
    ->extend('PagerDefault')
    ->limit(round(500 / (count($header) -1)));

  $select->innerJoin('locales_source', 'ls', 'ls.lid = lts.lid');
  $select->fields('ls', array('lid', 'source', 'textgroup', 'context'));

  if ($textgroup != 'all') {
    $select->condition('ls.textgroup', $textgroup);
  }

  $select->orderBy('source', 'ASC');
  $strings = $select->execute();

  // Get the strings.
  foreach ($strings as $string) {
    $group = '';
    if ($string->textgroup != 'default') {
      $group = $string->textgroup . ' - ' . $string->context;
    }
    if (!isset($rows['string-' . $string->lid])) {
      $number_of_strings++;
      $rows['string-' . $string->lid] = array(
        '#type' => 'tree',
      );

      // Check the source, this can be a translated string from the
      // the fallback language or the source if non exists.
      $source = $string->source;
      if (!empty($fallback)) {
        $translation = db_query("SELECT translation FROM {locales_target} WHERE lid = :lid AND language = :language", array(':lid' => $string->lid, ':language' => $fallback))->fetchField();
        if (!empty($translation)) {
          $source = $translation;
        }
      }
      $rows['string-' . $string->lid]['source'] = array(
        '#type' => 'markup',
        '#markup' => check_plain($source),
      );
    }

    foreach ($languages as $language) {
      $translation = db_query("SELECT translation FROM {locales_target} WHERE lid = :lid AND language = :language", array(':lid' => $string->lid, ':language' => $language))->fetchField();
      $rows['string-' . $string->lid][$language] = array(
        '#type' => 'textarea',
        '#title' => $group,
        '#cols' => 250,
        '#rows' => 2,
        '#default_value' => $translation,
        '#attributes' => array('class' => array('translation-input')),
      );
      if (empty($translation)) {

        if (!isset($untranslated[$language])) {
          $untranslated[$language] = 0;
        }
        $untranslated[$language]++;

        // Depending on $type, remove this row or not.
        if ($type == 'translated') {
          $number_of_strings--;
          $untranslated[$language]--;
          unset($rows['string-' . $string->lid]);
        }

      }
      else {
        // Depending on $type, remove this row or not.
        if ($type == 'untranslated') {
          $number_of_strings--;
          unset($rows['string-' . $string->lid]);
        }
      }
    }
  }

  // Add stats.
  $untranslated_array = array();
  foreach ($untranslated as $count) {
    $untranslated_array[] = $count;
  }
  $untranslated_string = !empty($untranslated_array) ? implode(', ', $untranslated_array) : 0;
  $stats = format_string('Number of strings: @strings_number - Untranslated strings: @untranslated', array('@strings_number' => $number_of_strings, '@untranslated' => $untranslated_string));

  $form['stats'] = array(
    '#type' => 'markup',
    '#markup' => '<p>' . $stats . '</p>'
  );

  $form['container'] = array(
    '#type' => 'container',
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );
  $form['container']['select_language'] = array(
    '#type' => 'select',
    '#title' => t('Language'),
    '#options' => $language_options,
    '#default_value' => $selected_language,
  );
  $form['container']['type'] = array(
    '#type' => 'select',
    '#title' => t('Filter'),
    '#options' => array('all' => 'all strings', 'untranslated' => 'untranslated strings', 'translated' => 'translated strings'),
    '#default_value' => $type,
  );
  $groups = module_invoke_all('locale', 'groups');
  $form['container']['textgroup'] = array(
    '#type' => 'select',
    '#title' => t('Textgroup'),
    '#options' => array_merge(array('all' => t('All text groups')), $groups),
    '#default_value' => $textgroup,
  );
  $form['container']['go'] = array(
    '#type' => 'submit',
    '#value' => t('Apply'),
    '#submit' => array('translate_set_filter_form_submit'),
  );

  if (!empty($rows)) {
    $rows['#tree'] = TRUE;
  }

  $form['table'] = array(
    '#theme' => 'translate_set_table',
    '#header' => $header,
    '#add_big_column_class' => TRUE,
    'strings' => $rows,
  );

  $form['pager'] = array(
    '#theme' => 'pager',
    '#weight' => 5,
  );

  if (!empty($rows)) {

    $form['#attached']['css'][] = drupal_get_path('module', 'translate_set') . '/translate_set.admin.css';

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Save translations',
    );
  }

  return $form;
}

/**
 * Submit callback: Saves the translate set form.
 */
function translate_set_translate_form_submit($form, $form_state) {
  $stats = array(
    'updated' => 0,
    'created' => 0,
    'deleted' => 0,
  );
  $strings = $form_state['values']['strings'];
  $invalidate_languages = array();
  foreach ($strings as $string_lid => $translations) {
    list (, $lid) = explode('-', $string_lid);
    foreach ($translations as $language => $value) {
      $invalidate_languages[$language] = $language;
      $translation = db_query("SELECT translation FROM {locales_target} WHERE lid = :lid AND language = :language", array(':lid' => $lid, ':language' => $language))->fetchField();
      if (!empty($value)) {
        // Only update or insert if we have a value to use.
        if (!empty($translation)) {
          $stats['updated']++;
          db_update('locales_target')
            ->fields(array(
              'translation' => $value,
            ))
            ->condition('lid', $lid)
            ->condition('language', $language)
            ->execute();
        }
        else {
          $stats['created']++;
          db_insert('locales_target')
            ->fields(array(
              'lid' => $lid,
              'translation' => $value,
              'language' => $language,
            ))
            ->execute();
        }
      }
      elseif (!empty($translation)) {
        $stats['deleted']++;
        // Empty translation entered: remove existing entry from database.
        db_delete('locales_target')
          ->condition('lid', $lid)
          ->condition('language', $language)
          ->execute();
      }
    }
  }

  // Invalidate caches and javascript translations.
  cache_clear_all('locale:', 'cache', TRUE);
  foreach ($invalidate_languages as $invalidate_language) {
    _locale_invalidate_js($invalidate_language);
  }

  drupal_set_message(t('Strings saved: @created created - @updated updated - @deleted deleted', array('@created' => $stats['created'], '@updated' => $stats['updated'], '@deleted' => $stats['deleted'])));
}

/**
 * Submit callback: filter the translate set form.
 */
function translate_set_filter_form_submit($form, &$form_state) {
  $form_state['redirect'] = array('admin/config/regional/translate/translate-set', array(
    'query' => array(
      'language' => $form_state['values']['select_language'],
      'type' => $form_state['values']['type'],
      'textgroup' => $form_state['values']['textgroup'],
    )
  ));
}

/**
 * Menu callback: present the manage the translate set form.
 */
function translate_set_manage_form($form, $form_state) {
  $rows = array();
  $header = array('', t('String'));
  $textgroup = isset($_GET['textgroup']) ? $_GET['textgroup'] : 'all';
  $only_in_set = isset($_GET['only_in_set']) ? $_GET['only_in_set'] : FALSE;

  $query = db_select('locales_source', 's')->extend('PagerDefault');
  $query->leftJoin('locales_translate_set', 'lts', "s.lid = lts.lid");
  $query->fields('s', array('source', 'lid'))
    ->fields('lts', array('lid'))
    ->orderBy('s.source');

  if ($textgroup != 'all') {
    $query->condition('s.textgroup', $textgroup);
  }

  if ($only_in_set) {
    $query->isNotNull('lts.lid');
  }

  $result = $query
    ->limit(50)
    ->execute();

  foreach ($result as $data) {

    $rows['string-' . $data->lid]['toggle'] = array(
      '#type' => 'checkbox',
      '#default_value' => $data->lts_lid ? TRUE : FALSE,
    );

    $rows['string-' . $data->lid]['source'] = array(
      '#type' => 'markup',
      '#markup' => check_plain($data->source),
    );

  }

  if (!empty($rows)) {
    $rows['#tree'] = TRUE;
  }

  $form['container'] = array(
    '#type' => 'container',
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
    '#weight' => -2
  );
  $groups = module_invoke_all('locale', 'groups');
  $form['container']['textgroup'] = array(
    '#type' => 'select',
    '#title' => t('Textgroup'),
    '#options' => array_merge(array('all' => t('All text groups')), $groups),
    '#default_value' => $textgroup,
  );
  $form['container']['only_in_set'] = array(
    '#type' => 'checkbox',
    '#title' => t('Only show strings in the set'),
    '#default_value' => $only_in_set,
  );
  $form['container']['go'] = array(
    '#type' => 'submit',
    '#value' => t('Apply'),
    '#submit' => array('translate_set_manage_filter_form_submit'),
  );


  $form['table'] = array(
    '#theme' => 'translate_set_table',
    '#header' => $header,
    '#add_big_column_class' => FALSE,
    '#weight' => 0,
    'strings' => $rows,
  );

  if (!empty($rows)) {

    $form['#attached']['css'][] = drupal_get_path('module', 'translate_set') . '/translate_set.admin.css';

    $form['info'] = array(
      '#weight' => -3,
      '#type' => 'markup',
      '#markup' => '<p>' . t('Add or remove strings from the translate set by toggling the checkboxes.') . '</p>'
    );

    $form['pager_top'] = array(
      '#theme' => 'pager',
      '#weight' => -1,
    );

    $form['pager_bottom'] = array(
      '#theme' => 'pager',
      '#weight' => 1,
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#weight' => 2,
    );
  }

  return $form;
}

/**
 * Submit callback: saves the manage translate set form.
 */
function translate_set_manage_form_submit($form, $form_state) {
  $stats = array(
    'added' => 0,
    'deleted' => 0,
  );

  foreach ($form_state['values']['strings'] as $key => $data) {
    list(, $lid) = explode('-', $key);
    $toggled = $data['toggle'] === 0 ? FALSE : TRUE;
    $exists = db_query('SELECT lid FROM {locales_translate_set} WHERE lid = :lid', array(':lid' => $lid))->fetchField();

    // Deleted.
    if ($exists && !$toggled) {
      $stats['deleted']++;
      db_query('DELETE FROM {locales_translate_set} WHERE lid = :lid', array(':lid' => $lid));
    }
    elseif (!$exists && $toggled) {
      $stats['added']++;
      db_query('INSERT INTO {locales_translate_set} (lid) VALUES (:lid)', array(':lid' => $lid));
    }
  }

  drupal_set_message(t('Saved translate set: @added added - @deleted deleted', array('@added' => $stats['added'], '@deleted' => $stats['deleted'])));
}

/**
 * Submit callback: filter the manage translate set form.
 */
function translate_set_manage_filter_form_submit($form, &$form_state) {
  $form_state['redirect'] = array('admin/config/regional/translate/translate-set/manage-set', array(
    'query' => array(
      'textgroup' => $form_state['values']['textgroup'],
      'only_in_set' => $form_state['values']['only_in_set'],
    )
  ));

}

/**
 * Themes a translate set table.
 */
function theme_translate_set_table(&$variables) {
  // Get the useful values.
  $form = $variables['form'];
  $rows = $form['strings'];
  $header = $form['#header'];
  $add_big_column_class = $form['#add_big_column_class'];

  // Setup the structure to be rendered and returned.
  $content = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => array(),
    '#empty' => t('No strings found'),
  );

  // Traverse each row.
  foreach (element_children($rows) as $row_index) {
    $row = array();
    $i = 0;
    // Traverse each column in the row.  @see element_children().
    foreach (element_children($rows[$row_index]) as $col_index) {
      // Render the column form element.
      if ($add_big_column_class) {
        $class = 'translate-set-form-column-big';
        $row[] = array('data' => drupal_render($rows[$row_index][$col_index]), 'class' => array($class));
      }
      elseif ($rows[$row_index][$col_index]['#type'] == 'checkbox') {
        $row[] = array('data' => drupal_render($rows[$row_index][$col_index]), 'class' => array('translate-set-form-checkbox'));
      }
      else {
        $row[] = array('data' => drupal_render($rows[$row_index][$col_index]));
      }
      $i++;
    }
    // Add the row to the table.
    $content['#rows'][] = $row;
  }

  // Render the table and return.
  return drupal_render($content);
}
