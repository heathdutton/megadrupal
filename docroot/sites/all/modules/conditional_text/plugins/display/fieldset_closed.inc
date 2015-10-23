<?php

/**
 * @file
 * Fieldset closed display plugin.
 */

$plugin = array(
  'title' => t('Fieldset'),
  'description' => t('User can show/hide text, closed by default'),
  'theme' => 'conditional_text_fieldset_closed',
  'js' => array(
    drupal_get_path('module', 'conditional_text') .
      '/plugins/display/fieldset/fieldset.js',
    'misc/form.js',
    'misc/collapse.js',
  ),
  'css' => array(
    drupal_get_path('module', 'conditional_text') .
      '/plugins/display/fieldset/fieldset.css',
  ),
);
