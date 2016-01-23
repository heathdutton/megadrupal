<?php

/**
 * @file
 * Provides helper functions for cultura_questionnaire_install().
 */

/**
 * Create the Questionnaire builder content type.
 */
function cultura_questionnaire_samples_en_fr_word_association() {
  return (object) array(
    'status' => NODE_NOT_PUBLISHED,
    'type' => CULTURA_QUESTIONNAIRE_BUILDER_NODE_TYPE,
    'language' => LANGUAGE_NONE,
    'cultura_question_type' => array(
      'und' => array(
        array(
          'tid' => variable_get(CULTURA_QUESTIONNAIRE_WORD_ASSOCIATION, 1),
        ),
      ),
    ),
    'cultura_questionnaire_intros' => array(
      'und' => array(
        array(
          'first' => 'What other words do you associate with the following ones? For each word, write the first two or three words that come to your mind (they can be nouns, verbs, adjectives, etc.). Separate the words by commas. Answer the three questionnaires in one session.',
          'second' => "Quels autres mots associez-vous aux mots suivants? Ecrivez les deux ou trois premiers mots qui vous viennent à l'esprit (noms, adjectifs, verbes, adverbes, etc.) Séparez les mots par des virgules. Répondez aux trois questionnaires pendant la même session.",
        ),
      ),
    ),
    'cultura_questionnaire_prompts' => array(
      'und' => array(
        array(
          'first' => 'France',
          'second' => 'France',
        ),
        array(
          'first' => 'competition',
          'second' => 'compétition',
        ),
        array(
          'first' => 'United States',
          'second' => 'Etats-Unis',
        ),
        array(
          'first' => 'money',
          'second' => 'argent',
        ),
        array(
          'first' => 'family',
          'second' => 'famille',
        ),
        array(
          'first' => 'happiness',
          'second' => 'bonheur',
        ),
        array(
          'first' => 'failure',
          'second' => 'échec',
        ),
        array(
          'first' => 'vacation time',
          'second' => 'vacances',
        ),
        array(
          'first' => 'success',
          'second' => 'réussite',
        ),
        array(
          'first' => 'death penalty',
          'second' => 'peine de mort',
        ),
        array(
          'first' => 'individualism',
          'second' => 'individualisme',
        ),
        array(
          'first' => 'work',
          'second' => 'travail',
        ),
      ),
    ),
  );
}
