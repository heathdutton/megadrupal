<?php

/**
 * Extends the default i18n string textgroup class providing language fallback.
 *
 * The overridden methods contain a bit modified code of the original methods.
 * It would be nice to not copy the code and use hook_query_alter(), but the
 * i18n_string_textgroup_default class does not uses query tags, so
 * hook_query_alter() is never called for its queries.
 */
class language_hierarchy_i18n_string_textgroup_default extends i18n_string_textgroup_default {

  /**
   * {@inheritdoc}
   */
  public static function load_translation($i18nstring, $langcode) {
    $chain = language_hierarchy_get_ancestors($langcode);
    if (empty($chain)) {
      return parent::load_translation($i18nstring, $langcode);
    }
    $langcodes = array_values(array_merge(array($langcode), array_keys($chain)));
    if (!empty($i18nstring->lid)) {
      $query = db_select('locales_target', 't');
      $query->condition('t.lid', $i18nstring->lid);
    }
    else {
      $query = db_select('i18n_string', 's')->fields('s');
      $query->leftJoin('locales_target', 't', 's.lid = t.lid');
      $query->condition('s.textgroup', $i18nstring->textgroup);
      $query->condition('s.context', $i18nstring->context);
    }
    $query->fields('t', array('translation', 'i18n_status'));
    if (drupal_static('language_hierarchy_i18n_show_language')) {
      $query->addField('t', 'language');
    }
    // The i18n module can save empty strings as translations. In this case, it
    // behaves as there is no translation and uses the original strings.
    // Handle this case by simple excluding of empty translations.
    $query->condition('t.translation', '', '<>');
    // Pass all language chain instead of one given language.
    $query->condition('t.language', $langcodes, 'IN');
    // Sort results according to the language chain order.
    $order = '(CASE ';
    foreach ($langcodes as $index => $lang) {
      $order .= "WHEN t.language = '$lang' THEN $index ";
    }
    $order .= 'END)';
    $query->orderBy($order);
    $query->range(0, 1);
    return $query->execute()->fetchObject();
  }

  /**
   * {@inheritdoc}
   */
  protected function multiple_translation_load($conditions, $langcode) {
    $conditions += array(
      'language' => $langcode,
      'textgroup' => $this->textgroup
    );
    if (is_array($conditions['language'])) {
      if (count($conditions['language']) !== 1) {
        // This should never happen, but if it does, we don't know what to do.
        return parent::multiple_translation_load($conditions, $langcode);
      }
      $conditions['language'] = reset($conditions['language']);
    }
    $chain = language_hierarchy_get_ancestors($conditions['language']);
    if (empty($chain)) {
      return parent::multiple_translation_load($conditions, $langcode);
    }
    $conditions['language'] = array_values(array_merge(array($conditions['language']), array_keys($chain)));
    $query = db_select('i18n_string', 's')->fields('s');
    $query->leftJoin('locales_target', 't', 's.lid = t.lid');
    $query->fields('t', array('translation', 'language', 'i18n_status'));
    foreach ($conditions as $field => $value) {
      if (is_array($value) && count($value) == 1) {
        $value = reset($value);
      }
      if ($value === '*') {
        continue;
      }
      elseif ($field == 'language') {
        $query->condition('t.language', $value);
      }
      else {
        $query->condition('s.' . $field, $value);
      }
    }
    // The i18n module can save empty strings as translations. In this case, it
    // behaves as there is no translation and uses the original strings.
    // Handle this case by simple excluding of empty translations.
    $query->condition('t.translation', '', '<>');
    // Sort results according to the language chain order.
    $order = '(CASE ';
    foreach ($conditions['language'] as $index => $lang) {
      $order .= "WHEN t.language = '$lang' THEN $index ";
    }
    $order .= 'END)';
    $query->orderBy($order);
    // The query now is ordered by language, but we will need to use the GROUP
    // BY operator which is applied *before* the ORDER BY, so in the end we may
    // have wrong translations selected. To ovid this, do the trick with
    // subquery, so we have the correct grouping.
    $main_query = db_select($query, 'main')
      ->fields('main')
      ->groupBy('main.lid');
    return $this->multiple_translation_build($main_query->execute()->fetchAll(), $langcode);
  }
}
