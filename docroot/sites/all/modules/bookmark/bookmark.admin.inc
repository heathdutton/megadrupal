<?php

function bookmark_settings_form($form, $form_state) {
  $form['nothing_here_yet'] = array(
    '#markup' => 'No settings yet. Move along.',
  );

  return system_settings_form($form);
}
