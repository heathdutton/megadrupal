<?php

/**
 * @file
 * Administrator-only menu callbacks and page-specific functions for the Ooyala
 * Channels module.
 */

/**
 * hook_node_alter() helper function to alter the node type form to show
 * the settings we expose on a per-content-type basis.
 *
 * @param &$form
 *   The form being altered.
 * @param &$form_state
 *   The current state of the form being altered.
 * @param $form_id
 *   The ID of the form being altered.
 * @param $node_type
 *   The machine name of the content type who's settings are being displayed.
 * @param $node_type_name
 *   The human-readable name of the content type.
 */
function ooyala_channels_alter_node_type_form(&$form, &$form_state, $form_id, $node_type, $node_type_name) {
  $form['ooyala_channels'] = array(
    '#type' => 'fieldset',
    '#title' => t('Ooyala channels settings'),
    '#collapsed' => TRUE,
    '#collapsible' => TRUE,
    '#group' => 'additional_settings',
  );

  if (!variable_get('ooyala_global_pcode', '')) {
    $form['ooyala_channels']['ooyala_configure_help'] = array(
      '#prefix' => '<p>',
      '#value' => t('Before you can create or manage Ooyala channels, you need to <a href="@ooyala-settings">configure your site for syncronizing with Ooyala</a>.', array('@ooyala-settings' => url('admin/settings/ooyala'))),
      '#suffix' => '</p>',
    );
    return;
  }

  $form['ooyala_channels']['ooyala_channels_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t("Associate Ooyala channels with the %type content type", array('%type' => $node_type_name)),
    '#default_value' => variable_get('ooyala_channels_enabled_' . $node_type, FALSE),
    '#description' => t('If the body field is enabled for this content type, it will be used as the description field for the channel. As the Ooyala player will not parse HTML, it is highly recommended to create a "Plain text" input format that uses the HTML filter to escape all HTML tags.'),
  );

  $form['ooyala_channels']['ooyala_channels_player_view'] = array(
    '#type' => 'select',
    '#title' => t('Start channel with'),
    '#options' => array(
      'backlot' => t('The setting assigned in the Ooyala backlot'),
      'normal' => t('Preview image'),
      'channel' => t('Channel selector'),
    ),
    '#default_value' => variable_get('ooyala_channels_player_view_' . $node_type, 'backlot'),
    '#description' => t('An Ooyala Channel can be configured to show a preview image or a channel selector when first displayed. This setting can be used to override the settings in the Ooyala backlot and force a specific player display.'),
  );

  $form['ooyala_channels']['ooyala_channels_player_transition'] = array(
    '#type' => 'select',
    '#title' => t('Channel transition'),
    '#options' => array(
      'backlot' => t('The setting assigned in the Ooyala backlot'),
      'play' => t('Play next video'),
      'selector' => t('Show channel selector'),
    ),
    '#default_value' => variable_get('ooyala_channels_player_view_' . $node_type, 'backlot'),
    '#description' => t('An Ooyala Channel can be configured to play the next video or show the channel selector when a video has finished playing. This setting can be used to override the settings in the Ooyala backlot and force a specific channel transition to be used.')
  );

}

