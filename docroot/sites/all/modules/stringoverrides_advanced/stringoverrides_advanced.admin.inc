<?php

/**
 * String search screen.
 */
function stringoverrides_advanced_translate_seek_screen() {
  // Add CSS.
  drupal_add_css(drupal_get_path('module', 'locale') . '/locale.css');

  $elements = drupal_get_form('stringoverrides_advanced_translation_filter_form');
  $output = drupal_render($elements);
  $output .= _stringoverrides_advanced_translate_seek();
  return $output;
}

/**
 * Return form for locale translation filters.
 *
 * @ingroup forms
 */
function stringoverrides_advanced_translation_filter_form() {
  $filters = stringoverrides_advanced_translation_filters();

  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filter overridable strings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  foreach ($filters as $key => $filter) {
    // Special case for 'string' filter.
    if ($key == 'string') {
      $form['filters']['status']['string'] = array(
        '#type' => 'textfield',
        '#title' => $filter['title'],
        '#description' => $filter['description'],
      );
    }
    else {
      $form['filters']['status'][$key] = array(
        '#title' => $filter['title'],
        '#type' => 'select',
        '#empty_value' => 'all',
        '#empty_option' => $filter['options']['all'],
        '#size' => 0,
        '#options' => $filter['options'],
      );
    }
    if (!empty($_SESSION['stringoverrides_advanced_translation_filter'][$key])) {
      $form['filters']['status'][$key]['#default_value'] = $_SESSION['stringoverrides_advanced_translation_filter'][$key];
    }
  }

  $form['filters']['actions'] = array(
    '#type' => 'actions',
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['filters']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
  );
  if (!empty($_SESSION['stringoverrides_advanced_translation_filter'])) {
    $form['filters']['actions']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset')
    );
  }

  return $form;
}

/**
 * Process result from locale translation filter form.
 */
function stringoverrides_advanced_translation_filter_form_submit($form, &$form_state) {
  $op = $form_state['values']['op'];
  $filters = stringoverrides_advanced_translation_filters();
  switch ($op) {
    case t('Filter'):
      foreach ($filters as $name => $filter) {
        if (isset($form_state['values'][$name])) {
          $_SESSION['stringoverrides_advanced_translation_filter'][$name] = $form_state['values'][$name];
        }
      }
      break;
    case t('Reset'):
      $_SESSION['stringoverrides_advanced_translation_filter'] = array();
      break;
  }

  $form_state['redirect'] = 'admin/config/regional/stringoverrides_advanced';
}

/**
 * Perform a string search and display results in a table
 */
function _stringoverrides_advanced_translate_seek() {
  $output = '';

  // We have at least one criterion to match
  if (!($query = _stringoverrides_advanced_translate_seek_query())) {
    $query = array(
      'translation' => 'all',
      'group' => 'all',
      'language' => 'all',
      'string' => '',
    );
  }

  $sql_query = db_select('stringoverrides_advanced_source', 's');

  $sql_query->leftJoin('stringoverrides_advanced_target', 't', 't.lid = s.lid');

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
        // Only search in translations if the language is not forced to English.
      $condition->condition('t.translation', '%' . db_like($query['string']) . '%', 'LIKE');
      $sql_query->condition($condition);
      break;
  }

  // Add a condition on the text group.
  $sql_query->condition('s.textgroup', 'default');

  $sql_query = $sql_query->extend('PagerDefault')->limit(50);
  $locales = $sql_query->execute();

  $header = array(t('String'), t('Context'), array('data' => t('Operations'), 'colspan' => '2'));

  $strings = array();
  foreach ($locales as $locale) {
    if (!isset($strings[$locale->lid])) {
      $strings[$locale->lid] = array(
        'group' => $locale->textgroup,
        'location' => $locale->location,
        'source' => $locale->source,
        'context' => $locale->context,
      );
    }
  }

  $rows = array();
  foreach ($strings as $lid => $string) {
    $rows[] = array(
      array('data' => check_plain(truncate_utf8($string['source'], 150, FALSE, TRUE)) . '<br /><small>' . $string['location'] . '</small>'),
      $string['context'],
      array('data' => l(t('edit'), "admin/config/regional/stringoverrides_advanced/edit/$lid", array('query' => drupal_get_destination())), 'class' => array('nowrap')),
      array('data' => l(t('delete'), "admin/config/regional/stringoverrides_advanced/delete/$lid", array('query' => drupal_get_destination())), 'class' => array('nowrap')),
    );
  }

  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('No strings available.')));
  $output .= theme('pager');

  return $output;
}

