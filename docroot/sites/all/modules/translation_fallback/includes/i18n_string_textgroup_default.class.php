<?php

/**
 * @file
 * Implementation of wrapper class for i18n_string_textgroup_default.
 *
 * @addtogroup translation_fallback_wrappers
 * @{
 */

/**
 * Wrapper of i18n_string_textgroup_default that enables translation fallback.
 */
class translation_fallback__i18n_string_textgroup_default extends i18n_string_textgroup_default {

  protected function string_translate($i18nstring, $options = array()) {
    $i18nstring = parent::string_translate($i18nstring, $options);
    $langcode = isset($options['langcode']) ? $options['langcode'] : i18n_langcode();
    if (empty($i18nstring->translations[$langcode])) {
      if ($t = translation_fallback_localize($i18nstring->string, $langcode)) {
        $i18nstring->translations[$langcode] = $t;
      }
    }
    return $i18nstring;
  }

  public function multiple_translate($context, $strings = array(), $options = array()) {
    $translations = parent::multiple_translate($context, $strings, $options);
    $langcode = isset($options['langcode']) ? $options['langcode'] : i18n_langcode();
    foreach ($translations as $i18nstring) {
      if (empty($i18nstring->translations[$langcode])) {
        if ($t = translation_fallback_localize($i18nstring->string, $langcode)) {
          $i18nstring->translations[$langcode] = $t;
        }
      }
    }
    return $translations;
  }
}

/**
 * @} End of "addtogroup translation_fallback_wrappers".
 */

