<?php
/**
 * @file
 * Page callbacks for 500px module.
 */
 
 

/**
 * 500px settings form.
 */
function d500px_admin_form($form, &$form_state) {

  $form['oauth'] = array(
    '#type' => 'fieldset',
    '#title' => t('OAuth Settings'),
    '#access' => module_exists('oauth_common'),
    '#description' => t('To enable OAuth based access for 500px, you must <a href="@url">register your application</a> with 500px and add the provided keys here.', array('@url' => 'http://developers.500px.com/settings/applications?from=developers')),
  );

  $form['oauth']['callback_url'] = array(
    '#type' => 'item',
    '#title' => t('Callback URL'),
    '#markup' => url('d500px/oauth', array('absolute' => TRUE)),
  );
  
  $form['oauth']['d500px_consumer_key'] = array(
    '#type' => 'textfield',
    '#title' => t('OAuth Consumer key'),
    '#default_value' => variable_get('d500px_consumer_key', NULL),
  );  

  $form['oauth']['d500px_consumer_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('OAuth Consumer secret'),
    '#default_value' => variable_get('d500px_consumer_secret', NULL),
  );

  
  // 500px external APIs settings.
  $form['d500px'] = array(
    '#type' => 'fieldset',
    '#title' => t('500px Settings'),
    '#description' => t('The following settings connect 500px module with external APIs.'),
  );

  $form['d500px']['d500px_host'] = array(
    '#type' => 'textfield',
    '#title' => t('500px Host'),
    '#default_value' => variable_get('d500px_host', D500PX_HOST),
  );  

  $form['d500px']['d500px_api'] = array(
    '#type' => 'textfield',
    '#title' => t('500px API'),
    '#default_value' => variable_get('d500px_api', D500PX_API),
  );
      
  return system_settings_form($form);
}
