<?php

/**
 * @file
 * Theme for word_link.
 */

/**
 * Render a word_link link.
 */
function theme_word_link($vars) {
  $attributes = drupal_attributes($vars['attributes']);
  return '<' . $vars['tag'] . $attributes . '>' . check_plain($vars['text']) . '</' . $vars['tag'] . '>';
}