/**
 * Build array out of search criteria specified in request variables
 */
function _stringoverrides_advanced_translate_seek_query() {
  $query = &drupal_static(__FUNCTION__);
  if (!isset($query)) {
    $query = array();
    $fields = array('string', 'language', 'translation', 'group');
    foreach ($fields as $field) {
      if (isset($_SESSION['stringoverrides_advanced_translation_filter'][$field])) {
        $query[$field] = $_SESSION['stringoverrides_advanced_translation_filter'][$field];
      }
    }
  }
  return $query;
}

/**
 * List locale translation filters that can be applied.
 */
function stringoverrides_advanced_translation_filters() {
  $filters = array();

  $filters['string'] = array(
    'title' => t('String contains'),
    'description' => t('Leave blank to show all strings. The search is case sensitive.'),
  );

  $filters['translation'] = array(
    'title' => t('Search in'),
    'options' => array('all' => t('Both overridden and non-overridden strings'), 'translated' => t('Only overridden strings'), 'untranslated' => t('Only non-overridden strings')),
  );

  return $filters;
}

/**
 * List languages in search result table
 */
function _stringoverrides_advanced_translate_language_list($translation, $limit_language) {
  // Add CSS.
  drupal_add_css(drupal_get_path('module', 'locale') . '/locale.css');

  $languages = language_list();
  $output = '';
  foreach ($languages as $langcode => $language) {
    if (!$limit_language || $limit_language == $langcode) {
      $output .= (!empty($translation[$langcode])) ? $langcode . ' ' : "<em class=\"locale-untranslated\">$langcode</em> ";
    }
  }

  return $output;
}

/**
 * User interface for string editing.
 */
