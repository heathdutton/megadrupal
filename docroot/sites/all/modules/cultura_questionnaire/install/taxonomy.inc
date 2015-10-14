<?php

/**
 * @file
 * Provides helper functions for cultura_questionnaire_install().
 */

/**
 * Create the vocabulary for languages.
 *
 * Even though there will be only two languages for a Cultura Exchange, we do
 * the language as taxonomy for consistency with other key attributes for
 * questionnaires and answers and the content structure for the archive site.
 */
function cultura_questionnaire_create_language_vocabulary() {
  $description = st('The language categories.');
  $vocabulary = (object) array(
    'name' => st('Languages'),
    'description' => $description,
    'machine_name' => CULTURA_QUESTIONNAIRE_LANGUAGE_VOCABULARY,
  );
  taxonomy_vocabulary_save($vocabulary);
  return $vocabulary;
}
