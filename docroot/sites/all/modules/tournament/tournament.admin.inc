<?php

/**
 * @file
 * Admin page callback file for the tournament module.
 */

/**
 * Form builder; Configure tournament settings for this site.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function tournament_admin_settings() {
  $form = array();

  $form['tournament_title_lock'] = array(
    '#type' => 'checkbox',
    '#title' => t('Node title cannot be changed after a tournament has been started'),
    '#default_value' => variable_get('tournament_title_lock', 0),
  );
  $form['tournament_user_picture'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show user pictures on user tournament participant lists'),
    '#default_value' => variable_get('tournament_user_picture', 0),
  );
  $form['tournament_participants_max'] = array(
    '#type' => 'textfield',
    '#title' => t('Participants list limit'),
    '#default_value' => variable_get('tournament_participants_max', 0),
    '#size' => 5,
    '#description' => t('For tournaments with a lot of participants, the <em>Tournament participants</em> block can get vertically long and can slightly increase page load time. Setting an initial limit prevents these issues. Set to 0 for no limit.')
  );
  

  return system_settings_form($form);
}
