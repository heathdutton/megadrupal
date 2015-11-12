<?php

/**
 * @file
 * Definition of the 'zone' panel style.
 */

// Plugin definition
$plugin = array(
  'title' => t('ZoneStrap'),
  'description' => t('Bootstrap column style config.'),
  'settings form' => 'ctools_zonestyle_style_settings_form',
  'render region' => 'ctools_zonestyle_style_render_region',
);

/**
 * Render callback.
 *
 * @ingroup themeable
 */
function theme_ctools_zonestyle_style_render_region($vars) {
  if (empty($vars['panes'])) {
    return FALSE;
  }

  $settings  = $vars['settings'];
  $classes   = array();
  if (!empty($settings['bootstrap']['css'])) {
    $classes[] = check_plain($settings['bootstrap']['css']);
  }

  foreach (array('xs', 'sm', 'md', 'lg') as $key) {
    if (!empty($settings['bootstrap'][$key])) {
      $classes[] = "col-{$key}-" . $settings['bootstrap'][$key];
    }
  }
  $element['#prefix'] = @$settings['bootstrap']['wrapper']['prefix'] ?: "";
  $element['#suffix'] = @$settings['bootstrap']['wrapper']['suffix'] ?: "";
  $element['#attributes'] = array('class' => $classes);
  $element['#children'] = implode($vars['panes']);

  return '<div' . drupal_attributes($element['#attributes']) . '>' . $element['#prefix'] . $element['#children'] . $element['#suffix'] . '</div>';
}

/**
 * Settings form callback.
 */
function ctools_zonestyle_style_settings_form($style_settings) {

  $bootstrap_cols = range(0, 12);
  unset($bootstrap_cols[0]);

  $form['bootstrap'] = array(
    '#type'        => 'fieldset',
    '#tree'        => TRUE,
    '#title'       => t("Column Style"),
    '#collapsible' => FALSE,
    '#collapsed'   => FALSE
  );

  $form['bootstrap']['xs'] = array(
    '#type'          => 'select',
    '#required'      => FALSE,
    '#title'         => t('Extra small'),
    //'#description'   => t('Select column for extra small devices'),
    '#default_value' => @$style_settings['bootstrap']['xs'] ?: "",
    '#empty_option'  => t('- Select -'),
    '#options'       => $bootstrap_cols,
  );

  $form['bootstrap']['sm'] = array(
    '#type'          => 'select',
    '#required'      => FALSE,
    '#title'         => t('Small'),
    //'#description'   => t('Select column for small devices'),
    '#default_value' => @$style_settings['bootstrap']['sm'] ?: "",
    '#empty_option'  => t('- Select -'),
    '#options'       => $bootstrap_cols,
  );

  $form['bootstrap']['md'] = array(
    '#type'          => 'select',
    '#required'      => FALSE,
    '#title'         => t('Medium'),
    //'#description'   => t('Select column for medium devices'),
    '#default_value' => @$style_settings['bootstrap']['md'] ?: "",
    '#empty_option'  => t('- Select -'),
    '#options'       => $bootstrap_cols,
  );

  $form['bootstrap']['lg'] = array(
    '#type'          => 'select',
    '#required'      => FALSE,
    '#title'         => t('Large'),
    //'#description'   => t('Select column for medium devices'),
    '#default_value' => @$style_settings['bootstrap']['lg'] ?: "",
    '#empty_option'  => t('- Select -'),
    '#options'       => $bootstrap_cols,
  );

  $form['bootstrap']['css'] = array(
    '#type'          => 'textfield',
    '#required'      => FALSE,
    '#title'         => t('Extra CSS'),
    '#description'   => t('Add custom css to region'),
    '#default_value' => @$style_settings['bootstrap']['css'] ?: "",
  );

  $form['bootstrap']['wrapper'] = array(
    '#type'        => 'fieldset',
    '#tree'        => TRUE,
    '#title'       => t("Wrapper"),
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE
  );

  $form['bootstrap']['wrapper']['prefix'] = array(
    '#type'          => 'textarea',
    '#required'      => FALSE,
    '#title'         => t('Prefix'),
    '#default_value' => @$style_settings['bootstrap']['wrapper']['prefix'] ?: "",
  );

  $form['bootstrap']['wrapper']['suffix'] = array(
    '#type'          => 'textarea',
    '#required'      => FALSE,
    '#title'         => t('Suffix'),
    '#default_value' => @$style_settings['bootstrap']['wrapper']['suffix'] ?: "",
  );

  return $form;
}