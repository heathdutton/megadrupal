<?php

/**
 * @file
 * Theme functions for quiz words.
 */
function theme_mark_word_user_answer($variables) {
  $header = array(t('Correct Answer'), t('User Answer'));
  $row = array(array($variables['correct'], $variables['answer']));
  $output = theme('table', array('header' => $header, 'rows' => $row));
  return $output;
}

/**
 * Theme the quiz words response form
 */
function theme_mark_word_response_form(&$variables) {
  $form = &$variables['form'];
  return drupal_render_children($form);
}

/**
 * Theme the quiz words answering form
 */
function theme_mark_word_answering_form(&$variables) {
  $form = &$variables['form'];
  unset($form['question']);
  drupal_add_css(drupal_get_path('module', 'mark_word') . '/theme/mark_wordmark_word.css');
  return drupal_render_children($form);
}

