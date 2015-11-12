<?php

/**
 * @file
 * Miscellaneous utility functions for dealing with taxonomy.
 */

class TaxonomyTools {
  /**
   * Prepares taxonomy term(s) for the specified vocabulary and adds new ones.
   *
   * @param string $legacy_term
   *   The legacy term(s) from the migration source comma delimited if multiple.
   *
   * @param string $vocabulary
   *   The machine name of the vocabulary to save the term to.
   *
   * @return string
   *   The term to be saved at the destination. Multiple terms are separated by
   *   "|" characters.
   */
  public function prepareVocabTerms($legacy_term = '', $vocabulary = '') {
    $new_terms = '';
    $vocabulary = trim($vocabulary);

    if ($legacy_term && $vocabulary) {
      $terms = explode(',', $legacy_term);

      foreach ($terms as $key => $term) {
        // If the term does not exist in $vocabulary, create the new term.
        // Clean the term.
        $term = trim($term);
        $term = html_entity_decode($term, ENT_QUOTES, 'UTF-8');
        // Replace multiple spaces with a space.
        $term = preg_replace('!\s+!', ' ', $term);
        $term = preg_replace('/Ã³/', 'ó', $term);
        $term = mb_convert_encoding($term, "UTF-8", mb_detect_encoding($term, "UTF-8, ISO-8859-1, ISO-8859-15", TRUE));

        if (!empty($term)) {
          $term = $this->standardizeTerm($term, $vocabulary);
          // Create the term if it does not exist yet.
          if ($this->createUniqueTerm($term, $vocabulary)) {
            $new_terms = implode('|', $terms);
          }
        }
      }
    }

    return $new_terms;
  }

  /**
   * Creates a new taxonomy term for a given vocabulary if it doesn't exist.
   *
   * @param string $term
   *   The human readable value for the term.
   * @param string $vocab_name
   *   The name of the vocabulary.
   *
   * @return bool
   *   TRUE if the term was created, FALSE if it was not (already existed).
   */
  private function createUniqueTerm($term, $vocab_name) {
    if (!taxonomy_get_term_by_name($term, $vocab_name)) {
      MigrationMessage::makeMessage('Creating new topic term "@term" in vocabulary: @vocabulary.', array('@term' => $term, '@vocabulary' => $vocab_name));
      $vocab = taxonomy_vocabulary_machine_name_load($vocab_name);
      $term_obj = new stdClass();
      $term_obj->name = $term;
      $term_obj->vid = $vocab->vid;
      taxonomy_term_save($term_obj);
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Standardizes terms in the component vocabulary by replacing legacy terms.
   *
   * To standardize your terms (example: convert 'USA' into 'U.S.A.'):
   * 1) Extend this class in your local migration module.
   * 2) Create a public array in the class named $replacements_{vocabulary_name}
   * 3) The array should be in the form array('bad' => 'good').
   *
   * @param string $term
   *   The term to standardize.
   * @param string $vocabulary
   *   The vocabulary to standardize against.
   *
   * @return string
   *   The taxonomy term (which might be different from the original term).
   */
  public function standardizeTerm($term, $vocabulary) {
    if (!empty($this->replacements_{$vocabulary}) && !empty($this->replacements_{$vocabulary}[$term])) {
      // A replacement exists, so use it instead of the original.
      $term = $this->replacements_{$vocabulary}[$term];
    }

    return $term;
  }
}
