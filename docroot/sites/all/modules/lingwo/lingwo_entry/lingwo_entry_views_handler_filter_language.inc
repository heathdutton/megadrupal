<?php

/**
 * @file
 * Views handler intergration: displays the language field in a human readable way
 */

class lingwo_entry_views_handler_filter_language extends views_handler_filter_in_operator {
  function get_value_options() {
    $options = array(
      '***CURRENT_LANGUAGE***' => t("Current user's language"),
      '***DEFAULT_LANGUAGE***' => t("Default site language"),
    );

    $languages = language_list('enabled');
    $languages = $languages[1];
    foreach ($languages as $language) {
      $options[$language->language] = t($language->name);
    }

    $this->value_options = $options;
  }
}

