<?php

/**
 * @file
 * Views handler intergration: displays the language field in a human readable way
 */

class lingwo_entry_views_handler_argument_language extends views_handler_argument {
  function summary_name($data) {
    return $this->human_language($data->{$this->name_alias});
  }

  function title() {
    return $this->human_language($this->argument);
  }

  function human_language($langcode) {
    $languages = language_list();
    $value = isset($languages[$langcode]) ? t($languages[$langcode]->name) : t('Unknown language');
    return $value;
  }
}

