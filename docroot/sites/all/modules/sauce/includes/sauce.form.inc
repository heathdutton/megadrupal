<?php

/**
 * @file
 * Handles the form elements of the customer survey form
 * All hooks are in the .module file.
 */

/**
 * Master form which calls an individual form for each step.
 * 
 * @see sauce_form_validate().
 * @see sauce_form_submit().
 * 
 * @param type $form
 * @param string $form_state
 * @return type 
 */
function sauce_form($form, &$form_state) {
     
  drupal_add_css(drupal_get_path('module', 'sauce') . '/css/sauce_styles.css');
  if (!isset($form_state['stage'])) $form_state['stage'] = 'sauce_install';
 
  $form = array();
  //$form = sauce_get_header($form, $form_state);

  switch ($form_state['stage']) {
    
    case 'sauce_install':
      return sauce_install_form($form, $form_state);
     break;  
 
    case 'sauce_configure': 
      return sauce_configure_form($form, $form_state);  
     break;
 
    default:
      return sauce_install_form($form, $form_state);
     break; 
  }
  
  return $form;
    
}


/**
 * Form for the rate_the_room step.
 * 
 * @see sauce_form().
 *  
 * @param type $form
 * @param type $form_state
 * @return type 
 */
function sauce_install_form($form, &$form_state) {
    
  $values = isset($form_state['multistep_values']['sauce_install']) ? $form_state['multistep_values']['sauce_install'] : array();
/*
  $form['rate_the_room']['room_rating'] = array(
    '#type' => 'radios',
    '#title' => 'How would you rate the room you stayed in?',
    '#options' => array(1 => 1, 2 => 2, 3 => 3, 4 => 4, 5 => 5),
    '#default_value' => isset($values['room_rating']) ? $values['room_rating'] : NULL,
  ); */
  $sauce_markup = '<script>(function ($) {
      function receiveMessage(event)
      {
        if (event.origin !== "http://drupalsauce.technikh.com")
          return;
        var addon_nid = event.data;
      //  alert("addon_nid: "+addon_nid);
      if(addon_nid.indexOf("{") != -1){
				// { exists
      //invalid addon_nid
			}else{

      $("#edit-sauce-installed-item-id").val(addon_nid);
        $("#edit-next").click();
      }
   /*     $.post("/sauce/import/"+addon_nid, function(data) {
        alert("data recieved: "+data);
        $("#edit-sauce-installed-item-id").val(data);
       // $("#edit-submit").click();
      });
    */
      }
      window.addEventListener("message", receiveMessage, false);
    }(jQuery));</script>';
  global $base_url;
  $sauce_markup .= '<iframe src="http://drupalsauce.technikh.com?site_id='.$base_url.'" width="100%" height="500px"></iframe>';
  $form['sauce_iframe'] = array(
      '#markup' => $sauce_markup,
  );
  $form['sauce_installed_item_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Package ID'),
      '#default_value' => '',
  );
  $form['sauce_config_markup_hidden'] = array(
      '#type' => 'hidden',
      '#default_value' => 'sauce_config_markup_hidden',
  );
  $form['next'] = array(
    '#type' => 'submit',
    '#value' => t('Install')
  );
    
  return $form;
}


/**
 * Form for the rate_the_service step.
 * 
 * @see sauce_form().
 *  
 * @param type $form
 * @param type $form_state
 * @return type 
 */
function sauce_rate_the_service_form($form, &$form_state) {
    
  $values = isset($form_state['multistep_values']['rate_the_service']) ? $form_state['multistep_values']['rate_the_service'] : array();

  $form['rate_the_service']['service_rating'] = array(
    '#type' => 'radios',
    '#title' => 'How would you rate our service?',
    '#options' => array(1 => 1, 2 => 2, 3 => 3, 4 => 4, 5 => 5),
    '#default_value' => isset($values['service_rating']) ? $values['service_rating'] : NULL,
  );
  
  $form['back'] = array(
    '#type' => 'submit',
    '#value' => t('Back')
  );
  
  $form['next'] = array(
    '#type' => 'submit',
    '#value' => t('Next')
  );
    
  return $form;
}


/**
 * Form for the personal_details step.
 * 
 * @see sauce_form().
 *  
 * @param type $form
 * @param type $form_state
 * @return type 
 */
function sauce_configure_form($form, &$form_state) {
//dsm($form_state);
  $values = isset($form_state['multistep_values']['sauce_configure']) ? $form_state['multistep_values']['sauce_configure'] : array();
  $form['section_config'] = array(
      '#type' => 'fieldset',
      '#title' => t('Next steps'),
   /*   '#weight' => 2, */
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
  );
  $sauce_config_markup = $form_state['values']['sauce_config_markup_hidden'];
  $form['section_config']['sauce_config_markup'] = array(
      '#markup' => $sauce_config_markup,
  );
/*
  $form['personal_details']['name'] = array(
    '#type' => 'textfield',
    '#title' => 'Name',
    '#default_value' => isset($values['name']) ? $values['name'] : NULL,
  );
  
  $form['personal_details']['email'] = array(
    '#type' => 'textfield',
    '#title' => 'Email',
    '#default_value' => isset($values['email']) ? $values['email'] : NULL,
  );
  */
 /*
  $form['back'] = array(
    '#type' => 'submit',
    '#value' => t('Back')
  );
  */
  $form['next'] = array(
    '#type' => 'submit',
    '#value' => t('Finish')
  );
  
  return $form;
}
