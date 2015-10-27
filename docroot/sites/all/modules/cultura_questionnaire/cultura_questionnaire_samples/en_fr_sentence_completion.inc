<?php

/**
 * @file
 * Provides helper functions for cultura_questionnaire_install().
 */

/**
 * Create the Questionnaire builder content type.
 */
function cultura_questionnaire_samples_en_fr_sentence_completion() {
  return (object) array(
    'status' => NODE_NOT_PUBLISHED,
    'type' => CULTURA_QUESTIONNAIRE_BUILDER_NODE_TYPE,
    'language' => LANGUAGE_NONE,
    'cultura_question_type' => array(
      'und' => array(
        array(
          'tid' => variable_get(CULTURA_QUESTIONNAIRE_SENTENCE_COMPLETION, 2),
        ),
      ),
    ),
    'cultura_questionnaire_intros' => array(
      'und' => array(
        array(
          'first' => 'Please complete the following sentences. You are encouraged to write two or three examples for each sentence. If you do so, separate them with commas.',
          'second' => "Veuillez compléter les phrases suivantes. Vous pouvez donner deux ou trois exemples pour chaque phrase si sous le désirez, mais n'oubliez pas de les séparer par des virgules.",
        ),
      ),
    ),
    'cultura_questionnaire_prompts' => array(
      'und' => array(
        array(
          'first' => 'Someone cool is someone who ...',
          'second' => "Quelqu'un de cool est quelqu'un qui ...",
        ),
        array(
          'first' => 'The best things parents can do for their children ...',
          'second' => 'Les meilleures choses que les parents puissent faire pour leurs enfants ...',
        ),
        array(
          'first' => 'What is most important to me right now ...',
          'second' => 'Ce qui est le plus important pour moi en ce moment ...',
        ),
        array(
          'first' => 'A rude person is someone who ...',
          'second' => "Une personne impolie est quelqu'un qui ...",
        ),
        array(
          'first' => 'A good teacher is someone ...',
          'second' => "Un bon prof. est quelqu'un ...",
        ),
        array(
          'first' => 'The greatest problems my country faces ...',
          'second' => 'Les plus grands problèmes auxquels est confronté  mon pays ...',
        ),
        array(
          'first' => 'A good job is a job...',
          'second' => 'Un bon travail est un travail...',
        ),
        array(
          'first' => "I can't stand when people ...",
          'second' => 'Je ne supporte pas quand les gens ...',
        ),
        array(
          'first' => 'What most people in my generation lack ...',
          'second' => 'Ce qui manque à la plupart des gens de ma génération ...',
        ),
        array(
          'first' => 'I wish I had more time to ...',
          'second' => "J'aimerais avoir plus de temps pour ...",
        ),
      ),
    ),
  );
}
