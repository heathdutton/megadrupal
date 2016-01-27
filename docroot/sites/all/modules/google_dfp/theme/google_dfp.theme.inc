<?php

/**
 * @file
 * Theme functions for the module.
 */

/**
 * Preprocess the ad block template.
 */
function google_dfp_preprocess_google_dfp_block(&$vars) {
  // Prepare vars for template.
  $vars['width'] = $vars['data']['width'] . 'px';
  $vars['height'] = $vars['data']['height'] . 'px';
  $vars['placement'] = $vars['data']['placement'];
  // @todo make this configurable.
  if ($vars['data']['placement'] == "skin_1" || $vars['data']['placement'] == "skin_2") {
    $vars['width'] = '100%';
  }

  // This is the ID used for our ad's container div.
  $vars['id'] = 'ga-publisher-' . $vars['placement'];

  // Variables for the Google Publisher (asynchronous) emebed method.
  $size = $vars['data']['width'] . ", " . $vars['data']['height'];
  $tier = $vars['data']['tier'];
  $screen = implode(array_keys(array_filter($vars['data']['screen'])));

  // Position setTargeting (if set).
  $pos_targeting = '';
  if ($vars['data']['pos'] != 0) {
    $pos_targeting = '.setTargeting("pos","' . $vars['data']['pos'] . '")';
  }

  // Keyword setTartgeting (if set).
  $keyword_targeting = '';
  if (!empty($vars['data']['keywords'])) {
    $keywords = array();
    foreach ($vars['data']['keywords'] as $keyword) {
      $keywords[] = '"' . $keyword . '"';
    }
    if (count($keywords) == 1) {
      $keyword_targeting = '.setTargeting("Keyword",' . reset($keywords) . ')';
    }
    else {
      $keywords = implode(',', $keywords);
      $keyword_targeting = '.setTargeting("Keyword",[' . $keywords . '])';
    }
  }

  // Add the define slot function in the head.
  if (!google_dfp_block_is_placed($vars['placement'])) {
    drupal_add_js('googletag.cmd.push(function() { window.' . $vars['placement'] . ' = googletag.defineSlot("' . $tier . '", [' . $size . '], "' . $vars['id'] . '").addService(googletag.pubads())' . $keyword_targeting . $pos_targeting . '; });',
      array(
        'type' => 'inline',
        'scope' => 'header',
        'weight' => 12,
      )
    );
  }

  // Build the setting array.
  $settings = array(
    'googleDfps' => array(
      $vars['id'] => array(
        'screenSize' => $screen,
        'placement' => $vars['placement'],
      ),
    ),
  );
  drupal_add_js($settings, 'setting');

}

/**
 * Returns HTML for a plugin order form.
 *
 * @param array $variables
 *   An associative array containing:
 *   - element: A render element representing the form.
 *
 * @return string
 *   HTML output
 * @ingroup themeable
 */
function theme_google_dfp_plugin_order($variables) {
  $element = $variables['element'];

  $rows = array();
  foreach (element_children($element, TRUE) as $name) {
    $element[$name]['weight']['#attributes']['class'][] = 'plugin-order-weight';
    $rows[] = array(
      'data' => array(
        drupal_render($element[$name]['title']),
        drupal_render($element[$name]['weight']),
      ),
      'class' => array('draggable'),
    );
  }
  $output = drupal_render_children($element);
  $id = drupal_html_id('google-dfp-plugin-order');
  $output .= theme('table', array('rows' => $rows, 'attributes' => array('id' => $id)));
  drupal_add_tabledrag($id, 'order', 'sibling', 'plugin-order-weight', NULL, NULL, TRUE);

  return $output;
}
