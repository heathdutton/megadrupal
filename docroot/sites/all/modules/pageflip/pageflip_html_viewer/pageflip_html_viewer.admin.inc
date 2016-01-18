<?php

/**
 * @file
 * PageFlip HTML viewer module administrative interface.
 */


/**
 * Build our admin settings form.
 */
function pageflip_html_viewer_settings_form($form_state) {
  $form = array();

  $form['pageflip_html_viewer_mobile_help_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Help message for mobile device users'),
    '#description' => t('Message to show to mobile device users entering the HTML viewer'),
    '#default_value' => variable_get('pageflip_html_viewer_mobile_help_message', ''),
  );
  $form['pageflip_html_viewer_share_widget'] = array(
    '#type' => 'textarea',
    '#title' => t('Share Widget Markup'),
    '#description' => t('Raw markup for a share widget (such as ShareThis) to be used within the player'),
    '#default_value' => variable_get('pageflip_html_viewer_share_widget', ''),
  );

  return system_settings_form($form);
}

