<?php

/**
 * @file
 * Handler code.
 */

/**
 * Handler for field target_language.
 */
class SmartlingHandlerFieldTargetLanguage extends views_handler_field {

  /**
   * Render.
   *
   * @param object $values
   *   Smartling entity.
   *
   * @return string
   *   Return target locale.
   */
  public function render($values) {
    $hm_languages = array();
    $languages = $this->get_value($values);
    if (strpos($languages, ',') !== FALSE) {
      $languages = explode(',', $languages);
    }
    else {
      $languages = array($languages);
    }

    foreach ($languages as $language) {
      $lang_name = smartling_get_full_language_name($language);

      if (is_null($lang_name)) {
        $hm_languages[] = t('Undefined name');
      }
      else {
        $hm_languages[] = $lang_name;
      }
    }


    return implode(', ', $hm_languages);
  }

}
