<?php
/**
 * @file
 * Form for advanced preset
 */

/**
 * Return form for advanced page.
 *
 * @param array $form
 *   A nested array form elements comprising the form.
 * @param array $form_state
 *   An associative array containing the current state of the form.
 * @param array $preset
 *   A preset options array.
 *
 * @return array
 *   The array containing the complete form.
 */
function hellobar_preset_advanced_form($form, $form_state, $preset) {
  $form = array();

  $form['googleFont'] = array(
    '#type' => 'textfield',
    '#title' => t('googleFont'),
    '#default_value' => isset($preset['googleFont']) ? $preset['googleFont'] : NULL,
    '#description' => t("A font name from Google's web font directory. eg: 'Ubuntu' or 'Lobster+Two:400,700'. Note: The font name can be suffixed with the weights desired and a plus symbol can be used for a space. It isn't recommended to use this feature if you already have Google Fonts on your site. If you're already loading Google fonts, just use CSS to style your Hello Bar's text."),
  );

  $form['tabRadius'] = array(
    '#type' => 'textfield',
    '#title' => t('tabRadius'),
    '#default_value' => isset($preset['tabRadius']) ? $preset['tabRadius'] : NULL,
    '#description' => t("This should be a CSS value (pixels, ems, ens) eg: 15px or 0.25em The default tab radius is technically set in the hellobar.css file."),
  );

  $form['speed'] = array(
    '#type' => 'textfield',
    '#title' => t('speed'),
    '#default_value' => isset($preset['speed']) ? $preset['speed'] : NULL,
    '#description' => t("How fast the Hello Bar opens/closes in milliseconds. (To be more precise, this is how long the animation lasts)"),
  );

  $form['shadow'] = array(
    '#type' => 'checkbox',
    '#title' => t('shadow'),
    '#return_value' => TRUE,
    '#default_value' => isset($preset['shadow']) ? $preset['shadow'] : TRUE,
    '#description' => t("Should the Hello Bar display a shadow along the bottom edge? true or false."),
  );

  $form['forgetful'] = array(
    '#type' => 'checkbox',
    '#title' => t('forgetful'),
    '#default_value' => isset($preset['forgetful']) ? $preset['forgetful'] : FALSE,
    '#description' => t("Should the Hello Bar persistently reappear? true or false."),
  );

  $form['transition'] = array(
    '#type' => 'select',
    '#title' => t('transition'),
    '#default_value' => isset($preset['transition']) ? $preset['transition'] : NULL,
    '#description' => t("The animator.js Transition that should be used to animate the bar."),
    '#options' => array(
      'easeInOut' => 'easeInOut',
      'linear' => 'linear',
      'easeIn' => 'easeIn',
      'strongEaseIn' => 'strongEaseIn',
      'easeOut' => 'easeOut',
      'strongEaseOut' => 'strongEaseOut',
      'elastic' => 'elastic',
      'veryElastic' => 'veryElastic',
      'bouncy' => 'bouncy',
      'veryBouncy' => 'veryBouncy',
    ),
  );

  return $form;
}
