<?php

use Drupal\quizz_question\Entity\Question;
use Drupal\quizz_text\ShortAnswerResponse;

/**
 * Page handler for displaying a scoring form.
 * This function is called directly from the menu router. It generates a form for
 * scoring a quiz.
 *
 * @param int $question_vid
 * @param int $result_id
 * @return string
 */
function quizz_text_long_answer_edit_score($question_vid, $result_id) {
  $question = quizz_question_load(NULL, $question_vid);
  $result = quizz_result_load($result_id);
  $answer = $result->loadAnswerByQuestion($question);
  if (!$question || !$result || !$answer || (!$input = $answer->getInput())) {
    return drupal_not_found();
  }

  if ('long_answer' !== $question->getQuestionType()->handler) {
    throw new RuntimeException('Unexpected question type: ' . $question->getQuestionType()->handler);
  }

  $input->rel_score = 0;
  if ($question->max_score > 0) {
    $input->rel_score = round($input->score * $input->rel_max_score / $question->max_score);
  }

  drupal_set_title(t('Score answer to "@title"', array('@title' => $question->title)), PASS_THROUGH);
  return drupal_get_form('quizz_text_long_answer_score_form', $question, $input);
}

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

/**
 * Page handler for displaying a scoring form.
 * This function is called directly from the menu router. It generates a form for
 * scoring a quiz.
 *
 * @param int $question_vid
 * @param int $result_id
 * @return string
 */
function quizz_text_short_answer_edit_score($question_vid, $result_id) {
  if (!$question = quizz_question_load(NULL, $question_vid)) {
    return drupal_not_found();
  }

  if ('short_answer' !== $question->getQuestionType()->handler) {
    throw new RuntimeException('Unexpected question type: ' . $question->getQuestionType()->handler);
  }

  if (!$answer = (object) quizz_text_short_answer_get_answer($question->qid, $question->vid, $result_id)) {
    return drupal_not_found();
  }

  $answer->rel_score = 0;
  if ($question->max_score > 0) {
    $answer->rel_score = round($answer->score * $answer->rel_max_score / $question->max_score);
  }

  drupal_set_title(t('Score answer to "@title"', array('@title' => $question->title)), PASS_THROUGH);
  return drupal_get_form('quizz_text_short_answer_score_form', $question, $answer);
}

/**
 * Build a form for scoring long-answer questions.
 *
 * @param Question $question
 * @param $answer
 *  An object containing an answer to the question. This form is for scoring that answer.
 * @return
 *  The form (as a FAPI array).
 */
function quizz_text_short_answer_score_form($form, $context, $question, $answer) {
  if (!$question || $question->type != 'short_answer' || !$answer) {
    drupal_not_found();
    return;
  }
  // Set up the form
  $form['question'] = array(
      '#type'   => 'item',
      '#title'  => t('Question'),
      '#markup' => check_markup($question->quiz_question_body['und'][0]['value'], $question->quiz_question_body['und'][0]['format']),
  );
  $form['answer'] = array(
      '#type'   => 'item',
      '#title'  => t('Answer'),
      '#markup' => check_markup($answer->answer, variable_get('short_answer_markup_filter')),
  );
  $form['show_max_score'] = array(
      '#type'   => 'item',
      '#title'  => t('Maximum Score'),
      '#markup' => (int) $answer->rel_max_score,
  );
  $form['score'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Score'),
      '#description'   => t('The score for this essay, between 0 and @max', array('@max' => $answer->rel_max_score)),
      '#size'          => 3,
      '#maxlength'     => 3,
      '#default_value' => (int) $answer->rel_score,
      '#required'      => TRUE,
  );
  $form['answer_feedback'] = array(
      '#title'         => t('Feedback'),
      '#type'          => 'text_format',
      '#description'   => t('The text entered here would be shown to attendees'),
      '#default_value' => $answer->answer_feedback,
      '#format'        => isset($answer->answer_feedback_format) ? $answer->answer_feedback_format : NULL
  );
  $form['submit'] = array(
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
 * Validate the score form
 */
function quizz_text_short_answer_score_form_validate($form, $form_state) {
  // Check to make sure that entered score is not higher than max allowed score.
  $max = (int) $form_state['values']['rel_max_score'];
  $given = (int) $form_state['values']['score'];

  if (!quizz_valid_integer($form_state['values']['score'], 0, $max)) {
    $args = array('@score' => $given, '@max' => $max);
    form_error($form['score'], t('The score needs to be a number between 0 and @max', $args));
  }
}

/**
 * Submit the score form
 */
function quizz_text_short_answer_score_form_submit($form, &$form_state) {
  $result = quizz_result_load($form_state['values']['result_id']);

  $return = quizz_text_short_answer_score_an_answer($result, array(
      'quiz'            => $result->getQuiz(),
      'vid'             => $form_state['values']['question_vid'],
      'score'           => $form_state['values']['score'],
      'max_score'       => $form_state['values']['rel_max_score'],
      'answer_feedback' => $form_state['values']['answer_feedback']
  ));

  if ($return == 1) {
    drupal_set_message("The score has been saved.");
    $form_state['redirect'] = 'admin/quizz/reports/score-short-answer';
  }
  else {
    drupal_set_message(t('Error saving the score. The selected answer was not scored.'), 'error');
  }
}

/**
 * Generate a view of all unscored answer questions.
 * Callback for /admin/quizz/reports/score-long-answer.
 */
function quizz_text_long_answer_view_unscored() {
  return theme('quizz_text_long_answer_view_unscored', array(
      'unscored' => quiz_text_find_unscored_answers('quizz_long_answer')
  ));
}

/**
 * Generate a view of all unscored short_answer questions.
 * Callback for /admin/quizz/reports/score-short-answer
 */
function quizz_text_short_answer_view_unscored() {
  return theme('quizz_text_short_answer_view_unscored', array(
      'unscored' => quiz_text_find_unscored_answers('quizz_short_answer')
  ));
}

/**
 * Get all scores that have not yet been evaluated.
 *
 * @param int $length Number of items to return (default: 50).
 * @param int $offset
 * @return object[]
 */
function quiz_text_find_unscored_answers($base_table) {
  global $user;

  $query = db_select($base_table, 'properties');
  $query->innerJoin('quiz_answer_entity', 'answer', 'properties.answer_id = answer.id');
  $query->innerJoin('quiz_results', 'result', 'answer.result_id = result.result_id');
  $query->innerJoin('quiz_question_revision', 'question_revision', 'answer.question_vid = question_revision.vid');

  $query
    ->fields('properties', array('answer_feedback', 'answer_feedback_format'))
    ->fields('answer', array('result_id', 'question_qid', 'question_vid'))
    ->fields('question_revision', array('title'))
    ->fields('result', array('time_end', 'time_start', 'uid'))
    ->condition('properties.is_evaluated', 0);

  // Access filter
  user_access('score taken quiz answer') && $query->condition('result.uid', $user->uid);
  if (user_access('score own quiz')) {
    $query->innerJoin('quiz_entity', 'quiz', 'result.quiz_qid = quiz.qid');
    $query->condition('quiz.uid', $user->uid);
  }

  foreach ($query->execute() as $row) {
    $unscored[] = $row;
  }
  return !empty($unscored) ? $unscored : array();
}
