<?php

/**
 * @file
 * Provides helper functions for cultura_discussion_install().
 */

/**
 * Create the vocabulary for host and guest school names.
 */
function cultura_archive_create_school_vocabulary() {
  $description = st('All names of host and guest schools or universities.');
  $vocabulary = (object) array(
    'name' => st('Schools'),
    'description' => $description,
    'machine_name' => CULTURA_SCHOOLS,
  );
  taxonomy_vocabulary_save($vocabulary);

  // Populate the vocabulary with sample terms.
  $term = (object) array(
    'name' => st('MIT'),
    'vid' => $vocabulary->vid,
  );
  taxonomy_term_save($term);

  $term = (object) array(
    'name' => st('Brandeis'),
    'vid' => $vocabulary->vid,
  );
  taxonomy_term_save($term);

  $term = (object) array(
    'name' => st('Brown'),
    'vid' => $vocabulary->vid,
  );
  taxonomy_term_save($term);

  $term = (object) array(
    'name' => st('École Polytechnique'),
    'vid' => $vocabulary->vid,
  );
  taxonomy_term_save($term);

  $term = (object) array(
    'name' => st('Institut National des Télécommunications'),
    'vid' => $vocabulary->vid,
  );
  taxonomy_term_save($term);

  $term = (object) array(
    'name' => st('Paris II'),
    'vid' => $vocabulary->vid,
  );
  taxonomy_term_save($term);

  $term = (object) array(
    'name' => st('SUPAERO'),
    'vid' => $vocabulary->vid,
  );
  taxonomy_term_save($term);

  $term = (object) array(
    'name' => st('Université de Lille 3'),
    'vid' => $vocabulary->vid,
  );
  taxonomy_term_save($term);

  return $vocabulary;
}

/**
 * Create the vocabulary for host and guest school languages.
 */
function cultura_archive_create_language_vocabulary() {
  $description = st('All languages of host and guest schools or universities.');
  $vocabulary = (object) array(
    'name' => st('Languages'),
    'description' => $description,
    'machine_name' => CULTURA_LANGUAGES,
  );
  taxonomy_vocabulary_save($vocabulary);

  // Populate the vocabulary with sample terms.
  $term = (object) array(
    'name' => st('English'),
    'vid' => $vocabulary->vid,
  );
  taxonomy_term_save($term);

  $term = (object) array(
    'name' => st('French'),
    'vid' => $vocabulary->vid,
  );
  taxonomy_term_save($term);

  $term = (object) array(
    'name' => st('Spanish'),
    'vid' => $vocabulary->vid,
  );
  taxonomy_term_save($term);

  $term = (object) array(
    'name' => st('German'),
    'vid' => $vocabulary->vid,
  );
  taxonomy_term_save($term);

  $term = (object) array(
    'name' => st('Russian'),
    'vid' => $vocabulary->vid,
  );
  taxonomy_term_save($term);

  $term = (object) array(
    'name' => st('Mandarin'),
    'vid' => $vocabulary->vid,
  );
  taxonomy_term_save($term);

  $term = (object) array(
    'name' => st('Italian'),
    'vid' => $vocabulary->vid,
  );
  taxonomy_term_save($term);

  return $vocabulary;
}

/**
 * Create the Cultura tags vocabulary.
 */
function cultura_archive_create_year_vocabulary() {
  $description = st('Use numeric years for the academic year an Exchange took place.');
  $vocabulary = (object) array(
    'name' => st('Years'),
    'description' => $description,
    'machine_name' => CULTURA_YEAR,
  );
  taxonomy_vocabulary_save($vocabulary);

  $start = date('Y') - 18;
  for ($year = $start; $year < $start+24; $year++) {
    $term = (object) array(
      'name' => $year,
      'vid' => $vocabulary->vid,
    );
    taxonomy_term_save($term);
    $terms[] = $term->tid;
  }

  return $vocab_tags;
}

/**
 * Create the vocabulary for categorizing archived discussions by semester.
 */
function cultura_archive_create_semester_vocabulary() {
  $description = st('The semester of the academic year in which an Exchange took place.');
  $vocabulary = (object) array(
    'name' => st('Semesters'),
    'description' => $description,
    'machine_name' => CULTURA_SEMESTER,
  );
  taxonomy_vocabulary_save($vocabulary);

  // Populate the vocabulary with a sample term.
  $term = (object) array(
    'name' => st('Fall'),
    'vid' => $vocabulary->vid,
  );
  taxonomy_term_save($term);
  $term = (object) array(
    'name' => st('Spring'),
    'vid' => $vocabulary->vid,
  );
  taxonomy_term_save($term);

  return $vocabulary;
}
