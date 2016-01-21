<?php

use Drupal\quizz_text\LongAnswerResponse;

/**
 * Administration pages for the long answer questions module.
 * @file
 */

/**
 * Build a form for scoring long-answer questions.
 *
 * @param \Drupal\quizz_question\Entity\Question $question
 * @param $answer
 *  An object containing an answer to the question. This form is for scoring that answer.
 * @return
 *  The form (as a FAPI array).
 */
function quizz_text_long_answer_score_form($form, $form_state, $question, $answer) {
  if (!$question || $question->type != 'long_answer' || !$answer) {
    drupal_not_found();
    return;
  }
  // Set up the form
  $form['question'] = array(
      '#type'   => 'item',
      '#title'  => t('Question'),
      '#markup' => check_markup($question->quiz_question_body['und'][0]['value'], $question->quiz_question_body['und'][0]['format']),
  );
  $form['rubric'] = array(
      '#type'   => 'item',
      '#title'  => t('Rubric'),
      '#markup' => check_markup($question->rubric),
  );
  $form['show_max_score'] = array(
      '#type'   => 'item',
      '#title'  => t('Maximum Score'),
      '#markup' => (int) $answer->rel_max_score,
  );
  $form['score_answer'] = array(
      '#type'  => 'fieldset',
      '#title' => t('Score answer'),
  );
  $form['score_answer']['answer'] = array(
      '#type'   => 'item',
      '#title'  => t('Answer'),
      '#markup' => check_markup($answer->answer),
  );
  $form['score_answer']['score'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Score'),
      '#description'   => t('The score for this essay, between 0 and @max', array('@max' => $answer->rel_max_score)),
      '#size'          => 3,
      '#maxlength'     => 3,
      '#default_value' => (int) $answer->rel_score,
      '#required'      => TRUE,
  );
  $form['score_answer']['answer_feedback'] = array(
      '#title'         => t('Feedback'),
      '#type'          => 'text_format',
      '#description'   => t('The text entered here would be shown to attendees'),
      '#default_value' => $answer->answer_feedback,
      '#format'        => isset($answer->answer_feedback_format) ? $answer->answer_feedback_format : NULL
  );

  $form['score_answer']['submit'] = array(
      '#type'  => 'submit',
      '#value' => t('Save this score'),
  );

  // Save some work by keeping these.
  $form['max_score'] = array(
      '#type'  => 'value',
      '#value' => $question->max_score,
  );
  $form['rel_max_score'] = array(
      '#type'  => 'value',
      '#value' => $answer->rel_max_score,
  );
  $form['question_qid'] = array(
      '#type'  => 'value',
      '#value' => $answer->question_qid,
  );
  $form['question_vid'] = array(
      '#type'  => 'value',
      '#value' => $answer->question_vid,
  );
  $form['result_id'] = array(
      '#type'  => 'value',
      '#value' => $answer->result_id,
  );

  return $form;
}

/**
 * Validates the long answer score form
 */
function quizz_text_long_answer_score_form_validate($form, $form_state) {
  // Check to make sure that entered score is not higher than max allowed score.
  $max = (int) $form_state['values']['rel_max_score'];
  $given = $form_state['values']['score'];
  if (!quizz_valid_integer($given, 0, $max)) {
    $args = array('@score' => $given, '@max' => $max);
    form_set_error('score', t('The given score (@score) must be an integer between 0 and @max', $args));
  }
}

/**
 * Submit handler for the long answer score form
 */
function quizz_text_long_answer_score_form_submit($form, &$form_state) {
  $result = quizz_result_load($form_state['values']['result_id']);

  $return = quizz_text_long_answer_score_an_answer(array(
      'quiz'            => $result->getQuiz(),
      'qid'             => $form_state['values']['question_qid'],
      'vid'             => $form_state['values']['question_vid'],
      'result_id'       => $form_state['values']['result_id'],
      'score'           => $form_state['values']['score'],
      'max_score'       => $form_state['values']['rel_max_score'],
      'answer_feedback' => $form_state['values']['answer_feedback']
  ));

  if ($return == 1) {
    drupal_set_message(t('The score has been saved.'));
    $form_state['redirect'] = 'admin/quizz/reports/score-long-answer';
  }
  else {
    drupal_set_message(t('Error saving the score. The selected answer was not scored.'), 'error');
  }
}
