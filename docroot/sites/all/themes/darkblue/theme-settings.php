<?php // $Id$

/**
* Implement THEMENAME_form_system_theme_settings_alter(&$form, $form_state).
*
* @param $form Nested array of form elements that comprise the form.
* @param $form_state A keyed array containing the current state of the form.
* @return
*   array A form array.
*/
function darkblue_form_system_theme_settings_alter(&$form, $form_state) {
   $form['darkblue_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Darkblue settings')
   );
  $form['darkblue_settings']['darkblue_fancydates'] = array(
    '#type' => 'checkbox',
    '#title' => t('Fancy dates for blog posts'),
    '#default_value' => theme_get_setting('darkblue_fancydates'),
  );
  // Return the additional form widgets

  return $form;
}
