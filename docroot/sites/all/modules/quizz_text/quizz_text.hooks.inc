<?php

use Drupal\quizz_question\Entity\QuestionType;

/**
 * Implements hook_permission().
 */
function quizz_text_permission() {
  return array(
      'use regex for short answer' => array(
          'title'           => t('use regex for short answer'),
          'description'     => t('Use PHP "regular expressions" the advanced option for automated response evaluation.'),
          'restrict access' => TRUE,
      ),
  );
}

/**
 * Implements hook_quizz_question_info().
 */
function quizz_text_quizz_question_info() {
  return array(
      'short_answer' => array(
          'name'              => t('Short answer question'),
          'description'       => t('Quiz questions that allow a user to enter a line of text.'),
          'question provider' => 'Drupal\quizz_text\ShortAnswerQuestion',
          'response provider' => 'Drupal\quizz_text\ShortAnswerResponse',
          'module'            => 'quizz_question', // All wrapper functions are in that module.
      ),
      'long_answer'  => array(
          'name'              => t('Long answer question'),
          'description'       => t('Quiz questions that allow a user to enter multiple paragraphs of text.'),
          'question provider' => 'Drupal\quizz_text\LongAnswerQuestion',
          'response provider' => 'Drupal\quizz_text\LongAnswerResponse',
          'module'            => 'quizz_question', // All wrapper functions are in that module.
      ),
  );
}

/**
 * Implements hook_theme().
 */
function quizz_text_theme() {
  return array(
      'quizz_text_long_answer_view_unscored' => array(
          'variables' => array('unscored' => array()),
          'path'      => drupal_get_path('module', 'quizz_text') . '/theme',
          'file'      => 'quizz_text.theme.inc',
      ),
      'quizz_text_long_answer_response_form' => array(
          'render element' => 'form',
          'path'           => drupal_get_path('module', 'quizz_text') . '/theme',
          'file'           => 'quizz_text.theme.inc',
      ),
      'long_answer_answering_form'           => array(
          'render element' => 'form',
          'path'           => drupal_get_path('module', 'quizz_text') . '/theme',
          'template'       => 'long-answer-answering-form',
      ),
  );
}

/**
 * Implements hook_menu().
 */
function quizz_text_menu() {
  $items['admin/quizz/reports/score-short-answer'] = array(
      'title'            => 'Score short answer questions',
      'description'      => 'Score the answers from quizzes that use short answer questions.',
      'page callback'    => 'quizz_text_short_answer_view_unscored',
      'access arguments' => array('score any quiz', 'score own quiz', 'score taken quiz answer'),
      'access callback'  => 'quizz_access_multi_or',
      'type'             => MENU_NORMAL_ITEM,
      'file'             => 'quizz_text.admin.inc',
  );

  // Pass vid and rid to this path.
  $items['admin/quizz/reports/score-short-answer/%/%'] = array(
      'title'            => 'Score short answer response',
      'description'      => 'Score a response to a short answer question.',
      'access arguments' => array(4, 5),
      'access callback'  => 'quizz_question_access_to_score',
      'page callback'    => 'quizz_text_short_answer_edit_score',
      'page arguments'   => array(4, 5),
      'type'             => MENU_NORMAL_ITEM,
      'file'             => 'quizz_text.admin.inc'
  );

  $items['admin/quizz/reports/score-long-answer'] = array(
      'title'            => 'Score long answer questions',
      'description'      => 'Score the answers from quizzes that use long answer questions.',
      'page callback'    => 'quizz_text_long_answer_view_unscored',
      'access arguments' => array('score any quiz', 'score own quiz', 'score taken quiz answer'),
      'access callback'  => 'quizz_access_multi_or',
      'type'             => MENU_NORMAL_ITEM,
      'file'             => 'quizz_text.admin.inc',
  );

  // Pass vid and rid to this path.
  $items['admin/quizz/reports/score-long-answer/%/%'] = array(
      'title'            => 'Score long answer response',
      'description'      => 'Score a response to a long answer question.',
      'access arguments' => array(4, 5),
      'access callback'  => 'quizz_question_access_to_score',
      'page callback'    => 'quizz_text_long_answer_edit_score',
      'page arguments'   => array(4, 5),
      'type'             => MENU_NORMAL_ITEM,
      'file'             => 'quizz_text.admin.inc'
  );

  return $items;
}

/**
 * Implements hook_theme().
 */
function quizz_text_short_answer_theme($existing, $type, $theme, $path) {
  $module_path = drupal_get_path('module', 'quizz_text');
  return array(
      'quizz_text_short_answer_view_unscored' => array(
          'variables' => array('unscored' => array()),
          'path'      => $module_path . '/theme',
          'file'      => 'quizz_text.theme.inc',
      ),
      'quizz_text_short_answer_response_form' => array(
          'render element' => 'form',
          'path'           => $module_path . '/theme',
          'file'           => 'quizz_text.theme.inc',
      ),
      'quizz_text_short_answer_user_answer'   => array(
          'variables' => array('answer' => NULL, 'correct' => NULL),
          'path'      => $module_path . '/theme',
          'file'      => 'quizz_text.theme.inc',
      ),
      'short_answer_answering_form'           => array(
          'render element' => 'form',
          'path'           => $module_path . '/theme',
          'template'       => 'short-answer-answering-form',
      ),
  );
}
