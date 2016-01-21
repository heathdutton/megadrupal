<?php

/**
 * @file
 * Smartling log.
 */

namespace Drupal\smartling\Wrappers;

/**
 * Class DrupalAPIWrapper.
 */
class InternationalizationWrapper {
  public function i18nTaxonomyTermGetTranslation($term, $langcode) {
    return i18n_taxonomy_term_get_translation($term, $langcode);
  }

  public function i18nTaxonomyVocabularyMode($vid, $mode = NULL) {
    return i18n_taxonomy_vocabulary_mode($vid, $mode);
  }

  public function i18nLanguageObject($language) {
    return i18n_language_object($language);
  }

  public function i18nLanguageContext($language = NULL) {
    return i18n_language_context($language);
  }

  public function i18nTaxonomyTranslationSetLoad($tsid) {
    return i18n_taxonomy_translation_set_load($tsid);
  }

  public function i18nTranslationSetCreate($type, $bundle = '', $translations = NULL, $master_id = 0) {
    return i18n_translation_set_create($type, $bundle, $translations, $master_id);
  }
}
