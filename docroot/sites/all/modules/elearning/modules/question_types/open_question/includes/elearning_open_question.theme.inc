<?php
/**
 * @file
 * Theme functions for the open question type.
 */

/**
 * Theme function for showing the given answer.
 */
function theme_elearning_question_given_answer_elearning_open_question($variables) {
  $correct_answer = $variables['correct_answer'];
  $given_answer = $variables['given_answer'];
  $question = $variables['question'];
  $correct = $variables['correct'];
  $settings = unserialize($question->settings);
  $class = $correct ? 'correct' : 'incorrect';
  $html_element = count($correct_answer) > 1 ? 'ol' : 'ul';

  $given_answer_markup = "<$html_element class='given-answer $class'>";

  foreach ($correct_answer as $key => $value) {
    $answer_value = trim($given_answer[$key]);
    $correct_value = trim($value['value']);
    $correct_child = elearning_open_question_check_per_option($settings, $answer_value, $correct_value);
    $class = $correct_child ? 'correct' : 'incorrect';

    $answer = !empty($given_answer[$key]) ? $given_answer[$key] : t("No answer given");
    $given_answer_markup .= "<li class='answer $class'>" . $answer . '</li>';
  }

  $given_answer_markup .= "</$html_element>";
  $markup = $given_answer_markup;
  return $markup;
}

/**
 * Theme function for showing the feedback for the answer.
 */
function theme_elearning_question_feedback_elearning_open_question($variables) {

  $given_answer = $variables['given_answer'];
  $correct_answer = $variables['correct_answer'];
  $question = $variables['question'];
  $correct = $variables['correct'];
  $settings = unserialize($question->settings);
  $class = $correct ? 'correct' : 'incorrect';
  $html_element = count($correct_answer) > 1 ? 'ol' : 'ul';

  $correct_answer_markup = "<$html_element class='correct-answer $class'>";

  foreach ($correct_answer as $key => $value) {
    $answer_value = trim($given_answer[$key]);
    $correct_value = trim($value['value']);
    $correct_child = elearning_open_question_check_per_option($settings, $answer_value, $correct_value);
    $class = $correct_child ? 'correct' : 'incorrect';

    if (!$settings['multiple_options']) {
      $correct_answer_markup .= "<li class='item $class'>" . $value['value'] . '</li>';
    }
    else {
      $options = preg_split('/\r\n|[\r\n]/', $value['value']);
      $options_markup = '<ul><li>' . implode('</li><li>', $options) . '</li></ul>';
      $correct_answer_markup .= "<li class='item $class'>" . t('One of the following: !options', array('!options' => $options_markup)) . '</li>';
    }
  }

  $correct_answer_markup .= "</$html_element>";

  if (!$correct || $settings['multiple_options']) {
    $markup = $correct_answer_markup;
    return $markup;
  }
}
