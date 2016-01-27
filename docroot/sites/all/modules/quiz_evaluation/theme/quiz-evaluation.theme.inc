<?php

/**
 * @file
 * Theming functions for the evaluation question type.
 */

/**
 *
 * @param $args
 *    An associative array containing:
 *    - form: The form.
 *      - #options: An associative array containing:
 *        - title: The title of the group of questions.
 *        - quiz_evaluation_range: An array of the range objects for each
 *                                 answer. Each object contains the value_id.
 *        - quiz_evaluation_answers: An array of answers for this group. Each
 *                                   answer contains:
 *          - question_id:
 *
 * @return
 *    The rendered form table element.
 */
function theme_quiz_evaluation_answering($args) {
  $form = $args['form'];

  $range = $form['#options']['quiz_evaluation_range'];
  $answers = $form['#options']['quiz_evaluation_answers'];

  $header = array(
    array(
      'data' => '',
      'width' => 450
    )
  );

  foreach ($range as $item) {
    $header[] = $item->value;
  }

  $rows = array();

  foreach ($answers as $answer) {
    $row = array();
    $row[] = drupal_render($form['title_' . $answer['question_id']]);
    foreach ($range as $item) {
      $row[] = drupal_render($form['answer_' . $answer['question_id'] . '_' . $item->value_id]);
    }
    $rows[] = $row;
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}