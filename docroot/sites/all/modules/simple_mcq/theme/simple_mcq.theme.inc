<?php

/**
 * The theme file for multichoice.
 *
 * @file
 * Theming functions for the multichoice question type.
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_simple_mcq_creation_form($variables) {
}

/**
 * Theme the answer part of the node view
 *
 * @param $alternatives
 *  Array of alternatives. Each alternative is also an array with all the
 *  data for each alternative.
 * @param $show_correct
 *  True if the user is allowed to view the solution
 */
function theme_simple_mcq_answer_node_view($variables) {
  $alternatives = $variables['alternatives'];
  $show_correct = $variables['show_correct'];
  $header = array('', '');

  $p = drupal_get_path('module', 'simple_mcq');
  drupal_add_css($p .'/theme/simple_mcq.css');

  foreach ($alternatives as $i => $short) {
    $answer_markup = check_markup($short['answer'], $short['answer_format']);
    // Find the is_correct status
    $is_correct = $short['correct'];
    $image = $is_correct ? 'correct' : 'wrong';
    if (!$show_correct) {
      $image = 'unknown';
    }

    $rows[] = array(
        array(
          'data' => array(
            '#theme' => 'html_tag',
            '#tag' => 'span',
            '#attributes' => array(
              'class' => array(
                'multichoice-icon',
                $image,
              ),
              'title' => $show_correct ?
                '' :
                t('You are not allowed to view the solution for this question'),
            ),
          ),
          'class' => array('multichoice-icon-cell'),
        ),
        $answer_markup
      );
  }
  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * Theme function for the multichoice report
 *
 * @param $data
 *  Array of data to be presented in the report
 */
function theme_simple_mcq_response($variables) {
  static $css_added;
  if (!$css_added) {
    drupal_add_css(drupal_get_path('module', 'simple_mcq') . '/theme/simple_mcq.css');
    $css_added = TRUE;
  }
  $choice_multi = $variables['data']['choice_multi'];
  $rows = array();
  foreach ($variables['data']['answer'] as &$alternative) {
    if ($alternative['is_chosen'] && $alternative['is_correct'] && !$choice_multi) {
      $icon = '<span class="multichoice-icon correct" title="' . t('This option is correct.') . '"></span>';
    }
    elseif ($alternative['is_chosen'] && $alternative['is_correct'] && $choice_multi) {
      //print_r($variables['data']['answer']);exit;
      $icon = '<span class="multichoice-icon almost" title="' . t('This option is correct, but there could be more than one choice to be selected.') . '"></span>';
    }
    elseif (!$alternative['is_chosen'] && $alternative['is_correct']) {
      $icon = '<span class="multichoice-icon should" title="' . t("This option is correct, but you didn't select it.") . '"></span>';
    }
    else {
      $icon = '<span class="multichoice-icon wrong" title="' . t('This option is wrong.') . '"></span>';
    }
    $rows[] = array(
      array(
        'data' => $icon,
        'class' => 'selector-td multichoice-icon-cell',
      ),
      $alternative['answer']
    );
  }
  $feedback = '';
  if (!empty($variables['data']['feedback'])) {
    $feedback = '<div class="multichoice-label">' . t('Feedback') . ':</div>';
    $feedback .= '<div class="multichoice-feedback">' . $variables['data']['feedback'] . '</div>';
  }
  return theme('table', array('header' => NULL, 'rows' => $rows)) . $feedback;
}

/**
 * Theme function that returns the feedback for the current question.
 *
 * @param $data
 *  Array of data to be presented in the report
 */
function theme_feedback_simple_mcq_response($variables) {
  static $css_added;
  if (!$css_added) {
    drupal_add_css(drupal_get_path('module', 'simple_mcq') . '/theme/multichoice.css');
    $css_added = TRUE;
  }

  $rows = array();
  die("line 146");
  foreach ($variables['data'] as &$alternative) {
    if ($alternative['is_chosen']) {
      switch ($alternative['is_correct']) {
        case 0:
          $icon = '<span class="multichoice-icon wrong" title="' . t('This option is wrong.') . '"></span>';
          break;
        case 1:
          $icon = '<span class="multichoice-icon almost" title="' . t('This option is correct, but there is another that gives a higher score.') . '"></span>';
          break;
        case 2:
          $icon = '<span class="multichoice-icon correct" title="' . t('This option is correct.') . '"></span>';
          break;
      }
    }
    else {
      $icon = $alternative['is_correct'] == 2 ? '<span class="multichoice-icon should" title="' . t("This option is correct, but you didn't select it.") . '"></span>' : '';
    }
    $rowspan = $alternative['feedback'] ? 2 : 1;
    $rows[] = array(
      array(
        'data' => $icon,
        'rowspan' => $rowspan,
        'class' => 'selector-td multichoice-icon-cell',
      ),
      $alternative['answer']
    );
    if ($rowspan == 2) {
      $rows[] = array('<div class="multichoice-label">' . t('Feedback') . ':</div><div class="multichoice-feedback">' . $alternative['feedback'] . '</div>');
    }
  }
  return theme('table', array('header' => NULL, 'rows' => $rows));
}
