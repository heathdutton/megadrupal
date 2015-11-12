<?php

/**
 * @file
 * Provides helper functions for cultura_questionnaire_install().
 */

/**
 * Create the Questionnaire builder content type.
 */
function cultura_questionnaire_samples_en_fr_situation_reaction() {
  return (object) array(
    'status' => NODE_NOT_PUBLISHED,
    'type' => CULTURA_QUESTIONNAIRE_BUILDER_NODE_TYPE,
    'language' => LANGUAGE_NONE,
    'cultura_question_type' => array(
      'und' => array(
        array(
          'tid' => variable_get(CULTURA_QUESTIONNAIRE_SITUATION_REACTION, 3),
        ),
      ),
    ),
    'cultura_questionnaire_intros' => array(
      'und' => array(
        array(
          'first' => 'Please respond to the following hypothetical situations, saying how you would react. State the first thing you would think, feel or do.',
          'second' => 'Voici un certain nombre de situations hypothétiques . Dites comment vous réagiriez. Indiquez la première chose que vous penseriez, diriez ou feriez.',
        ),
      ),
    ),
    'cultura_questionnaire_prompts' => array(
      'und' => array(
        array(
          'first' => 'You are in a restaurant with your best friend. S/he is constantly on his/her iPhone.',
          'second' => 'Vous êtes dans un restaurant avec votre meilleur/e ami/e. Il/elle passe son temps sur son iPhone.',
        ),
        array(
          'first' => 'You have been waiting in line for ten minutes. Someone cuts the line in front of you.',
          'second' => "Vous faites la queue depuis dix minutes. Quelqu'un passe juste devant vous dans la file d'attente.",
        ),
        array(
          'first' => 'You come upon two of your friends who are arguing.',
          'second' => 'Vous surprenez deux de vos amis en train de se disputer.',
        ),
        array(
          'first' => 'You see a mother in a supermarket slap her child.',
          'second' => 'Vous voyez une mère dans un supermarché donner une gifle à son enfant.',
        ),
        array(
          'first' => 'You see a student next to you cheating on an exam.',
          'second' => "Vous voyez un étudiant à côté de vous qui triche lors d'un examen.",
        ),
        array(
          'first' => 'You are on a long bus ride. The person sitting next to you starts talking and asks you friendly but personal questions about your life (where you are from, what your parents do, what are you pastimes...).',
          'second' => "Vous êtes dans le bus pour un long trajet. La personne à côté de vous commence à vous parler et vous pose des questions amicales mais personnelles sur votre vie (d'où vous venez, ce que font vos parents, quels sont vos passe-temps...).",
        ),
        array(
          'first' => 'You are at a party and you see one of your friends getting very drunk.',
          'second' => 'Vous êtes à une soirée et vous voyez un/e de vos amis/es qui boit beaucoup trop.',
        ),
        array(
          'first' => 'You are lost in a foreign city. You ask for directions to someone walking by. S/he just walks away. ',
          'second' => "Vous êtes perdu dans une ville à l'étranger. Vous demandez votre chemin à quelqu'un. Il passe sans s'arrêter.",
        ),
      ),
    ),
  );
}
