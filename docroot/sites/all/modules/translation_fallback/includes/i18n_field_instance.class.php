<?php

/**
 * @file
 * Implementation of wrapper class for i18n_field_instance.
 *
 * @addtogroup translation_fallback_wrappers
 * @{
 */

/**
 * Wrapper of i18n_field_instance that enables translation fallback.
 */
class translation_fallback__i18n_field_instance extends i18n_field_instance {
  protected function translate_object($langcode, $options) {
    translation_fallback_translate_object_strings($this, $langcode);
    return parent::translate_object($langcode, $options);
  }
}

/**
 * @} End of "addtogroup translation_fallback_wrappers".
 */

