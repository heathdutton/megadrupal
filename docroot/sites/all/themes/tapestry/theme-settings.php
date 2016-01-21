<?php

/**
 * @file
 * tapestry theme-settings.php
 */
 
function tapestry_form_system_theme_settings_alter(&$form, $form_state) {

  $form['tapestry_style'] = array(
    '#type' => 'select',
    '#title' => t('Style'),
    '#description' => t('Choose your favorite color.'),
    '#default_value' => theme_get_setting('tapestry_style'),
    '#options' => array(
      'allstar' => t('All Star'),
      'bluecollar' => t('Bluecollar'),
      'bogart' => t('Bogart'),
      'bizcasual' => t('Business Casual'),
      'cactusbloom' => t('Cactus Bloom'),
      'dolores' => t('Dolores'),
      'dustypetrol' => t('Dusty Petrol'),
      'firenze' => t('Firenze'),
      'fusion' => t('Fusion'),
      'gerberdaisy' => t('Gerber Daisy'),
      'haarlemmod' => t('Haarlem Modern'),
      'kalamata' => t('Kalamata Cream'),
      'kobenhavn' => t('Kobenhavn Classic'),
      'antoinette' => t('Marie Antoinette'),
      'modhome' => t('Modern Home'),
      'modoffice' => t('Modern Office'),
      'orientexpress' => t('Orient Express'),
      'woodworks' => t('Scandinavian Woodworks'),
      'techoffice' => t('Tech Office'),
      'watermelon' => t('Watermelon'),
    ),
  );

  $form['tapestry_pickstyle'] = array
  (
    '#type' => 'checkbox',
    '#title' => t('Enable StylePicker JavaScript'),
    '#description' => t('If enabled then you can use stylepicker see README.txt.'),
    '#default_value' => theme_get_setting('tapestry_pickstyle'),
  );

  $form['tapestry_fixedwidth'] = array(
    '#type' => 'textfield',
    '#title' => t('Width Size'),
    '#description' => t('Set the width. You  can set the width only in Pixel.'),
    '#default_value' => theme_get_setting('tapestry_fixedwidth'),
    '#size' => 5,
    '#maxlength' => 5,
  );

  $form['tapestry_outsidebar'] = array(
    '#type' => 'select',
    '#title' => t('Outside Sidebar Location'),
    '#description' => t('Sets the Outside Sidebar position on left or right.'),
    '#default_value' => theme_get_setting('tapestry_outsidebar'),
    '#options' => array(
      'left' => t('Outside Sidebar on Left'),
      'right' => t('Outside Sidebar on Right'),
    ),
  );

  $form['tapestry_outsidebarwidth'] = array(
    '#type' => 'textfield',
    '#title' => t('Outside Sidebar Width'),
    '#description' => t('Set the width in pixel.'),
    '#default_value' => theme_get_setting('tapestry_outsidebarwidth'),
    '#size' => 5,
    '#maxlength' => 5,
  );

  $form['tapestry_sidebarmode'] = array(
    '#type' => 'select',
    '#title' => t('Inside Sidebar Mode'),
    '#description' => t('Sets the Sidebar First and Sidebar Second positions.'),
    '#default_value' => theme_get_setting('tapestry_sidebarmode'),
    '#options' => array(
      'center' => t('Content Between Inside Sidebars'),
      'left' => t('Inside Sidebars on Left'),
      'right' => t('Inside Sidebars on Right'),
    ),
  );

  $form['tapestry_leftsidebarwidth'] = array(
    '#type' => 'textfield',
    '#title' => t('Left Sidebar Width'),
    '#description' => t('Set the width in Pixel.'),
    '#default_value' => theme_get_setting('tapestry_leftsidebarwidth'),
    '#size' => 5,
    '#maxlength' => 5,
  );

  $form['tapestry_rightsidebarwidth'] = array(
    '#type' => 'textfield',
    '#title' => t('Right Sidebar Width'),
    '#description' => t('Set the width in Pixel.'),
    '#default_value' => theme_get_setting('tapestry_rightsidebarwidth'),
    '#size' => 5,
    '#maxlength' => 5,
  );

  $form['tapestry_fontfamily'] = array(
    '#type' => 'select',
    '#title' => t('Font Family'),
    '#description' => t('Choose your favorite Fonts'),
    '#default_value' => theme_get_setting('tapestry_fontfamily'),
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

  $form['tapestry_customfont'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom Font-Family Setting'),
    '#description' => t('type your fonts separated by ,<br />eg. <b>"Lucida Grande", Verdana, sans-serif</b>'),
    '#default_value' => theme_get_setting('tapestry_customfont'),
    '#size' => 40,
    '#maxlength' => 75,
  );

  $form['tapestry_uselocalcontent'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include Custom Stylesheet'),
    '#default_value' => theme_get_setting('tapestry_uselocalcontent'),
  );

  $form['tapestry_localcontentfile'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to Custom Stylesheet'),
    '#description' => t('type the location of your custom css without leading slash<br />eg. <b>sites/all/themes/newsflash/css/icons.css</b>'),
    '#default_value' => theme_get_setting('tapestry_localcontentfile'),
    '#size' => 40,
    '#maxlength' => 75,
  );

  $form['tapestry_suckerfishalign'] = array(
    '#type' => 'select',
    '#title' => t('Suckerfish Menu Alignment'),
    '#description' => t('Sets the Suckerfish position.'),
    '#default_value' => theme_get_setting('tapestry_suckerfishalign'),
    '#options' => array(
      'right' => t('Right'),
      'center' => t('Centered'),
      'left' => t('Left'),
    ),
  );

  $form['tapestry_suckerfishmenus'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Suckerfish Javascript for IE6'),
    '#default_value' => theme_get_setting('tapestry_suckerfishmenus'),
  );

  $form['tapestry_themelogo'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Themed Logo'),
    '#default_value' => theme_get_setting('tapestry_themelogo'),
  );

  $form['tapestry_breadcrumb'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Breadcrumbs'),
    '#default_value' => theme_get_setting('tapestry_breadcrumb'),
  );

  $form['tapestry_useicons'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Icons'),
    '#default_value' => theme_get_setting('tapestry_useicons'),
  );

  $form['tapestry_footerlogo'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Roople Footer Logo'),
    '#description' => t('if unchecked then roople logo in the footer will disapear<br>so you don\'t need touch the code'),
    '#default_value' => theme_get_setting('tapestry_footerlogo'),
  );

  return $form;
}
