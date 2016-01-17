<?php

/**
 * @file
 *  Search log Views filter by language.
 */

class views_handler_filter_search_log_language extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Language');
      foreach (language_list() as $langcode => $language) {
        $languages[$langcode] = $language->name;
      }
      $this->value_options = $languages;
    }
  }
}
