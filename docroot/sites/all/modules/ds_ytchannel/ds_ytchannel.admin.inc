<?php
/**
 * 
 * @param type $form
 * @param type $form_state
 * @return string
 */
function ds_ytchannel_system_form($form, &$form_state) {  
  $form['ds_ytchannel_channel_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Channel username'),
      '#description' => t('The username in the url. es. https://www.youtube.com/user/<strong>MYUSERNAME</strong>'),
      '#default_value' => variable_get('ds_ytchannel_channel_name', ''),
      '#size' => 20,
      '#required' => TRUE
  );
  
  $form['ds_ytchannel_max_items'] = array(
      '#type' => 'textfield',
      '#title' => t('Max videos'),
      '#description' => t('Maximum number of videos in the block.'),
      '#default_value' => variable_get('ds_ytchannel_max_items', 10),
      '#size' => 5,
      '#required' => TRUE
  );
  
  $form['submit'] = array (
      '#type' => 'submit',
      '#value' => t('Save')
  );
  
  return $form;
}

/**
 * hook_FORM_save()
 * @param type $form
 * @param type $form_state
 */
function ds_ytchannel_system_form_submit($form, &$form_state) {
  variable_set('ds_ytchannel_channel_name', $form_state['values']['ds_ytchannel_channel_name']);
  variable_set('ds_ytchannel_max_items', $form_state['values']['ds_ytchannel_max_items']);
  
  drupal_set_message(t('YouTube channel name saved'));
}
