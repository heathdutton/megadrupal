<?php

/**
 * @file
 * Provides helper functions for cultura_discussion_install().
 */

/**
 * Create the vocabulary for categorizing Question prompts (and answers).
 */
function cultura_discussion_create_question_types_vocabulary() {
  $description = st('Categorize question and answer sets by type of prompt.');
  $vocabulary = (object) array(
    'name' => st('Question types'),
    'description' => $description,
    'machine_name' => CULTURA_QUESTIONNAIRE_VOCABULARY,
  );
  taxonomy_vocabulary_save($vocabulary);

  // Populate the vocabulary.
  $names = array(
    st('Word Association'),
    st('Sentence Completion'),
    st('Situation Reaction'),
    st('Other Discussion Topics'),
  );

  $weight = -20;
  foreach ($names as $name) {
    $term = (object) array(
      'name' => $name,
      'vid' => $vocabulary->vid,
      'weight' => $weight,
    );
    taxonomy_term_save($term);
    $weight += 10;
    $terms[] = $term->tid;
  }

  // Save term IDs for the question types to variables for later retrieval.
  variable_set(CULTURA_QUESTIONNAIRE_WORD_ASSOCIATION, $terms[0]);
  variable_set(CULTURA_QUESTIONNAIRE_SENTENCE_COMPLETION, $terms[1]);
  variable_set(CULTURA_QUESTIONNAIRE_SITUATION_REACTION, $terms[2]);
  variable_set(CULTURA_QUESTIONNAIRE_OTHER_DISCUSSION, $terms[3]);

  return $vocabulary;
}

/**
 * Create the Cultura tags vocabulary.
 */
function cultura_discussion_create_tags_vocabulary() {
  $description = st('Use tags to group posts on similar topics into categories.');
  $vocab_tags = (object) array(
    'name' => st('Tags'),
    'description' => $description,
    'machine_name' => CULTURA_TAGS,
  );
  taxonomy_vocabulary_save($vocab_tags);
  return $vocab_tags;
}