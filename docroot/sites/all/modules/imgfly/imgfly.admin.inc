<?php
/**
 * @file
 * Administrative page callbacks for the imgfly module.
 */

/**
 * General configuration form for controlling the cxense behaviour.
 */
function imgfly_admin() {
  $form = array();

  $form['imgfly_suffix'] = array(
    '#type' => 'textfield',
    '#title' => t('The preferred suffix for your responsive image styles'),
    '#default_value' => variable_get('imgfly_suffix', '_imgfly'),
    '#size' => 12,
    '#maxlength' => 12,
    '#required' => TRUE,
    '#description' => t("Create your image styles with this suffix for imgfly
      to be able to handle them and make them responsive."),
  );

  $form['imgfly_max_dim'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum allowed dimension'),
    '#default_value' => variable_get('imgfly_max_dim', '1000x1000'),
    '#size' => 12,
    '#maxlength' => 12,
    '#required' => TRUE,
    '#description' => t("The maximum allowed dimension. Any dimension request
      greater than this will be ignored. Specify in the format widthxheight.
      Example: 1000x1000"),
  );

  $form['imgfly_lazyload'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use lazy loading of images?'),
    '#default_value' => variable_get('imgfly_lazyload', '0'),
    '#return_value' => 1,
    '#description' => t("If checked, this allows images to be lazy loaded."),
  );

  $form['imgfly_loadvalue'] = array(
    '#type' => 'textfield',
    '#title' => t('Preferred length below viewport to start loading.'),
    '#default_value' => variable_get('imgfly_loadvalue', '2000'),
    '#size' => 6,
    '#maxlength' => 6,
    '#description' => t("The preferred length below the viewport where the 
      images should start loading."),
  );

  return system_settings_form($form);
}
