<?php
/**
 * @file
 * Panel style for the Willibald theme.
 */

/**
 * Implements hook_panels_styles().
 */
$plugin = array(
  'willibald_panelstyle' => array(
    'title' => t('Willibald panel style'),
    'description'   => t('Panel style for the Willibald theme.'),
    'render pane' => 'willibald_panelstyle_render_pane',
    'render region' => 'willibald_panelstyle_render_region',
    'pane settings form' => 'willibald_panelstyle_settings_form',
  ),
);

/**
 * Render callback.
 *
 * @ingroup themeable
 */
function theme_willibald_panelstyle_render_pane($variables) {
  $settings = $variables['settings'];
  $content = $variables['content'];
  $pane = $variables['pane'];

  $attributes = array();
  $attributes['class'][] = 'willibald-pane';
  empty($settings['background_color']) ? $attributes['class'][] = 'bg-orange' : $attributes['class'][] = $settings['background_color'];
  if (empty($settings['text_color'])) {
    $settings['text_color'] = 'tx-white';
  }
  $attributes['class'][] = $settings['text_color'];

  $path = drupal_get_path('module', 'willibald_panelstyle') . '/css/willibald-panelstyle';
  drupal_add_css($path . '.css');
  drupal_add_css($path . '.' . $settings['text_color'] . '.css');

  if (!empty($pane->css['css_class'])) {
    $attributes['class'][] = $pane->css['css_class'];
  }
  if (!empty($pane->css['css_id'])) {
    $attributes['id'][] = $pane->css['css_id'];
  }

  $output = '<section' . drupal_attributes($attributes) . '>';
  if (!empty($content->title)) {
    $output .= '  <h2>' .  $content->title . '</h2>';
  }
  $output .= '  <div class="content">' . render($content->content) . '</div>';
  $output .= '</section>';
  return $output;
}

/**
 * Render callback.
 *
 * @ingroup themeable
 */
function theme_willibald_panelstyle_render_region($variables) {
  return implode($variables['panes']);
}

/**
 * Form callback.
 */
function willibald_panelstyle_settings_form($settings) {
  $form['background_color'] = array(
    '#type' => 'select',
    '#title' => t('Background color'),
    '#options' => array(
      'bg-none' => t('No background color'),
      'bg-dark-blue' => t('dark blue'),
      'bg-light-blue' => t('light blue'),
      'bg-orange' => t('orange'),
      'bg-green' => t('green'),
    ),
    '#default_value' => (isset($settings['background_color'])) ? $settings['background_color'] : 'willibald-bg-orange',
  );

  $form['text_color'] = array(
    '#type' => 'select',
    '#title' => t('Text color'),
    '#options' => array(
      'tx-none' => t('No text color'),
      'tx-white' => t('white'),
      'tx-dark-blue' => t('dark blue'),
      'tx-light-blue' => t('light blue'),
      'tx-orange' => t('orange'),
      'tx-green' => t('green'),
    ),
    '#default_value' => (isset($settings['text_color'])) ? $settings['text_color'] : 'tx-white',
  );

  return $form;
}
