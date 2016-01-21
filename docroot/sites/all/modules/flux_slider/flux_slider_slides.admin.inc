<?php
/**
 * @file
 * Generate configuration form and save settings.
 */

/**
 * Configuration form for slider settings.
 */
function flux_slider_slide_configuration_form($form, &$form_state) {
  // Get the path to the module.
  $path = drupal_get_path('module', 'flux_slider');
  // Attach the CSS to the form.
  $form['#attached'] = array(
    'css' => array(
      'type' => 'file',
      'data' => $path . '/css/form_theme.css',
    ),
  );

  $form['slider_imgs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Images in use for slider'),
    '#description' => t('Currently this module only has support for 4 images to be contained within the slider element.'),
    '#weight' => 1,
    '#collapsible' => TRUE,
  );

  $form['slider_imgs']['slider_imgs_first'] = array(
    '#type' => 'fieldset',
    '#title' => t('First image'),
    '#weight' => 2,
    '#collapsible' => TRUE,
  );

  // First image field.
  $form['slider_imgs']['slider_imgs_first']['first_img'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('first_img', $path . '/images/slider-img-1.jpg'),
    '#title' => t('First image path'),
    '#size' => 60,
    '#weight' => 3,
  );
  // First caption field.
  $form['slider_imgs']['slider_imgs_first']['first_capt'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('first_capt', 'This is the first caption'),
    '#title' => t('First caption'),
    '#size' => 40,
    '#weight' => 4,
  );
  // First alt text field.
  $form['slider_imgs']['slider_imgs_first']['first_alt'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('first_alt', 'This is the first alt text'),
    '#title' => t('Alternative text for the first image'),
    '#size' => 40,
    '#weight' => 5,
  );

  $form['slider_imgs']['slider_imgs_second'] = array(
    '#type' => 'fieldset',
    '#title' => t('second Image'),
    '#weight' => 6,
    '#collapsible' => TRUE,
  );
  // Second image field.
  $form['slider_imgs']['slider_imgs_second']['second_img'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('second_img', $path . '/images/slider-img-2.jpg'),
    '#title' => t('Second image path'),
    '#size' => 60,
    '#weight' => 7,
  );
  // Second caption field.
  $form['slider_imgs']['slider_imgs_second']['second_capt'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('second_capt', 'This is the second caption'),
    '#title' => t('Second caption'),
    '#size' => 40,
    '#weight' => 8,
  );
  // Second alt text field.
  $form['slider_imgs']['slider_imgs_second']['second_alt'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('second_alt', 'This is the second alt text'),
    '#title' => t('Alternative text for the second image'),
    '#size' => 40,
    '#weight' => 9,
  );

  $form['slider_imgs']['slider_imgs_third'] = array(
    '#type' => 'fieldset',
    '#title' => t('Third image'),
    '#weight' => 8,
    '#collapsible' => TRUE,
  );
  // Third image field.
  $form['slider_imgs']['slider_imgs_third']['third_img'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('third_img', $path . '/images/slider-img-3.jpg'),
    '#title' => t('Third image path'),
    '#size' => 60,
    '#weight' => 10,
  );
  // Third caption field.
  $form['slider_imgs']['slider_imgs_third']['third_capt'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('third_capt', 'This is the third caption'),
    '#title' => t('Third caption'),
    '#size' => 40,
    '#weight' => 11,
  );
  // Third alt text field.
  $form['slider_imgs']['slider_imgs_third']['third_alt'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('third_alt', 'This is the third alt text'),
    '#title' => t('Alternative text for the third image'),
    '#size' => 40,
    '#weight' => 12,
  );

  $form['slider_imgs']['slider_imgs_fourth'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fourth image'),
    '#weight' => 13,
    '#collapsible' => TRUE,
  );
  // Fourth image field.
  $form['slider_imgs']['slider_imgs_fourth']['fourth_img'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('fourth_img', $path . '/images/slider-img-4.jpg'),
    '#title' => t('Fourth image path'),
    '#size' => 60,
    '#weight' => 14,
  );
  // Fourth caption field.
  $form['slider_imgs']['slider_imgs_fourth']['fourth_capt'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('fourth_capt', 'This is the fourth caption'),
    '#title' => t('Fourth caption'),
    '#size' => 40,
    '#weight' => 15,
  );
  // Fourth alt text field.
  $form['slider_imgs']['slider_imgs_fourth']['fourth_alt'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('fourth_alt', 'This is the fourth alt text'),
    '#title' => t('Alternative text for the fourth image'),
    '#size' => 40,
    '#weight' => 16,
  );
  return system_settings_form($form);
}
