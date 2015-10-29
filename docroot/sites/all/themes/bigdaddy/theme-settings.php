<?php
/**
 * THEME SETTINGS
 */
function bigdaddy_form_system_theme_settings_alter(&$form, $form_state) {


  // Removing the empty spaces between the input and the description
  drupal_add_css('#block-system-main .description {line-height: 1.5em;margin-top: -1px;font-size: 11px}',$option['type'] = 'inline');

  // Check for installed module SASSY and PREPRO (needed to work with SASS)
  if (!module_exists('sassy')) {
  drupal_add_css('#edit-module-dependencies {border: none;padding-bottom: 0;margin: 0;} .messages.warning {background-position: 20px 25px;}',$option['type'] = 'inline');
  $form['module dependencies'] = array(
      '#type' => 'fieldset',
      '#title' => t('Needed modules for working with the SASS files'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#description' => t('In order to use SASS in this theme you have to install <a href="https://drupal.org/project/sassy" target="_blank">SASSY</a> and <a href="https://drupal.org/project/prepro" target="_blank">PREPROCESSOR</a>.'),
      '#weight' => -100,
      '#prefix' => '<div class="messages warning">',
      '#suffix' => '</div>'
    );
  }

  $form['title'] = array(
    '#weight'=> -30,
    '#markup' => '<h1>BigDaddy - theme settings</h1>'
  );

  // Defining all the fieldsets
  $form['design'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('DESIGN'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#weight'=> -20
  );
  $form['responsive'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('RESPONSIVE'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#weight'=> -19
  );
  $form['javascript'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('JAVASCRIPT'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#weight'=> -18
  );
  $form['default'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('CORE SETTINGS'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight'=> -15
  );

  // Defining the theme settings types
  $form['design']['bigdaddy_wireframes'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Activate Wireframes'),
    '#default_value' => theme_get_setting('bigdaddy_wireframes'),
    '#description'   => t('Very useful for checking your activated regions.'),
  );
  $form['design']['bigdaddy_grid_system'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Activate Grid system'),
    '#default_value' => theme_get_setting('bigdaddy_grid_system'),
    '#description'   => t('Show your grid system as background image. To change the default grid system, modify the last line in the <strong>theme-settings.css</strong> file. <strong>IMPORTANT: you have to activate Compass and the layout module</strong>.'),
  );
  $form['responsive']['bigdaddy_display_viewport'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Activate Viewport display'),
    '#default_value' => theme_get_setting('bigdaddy_display_viewport'),
    '#description'   => t('Show the current viewport size (in PX and EMs). Useful for checking your breakpoints.'),
  );
  $form['javascript']['bigdaddy_js_jquery'] = array(
      '#type'          => 'checkbox',
      '#title'         => t('Replace Drupal\'s default jQuery version the last CDN version') ,
      '#description'   => t('The replaced version will be <strong>//ajax.googleapis.com/ajax/libs/jquery/1.10.1/jquery.min.js</strong>. If you are interested in other JS libraries check <a href="http://cdnjs.com/" target="_blank">http://cdnjs.com/</a>'),
      '#default_value' => theme_get_setting('bigdaddy_js_jquery'),
  );

  $form['default']['theme_settings'] = $form['theme_settings'];
  $form['default']['logo'] = $form['logo'];
  $form['default']['favicon'] = $form['favicon'];
  unset($form['theme_settings']);
  unset($form['logo']);
  unset($form['favicon']);

}
