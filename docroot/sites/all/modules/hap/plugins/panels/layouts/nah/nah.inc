<?php

/**
 * @file
 * Provides a single wrapping div for attributes.. Or nothing at all.
 */

$plugin = array(
  'title' => t('Nah..'),
  'category' => t('Highly Attractive'),
  'icon' => 'nah.png',
  'settings form' => 'hap_nah_layout_settings_form',
  'theme' => 'hap_nah_panels_layout',
  'regions' => array('content' => t('Content')),
  'default' => array(
    'nowrapper' => 0,
  ),
);

/**
 * Implements template_preprocess_HOOK().
 */
function template_preprocess_hap_nah_panels_layout(&$vars) {
  $vars['attributes']['class'][] = 'hap-nah-panels-layout';

  // Add the CSS ID if applicable, all things considered.
  if (!$vars['settings']['nowrapper'] && isset($vars['display']->css_id) && $vars['display']->css_id) {
    $vars['attributes']['id'] = $display->css_id;
  }
}

/**
 * Settings form callback.
 */
function hap_nah_layout_settings_form($display, $layout, $conf) {
  $form['nowrapper'] = array(
    '#type' => 'checkbox',
    '#title' => t("Display the contents of this layout WITHOUT ANY wrapping markup!"),
    '#description' => t('Check this box if you want your content to just display without any wrapping divs. Note that any configured CSS attributes (like CSS ID) will be ignored.'),
    '#default_value' => $conf['nowrapper'],
  );

  return $form;
}

/**
 * Theme callback.
 */
function theme_hap_nah_panels_layout($vars) {
  if ($vars['settings']['nowrapper']) {
    return $vars['content']['content'];
  }
  else {
    return '<div' . drupal_attributes($vars['attributes']) . '>' . $vars['content']['content'] . '</div>';
  }
}
