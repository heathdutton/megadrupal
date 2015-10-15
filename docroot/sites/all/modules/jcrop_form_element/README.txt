-------------------------------------------------------------------------------
Jcrop Form Element for Drupal 7.x
	by ADCI, LLC team - http://www.adcisolutions.com/
-------------------------------------------------------------------------------
Dscription:
This module provides an integration of popular jquery library Jcrop with the Drupal FAPI. 
Basically it provides a new form element which can be used in your custom forms. This is 
an API module mostly intended for developers.
Webform Jcrop Form Element - it's a submodule which provides an integration with
webform module. 

Usage:
Can be used in any custom forms or for altering existing forms. New form element 'jcrop_image'  
is similar to 'managed_file' and can be used instead of them, but for images of course. For 
example you can create a form with a webform module and add File component. On hook_form_alter 
you can simply change the type of the file field to 'jcrop_image'. Also 'jcrop_image' form 
element has #jcrop_settings array that allows to control the Jcrop API object on the client side.

At the request of users, here is a code of a example module which demonstrates 
how to use Jcrop Form Element module:

/**
 * Implements hook_menu().
 */
function jcrop_fapi_example_menu() {
  $menu['fapi_example_form'] = array(
    'title' => 'My Custom Form',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('jcrop_fapi_example_form'),
    'access arguments' => array(TRUE),
  );
	
  return $menu;
}

/*
 * Implements hook_form().
 */
function jcrop_fapi_example_form($form, &$form_state) {
	$form = array();

  $form['contact'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Contact info'), 
    '#weight' => 5, 
    '#collapsible' => TRUE, 
    '#collapsed' => FALSE,
    '#weight' => -10,
  );
  
  $form['contact']['name'] = array(
    '#type' => 'textfield',
    '#size' => 22,
    '#title' => t('Your Name'),
    '#weight' => 0,
  );
  
  $settings = array();
  if (($info = element_info('jcrop_image')) && isset($info['#process'])) {
    $settings = $info['#jcrop_settings'];
    $settings['resolution'] = '50x50';
    $settings['enforce_ratio'] = 1;
    $settings['enforce_minimum'] = '30x30';
    $settings['croparea'] = '500x500';
    $settings['cropped_img_width'] = FALSE;  
    $settings['cropbox_title'] = t('Ho-ho-ho!');      
    $settings['cropbox_description'] = t('Bottom text...');    
}
  
	$form['contact']['jcrop_image'] = array(
	  '#type' => 'jcrop_image',
		'#title' => 'Your Photo',
    '#progress_message' => 'Please, Wait!',
    '#description' => "Uploaded image can be cropped.",
    '#jcrop_settings' => $settings,
	);
  
	$form['jcrop_image_1'] = array(
	  '#type' => 'jcrop_image',
		'#title' => 'Default Jcrop',
	);
  
	$form['managed_file'] = array(
	  '#type' => 'managed_file',
		'#title' => 'Managed file',
	);
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#weight' => 20,
  );
	
	return $form;
}

/*
 * Submit hanlder.
 */
function jcrop_fapi_example_form_submit($form, &$form_state) {
  dsm($form_state['values']);
}

/*
 * Implements hook_form_alter().
 * 
 * Example of how to alter forms. You can make #managed_file elements to use jcrop.
 * You just need to override the type of the element.
 */
function jcrop_fapi_example_form_webform_client_form_3_alter(&$form, &$form_state) {
  // It was $form['submitted']['image']['#type'] = 'managed_file';
  $form['submitted']['image']['#type'] = 'jcrop_image';
}

 - quite clear. 

Usage of Webform Jcrop Form Element:
You just need to create new webform instance, add webform component of type file. 
Use checkbox to enable jcrop and you'll 
see jcrop settings list.


Installation:
Download and install the Libraries API 2 module and the Jcrop Form Element module as a normal module.
Then download the Jcrop plugin into libraries directory sites/all/libraries/jcrop.

Dependences: 
Libraries API 2, File, Image.