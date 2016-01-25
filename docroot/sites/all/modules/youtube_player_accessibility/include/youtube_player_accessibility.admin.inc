<?php
/**
 *
 * @file : youtube_player_accessibility.admin.inc
 * 
 * This file contains administration functions/functionality 
 * implementation.
 * 
 */
function youtube_player_accessibility_page() {
  return drupal_get_form('youtube_player_accessibility_form');
}

/**
 * jquery twitter search block creation form.
 * 
 * @param $delta
 *   The id of the block.
 * 
 * @return the form array structre.
 */
function youtube_player_accessibility_form($delta = NULL) {

  $form = array();

  $form['requirements_txt'] = array(
    '#type' => 'textarea',
    '#title' => t('Requirment text'),
    '#description' => t('The text that will be displied if there is some missing requiremnt.'),
    '#default_value' => variable_get('requirements_txt' , t("You need Flash player 8+ and JavaScript enabled to view this video.")) ,
  );

  $form['header_txt'] = array(
    '#type' => 'textfield',
    '#title' => t('Controlls heading text'),
    '#description' => t('Player controlls heading text.'),
    '#default_value' => variable_get('header_txt', t("Player Controls")),
  );

  $form['play_txt'] = array(
    '#type' => 'textfield',
    '#title' => t('Play button text'),
    '#description' => t('Play button text'),
    '#default_value' => variable_get('play_txt', t("Play")),
  );

  $form['forward_txt'] = array(
    '#type' => 'textfield',
    '#title' => t('Forward button text'),
    '#description' => t('Forward button text'),
    '#default_value' => variable_get('forward_txt', t("Forward 20%")),
  );

  $form['back_txt'] = array(
    '#type' => 'textfield',
    '#title' => t('Back button text'),
    '#description' => t('Back button text'),
    '#default_value' => variable_get('back_txt', t("Back 20%")),
  );

  $form['stop_txt'] = array(
    '#type' => 'textfield',
    '#title' => t('Stop button text'),
    '#description' => t('Stop button text'),
    '#default_value' => variable_get('stop_txt', t("Stop")),
  );

  $form['vol_up_txt'] = array(
    '#type' => 'textfield',
    '#title' => t('Volume up button text'),
    '#description' => t('Volume up button text'),
    '#default_value' => variable_get('vol_up_txt', t("Volume Up")),
  );

  $form['vol_dn_txt'] = array(
    '#type' => 'textfield',
    '#title' => t('Volume down text'),
    '#description' => t('Volume down text'),
    '#default_value' => variable_get('vol_dn_txt', t("Volume Down")),
  );

  $form['mute_txt'] = array(
    '#type' => 'textfield',
    '#title' => t('Mute button text'),
    '#description' => t('Mute button text'),
    '#default_value' => variable_get('mute_txt', t("Mute")),
  );

  $form['loop_txt'] = array(
    '#type' => 'textfield',
    '#title' => t('Loop button text'),
    '#description' => t('Loop button text'),
    '#default_value' => variable_get('loop_txt', t("Loop")),
  );

  $form['currently_playing_txt'] = array(
    '#type' => 'textfield',
    '#title' => t('Currently playing button text'),
    '#description' => t('Currently playing button text'),
    '#default_value' => variable_get('currently_playing_txt', t("Currently Playing:")),
  );

  $form['time_txt'] = array(
    '#type' => 'textfield',
    '#title' => t('Time status bar'),
    '#description' => t('Loop button text'),
    '#default_value' => variable_get('time_txt', t("Time:")),
  );

  $form['unmute_txt'] = array(
    '#type' => 'textfield',
    '#title' => t('Unmute button text'),
    '#description' => t('UnMute button text'),
    '#default_value' => variable_get('unmute_txt', t("Unmute")),
  );

  $form['pause_txt'] = array(
    '#type' => 'textfield',
    '#title' => t('Pause button text'),
    '#description' => t('Pause button text'),
    '#default_value' => variable_get('pause_txt', t("Pause")),
  );

  $form['unloop_txt'] = array(
    '#type' => 'textfield',
    '#title' => t('Unloop button text'),
    '#description' => t('Unloop button text'),
    '#default_value' => variable_get('unloop_txt', t("UnLoop")),
  );

  $form['video_play_list_txt'] = array(
    '#type' => 'textfield',
    '#title' => t('Video play list button text'),
    '#description' => t('Video play list button text'),
    '#default_value' => variable_get('video_play_list_txt', t("Video Play List")),
  );

  $form['of_txt'] = array(
    '#type' => 'textfield',
    '#title' => t('Off button text'),
    '#description' => t('Off button text'),
    '#default_value' => variable_get('of_txt', t("of")),
  );

  return system_settings_form($form);
}