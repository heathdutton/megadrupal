<?php
/**
 * Implements hook_form_system_theme_settings_alter() function.
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form.
 */
function pure_theme_form_system_theme_settings_alter(&$form, &$form_state) {
// Work-around for a core bug affecting admin themes.
  if (isset($form_id)) {
    return;
  }

// Create theme settings form widgets using Forms API

// Layout Settings
  $form['tnt_container']['layout_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Layout settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 3,
  );
  $form['tnt_container']['layout_settings']['fntsize'] = array(
    '#type' => 'checkbox',
    '#title' => t('Smaller layout font'),
    '#default_value' => theme_get_setting('fntsize'),
  );
  $form['tnt_container']['layout_settings']['navpos'] = array(
    '#type' => 'select',
    '#title' => t('Drop-down and secondary menus position'),
    '#default_value' => theme_get_setting('navpos'),
    '#options' => array(
      0 => t('Left'),
      1 => t('Center'),
      2 => t('Right'),
    )
  );
  $form['tnt_container']['layout_settings']['menu2'] = array(
    '#type' => 'checkbox',
    '#title' => t('Duplicate the Main Menu at the bottom of the page.'),
    '#default_value' => theme_get_setting('menu2'),
  );
  $form['tnt_container']['layout_settings']['loginlinks'] = array(
    '#type' => 'checkbox',
    '#title' => t('Login/register links'),
    '#default_value' => theme_get_setting('loginlinks'),
  );
  $form['tnt_container']['layout_settings']['breadcrumb_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display breadcrumb'),
    '#default_value' => theme_get_setting('breadcrumb_display'),
  );
  $form['tnt_container']['layout_settings']['roundcorners'] = array(
    '#type' => 'checkbox',
    '#title' => t('Rounded corners'),
    '#description' => t('Some page elements will have rounded corners.'),
    '#default_value' => theme_get_setting('roundcorners'),
  );
  $form['tnt_container']['layout_settings']['pageicons'] = array(
    '#type' => 'checkbox',
    '#title' => t('Page icons'),
    '#default_value' => theme_get_setting('pageicons'),
  );
  $form['tnt_container']['layout_settings']['devlink'] = array(
    '#type' => 'checkbox',
    '#title' => t('Developer link'),
    '#default_value' => theme_get_setting('devlink'),
  );
  $form['tnt_container']['layout_settings']['postedby'] = array(
    '#type' => 'select',
    '#default_value' => theme_get_setting('postedby'),
    '#description' => t('Change "Submitted by" display on all nodes, site-wide.'),
    '#options' => array(
      0 => t('Date & Username'),
      1 => t('Username only'),
      2 => t('Date only'),
    )
  );

// Development settings
  $form['tnt_container']['themedev'] = array(
    '#type' => 'fieldset',
    '#title' => t('Theme development settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 8,
  );
  $form['tnt_container']['themedev']['rebuild_registry'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Rebuild theme registry on every page.'),
    '#default_value' => theme_get_setting('rebuild_registry'),
    '#description'   => t('During theme development, it can be very useful to continuously <a href="https://drupal.org/node/173880#theme-registry">rebuild the theme registry</a>. <br /> <div class="messages warning">This is a huge performance penalty and must be turned off on production websites.</div>'),
  );
  $form['tnt_container']['themedev']['siteid'] = array(
    '#type' => 'textfield',
    '#title' => t('Site ID body class.'),
   	'#description' => t('In order to have different styles of "Pure" in a multisite/multi-theme environment you may find usefull to choose an "ID" and customize the look of each site/theme in custom-style.css file.'),
    '#default_value' => theme_get_setting('siteid'),
    '#size' => 10,
	);

}
