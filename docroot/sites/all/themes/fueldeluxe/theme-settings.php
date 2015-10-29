<?php
/*  Drupal starter theme provided by gazwal.com - Freelance Drupal Development Services  */


// Form override fo theme settings
function fueldeluxe_form_system_theme_settings_alter(&$form, $form_state) {

  $form['options_settings'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('FuelDeLuxe D7 theme settings'),
    '#description'  => t('Customize the appearance of your FuelDeLuxe base theme or sub-theme if you are on sub-theme configuration page.'),
    '#collapsible' => FALSE,
    '#collapsed'   => FALSE
  );


  // FONT
  $form['options_settings']['font'] = array(
    '#type' => 'fieldset',
    '#title' => t('Font'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['options_settings']['font']['font_family'] = array(
    '#type' => 'select',
    '#title' => t('Font family'),
    '#default_value' => theme_get_setting('font_family'),
    '#options' => array(
      'ff-s01' => t('Times, Times New Roman, Liberation Serif, serif'),
      'ff-s02' => t('Georgia, DejaVu Serif, Norasi, serif'),
      'ff-ss01' => t('Arial, Helvetica, Liberation Sans, FreeSans, sans-serif'),
      'ff-ss02' => t('Trebuchet MS, Arial, Helvetica, sans-serif'),
      'ff-ss03' => t('Lucida Sans, Lucida Grande, Lucida Sans Unicode, Luxi Sans, sans-serif'),
      'ff-ss04' => t('Tahoma, Geneva, Kalimati, sans-seri'),
      'ff-ss05' => t('Verdana, DejaVu Sans, Bitstream Vera Sans, Geneva, sans-serif'),
    ),
  );

  $form['options_settings']['font']['font_size'] = array(
    '#type' => 'select',
    '#title' => t('Font size'),
    '#default_value' => theme_get_setting('font_size'),
    '#options' => array(
      'fs-10' => t('10px'),
      'fs-11' => t('11px'),
      'fs-12' => t('12px'),
      'fs-13' => t('13px'),
      'fs-14' => t('14px'),
      'fs-15' => t('15px'),
      'fs-16' => t('16px'),
    ),
  );


  // Breadcrumb
  $form['options_settings']['fdl_breadcrumb'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Breadcrumb'),
    '#attributes'    => array('id' => 'fdl-breadcrumb'),
    '#collapsible'   => TRUE,
    '#collapsed'     => TRUE,
  );

  $form['options_settings']['fdl_breadcrumb']['fdl_breadcrumb'] = array(
    '#type'          => 'select',
    '#title'         => t('Display breadcrumb'),
    '#default_value' => theme_get_setting('fdl_breadcrumb'),
    '#options'       => array(
      'yes'   => t('Yes'),
      'admin' => t('Only in admin section'),
      'no'    => t('No'),
    ),
  );

  $form['options_settings']['fdl_breadcrumb']['fdl_breadcrumb_separator'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Breadcrumb separator'),
    '#description'   => t('Text only. Don’t forget to include spaces.'),
    '#default_value' => theme_get_setting('fdl_breadcrumb_separator'),
    '#size'          => 5,
    '#maxlength'     => 10,
  );

  $form['options_settings']['fdl_breadcrumb']['fdl_breadcrumb_home'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Show home page link in breadcrumb'),
    '#default_value' => theme_get_setting('fdl_breadcrumb_home'),
  );

  $form['options_settings']['fdl_breadcrumb']['fdl_breadcrumb_trailing'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Append a separator to the end of the breadcrumb'),
    '#default_value' => theme_get_setting('fdl_breadcrumb_trailing'),
    '#description'   => t('Useful when the breadcrumb is placed just before the title.'),
  );

  $form['options_settings']['fdl_breadcrumb']['fdl_breadcrumb_title'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Append the content title to the end of the breadcrumb'),
    '#default_value' => theme_get_setting('fdl_breadcrumb_title'),
    '#description'   => t('Useful when the breadcrumb is not placed just before the title.'),
  );


  // Search box settings (from Pop)
  if (theme_get_setting('enable_search_box_settings')) {
    $form['options_settings']['search_box'] = array(
      '#type'        => 'fieldset',
      '#title'       => t('Search box'),
      '#description' => t('The search box customization.'),
      '#collapsible' => TRUE,
      '#collapsed'   => TRUE,
    );
    $form['options_settings']['search_box']['search_box_label_inline'] = array(
      '#type'          => 'checkbox',
      '#title'         => t('Display search box label inline'),
      '#description'   => t('Label is shown as placeholder in the search text field.'),
      '#default_value' => theme_get_setting('search_box_label_inline'),
    );
    $form['options_settings']['search_box']['search_box_label_text'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Search box label'),
      '#description'   => t('Text only. Default is "Search".'),
      '#default_value' => theme_get_setting('search_box_label_text'),
      '#size'          => 30,
      '#maxlength'     => 255,
    );
    $form['options_settings']['search_box']['search_box_tooltip_text'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Search box tooltip, i.e. input field\'s title value'),
      '#description'   => t('Text only, <em>&lt;none&gt;</em> to disable. Default is "Enter the terms you wish to search for.".'),
      '#default_value' => theme_get_setting('search_box_tooltip_text'),
      '#size'          => 30,
      '#maxlength'     => 255,
    );
    $form['options_settings']['search_box']['search_box_button'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display the search button'),
      '#default_value' => theme_get_setting('search_box_button'),
    );
    $form['options_settings']['search_box']['search_box_button_text'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Search box button text'),
      '#description'   => t('Text only. Default is "Search".'),
      '#default_value' => theme_get_setting('search_box_button_text'),
      '#size'          => 20,
      '#maxlength'     => 30,
    );
  } // end IF enable_search_box_settings

  // Clear registry
  $form['options_settings']['clear_registry'] = array(
    '#type' => 'checkbox',
    '#title' =>  t('Rebuild theme registry on every page.'),
    '#description'   =>t('During theme development, it can be very useful to continuously <a href="!link">rebuild the theme registry</a>. WARNING: this is a huge performance penalty and must be turned off on production websites.', array('!link' => 'http://drupal.org/node/173880#theme-registry')),
    '#default_value' => theme_get_setting('clear_registry'),
  );

}
