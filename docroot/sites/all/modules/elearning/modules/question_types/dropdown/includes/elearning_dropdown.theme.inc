<?php
/**
 * @file
 * Theme functions for this question type.
 */

/**
 * Theme function for showing the given answer.
 */
function theme_elearning_question_given_answer_elearning_dropdown($variables) {

  $given_answer = $variables['given_answer'];
  $correct_answer = $variables['correct_answer'];
  $question = $variables['question'];
  $correct = $variables['correct'];
  $settings = unserialize($question->settings);
  $class = $correct ? 'correct' : 'incorrect';

  $given_answer_markup = '';

  $html_element = count($correct_answer) > 1 ? 'ol' : 'ul';

  $given_answer_markup .= "<$html_element class='given-answer $class'>";
  foreach ($given_answer as $index => $item) {
    $options = preg_split("/\\r\\n|\\r|\\n/", $correct_answer[$index]['key']);
    if (($given_answer[$index] - 1 >= 0) && $correct_answer[$index]['value'] == $options[$given_answer[$index] - 1]) {
      $class = "correct";
    }
    else {
      $class = "incorrect";
    }
    $answer = !empty($options[$item - 1]) ? $options[$item - 1] : t('No answer given');
    $answer .= '<span class="details-wrapper"><div class="details">';
    foreach ($options as $option) {
      $class_option = $option == $correct_answer[$index]['value'] ? 'correct' : 'incorrect';
      $answer .= "<div class='option $class_option'>" . $option . "</div>";
    }
    $answer .= '</div></span>';
    $given_answer_markup .= "<li class='answer $class'>" . $answer . '</li>';
  }

  $given_answer_markup .= "</$html_element>";

  $markup = $given_answer_markup;
  return $markup;
}

/**
 * Theme function for showing the feedback for the answer.
 */
function theme_elearning_question_feedback_elearning_dropdown($variables) {

  $given_answer = $variables['given_answer'];
  $correct_answer = $variables['correct_answer'];
  $question = $variables['question'];
  $correct = $variables['correct'];
  $settings = unserialize($question->settings);
  $class = $correct ? 'correct' : 'incorrect';

  $correct_answer_value = '';

  $html_element = count($correct_answer) > 1 ? 'ol' : 'ul';

  $correct_answer_value .= "<$html_element class='correct-answer $class'>";
  foreach ($correct_answer as $index => $item) {
    $options = preg_split("/\\r\\n|\\r|\\n/", $correct_answer[$index]['key']);

    if ($given_answer[$index] - 1 >= 0 && $options[$given_answer[$index] - 1] == $item['value']) {
      $class = "correct";
    }
    else {
      $class = "incorrect";
    }
    $correct_answer_value .= "<li class='option $class'>" . $item['value'] . "</li>";
  }

  $correct_answer_value .= "</$html_element>";

  if (!$correct) {
    $markup = $correct_answer_value;
    return $markup;
  }
}
