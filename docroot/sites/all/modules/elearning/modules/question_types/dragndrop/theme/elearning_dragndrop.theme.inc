<?php
/**
 * @file
 * Theme functions for this question type.
 */

/**
 * Theme function for showing the given answer.
 */
function theme_elearning_question_given_answer_elearning_dragndrop($variables) {

  $given_answer = $variables['given_answer'];
  $correct_answer = $variables['correct_answer'];
  $question = $variables['question'];
  $correct = $variables['correct'];
  $settings = unserialize($question->settings);
  $class = $correct ? 'correct' : 'incorrect';

  $given_answer_markup = "<ol class='given-answer $class'>";
  foreach ($given_answer as $position) {
    $answer = !empty($correct_answer[$position]['key']) ? $correct_answer[$position]['key'] : t('No answer given');
    $given_answer_markup .= '<li class="item">' . $answer . '</li>';
  }
  $given_answer_markup .= "</ol>";

  return $given_answer_markup;
}

/**
 * Theme function for showing the feedback for the answer.
 */
function theme_elearning_question_feedback_elearning_dragndrop($variables) {

  $given_answer = $variables['given_answer'];
  $correct_answer = $variables['correct_answer'];
  $question = $variables['question'];
  $correct = $variables['correct'];
  $settings = unserialize($question->settings);
  $class = $correct ? 'correct' : 'incorrect';

  $correct_answer_markup = "<ol class='correct-answer $class'>";

  $items = array();

  foreach ($correct_answer as $correct_answer_item) {
    if (!empty($correct_answer_item['value'])) {
      $items[$correct_answer_item['value']] = $correct_answer_item['key'];
    }
  }
  ksort($items);
  foreach ($items as $answer_value) {
    $correct_answer_markup .= '<li class="item">' . $answer_value . '</li>';
  }

  $correct_answer_markup .= "</ol>";
  $correct_answer_markup .= "<ul class='unused'>";
  foreach ($correct_answer as $correct_answer_item) {
    if (empty($correct_answer_item['value'])) {
      $correct_answer_markup .= '<li class="item incorrect">' . $correct_answer_item['key'] . '</li>';
    }
  }
  $correct_answer_markup .= "</ul>";

  return $correct_answer_markup;
}
