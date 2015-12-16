<?php
/**
 * @file
 * Form for basic preset
 */

/**
 * Return form for basic page.
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
function hellobar_preset_basic_form($form, $form_state, $preset) {
  $form = array();

  $form['positioning'] = array(
    '#type' => 'select',
    '#title' => t('positioning'),
    '#default_value' => isset($preset['positioning']) ? $preset['positioning'] : NULL,
    '#description' => t('The positioning of the bar.'),
    '#options' => array(
      'sticky' => 'sticky',
      'overlap' => 'overlap',
      'fixed' => 'fixed',
      'push' => 'push',
    ),
  );

  $form['tabSide'] = array(
    '#type' => 'select',
    '#title' => t('tabSide'),
    '#default_value' => isset($preset['tabSide']) ? $preset['tabSide'] : NULL,
    '#description' => t('The side of the bar the tab is on.'),
    '#options' => array(
      'right' => 'right',
      'left' => 'left',
    ),
  );

  $form['barColor'] = array(
    '#type' => 'jquery_colorpicker',
    '#title' => t('barColor'),
    '#default_value' => isset($preset['barColor']) ? $preset['barColor'] : NULL,
    '#description' => t('The background color of the Hello Bar. This can be either a hex string or a rgb string: #eb593c or rgb(235,89,60)'),
  );

  $form['textColor'] = array(
    '#type' => 'jquery_colorpicker',
    '#title' => t('textColor'),
    '#default_value' => isset($preset['textColor']) ? $preset['textColor'] : NULL,
    '#description' => t('The text color of the Hello Bar. This can be either a hex string or a rgb string: #ffffff or rgb(255,255,255)'),
  );

  $form['linkColor'] = array(
    '#type' => 'jquery_colorpicker',
    '#title' => t('linkColor'),
    '#default_value' => isset($preset['linkColor']) ? $preset['linkColor'] : NULL,
    '#description' => t('The link color of the Hello Bar. This can be either a hex string or a rgb string: #80ccff or rgb(128,204,255)'),
  );

  $form['borderColor'] = array(
    '#type' => 'jquery_colorpicker',
    '#title' => t('borderColor'),
    '#default_value' => isset($preset['borderColor']) ? $preset['borderColor'] : NULL,
    '#description' => t('The border color of the Hello Bar. This can be either a hex string or a rgb string: #ffffff or rgb(255,255,255)'),
  );

  $form['borderSize'] = array(
    '#type' => 'textfield',
    '#title' => t('borderSize'),
    '#default_value' => isset($preset['borderSize']) ? $preset['borderSize'] : NULL,
    '#description' => t('The size of the Hello Bar border in pixels. Zero removes the border.'),
  );

  $form['texture'] = array(
    '#type' => 'select',
    '#title' => t('texture'),
    '#default_value' => isset($preset['texture']) ? $preset['texture'] : NULL,
    '#description' => t('The background texture of the Hello Bar. Note: Only works on bars that are the default 30 pixels tall.'),
    '#empty_option' => 'none',
    '#options' => array(
      'noise' => 'noise',
      'hard-shine' => 'hard-shine',
      'light-gradient' => 'light-gradient',
      'dark-gradient' => 'dark-gradient',
      'carbon' => 'carbon',
      'paper' => 'paper',
      'diagonal' => 'diagonal',
      'linen' => 'linen',
      'stitch' => 'stitch',
      'diamond' => 'diamond',
    ),
  );

  $form['helloBarLogo'] = array(
    '#type' => 'checkbox',
    '#title' => t('helloBarLogo'),
    '#return_value' => TRUE,
    '#default_value' => isset($preset['helloBarLogo']) ? $preset['helloBarLogo'] : FALSE,
    '#description' => t('Should the Hello Bar logo be displayed? I think no ;)'),
  );

  return $form;
}
