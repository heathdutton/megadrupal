<?php

/**
 * @file
 * Theme functions for this question type.
 */

/**
 * Theme function for showing the given answer.
 */
function theme_elearning_question_given_answer_elearning_order_question($variables) {

  $correct_answer = $variables['correct_answer'];

  $given_answer = $variables['given_answer'];
  $correct_answer = $variables['correct_answer'];
  $question = $variables['question'];
  $correct = $variables['correct'];
  $settings = unserialize($question->settings);
  $class = $correct ? 'correct' : 'incorrect';

  $given_answer_markup = '';

  $given_answer_markup .= '<div class="given-answer ' . $class . '">';

  if ($given_answer[0] == "") {
    $given_answer_markup .= '<div>' . t('No answer given') . '</div>';
  }
  else {
    foreach ($given_answer as $answer) {
      $given_answer_markup .= '<span class="order-item">' . $correct_answer[$answer]['value'] . '</span>';
    }
    $given_answer_markup .= '</div>';
  }
  $markup = $given_answer_markup;
  return $markup;
}

/**
 * Theme function for showing the feedback for the answer.
 */
function theme_elearning_question_feedback_elearning_order_question($variables) {

  $given_answer = $variables['given_answer'];
  $correct_answer = $variables['correct_answer'];
  $question = $variables['question'];
  $correct = $variables['correct'];
  $settings = unserialize($question->settings);
  $class = $correct ? 'correct' : 'incorrect';

  $correct_answer_markup = '';

  $correct_answer_markup .= '<div class="correct-answer ' . $class . '">';

  foreach ($correct_answer as $key => $answer) {
    $correct_answer_markup .= '<span class="order-item">' . $correct_answer[$key]['value'] . '</span>';
  }
  $correct_answer_markup .= '</div>';

  if (!$correct) {
    $markup = $correct_answer_markup;
    return $markup;
  }
}
