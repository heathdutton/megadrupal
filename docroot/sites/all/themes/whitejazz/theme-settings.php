<?php

/**
 * @file
 * WhiteJazz D7.x theme_settings.php
 *
 */
function whitejazz_form_system_theme_settings_alter(&$form, $form_state) {

  $form['whitejazz_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width Size'),
    '#description' => t('Set the width. You  can set the width in percent for dynamic width or in px for fixed width.<br /><b>eg.: 850px or 60%</b>'),
    '#default_value' => theme_get_setting('whitejazz_width'),
    '#size' => 7,
    '#maxlength' => 7,
  );

  $form['whitejazz_breadcrumb'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Breadcrumbs'),
    '#default_value' => theme_get_setting('whitejazz_breadcrumb'),
  );

  $form['whitejazz_fontfamily'] = array(
    '#type' => 'select',
    '#title' => t('Font Family'),
    '#description' => t('Choose your favorite Fonts'),
    '#default_value' => theme_get_setting('whitejazz_fontfamily'),
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

  $form['whitejazz_customfont'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom Font-Family Setting'),
    '#description' => t('type your fonts separated by ,<br />eg. <b>"Lucida Grande", Verdana, sans-serif</b>'),
    '#default_value' => theme_get_setting('whitejazz_customfont'),
    '#size' => 40,
    '#maxlength' => 75,
  );

  $form['whitejazz_uselocalcontent'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include Custom Stylesheet'),
    '#default_value' => theme_get_setting('whitejazz_uselocalcontent'),
  );

  $form['whitejazz_localcontentfile'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to Custom Stylesheet'),
    '#description' => t('type the location of your custom css without leading slash<br />eg. <b>sites/all/themes/newsflash/css/icons.css</b>'),
    '#default_value' => theme_get_setting('whitejazz_localcontentfile'),
    '#size' => 40,
    '#maxlength' => 75,
  );

  $form['whitejazz_leftsidebarwidth'] = array(
    '#type' => 'textfield',
    '#title' => t('Left Sidebar Width'),
    '#description' => t('Set the width in Pixel.'),
    '#default_value' => theme_get_setting('whitejazz_leftsidebarwidth'),
    '#size' => 5,
    '#maxlength' => 5,
  );

  $form['whitejazz_rightsidebarwidth'] = array(
    '#type' => 'textfield',
    '#title' => t('Right Sidebar Width'),
    '#description' => t('Set the width in Pixel.'),
    '#default_value' => theme_get_setting('whitejazz_rightsidebarwidth'),
    '#size' => 5,
    '#maxlength' => 5,
  );

  $form['whitejazz_usecustomlogosize'] = array(
    '#type' => 'checkbox',
    '#title' => t('Specify Custom Logo Size'),
    '#default_value' => theme_get_setting('whitejazz_usecustomlogosize'),
  );

  $form['whitejazz_logowidth'] = array(
    '#type' => 'textfield',
    '#title' => t('Logo Width'),
    '#description' => t('Set the width in Pixel.'),
    '#default_value' => theme_get_setting('whitejazz_logowidth'),
    '#size' => 5,
    '#maxlength' => 5,
  );

  $form['whitejazz_logoheight'] = array(
    '#type' => 'textfield',
    '#title' => t('Logo Height'),
    '#description' => t('Set the width in Pixel.'),
    '#default_value' => theme_get_setting('whitejazz_logoheight'),
    '#size' => 5,
    '#maxlength' => 5,
  );

  $form['whitejazz_rooplefooterlogo'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show RoopleTheme footer logo'),
    '#description' => t('if unchecked then roopletheme logo in the footer will disapear<br />so you don\'t need touch the code'),
    '#default_value' => theme_get_setting('whitejazz_rooplefooterlogo'),
  );

  return $form;
}
