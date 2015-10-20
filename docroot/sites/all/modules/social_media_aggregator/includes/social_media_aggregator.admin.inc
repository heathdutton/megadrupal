<?php

/**
 * @file
 * Social media aggregator module administration screens.
 */

/**
 * Form: Administration page settings.
 */
function social_media_aggregator_settings($form_state) {
  $form = array();

  $form['social_media_aggregator_cache_length'] = array(
    '#type' => 'textfield',
    '#title' => t('Cache Length'),
    '#default_value' => variable_get('social_media_aggregator_cache_length', SOCIAL_MEDIA_AGGREGATOR_CACHE_LENGTH),
    '#description' => t('Standard length to keep social media atoms in cache in seconds'),
  );

  return system_settings_form($form);
}
