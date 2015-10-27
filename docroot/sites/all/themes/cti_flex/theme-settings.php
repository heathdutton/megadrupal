<?php
/**
 * Implements hook_form_system_theme_settings_alter().
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form.
 */
function cti_flex_form_system_theme_settings_alter(&$form, &$form_state)  {

  // Create the form using Forms API: http://api.drupal.org/api/7

  $form['cti_flex_layout'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Page layout'),
    '#weight' => -3,
    '#description'   => t("Select whether to use a fluid layout or choose from two fixed (960px) layout options"),
  );

 $form['cti_flex_layout']['page_layout'] = array(
    '#type'          => 'radios',
    '#options'       => array(
      'fixed-layout full-background' => t('Fixed width content (960px), full width background colors'),
      'fixed-layout fixed-background' => t('Fixed width content and background colors'),
      'fluid-layout' => t('Fluid, full width layout'),
     ),
    '#default_value' => theme_get_setting('page_layout'),
  );

  $form['cti_flex_colors'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Color Scheme'),
    '#weight' => -2,
    '#description'   => t("Select a predesigned color scheme for the site. If you want to create your own, select 'Custom' and follow the instructions in local.css in the theme.  Note, if you have the Skinr module enabled, you may also change the block styles and color individually on the block configuration pages."),
  );

  $form['cti_flex_colors']['color_scheme'] = array(
    '#type'          => 'select',
    '#title'         => t('Color Scheme'),
    '#default_value' => theme_get_setting('color_scheme'),
    '#description'   => t('Select a predesigned color scheme or choose "Custom" to create your own.'),
    '#options'       => array(
      'black_bg black_accent' => t('Black'),
      'red_bg red_accent' => t('Red'),
      'blue_bg blue_accent' => t('Blue'),
      'green_bg green_accent' => t('Green'),
      'teal_bg teal_accent' => t('Teal'),
      'purple_bg purple_accent' => t('Purple'),
      'orange_bg orange_accent' => t('Orange'),
      'black_bg red_accent' => t('Black and Red'),
      'black_bg blue_accent' => t('Black and Blue'),
      'black_bg green_accent' => t('Black and Green'),
      'black_bg teal_accent' => t('Black and Teal'),
      'black_bg purple_accent' => t('Black and Purple'),
      'black_bg orange_accent' => t('Black and Orange'),
      'white_bg red_accent' => t('White and Red'),
      'white_bg blue_accent' => t('White and Blue'),
      'white_bg green_accent' => t('White and Green'),
      'white_bg teal_accent' => t('White and Teal'),
      'white_bg purple_accent' => t('White and Purple'),
      'white_bg orange_accent' => t('White and Orange'),
      'blue_bg orange_accent' => t('Blue and Orange'),
      'green_bg orange_accent' => t('Green and Orange'),
      'teal_bg orange_accent' => t('Teal and Orange'),
      'custom custom_blocks' => t('Custom'),
     ),
  );

  $form['cti_flex_font'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Fonts'),
    '#weight' => 0,
  );
  
  $form['cti_flex_font']['font_family'] = array(
    '#type'          => 'radios',
    '#title'         => t('Font Family'),
    '#default_value' => theme_get_setting('font_family'),
    '#options'       => array(
      'font-family-none' => t('None - set manually in stylesheet'),
      'font-family-arial' => t('<span class="font-family-arial">Arial, Helvetica, Bitstream Vera Sans, sans-serif</span>'),
      'font-family-lucida'=> t('<span class="font-family-lucida">Lucida Sans, Verdana, Arial, sans-serif</span>'),
      'font-family-times' => t('<span class="font-family-times">Times, Times New Roman, Georgia, Bitstream Vera Serif, serif</span>'),
      'font-family-georgia' => t('<span class="font-family-georgia">Georgia, Times New Roman, Bitstream Vera Serif, serif</span>'),
      'font-family-verdana' => t('<span class="font-family-verdana">Verdana, Tahoma, Arial, Helvetica, Bitstream Vera Sans, sans-serif</span>'),
      'font-family-tahoma' => t('<span class="font-family-tahoma">Tahoma, Verdana, Arial, Helvetica, Bitstream Vera Sans, sans-serif</span>'),
      ),
  );

  $form['cti_flex_font']['font_size'] = array(
    '#type'          => 'radios',
    '#title'         => t('Font Size'),
    '#default_value' => theme_get_setting('font_size'),
    '#options'       => array(
      'font-size-11' => t('11px'),
      'font-size-12' => t('12px - theme default'),
      'font-size-13'=> t('13px'),
      'font-size-14' => t('14px'),
      'font-size-15' => t('15px'),
      'font-size-16' => t('16px'),
      ),
  );

  $form['cti_flex_css3'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Round Corners'),
    '#weight' => 0,
    '#description'   => t('Add round corners to blocks and some other page elements.  These will only be rendered in CSS3-compliant browsers.'),
  );

  $form['cti_flex_css3']['round_corners'] = array(
    '#type'          => 'radios',
    '#default_value' => theme_get_setting('round_corners'),
    '#options'       => array(
      'none' => t('None'),
      'three-px-corners' => t('3 pixel radius'),
      'seven-px-corners' => t('7 pixel radius'),
      'eleven-px-corners' => t('11 pixel radius'),
    ),
  );

  // Remove some of the base theme's settings.
  unset($form['themedev']['zen_layout']); // We don't need to select the base stylesheet.

}
