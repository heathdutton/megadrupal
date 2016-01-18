<?php

/**
 * @file
 * Views handler intergration: displays the language field in a human readable way
 */

class lingwo_entry_views_handler_argument_language_pair extends views_handler_argument {
  function summary_name($data) {
    return $this->human_language($data->{$this->name_alias});
  }

  function title() {
    return $this->human_language($this->argument);
  }

  function human_language($value) {
    $languages = language_list();
    $pair = explode('-', $value);
    for ($i = 0; $i < count($pair); $i++) {
      $pair[$i] = isset($languages[$pair[$i]]) ? t($languages[$pair[$i]]->name) : '';
    }

    return implode('-', $pair);
  }
}

