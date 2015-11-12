<?php
function addtocopy_settings_form() {
  $form = array();
  
  $form['addtocopy_selector'] = array(
    '#type' => 'textfield',
    '#title' => t('jQuery selector'),
    '#default_value' => variable_get('addtocopy_selector', '#content'),
    '#description' => t('jQuery selector expression to apply Add to Copy.'),
  );
  $form['addtocopy_minlen'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum text length'),
    '#default_value' => variable_get('addtocopy_minlen', 25),
    '#description' => t('Minimum selected text length to activate Add to Copy.'),
  );
  $form['addtocopy_htmlcopytxt'] = array(
    '#type' => 'textfield',
    '#title' => t('HTML to add to selected text'),
    '#default_value' => variable_get('addtocopy_htmlcopytxt', '<br>More: <a href="[link]">[link]</a><br>'),
    '#description' => t('[link] will be replaced with the current page link.'),
  );
  $form['addtocopy_addcopyfirst'] = array(
    '#type' => 'radios',
    '#title' => t('Add before or after selected text'),
    '#options' => array(0 => t('After'), 1 => t('Before')),
    '#default_value' => variable_get('addtocopy_addcopyfirst', 0),
  );
  
  return system_settings_form($form);
}
