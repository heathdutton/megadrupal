<?php
/**
 * @file
 * Override of theme settings.
 */

/**
 * Impliments hook_form_system_theme_settings_alter().
 */
function ishopping_form_system_theme_settings_alter(&$form, $form_state) {
  $form['nucleus']['global_settings']['theme_settings']['hide_frontpage_main_content'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide Main Content Block in Home Page'),
    '#default_value' => theme_get_setting('hide_frontpage_main_content'),
  );
  $form['nucleus']['about_nucleus'] = array(
    '#type' => 'fieldset',
    '#title' => t('Feedback'),
    '#weight' => 40,
  );

  $form['nucleus']['about_nucleus']['about_nucleus_wrapper'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('about-nucleus-wrapper')),
  );
  $form['nucleus']['about_nucleus']['about_nucleus_wrapper']['about_nucleus_content'] = array(
    '#markup' => '<iframe width="100%" height="650" scrolling="no" class="nucleus_frame" frameborder="0" src="http://www.weebpal.com/static/feedback/"></iframe>',
  );
}
