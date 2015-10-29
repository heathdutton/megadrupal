<?php
/**
 * @file
 * Theme setting callbacks for the nucleus theme.
 */
include_once(drupal_get_path('theme', 'light') . '/common.inc');

function light_reset_settings() {
  global $theme_key;
  variable_del('theme_' . $theme_key . '_settings');
  variable_del('theme_settings');
  $cache = &drupal_static('theme_get_setting', array());
  $cache[$theme_key] = NULL;
}

// Impliments hook_form_system_theme_settings_alter().
function light_form_system_theme_settings_alter(&$form, $form_state) {
  if (theme_get_setting('light_use_default_settings')) {
    light_reset_settings();
  }
  $form['#attached']['js'][] = array(
    'data' => drupal_get_path('theme', 'light') . '/js/light.js',
    'type' => 'file',
  );
  $form['light']['light_version'] = array(
    '#type' => 'hidden',
    '#default' => '1.0',
  );
  light_settings_layout_tab($form);
  light_feedback_form($form);
  $form['#submit'][] = 'light_form_system_theme_settings_submit';
}

function light_settings_layout_tab(&$form) {
  global $theme_key;
  $skins = light_get_predefined_param('skins', array('' => t("Default skin")));
  $form['light']['settings'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Settings'),
    '#weight' => 0,
  );

  if (count($skins) > 1) {
    $form['light']['settings']['configs'] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#title' => t('Configs'),
      '#weight' => 0,
    );
  	$form['light']['settings']['configs']['skin'] = array(
      '#type' => 'select',
      '#title' => t('Skin'),
      '#default_value' => theme_get_setting('skin'),
      '#options' => $skins,
    );
  }
  $form['theme_settings']['toggle_logo']['#default_value'] = theme_get_setting('toggle_logo');
  $form['theme_settings']['toggle_name']['#default_value'] = theme_get_setting('toggle_name');
  $form['theme_settings']['toggle_slogan']['#default_value'] = theme_get_setting('toggle_slogan');
  $form['theme_settings']['toggle_node_user_picture']['#default_value'] = theme_get_setting('toggle_node_user_picture');
  $form['theme_settings']['toggle_comment_user_picture']['#default_value'] = theme_get_setting('toggle_comment_user_picture');
  $form['theme_settings']['toggle_comment_user_verification']['#default_value'] = theme_get_setting('toggle_comment_user_verification');
  $form['theme_settings']['toggle_favicon']['#default_value'] = theme_get_setting('toggle_favicon');
  $form['theme_settings']['toggle_secondary_menu']['#default_value'] = theme_get_setting('toggle_secondary_menu');
  $form['theme_settings']['show_skins_menu'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Skins Menu'),
    '#default_value' => theme_get_setting('show_skins_menu'),
  );

  $form['logo']['default_logo']['#default_value'] = theme_get_setting('default_logo');
  $form['logo']['settings']['logo_path']['#default_value'] = theme_get_setting('logo_path');
  $form['favicon']['default_favicon']['#default_value'] = theme_get_setting('default_favicon');
  $form['favicon']['settings']['favicon_path']['#default_value'] = theme_get_setting('favicon_path');
  $form['theme_settings']['#collapsible'] = TRUE;
  $form['theme_settings']['#collapsed'] = FALSE;
  $form['logo']['#collapsible'] = TRUE;
  $form['logo']['#collapsed'] = FALSE;
  $form['favicon']['#collapsible'] = TRUE;
  $form['favicon']['#collapsed'] = FALSE;
  $form['light']['settings']['theme_settings'] = $form['theme_settings'];
  $form['light']['settings']['logo'] = $form['logo'];
  $form['light']['settings']['favicon'] = $form['favicon'];

  unset($form['theme_settings']);
  unset($form['logo']);
  unset($form['favicon']);

  $form['light']['light_use_default_settings'] = array(
    '#type' => 'hidden',
    '#default_value' => 0,
  );
  $form['actions']['light_use_default_settings_wrapper'] = array(
    '#markup' => '<input type="submit" value="' . t('Reset theme settings') . '" class="form-submit form-reset" onclick="return Drupal.Light.onClickResetDefaultSettings();" style="float: right;">',
  );  
}

function light_feedback_form(&$form) {
  $form['light']['about_light'] = array(
    '#type' => 'fieldset',
    '#title' => t('Feedback Form'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 40,
  );

  $form['light']['about_light']['about_light_wrapper'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('about-light-wrapper')),
  );

  $form['light']['about_light']['about_light_wrapper']['about_light_content'] = array(
    '#markup' => '<iframe width="100%" height="650" scrolling="no" class="nucleus_frame" frameborder="0" src="http://www.weebpal.com/static/feedback/"></iframe>',
  );
}

function light_form_system_theme_settings_submit($form, &$form_state) {
  if(isset($form_state['input']['skin']) && $form_state['input']['skin'] != $form_state['complete form']['light']['settings']['configs']['skin']['#default_value']) {
    setcookie('light_skin', $form_state['input']['skin'], time() + 100000, base_path());
  }
}
