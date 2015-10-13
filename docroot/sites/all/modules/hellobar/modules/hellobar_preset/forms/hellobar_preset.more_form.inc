<?php
/**
 * @file
 * Form for more preset
 */

/**
 * Return form for more page.
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
function hellobar_preset_more_form($form, $form_state, $preset) {
  $form = array();

  $form['height'] = array(
    '#type' => 'textfield',
    '#title' => t('height'),
    '#delta' => 30,
    '#default_value' => isset($preset['height']) ? $preset['height'] : NULL,
    '#description' => t('The height off the colored bar portion of the hello bar in pixels. (There is a minimum value of 30 pixels)'),
  );

  $form['showWait'] = array(
    '#type' => 'textfield',
    '#title' => t('showWait'),
    '#default_value' => isset($preset['showWait']) ? $preset['showWait'] : NULL,
    '#description' => t('The number of milliseconds the bar stays hidden for before being shown the first time. Zero means the bar is shown immediately.'),
  );

  $form['hideAfter'] = array(
    '#type' => 'textfield',
    '#title' => t('hideAfter'),
    '#default_value' => isset($preset['hideAfter']) ? $preset['hideAfter'] : NULL,
    '#description' => t('The number of milliseconds after the bar is shown before it hides itself.'),
  );

  $form['wiggleWait'] = array(
    '#type' => 'textfield',
    '#title' => t('wiggleWait'),
    '#default_value' => isset($preset['wiggleWait']) ? $preset['wiggleWait'] : NULL,
    '#description' => t('The number of milliseconds between tab wiggles.'),
  );

  $form['fontSize'] = array(
    '#type' => 'textfield',
    '#title' => t('fontSize'),
    '#default_value' => isset($preset['fontSize']) ? $preset['fontSize'] : NULL,
    '#description' => t("The font size of the Hello Bar's content as a CSS value (pixels, ems, ens) eg: 17px or 1.15em"),
  );

  $form['lineHeight'] = array(
    '#type' => 'textfield',
    '#title' => t('lineHeight'),
    '#default_value' => isset($preset['lineHeight']) ? $preset['lineHeight'] : NULL,
    '#description' => t("The line height of the Hello Bar's content as a CSS value (pixels, ems, ens) eg: 17px or 1.15em"),
  );

  $form['fontWeight'] = array(
    '#type' => 'textfield',
    '#title' => t('fontWeight'),
    '#default_value' => isset($preset['fontWeight']) ? $preset['fontWeight'] : NULL,
    '#description' => t("The font weight of the Hello Bar's content as a CSS value (normal, bold, 100, 400, 500, 700, etc.)"),
  );

  $form['fontStyle'] = array(
    '#type' => 'textfield',
    '#title' => t('fontStyle'),
    '#default_value' => isset($preset['fontStyle']) ? $preset['fontStyle'] : NULL,
    '#description' => t("The font style of the Hello Bar's content as a CSS value (normal, italic, oblique, underline)"),
  );

  $form['fontFamily'] = array(
    '#type' => 'select',
    '#title' => t('fontFamily'),
    '#default_value' => isset($preset['fontFamily']) ? $preset['fontFamily'] : NULL,
    '#description' => t("A CSS font stack for the Hello Bar's content. eg: 'Georgia, Cambria, Times New Roman, Times, serif'"),
    '#empty_option' => 'none',
    '#options' => array(
      'Georgia,Times New Roman,Times,serif' => 'Serif',
      'Helvetica,Arial,sans-serif' => 'Sans-Serif',
      'Arial,Helvetica,sans-serif' => 'Arial',
      'Arial Black,Gadget,sans-serif' => 'Arial Black',
      'Cambria,serif' => 'Cambria',
      'Calibri,sans-serif' => 'Calibri',
      'Courier New,monospace' => 'Courier New',
      'Futura,sans-serif' => 'Futura',
      'Georgia,serif' => 'Georgia',
      'Helvetica,sans-serif' => 'Helvetica',
      'Lucida Grande,Lucida Sans Unicode,sans-serif' => 'Lucida Grande',
      'Tahoma,Geneva,sans-serif' => 'Tahoma',
      'Times New Roman,Times,serif' => 'Times New Roman',
      'Trebuchet MS,sans-serif' => 'Trebuchet MS',
      'Verdana,Geneva' => 'Verdana',
    ),
  );

  return $form;
}
