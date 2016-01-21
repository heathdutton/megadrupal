<?php

function delta_form_system_theme_settings_alter(&$form, &$form_state) {

  $form['theme_settings']['#collapsible'] = TRUE;
  $form['theme_settings']['#collapsed'] = TRUE;
  $form['favicon']['#collapsible'] = TRUE;
  $form['favicon']['#collapsed'] = TRUE;
  $form['logo']['#collapsible'] = TRUE;
  $form['logo']['#collapsed'] = TRUE;

  $form['logo']['width_logo'] = array(
    '#type' => 'textfield',
    '#title' => t('Width logo'),
    '#default_value' => theme_get_setting('width_logo', 'delta'),
    '#description' => t("Size width logo. example: 170px"),
  );

  $form['delta_settings']['link_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Links settings'),
    '#description'   => t("Check this option to link-decoration. Uncheck to remove."),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['delta_settings']['link_settings']['body_link'] = array(
    '#type' => 'fieldset',
    '#title' => t('Body link'),
  );
  $form['delta_settings']['link_settings']['main_menu_link'] = array(
    '#type' => 'fieldset',
    '#title' => t('Main menu link'),
  );
  $form['delta_settings']['link_settings']['footer_link'] = array(
    '#type' => 'fieldset',
    '#title' => t('Footer link'),
  );

  $form['delta_settings']['link_settings']['body_link']['b_decor'] = array(
    '#type' => 'checkbox',
    '#title' => t('Body link decoration'),
    '#default_value' => theme_get_setting('b_decor', 'delta'),
  );
  $form['delta_settings']['link_settings']['main_menu_link']['m_decor'] = array(
    '#type' => 'checkbox',
    '#title' => t('Main menu link decoration'),
    '#default_value' => theme_get_setting('m_decor', 'delta'),
  );
  $form['delta_settings']['link_settings']['footer_link']['f_decor'] = array(
    '#type' => 'checkbox',
    '#title' => t('Footer link decoration'),
    '#default_value' => theme_get_setting('f_decor', 'delta'),
  );

  $form['delta_settings']['link_settings']['body_link']['b_decor_hover'] = array(
    '#type' => 'checkbox',
    '#title' => t('Body link decoration hover'),
    '#default_value' => theme_get_setting('b_decor_hover', 'delta'),
  );
  $form['delta_settings']['link_settings']['main_menu_link']['m_decor_hover'] = array(
    '#type' => 'checkbox',
    '#title' => t('Main menu link decoration hover'),
    '#default_value' => theme_get_setting('m_decor_hover', 'delta'),
  );
  $form['delta_settings']['link_settings']['footer_link']['f_decor_hover'] = array(
    '#type' => 'checkbox',
    '#title' => t('Footer link decoration hover'),
    '#default_value' => theme_get_setting('f_decor_hover', 'delta'),
  );

//----------------------------------------------------------------------------------

  $form['delta_settings']['social'] = array(
    '#type' => 'fieldset',
    '#title' => t('Social Icon'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['delta_settings']['social']['show_hide_icon'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Social Icon'),
    '#default_value' => theme_get_setting('show_hide_icon', 'delta'),
    '#description' => t("Check this option to show Social Icon. Uncheck to hide."),
  );

  $socmass = array (
    'twitter' => t('Twitter URL'),
    'facebook' => t('Facebook URL'),
    'flickr' => t('Flickr URL'),
    'linkedin' => t('Linkedin URL'),
    'youtube' => t('YouTube URL'),
    'pinterest' => t('Pinterest URL'),
    'google' => t('Google+ URL'),
    'dribbble' => t('Dribbble URL'),
    'vimeo' => t('Vimeo URL'),
    'instagram' => t('Instagram URL'),
    'vk' => t('Vk URL')
  );

  foreach($socmass as $key => $value) {
    $form['delta_settings']['social'][$key] = array(
    '#type' => 'textfield',
    '#title' => $value,
    '#default_value' => theme_get_setting($key, 'delta'),
    '#description' => t("Enter your $key Profile URL. example: http://www.$key.com"),
    );
  }

//----------------------------------------------------------------------------------

  $form['delta_settings']['video'] = array(
    '#type' => 'fieldset',
    '#title' => t('Video box'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['delta_settings']['video']['show_hide_video'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show video box'),
    '#default_value' => theme_get_setting('show_hide_video', 'delta'),
    '#description' => t("Check this option to show video box. Uncheck to hide."),
  );

  $form['delta_settings']['video']['video_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Add YouTube ID video'),
    '#default_value' => theme_get_setting('video_id', 'delta'),
    '#description'   => t("Add ID video. example: NoVW62mwSQQ"),
  );

//------------------------------------------------------------------------------------

  $form['delta_settings']['body_pattern'] = array(
    '#type' => 'fieldset',
    '#title' => t('Body pattern'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['delta_settings']['body_pattern']['layout_pattern'] = array(
      '#type' => 'select',
      '#options' => array (
          'none' => t('None'),
          'body_bg1' => t('Stone'),
          'body_bg2' => t('Lines'),
          'body_bg3' => t('Rhombus'),
          'body_bg4' => t('Noise'),
          'body_bg5' => t('Сloth')
      ),
      '#title' => t('Select Pattern'),
      '#default_value' => theme_get_setting('layout_pattern', 'delta'),
      );

//--------------------------------------------------------------------------------------------

  $form['delta_settings']['copyfoot'] = array(
    '#type' => 'fieldset',
    '#title' => t('Copyright'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['delta_settings']['copyfoot']['show_hide_copyright'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show copyright text in footer'),
    '#default_value' => theme_get_setting('show_hide_copyright', 'delta'),
    '#description'   => t("Check this option to show copyright text in footer. Uncheck to hide."),
  );

  $form['delta_settings']['copyfoot']['copyright_developedby'] = array(
    '#type' => 'textfield',
    '#title' => t('Add name developed by in footer'),
    '#default_value' => theme_get_setting('copyright_developedby', 'delta'),
    '#description'   => t("Add name developed by in footer"),
  );
  $form['delta_settings']['copyfoot']['copyright_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Add link to developed by in footer'),
    '#default_value' => theme_get_setting('copyright_url', 'delta'),
    '#description'   => t("Add url developed by in footer. example: http://www.xyz.com"),
  );

//--------------------------------------------------------------------------------------------

  $form['delta_settings']['fonts'] = array(
    '#type' => 'fieldset',
    '#title' => t('Typography'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $options_font = array (
      'Abel, sans-serif' => 'Abel',
      'Arial, sans-serif' => 'Arial',
      'Comfortaa' => 'Comfortaa',
      'Dosis, sans-serif' => 'Dosis',
      'Droid Sans, sans-serif' => 'Droid Sans',
      'Francois One, sans-serif' => 'Francois One',
      'Georgia, serif' => 'Georgia',
      'Helvetica, sans-serif' => 'Helvetica',
      'Lato, sans-serif' => 'Lato',
      'Lobster, sans-serif' => 'Lobster',
      'Lora, sans-serif' => 'Lora',
      'Open Sans Condensed, sans-serif' => 'Open Sans Condensed',
      'Open Sans, sans-serif' => 'Open Sans',
      'Oswald, sans-serif' => 'Oswald',
      'Oxygen, sans-serif' => 'Oxygen',
      'PT Sans Narrow, sans-serif' => 'PT Sans Narrow',
      'PT Sans' => 'PT Sans',
      'Prosto One, cursive' => 'Prosto One',
      'Quicksand, sans-serif' => 'Quicksand',
      'Raleway, sans-serif' => 'Raleway',
      'Roboto Condensed, sans-serif' => 'Roboto Condensed',
      'Roboto, sans-serif' => 'Roboto',
      'Share, cursive' => 'Share',
      'Source Sans Pro, sans-serif' => 'Source Sans Pro',
      'Times New Roman, sans-serif' => 'Times New Roman',
      'Ubuntu Condensed, sans-serif' => 'Ubuntu Condensed',
      'Ubuntu, sans-serif' => 'Ubuntu',
      'Verdana, sans-serif' => 'Verdana'
  );
  $fmass = array (
      'body_font' => t('Body font'),
      'main_menu_font' => t('Main menu font'),
      'body_links_font' => t('Body link font'),
      'footer_links_font' => t('Footer link font'),
      'h1_font' => 'h1',
      'h2_font' => 'h2',
      'h3_font' => 'h3',
      'h4_font' => 'h4',
      'h5_font' => 'h5',
      'h6_font' => 'h6'
  );
  foreach($fmass as $key => $value) {
    $form['delta_settings']['fonts'][$key] = array(
      '#type' => 'select',
      '#options' => $options_font,
      '#title' => $value,
      '#default_value' => theme_get_setting($key, 'delta'),
      );
  }

//--------------------------------------------------------------------------
  
}