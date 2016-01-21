<?php

/**
 * @file
 * Ctools content-type plugin to provide a whos.amung.us form
 *
 * @author Francisco J. Cruz Romanos <grisendo@gmail.com>
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("Whos.amung.us"),
  'description' => t("Whos.amung.us"),
  'category' => t('Widgets'),
  'defaults' => whos_amung_us_default_form_values(),
  'icon' => 'icon_whos_amung_us.png',
);

/**
 * Output function for the 'Whos.amung.us' content type.
 */
function whos_amung_us_ctools_whos_amung_us_content_type_render($subtype, $conf, $panel_args, $context) {

  $block = new stdClass();
  $block->subject = 'foo';
  $conf += whos_amung_us_default_form_values();
  $block->content = theme(
    'whos_amung_us',
    array(
      'widget' => $conf['widget'],
      'options' => $conf,
      'id' => 'ctl' . rand(1, 999),
    )
  );
  return $block;

}

/**
 * The form to add or edit Whos.amung.us widget.
 */
function whos_amung_us_ctools_whos_amung_us_content_type_edit_form($form, &$form_state) {

  module_load_include('inc', 'whos_amung_us', 'includes/whos_amung_us.admin');
  $form_state['conf'] += whos_amung_us_default_form_values();
  if (!isset($form_state['values']) || (!$form_state['values'])) {
    $form_state['values'] = $form_state['conf'];
  }
  $form += whos_amung_us_configuration_form(array(), $form_state, 'ctools');
  return $form;

}

/**
 * Save the Whos.amung.us widget.
 */
function whos_amung_us_ctools_whos_amung_us_content_type_edit_form_submit($form, &$form_state) {

  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    if (isset($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }

}

/**
 * Returns the administrative title for a Whos.amung.us widget.
 */
function whos_amung_us_ctools_whos_amung_us_content_type_admin_title($subtype, $conf, $context) {

  return t('Whos.amung.us: @widget', array('@widget' => $conf['widget']));

}

/**
 * Display the administrative information for a Whos.amung.us widget.
 */
function whos_amung_us_ctools_whos_amung_us_content_type_admin_info($subtype, $conf) {

  return '';

}
