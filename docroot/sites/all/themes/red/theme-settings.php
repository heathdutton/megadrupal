<?php

/**
 * @file
 * red theme-settings.php
 */
 
function red_form_system_theme_settings_alter(&$form, $form_state) {

  $form['red_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Set width of site'),
    '#description' => t('For liquid with set as 100% for fix width set width in px like 960px'),
    '#default_value' => theme_get_setting('red_width', 'red'),
	'#size' => 11,
    '#maxlength' => 6,
  );
  
  $form['red_style'] = array(
    '#type' => 'select',
    '#title' => t('Red color style'),
    '#description' => t('Choose your favorite color.'),
    '#default_value' => theme_get_setting('red_style'),
    '#options' => array(
      'white' => t('Red and White'),
      'black' => t('Red and Black'),
    ),
  );

  $form['red_uselocalcontent'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include Custom Stylesheet'),
    '#default_value' => theme_get_setting('red_uselocalcontent'),
  );

  $form['red_localcontentfile'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to Custom Stylesheet'),
    '#description' => t('type the location of your custom css without leading slash<br />eg. <b>sites/all/themes/newsflash/css/icons.css</b>'),
    '#default_value' => theme_get_setting('red_localcontentfile'),
    '#size' => 40,
    '#maxlength' => 75,
  );

  $form['red_suckerfishalign'] = array(
    '#type' => 'select',
    '#title' => t('Suckerfish Menu Alignment'),
    '#description' => t('Sets the Suckerfish position.'),
    '#default_value' => theme_get_setting('red_suckerfishalign'),
    '#options' => array(
      'right' => t('Right'),
      'center' => t('Centered'),
      'left' => t('Left'),
    ),
  );

  $form['red_suckerfishmenus'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Suckerfish Javascript for IE6'),
    '#default_value' => theme_get_setting('red_suckerfishmenus'),
  );

  $form['red_themelogo'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Themed Logo'),
    '#default_value' => theme_get_setting('red_themelogo'),
  );

  $form['red_breadcrumb'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Breadcrumbs'),
    '#default_value' => theme_get_setting('red_breadcrumb'),
  );

  $form['red_useicons'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Icons'),
    '#default_value' => theme_get_setting('red_useicons'),
  );

  $form['red_footerlogo'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show theme developer credit'),
    '#description' => t('if unchecked then theme developer credit in the footer will disapear<br>so you don\'t need touch the code'),
    '#default_value' => theme_get_setting('red_footerlogo', 'red'),
  );

  return $form;
}
