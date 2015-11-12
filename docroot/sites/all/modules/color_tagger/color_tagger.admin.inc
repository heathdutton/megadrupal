<?php

/**
 * @file
 * Admin page callbacks for Color Tagger module.
 *
 * Gives an overview over the available options for the color tagger.
 */

/**
 * Info page for Color Tagger.
 */
function color_tagger_admin_page() {
  $output = array();
  $output['first_para'] = array(
    '#type' => 'markup',
    '#markup' => '<p>A paragraph about some stuff...</p>',
  );
  return $output;
}
/**
 * Settings form for Color Tagger.
 */
function color_tagger_admin_settings_form($form, &$form_state) {

  $form['#tree'] = TRUE;

  $form['use_lab'] = array(
    '#type' => 'radios',
    '#title' => t('Color Space'),
    '#description' => t("Select the color space to be used when processing images to find matching colors. LAB processing is more accurate, but takes significantly longer. RGB is faster, can yield less satisfactory results."),
    '#options' => array(0 => t('RGB'), 1 => t('LAB')),
    '#default_value' => color_tagger_get_record('use_lab', 1),
    '#required' => TRUE,
  );

  $form['ignore_background'] = array(
    '#type' => 'radios',
    '#title' => t('Ignore Background Color'),
    '#description' => t("Attempt to identify the background color in an image and ignore it. Works best when photos have solid backgrounds. </br> <strong>Specific colors can be ignored on all images in the field widget settings.</strong>"),
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => color_tagger_get_record('ignore_background', 0),
    '#required' => TRUE,
  );

  $form['color_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of colors'),
    '#default_value' => color_tagger_get_record('color_limit', 2),
    '#size' => 2,
    '#maxlength' => 2,
    '#description' => t("Enter the maximum number of colors each image should be tagged with."),
    '#required' => TRUE,
  );

  $form['threshold'] = array(
    '#type' => 'textfield',
    '#title' => t('Threshold'),
    '#default_value' => color_tagger_get_record('threshold', 10),
    '#size' => 2,
    '#maxlength' => 2,
    '#field_suffix' => '%',
    '#description' => t("Enter a percent threshold for tagging colors. Colors comprising less than this percent of the image will not be tagged. Leave blank to turn off."),
  );

  $form['force_process'] = array(
    '#type' => 'radios',
    '#title' => t('Force Image Processing'),
    '#description' => t("By default, Color Tagger will only process images for tagging if the taxonomy field is empty. Turning on this setting will force processing on every save action, even if the taxonomy field has values entered. This will cause any manually entered values to be overwritten."),
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => color_tagger_get_record('force_process', 0),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;

}

/**
 * Saves Color Tagger settings form values to the database.
 */
function color_tagger_admin_settings_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  try {
    color_tagger_set_record('use_lab', $values['use_lab']);
    color_tagger_set_record('ignore_background', $values['ignore_background']);
    color_tagger_set_record('color_limit', $values['color_limit']);
    color_tagger_set_record('threshold', $values['threshold']);
    color_tagger_set_record('force_process', $values['force_process']);
    drupal_set_message(t('Color Tagger settings saved.'));
  }
  catch (Exception $e) {
    drupal_set_message($e->getMessage(), 'error');
  }
}
