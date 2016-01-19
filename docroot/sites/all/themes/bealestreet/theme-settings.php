<?php
/**
 * @file
 * Beale Street theme-settings.php
 */

function bealestreet_form_system_theme_settings_alter(&$form, $form_state) {

  $form['bealestreet_style'] = array(
    '#type' => 'select',
    '#title' => t('Style'),
    '#description' => t('Choose your favorite color.'),
    '#default_value' => theme_get_setting('bealestreet_style'),
    '#options' => array(
      'blue' => t('Blue'),
      'orange' => t('Orange'),
      'green' => t('Green'),
      'red' => t('Red'),
    ),
  );

  $form['bealestreet_pickstyle'] = array
  (
    '#type' => 'checkbox',
    '#title' => t('Enables the StylePicker JS'),
    '#description' => t('If enabled then you can use stylepicker see README.txt.'),
    '#default_value' => theme_get_setting('bealestreet_pickstyle'),
  );

  $form['bealestreet_width'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Fixed Width'),
    '#default_value' => theme_get_setting('bealestreet_width'),
  );

  $form['bealestreet_fixedwidth'] = array(
    '#type' => 'textfield',
    '#title' => t('Fixed Width Size'),
    '#description' => t('Set the width in Pixel.'),
    '#default_value' => theme_get_setting('bealestreet_fixedwidth'),
    '#size' => 5,
    '#maxlength' => 5,
  );

  $form['bealestreet_fontfamily'] = array(
    '#type' => 'select',
    '#title' => t('Font Family'),
    '#description' => t('Choose your favorite Fonts'),
    '#default_value' => theme_get_setting('bealestreet_fontfamily'),
    '#options' => array(
     'Arial, Verdana, sans-serif' => t('Arial, Verdana, sans-serif'),
     '"Arial Narrow", Arial, Helvetica, sans-serif' => t('"Arial Narrow", Arial, Helvetica, sans-serif'),
     '"Times New Roman", Times, serif' => t('"Times New Roman", Times, serif'),
     '"Lucida Sans", Verdana, Arial, sans-serif' => t('"Lucida Sans", Verdana, Arial, sans-serif'),
     '"Lucida Grande", Verdana, sans-serif' => t('"Lucida Grande", Verdana, sans-serif'),
     'Tahoma, Verdana, Arial, Helvetica, sans-serif' => t('Tahoma, Verdana, Arial, Helvetica, sans-serif'),
     'Georgia, "Times New Roman", Times, serif' => t('Georgia, "Times New Roman", Times, serif'),
     'Custom' => t('Custom (specify below)'),
    ),
  );

  $form['bealestreet_customfont'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom Font-Family Setting'),
    '#description' => t('type your fonts separated by ,<br />eg. <b>"Lucida Grande", Verdana, sans-serif</b>'),
    '#default_value' => theme_get_setting('bealestreet_customfont'),
    '#size' => 40,
    '#maxlength' => 75,
  );

  $form['bealestreet_uselocalcontent'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include Custom Stylesheet'),
    '#default_value' => theme_get_setting('bealestreet_uselocalcontent'),
  );

  $form['bealestreet_localcontentfile'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to Custom Stylesheet'),
    '#description' => t('type the location of your custom css without leading slash<br />eg. <b>sites/all/themes/newsflash/css/icons.css</b>'),
    '#default_value' => theme_get_setting('bealestreet_localcontentfile'),
    '#size' => 40,
    '#maxlength' => 75,
  );

  $form['bealestreet_breadcrumb'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Breadcrumbs'),
    '#default_value' => theme_get_setting('bealestreet_breadcrumb'),
  );

  $form['bealestreet_leftsidebarwidth'] = array(
    '#type' => 'textfield',
    '#title' => t('Left Sidebar Width'),
    '#description' => t('Set the width in Pixel.'),
    '#default_value' => theme_get_setting('bealestreet_leftsidebarwidth'),
    '#size' => 5,
    '#maxlength' => 5,
  );

  $form['bealestreet_rightsidebarwidth'] = array(
    '#type' => 'textfield',
    '#title' => t('Right Sidebar Width'),
    '#description' => t('Set the width in Pixel.'),
    '#default_value' => theme_get_setting('bealestreet_rightsidebarwidth'),
    '#size' => 5,
    '#maxlength' => 5,
  );

  $form['bealestreet_suckerfish'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Suckerfish Menus'),
    '#default_value' => theme_get_setting('bealestreet_suckerfish'),
  );

  $form['bealestreet_footerlogo'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Ropple Footer Logo'),
    '#description' => t('if unchecked then roople logo in the footer will disapear<br>so you don\'t need touch the code'),
    '#default_value' => theme_get_setting('bealestreet_footerlogo'),
  );
  return $form;
}
