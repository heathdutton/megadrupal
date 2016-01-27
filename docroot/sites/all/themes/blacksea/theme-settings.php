<?php

function blacksea_form_system_theme_settings_alter(&$form, $form_state) {

  $form['appearance_container'] = array(
    '#type' => 'fieldset',
    '#title' => t('Appearance Settings'),
    '#description' => t('Customize theme colors and layout of your ThemeShark.com theme .'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#prefix' => '<h2 class="themeshark">Theme Settings </h2><div class="themeshark-settings">',
    '#suffix' => '</div>',
  );
  
  $form['appearance_container']['style'] = array(
    '#type' => 'select',
    '#title' => t('Theme Style'),
	'#description' => t('Select the color style of your theme'),
    '#default_value' => theme_get_setting('style'),
    '#options' => array (
      0 => t('Style 1'),
    ),
  );
  
  $form['appearance_container']['layout'] = array(
    '#type' => 'select',
    '#title' => t('Fixed/Fluid'),
    '#default_value' => theme_get_setting('layout'),
    '#options' => array (
      0 => t('Fixed Width'),
      1 => t('Fluid Width'),
    ),
  );
  
  $form['appearance_container']['fixedwidth'] = array(
    '#type' => 'textfield',
    '#title' => t('Fixed Width Size'),
    '#description' => t('Enter the width of your website in pixels (omitting px). Recommended width: 960'),
    '#default_value' => theme_get_setting('fixedwidth'),
    '#size' => 4,
    '#maxlength' => 4,
  ); 
  
  $form['appearance_container']['leftwidth'] = array(
    '#type' => 'textfield',
    '#title' => t('Left Sidebar Width'),
    '#description' => t('Enter the width of your left sidebar in pixels (omitting px)'),
    '#default_value' => theme_get_setting('leftwidth'),
    '#size' => 5,
    '#maxlength' => 5,
  );

  $form['appearance_container']['rightwidth'] = array(
    '#type' => 'textfield',
    '#title' => t('Right Sidebar Width'),
    '#description' => t('Enter the width of your right sidebar in pixels (omitting px)'),
    '#default_value' => theme_get_setting('rightwidth'),
    '#size' => 5,
    '#maxlength' => 5,
  );
  
  $form['appearance_container']['font_family'] = array(
    '#type' => 'select',
    '#title' => t('Font Family'),
    '#description' => t('Select your site font from the dropdown below'),
    '#default_value' => theme_get_setting('font_family'),
    '#options' => array (
      'Arial, Helvetica, sans-serif' => t('Tahoma, Arial, Helvetica, Sans-serif'),
      '"Times New Roman", Times, serif' => t('Times New Roman, Times, Serif'),
	  '"Courier New", Courier, monospace' => t('"Courier New", Courier, Monospace'),
	  'Georgia, "Times New Roman", Times, serif' => t('Georgia, "Times New Roman", Times, Serif'),
      'Verdana, Arial, Helvetica, sans-serif' => t('Verdana, Arial, Helvetica, Sans-serif'),
	  'Geneva, Arial, Helvetica, sans-serif' => t('Geneva, Arial, Helvetica, Sans-serif'),
    ),
  );  
  
  $form['appearance_container']['font_size'] = array(
    '#type' => 'select',
    '#title' => t('Font Size'),
    '#description' => t('Select the size of your font.  Note: too selecting a font size that is too large or too small may degrade the appearance of your theme.  Small or Default is recommended.'),
    '#default_value' => theme_get_setting('font_size'),
    '#options' => array (
      '0.8' => t('Smallest'),
      '0.9' => t('Small'),
	  '1.0' => t('Default'),
	  '1.1' => t('Large'),
	  '1.2' => t('Largest'),
    ),
  );

  $form['navigation_container'] = array(
    '#type' => 'fieldset',
    '#title' => t('Navigation Settings'),
    '#description' => t('Customize the your menus and site navigation.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#prefix' => '<div class="themeshark-settings">',
    '#suffix' => '</div>',
  );
  
  $form['navigation_container']['show_breadcrumb'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Breadcrumbs'),
    '#default_value' => theme_get_setting('show_breadcrumb'),
  );
  
  $form['misc_container'] = array(
    '#type' => 'fieldset',
    '#title' => t('Misc'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#prefix' => '<div class="themeshark-settings">',
    '#suffix' => '</div>',
  );

  $form['misc_container']['css'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use custom stylesheet (found in css/custom.css) for tweaking/adding CSS'),
    '#default_value' => theme_get_setting('css'),
  );
	
	$form['misc_container']['twitter_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter URL'),
    '#description' => t('Enter your Twitter URL'),
    '#default_value' => theme_get_setting('twitter_url'),
    '#size' => 50,
    '#maxlength' => 100,
  );
	
	$form['misc_container']['facebook_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook URL'),
    '#description' => t('Enter your Facebook URL'),
    '#default_value' => theme_get_setting('facebook_url'),
    '#size' => 50,
    '#maxlength' => 100,
  );		
  
}
