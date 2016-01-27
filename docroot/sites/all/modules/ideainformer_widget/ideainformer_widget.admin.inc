<?php
function ideainformer_widget_admin_form($form, &$form_state) {
  $form['ideainformer'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Configuration for Idea Informer widget'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['ideainformer']['ideainformer_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Domain'),
    '#required' => true,
    '#default_value' => variable_get('ideainformer_domain', ''),
  );
  $form['ideainformer']['ideainformer_short_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Short title'),
    '#required' => true,
    '#default_value' => variable_get('ideainformer_short_title'),
  ); 
  $form['ideainformer']['ideainformer_long_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Long title'),
    '#required' => true,
    '#default_value' => variable_get('ideainformer_long_title'),
  ); 
  // saves and executes a submission callback, see $form["#submit"] for that
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#executes_submit_callback' => true
  );
  $form['#submit'] = array(
    'ideainformer_admin_form_submit'
  );
  return $form;
}

function ideainformer_widget_admin_validate($form, &$form_state) {
  /*
  $list_id = $form_state['values']['intraface_newsletter_list_id'];
  if (!is_numeric($list_id)) {
    form_set_error('intraface_newsletter_list_id', t('Intraface newsletter list ID must be numeric.'));
  }
  */
}

function ideainformer_widget_admin_form_submit($form, &$form_state) {
  variable_set('ideainformer_domain', $form_state['values']['ideainformer_domain']);
  variable_set('ideainformer_long_title', $form_state['values']['ideainformer_long_title']);
  variable_set('ideainformer_short_title', $form_state['values']['ideainformer_short_title']);  
  drupal_set_message(t("Settings for Idea Informer widget has been set"));
}
