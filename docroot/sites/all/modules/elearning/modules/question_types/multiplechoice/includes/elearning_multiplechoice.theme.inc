<?php
/**
 * @file
 * Theme functions for this question type.
 */

/**
 * Implements theme_elearning_question_result_given_answer_HOOK().
 */
function theme_elearning_question_given_answer_elearning_multiplechoice($variables) {

  $given_answer = $variables['given_answer'];
  $correct_answer = $variables['correct_answer'];
  $question = $variables['question'];
  $correct = $variables['correct'];
  $settings = unserialize($question->settings);
  $class = $correct ? 'correct' : 'incorrect';
  $given_answer_markup = '';

  $given_answer_markup .= '<ul class="given-answer ' . $class . '">';

  if ($given_answer == "") {
    $given_answer_markup .= '<li>' . t('No answer given') . '</li>';
  }
  else {

    if ($settings['multiple_answers']) {
      if (empty($given_answer)) {
        $given_answer_markup .= '<li>' . t('No answer given') . '</li>';
      }
      else {
        foreach ($given_answer as $answer) {
          $answer = !empty($correct_answer[$answer]['key']) ? $correct_answer[$answer]['key'] : t('No answer given');
          $given_answer_markup .= '<li class="option">' . $answer;
          $given_answer_markup .= '<span class="details-wrapper"><div class="details">';
          foreach ($correct_answer as $index => $option) {
            $class = $option['value'] ? 'correct' : 'incorrect';
            $class .= $index == $given_answer ? ' answered' : '';
            $given_answer_markup .= '<div class="option ' . $class . '">' . $option['key'] . '</div>';
          }
          $given_answer_markup .= '</div></span>';
          $given_answer_markup .= '</li>';
        }
      }
    }
    else {
      $answer = !empty($correct_answer[$given_answer]) ? $correct_answer[$given_answer] : t('No answer given');

      $given_answer_markup .= '<li class="option">' . $correct_answer[$given_answer]['key'];
      $given_answer_markup .= '<span class="details-wrapper"><div class="details">';
      foreach ($correct_answer as $index => $option) {
        $class = $option['value'] ? 'correct' : 'incorrect';
        $class .= $index == $given_answer ? ' answered' : '';
        $given_answer_markup .= '<div class="option ' . $class . '">' . $option['key'] . '</div>';
      }
      $given_answer_markup .= '</div></span>';
      $given_answer_markup .= '</li>';
    }
  }
  $given_answer_markup .= '</ul>';

  $markup = $given_answer_markup;
  return $markup;
}

/**
 * Implements theme_elearning_question_result_feedback().
 */
function theme_elearning_question_feedback_elearning_multiplechoice($variables) {

  $given_answer = $variables['given_answer'];
  $correct_answer = $variables['correct_answer'];
  $question = $variables['question'];
  $correct = $variables['correct'];
  $settings = unserialize($question->settings);
  $class = $correct ? 'correct' : 'incorrect';

  $correct_answer_markup = "<ul class='correct-answer $class'>";
  $markup = '';

  foreach ($correct_answer as $index => $option) {

    $class .= $index == $given_answer ? ' answered' : '';
    $class = '';
    if ($option['value']) {
      $correct_answer_markup .= '<li>' . $option['key'] . '</li>';
    }
  }
  $correct_answer_markup .= '</ul>';

  if (!$correct || ($settings['multiple_answers'] && !$settings['all_correct'])) {
    $markup .= $correct_answer_markup;
    return $markup;
  }
}
