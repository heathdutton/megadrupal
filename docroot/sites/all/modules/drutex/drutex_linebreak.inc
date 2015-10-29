<?php

/**
 * @file
 * Provide line-breaking algorithms for DruTeX.
 *
 * There is currently the choice between Drupal-style linebreaking and
 * LaTeX-style linebreaking.
 */

/**
 * Implements hook_info().
 */
function drutex_linebreak_info($filter) {
  return (object) array(
    'title' => t('Line break converter'),
    'description' => check_plain(t('Converts line breaks into HTML (i.e. <br /> and <p> tags).')),
    'toggle' => TRUE,
    'weight' => 12,
  );
}

/**
 * Implementation of subhook_filter_tips().
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function drutex_linebreak_filter_tips($delta, $filter, $long = FALSE) {
  return t('Line and paragraphs break automatically.');
}

/**
 * Implements hook_defaults().
 */
function drutex_linebreak_defaults() {
  $D['drutex_linebreak_active'] = TRUE;
  $D['behaviour'] = 'drupal';

  return $D;
}

/**
 * Implements hook_node2html_postprocess().
 */
function drutex_linebreak_node2html_postprocess($text, $filter) {
  $method = $filter->settings['drutex_linebreak']['behaviour'];

  if ($method == 'drupal') {
    $text = _filter_autop($text);
  }
  else if ($method == 'latex') {
    $text = preg_replace('/(?<!\n)\n(?!\n)/s', ' ', $text);
    $text = _filter_autop($text);
  }

  return $text;
}

/**
 * Implements hook_filter_settings().
 */
function drutex_linebreak_filter_settings($filter) {
  $choices = array(
    'drupal' => 'Drupal style',
    'latex' => 'LaTeX style',
  );

  $form["behaviour"] = array(
    '#type' => 'select',
    '#title' => t('Linebreaking Method'),
    '#options' => $choices,
    '#default_value' => $filter->settings['drutex_linebreak']['behaviour'],
    '#description' => t('How to handle new and empty lines in your input.'),
  );

  return $form;
}
