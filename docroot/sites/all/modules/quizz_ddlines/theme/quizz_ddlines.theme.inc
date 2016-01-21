<?php

/**
 * The theme file for quizz ddlines.
 */

/**
 * Theming function for the results
 *
 * @param array $vars
 */
function theme_quizz_ddlines_response($vars) {
  if (!$css_added = &drupal_static(__FUNCTION__)) {
    drupal_add_css(drupal_get_path('module', 'quizz_ddlines') . '/theme/quizz_ddlines.css');
    $css_added = TRUE;
  }

  $rows = array();
  $css_class = array('no-answer', 'wrong', 'correct');
  $titles = array(
      t('You did not move this alternative to any hotspot'),
      t('Wrong answer'),
      t('Correct answer')
  );
  foreach ($vars['result'] as $title => $status) {
    $rows[] = array(
        array(
            'data'  => '<span'
            . ' class="quiz-ddlines-icon ' . $css_class[$status] . '"'
            . ' title="' . $titles[$status] . '"></span>',
            'class' => 'selector-td quiz-ddlines-icon-cell',
        ),
        $title,
    );
  }

  return theme('table', array('header' => NULL, 'rows' => $rows));
}