function stringoverrides_advanced_translate_edit_form($form, &$form_state, $lid) {
  // Fetch source string, if possible.
  $source = db_query('SELECT source, context, textgroup, location FROM {stringoverrides_advanced_source} WHERE lid = :lid', array(':lid' => $lid))->fetchObject();
  if (!$source) {
    drupal_set_message(t('String not found.'), 'error');
    drupal_goto('admin/config/regional/stringoverrides_advanced');
  }

  // Add original text to the top and some values for form altering.
  $form['original'] = array(
    '#type'  => 'item',
    '#title' => t('Original text'),
    '#markup' => check_plain(wordwrap($source->source, 0)),
  );
  if (!empty($source->context)) {
    $form['context'] = array(
      '#type' => 'item',
      '#title' => t('Context'),
      '#markup' => check_plain($source->context),
    );
  }
  $form['lid'] = array(
    '#type'  => 'value',
    '#value' => $lid
  );
  $form['textgroup'] = array(
    '#type'  => 'value',
    '#value' => $source->textgroup,
  );
  $form['location'] = array(
    '#type'  => 'value',
    '#value' => $source->location
  );

  $default = language_default();
  $default->name = t('Override text');
  $languages = array($default->language => $default);
  $form['translations'] = array('#tree' => TRUE);
  // Approximate the number of rows to use in the default textarea.
  $rows = min(ceil(str_word_count($source->source) / 12), 10);
  foreach ($languages as $langcode => $language) {
    $form['translations'][$langcode] = array(
      '#type' => 'textarea',
      '#title' => t($language->name),
      '#rows' => $rows,
      '#default_value' => '',
    );
  }

  // Fetch translations and fill in default values in the form.
  $result = db_query("SELECT DISTINCT translation, language FROM {stringoverrides_advanced_target} WHERE lid = :lid", array(':lid' => $lid));
  foreach ($result as $translation) {
    if (isset($form['translations'][$translation->language])) {
      $form['translations'][$translation->language]['#default_value'] = $translation->translation;
    }
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save override'));
  return $form;
}

/**
 * Validate string editing form submissions.
 */
function stringoverrides_advanced_translate_edit_form_validate($form, &$form_state) {
  // Locale string check is needed for default textgroup only.
  $safe_check_needed = $form_state['values']['textgroup'] == 'default';
  foreach ($form_state['values']['translations'] as $key => $value) {
    if ($safe_check_needed && !stringoverrides_advanced_string_is_safe($value)) {
      form_set_error('translations', t('The submitted string contains disallowed HTML: %string', array('%string' => $value)));
      watchdog('stringoverrides_advanced', 'Attempted submission of an override string with disallowed HTML: %string', array('%string' => $value), WATCHDOG_WARNING);
    }
  }
}

/**
 * Process string editing form submissions.
 *
 * Saves all translations of one string submitted from a form.
 */
function stringoverrides_advanced_translate_edit_form_submit($form, &$form_state) {
  $lid = $form_state['values']['lid'];
  foreach ($form_state['values']['translations'] as $key => $value) {
    $translation = db_query("SELECT translation FROM {stringoverrides_advanced_target} WHERE lid = :lid AND language = :language", array(':lid' => $lid, ':language' => $key))->fetchField();
    if (!empty($value)) {
      // Only update or insert if we have a value to use.
      if (!empty($translation)) {
        db_update('stringoverrides_advanced_target')
          ->fields(array(
            'translation' => $value,
          ))
          ->condition('lid', $lid)
          ->condition('language', $key)
          ->execute();
      }
      else {
        db_insert('stringoverrides_advanced_target')
          ->fields(array(
            'lid' => $lid,
            'translation' => $value,
            'language' => $key,
          ))
          ->execute();
      }
    }
    elseif (!empty($translation)) {
      // Empty translation entered: remove existing entry from database.
      db_delete('stringoverrides_advanced_target')
        ->condition('lid', $lid)
        ->condition('language', $key)
        ->execute();
    }
  }

  drupal_set_message(t('The string has been saved.'));

  cache_clear_all('advancedStringOverrides:', 'cache', TRUE);

  $form_state['redirect'] = 'admin/config/regional/stringoverrides_advanced';
  return;
}

/**
 * String deletion confirmation page.
 */
function stringoverrides_advanced_translate_delete_page($lid) {
  if ($source = db_query('SELECT lid, source FROM {stringoverrides_advanced_source} WHERE lid = :lid', array(':lid' => $lid))->fetchObject()) {
    return drupal_get_form('stringoverrides_advanced_translate_delete_form', $source);
  }
  else {
    return drupal_not_found();
  }
}

/**
 * User interface for the string deletion confirmation screen.
 */
function stringoverrides_advanced_translate_delete_form($form, &$form_state, $source) {
  $form['lid'] = array('#type' => 'value', '#value' => $source->lid);
  return confirm_form($form, t('Are you sure you want to delete the string "%source"?', array('%source' => $source->source)), 'admin/config/regional/stringoverrides_advanced', t('Deleting the string will remove all overrides of this string. This action cannot be undone.'), t('Delete'), t('Cancel'));
}

/**
 * Process string deletion submissions.
 */
function stringoverrides_advanced_translate_delete_form_submit($form, &$form_state) {
  db_delete('stringoverrides_advanced_source')
    ->condition('lid', $form_state['values']['lid'])
    ->execute();
  db_delete('stringoverrides_advanced_target')
    ->condition('lid', $form_state['values']['lid'])
    ->execute();
  cache_clear_all('advancedStringOverrides:', 'cache', TRUE);
  drupal_set_message(t('The string has been removed.'));
  $form_state['redirect'] = 'admin/config/regional/stringoverrides_advanced';
}