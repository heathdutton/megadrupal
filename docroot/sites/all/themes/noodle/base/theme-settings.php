<?php
/**
 * Implements hook_form_system_theme_settings_alter() function.
 */
function noodle_form_system_theme_settings_alter(&$form, $form_state) {
  
  // Setting vertical tabs wrapper
  $form['setnoodle'] = array(
    '#type'          => 'vertical_tabs',
    '#title'         => t('Theme settings'),
    '#default_tab' => 'defaults',
    '#weight' => -10,
  );
  
  // Developer tabs
  $form['setnoodle']['developer'] = array(
    '#type' => 'fieldset',
    '#weight' => 10,
    '#title'         => t('Developer settings'),
    '#description'   => t('Developer settings. Please use with caution.'),
  );

  $form['setnoodle']['developer']['noodle_rebuild_registry'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Rebuild theme registry on every page.'),
    '#default_value' => theme_get_setting('noodle_rebuild_registry'),
    '#description'   => t('During theme development, it can be very useful to continuously <a href="!link">rebuild the theme registry</a>. WARNING: this is a huge performance penalty and must be turned off on production websites.', array('!link' => 'http://drupal.org/node/173880#theme-registry')),
  );

  // Layout tabs
  $form['setnoodle']['colors'] = array(
    '#type' => 'fieldset',
    '#weight' => 2,
    '#title'         => t('Color settings'),
    '#description'   => t('Adjust theme layout settings'),
  );


  $form['setnoodle']['colors']['noodle_color'] = array(
    '#type' => 'radios',
    '#title' => t('Choose color scheme'),
    '#default_value' => theme_get_setting('noodle_color'),
    '#options' => array(
      'noodle_color_1' => t('Green'),
      'noodle_color_2' => t('Blue'),
      'noodle_color_3'  => t('Red'),
      'noodle_color_custom'  => t('Custom'),
    ),
  );
  
  // Theme options tabs
  $form['setnoodle']['options'] = array(
    '#type' => 'fieldset',
    '#weight' => 2,
    '#title'         => t('Theme options'),
    '#description'   => t('Change theme options here.'),
  );

  $form['setnoodle']['options']['noodle_comments_foldable'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enable foldable comment forms.'),
    '#default_value' => theme_get_setting('noodle_comments_foldable'),
    '#description'   => t('Enable foldable comment forms'),
  );

  $form['setnoodle']['options']['noodle_comments_title'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Hide comment title.'),
    '#default_value' => theme_get_setting('noodle_comments_title'),
    '#description'   => t('Hide comment titles if you want so.'),
  );

  $form['setnoodle']['options']['noodle_comments_format_tips'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Hide formatting tips.'),
    '#default_value' => theme_get_setting('noodle_comments_format_tips'),
    '#description'   => t('This will hide formatting tips and guidelines on comments forms.'),
  );

}
