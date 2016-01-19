<?php

/**
 * @file
 * Definition of views_handler_filter_locale_language.
 */

/**
 * Filter by language.
 *
 * @ingroup views_filter_handlers
 */
class language_hierarchy_views_handler_filter_language extends views_handler_filter_locale_language {

  /**
   * Inject option for fallback languages into language selection.
   */
  function get_value_options() {
    if (!isset($this->value_options)) {
      parent::get_value_options();

      $this->value_options['***FALLBACK_LANGUAGE***'] = t("Current user's language with fallback");
      // Move element to beginning of array
      $this->value_options = array('***FALLBACK_LANGUAGE***' => $this->value_options['***FALLBACK_LANGUAGE***']) + $this->value_options;
    }
  }

  /**
   * Replace special '***FALLBACK_LANGUAGE***' with an array of fallback languages.
   */
  function op_simple() {
    if (empty($this->value)) {
      return;
    }

    $value = $this->value;

    if (isset($value['***FALLBACK_LANGUAGE***'])) {
      if ($value['***FALLBACK_LANGUAGE***']) {
        global $language_content;

        $fallbacks = array($language_content->language => $language_content->language);
        $fallbacks += array_keys(language_hierarchy_get_ancestors($language_content->language));
        $value = array_merge($value, drupal_map_assoc($fallbacks));
      }

      // Remove our dummy language.
      unset($value['***FALLBACK_LANGUAGE***']);
    }

    $this->ensure_my_table();

    // We use array_values() because the checkboxes keep keys and that can cause
    // array addition problems.
    $this->query->add_where($this->options['group'], "$this->table_alias.$this->real_field", array_values($value), $this->operator);
  }

}
