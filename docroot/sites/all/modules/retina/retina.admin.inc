<?php

function retina_settings_form($form, &$form_state) {
  $form['cdn'] = array(
  '#type' => 'fieldset', 
  '#title' => t('CDN Settings'), 
  '#weight' => 0, 
  '#collapsible' => FALSE, 
  '#collapsed' => FALSE,
  );
	$form['cdn']['use'] = array(
	  '#type' => 'checkbox', 
	  '#title' => t('Use CDN'), 
	  '#default_value' => variable_get('retina_use_cdn',0),
	  '#description' => t('If you check this, RetinaJS will be loaded from the cloud. It is faster, but adds one more request to every page. If you uncheck this, it will be loaded from your server. So you may aggregate the JavaScript.'),
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return system_settings_form($form);
}

/*
 * Submit handler for import settings form.
 */
function retina_settings_form_submit($form, &$form_state) {
  variable_set('retina_use_cdn', $form_state['values']['cdn']['use']);

  // Clear all caches
  drupal_flush_all_caches();
}