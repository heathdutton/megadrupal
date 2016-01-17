<?php

/**
 * @file
 * CSS-Crush administration pages.
 */

/**
 * Admin page settings form.
 */
function csscrush_settings($form, &$form_state) {

  $form['csscrush_source_map'] = array(
    '#type' => 'checkbox',
    '#title' => t('Generate source map'),
    '#description' => t(
      'Generate a source map file for use with compliant web inspectors.
       Works with <a href="!sourcemaps">Google Chrome</a>.',
      array(
        '!sourcemaps' => url('https://developers.google.com/chrome-developer-tools/docs/css-preprocessors'),
      )),
    '#default_value' => variable_get('csscrush_source_map', FALSE),
    '#suffix' => '<br/>',
  );

  $form['csscrush_trace'] = array(
    '#type' => 'checkbox',
    '#title' => t('FireSass debug info'),
    '#description' => t(
      'Generate debug statements for the <a href="!firesass">FireSass</a> firebug extension.',
      array(
        '!firesass' => url('https://addons.mozilla.org/en-US/firefox/addon/firesass-for-firebug'),
      )),
    '#default_value' => variable_get('csscrush_trace', FALSE),
    '#suffix' => '<br/>',
  );

  return system_settings_form($form);
}
