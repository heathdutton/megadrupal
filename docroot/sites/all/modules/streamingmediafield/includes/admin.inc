<?php

/**
 * @file
 * Admin pages
 */

/**
 * Administrative settings form
 */
function streamingmediafield_admin_settings_form() {
  $form = array();

  $form['rtmp'] = array('#type' => 'fieldset', '#tree' => FALSE, '#title' => t('RTMP Streaming'), '#description' => t('Used for Flash player streaming'));
  $form['rtmp']['streamingmediafield_live_rtmp_instance'] = array(
      '#type' => 'textfield',
      '#title' => t('Default Live RTMP Streamer Instance'),
      '#default_value' => variable_get('streamingmediafield_live_rtmp_instance', ''),
  );
  $form['rtmp']['streamingmediafield_ondemand_rtmp_instance'] = array(
      '#type' => 'textfield',
      '#title' => t('Default On-Demand RTMP Streamer Instance'),
      '#default_value' => variable_get('streamingmediafield_ondemand_rtmp_instance', ''),
  );

  $form['http'] = array('#type' => 'fieldset', '#tree' => FALSE, '#title' => t('HTTP Streaming'), '#description' => t('Used on iPhone, iPad and other iOS devices'));
  $form['http']['streamingmediafield_http_live_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Default HTTP Live URL'),
      '#default_value' => variable_get('streamingmediafield_http_live_url', ''),
  );
  $form['http']['streamingmediafield_http_ondemand_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Default HTTP On-Demand URL'),
      '#default_value' => variable_get('streamingmediafield_http_ondemand_url', ''),
  );

  $form['rtsp'] = array('#type' => 'fieldset', '#tree' => FALSE, '#title' => t('RTSP Streaming'), '#description' => t('Used on Android and Blackberry devices'));
  $form['rtsp']['streamingmediafield_rtsp_live_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Default RTSP Live URL'),
      '#default_value' => variable_get('streamingmediafield_rtsp_live_url', ''),
  );
  $form['rtsp']['streamingmediafield_rtsp_ondemand_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Default RTSP On-Demand URL'),
      '#default_value' => variable_get('streamingmediafield_rtsp_ondemand_url', ''),
  );

  $form['progressive'] = array('#type' => 'fieldset', '#tree' => FALSE, '#title' => t('Progressive Download'), '#description' => t('An alternative playback method for on-demand content'));
  $form['progressive']['streamingmediafield_progressive_download_server'] = array(
      '#type' => 'textfield',
      '#title' => t('Default Progressive Download Server'),
      '#default_value' => variable_get('streamingmediafield_progressive_download_server', ''),
      '#description' => t('Do not include the http://'),
  );

  return system_settings_form($form);
}
