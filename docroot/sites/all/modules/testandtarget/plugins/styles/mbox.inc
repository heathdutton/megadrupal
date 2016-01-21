<?php

/**
 * @file
 * Definition of the 'mbox' panel style.
 */

// Plugin definition
$plugin = array(
  'title' => t('Test&Target mbox'),
  'description' => t('Presents the panes in a Test&Target Mbox.'),
  'render region' => 'testandtarget_mbox_style_render_region',
  'render pane' => 'testandtarget_mbox_style_render_pane',
  'settings form' => 'testandtarget_mbox_style_settings_form',
  'pane settings form' => 'testandtarget_mbox_style_settings_form',
);

/**
 * Render callback.
 *
 * @ingroup themeable
 */
function theme_testandtarget_mbox_style_render_region($vars) {
  $settings = $vars['settings'];

  $content = implode('<div class="panel-separator"></div>', $vars['panes']);
  return theme('testandtarget_mbox', array('mbox_name' => $settings['mbox_name'], 'default_content' => $content));
}

/**
 * Render callback.
 *
 * @ingroup themeable
 */
function theme_testandtarget_mbox_style_render_pane($vars) {
  $settings = $vars['settings'];

  $content = render($vars['content']->content);
  return theme('testandtarget_mbox', array('mbox_name' => $settings['mbox_name'], 'default_content' => $content));
}

/**
 * Settings form callback.
 */
function testandtarget_mbox_style_settings_form($style_settings) {
  $form['mbox_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Mbox name'),
    '#description' => t('Enter a name for your mbox. This will appear in the Test&Target web UI'),
    '#required' => TRUE,
    '#default_value' => (isset($style_settings['mbox_name'])) ? $style_settings['mbox_name'] : '',
  );

  return $form;
}
