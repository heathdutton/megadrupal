<?php
/**
 * Implements hook_form_FORM_ID_alter().
 *
 * @param $form
 *   The form.
 * @param $form_state
 *   The form state.
 */

function photo_form_system_theme_settings_alter(&$form, &$form_state) {

  $form['mtt_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('photo Theme Settings'),
    '#collapsible' => FALSE,
	'#collapsed' => FALSE,
  );
  $form['mtt_settings']['slideshow_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Slide show photo Settings'),
    '#collapsible' => TRUE,
	'#collapsed' => FALSE,
  );

  $form['mtt_settings']['slideshow_settings']['tabs'] = array(
    '#type' => 'vertical_tabs',
  );
  
   $form['mtt_settings']['slideshow_settings']['tabs']['bg_setting'] = array(
    '#type' => 'fieldset',
    '#title' => t('Background photo setting'),
    '#collapsible' => TRUE,
	'#collapsed' => TRUE,
  );
   $form['mtt_settings']['slideshow_settings']['tabs']['first_photo'] = array(
    '#type' => 'fieldset',
    '#title' => t('First Photo Setting'),
    '#collapsible' => TRUE,
	'#collapsed' => FALSE,
  );
   $form['mtt_settings']['slideshow_settings']['tabs']['second_photo'] = array(
    '#type' => 'fieldset',
    '#title' => t('Second Photo Setting'),
    '#collapsible' => TRUE,
	'#collapsed' => FALSE,
  );
   $form['mtt_settings']['slideshow_settings']['tabs']['Third_photo'] = array(
    '#type' => 'fieldset',
    '#title' => t('Third Photo Setting'),
    '#collapsible' => TRUE,
	'#collapsed' => FALSE,
  );  
  
   $form['mtt_settings']['slideshow_settings']['tabs']['bg_setting']['photo_bg'] = array(
    '#type' => 'textfield',
    '#title' => t('Background photo.'),
  	'#description'   => t('This photo is not part of slide-show. It will be display immediately after theme/site will be appear into browser.'),
	'#default_value' => theme_get_setting('photo_bg', 'photo'),
  );
   $form['mtt_settings']['slideshow_settings']['tabs']['first_photo']['photo_1'] = array(
    '#type' => 'textfield',
    '#title' => t('First photo.'),
  	'#description'   => t('First photo of slide-show.'),
	'#default_value' => theme_get_setting('photo_1', 'photo'),
  );
   $form['mtt_settings']['slideshow_settings']['tabs']['first_photo']['attribute_photo_1'] = array(
    '#type' => 'textfield',
    '#title' => t('First photo attribute.'),
  	'#description'   => t('Photographer credit for first photo'),
	'#default_value' => theme_get_setting('attribute_photo_1', 'photo'),
  );
  
  $form['mtt_settings']['slideshow_settings']['tabs']['second_photo']['photo_2'] = array(
    '#type' => 'textfield',
    '#title' => t('Second photo.'),
  	'#description'   => t('Second photo of slide-show.'),
	'#default_value' => theme_get_setting('photo_2', 'photo'),
  );
   $form['mtt_settings']['slideshow_settings']['tabs']['second_photo']['attribute_photo_2'] = array(
    '#type' => 'textfield',
    '#title' => t('Second photo attribute.'),
  	'#description'   => t('Photographer credit for second photo'),
	'#default_value' => theme_get_setting('attribute_photo_2', 'photo'),
  ); 
  
  $form['mtt_settings']['slideshow_settings']['tabs']['Third_photo']['photo_3'] = array(
    '#type' => 'textfield',
    '#title' => t('Third photo.'),
  	'#description'   => t('Third photo of slide-show.'),
	'#default_value' => theme_get_setting('photo_3', 'photo'),
  );
   $form['mtt_settings']['slideshow_settings']['tabs']['Third_photo']['attribute_photo_3'] = array(
    '#type' => 'textfield',
    '#title' => t('Third photo attribute.'),
  	'#description'   => t('Photographer credit for third photo'),
	'#default_value' => theme_get_setting('attribute_photo_3', 'photo'),
  );  
   
}