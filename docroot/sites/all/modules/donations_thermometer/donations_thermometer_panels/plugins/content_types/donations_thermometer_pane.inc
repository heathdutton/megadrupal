<?php
/**
 * @file
 */

$plugin = array(
  'single' => TRUE,  // Just do this one, it is needed.
  'title' => t('Donations Thermometer Pane'),  // Title to show up on the pane screen.
  'description' => t('Adds a donation thermometer'), // Description to show up on the pane screen.
  'category' => t('Thermometer'), // A category to put this under.
  'edit form' => 'donations_thermometer_panels_pane_custom_pane_edit_form', // A function that will return the settings form for the pane.
  'render callback' => 'donations_thermometer_panels_pane_custom_pane_render', // A function that will return the renderable content.
  'admin info' => 'donations_thermometer_panels_pane_custom_pane_admin_info', // A function that will return the information displayed on the admin screen (optional).
  'defaults' => array( // Array of defaults for the settings form.
    'donations_thermometer_target' => 100,
    'donations_thermometer_current' => 50,
    'donations_thermometer_prefix' => '$',
    'donations_thermometer_postfix' => '',
    'donations_thermometer_size' => 'large',
  ),
  'all contexts' => TRUE, // This is NEEDED to be able to use substitution strings in your pane.
);

/**
 * An edit form for the pane's settings.
 */
function donations_thermometer_panels_pane_custom_pane_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
 
  $form['donations_thermometer_target'] = array(
    '#type' => 'textfield',
    '#title' => t('Target'),
    '#default_value' => $conf['donations_thermometer_target'],
    '#size' => 9,
    '#maxlength' => 10,
    '#description' => t("The target."),
    '#required' => TRUE,
  );
  $form['donations_thermometer_current'] = array(
    '#type' => 'textfield',
    '#title' => t('Current number'),
    '#default_value' => $conf['donations_thermometer_current'],
    '#size' => 9,
    '#maxlength' => 10,
    '#description' => t("The current number."),
    '#required' => TRUE,
  );
  $form['donations_thermometer_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Prefix'),
    '#default_value' => $conf['donations_thermometer_prefix'],
    '#size' => 9,
    '#maxlength' => 10,
    '#description' => t("A prefix."),
    '#required' => FALSE,
  );
  $form['donations_thermometer_postfix'] = array(
    '#type' => 'textfield',
    '#title' => t('Postfix'),
    '#default_value' => $conf['donations_thermometer_postfix'],
    '#size' => 9,
    '#maxlength' => 10,
    '#description' => t("A postfix."),
    '#required' => FALSE,
  );
  $form['donations_thermometer_size'] = array(
    '#type' => 'select',
    '#title' => t('Size'),
    '#default_value' => $conf['donations_thermometer_size'],
    '#options' => array(
      'large' => t('Large'),
      'small' => t('Small'),
    ),
    '#description' => t("Choose what size to display the thermometer.")
  );

  return $form;
}
 
/**
 * Submit function, note anything in the formstate[conf] automatically gets saved
 * Notice, the magic that automatically does that for you.
 */
function donations_thermometer_panels_pane_custom_pane_edit_form_submit(&$form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    if (isset($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }
}

/**
 * Run-time rendering of the body of the block (content type)
 * See ctools_plugin_examples for more advanced info
 */
function donations_thermometer_panels_pane_custom_pane_render($subtype, $conf, $args, $contexts) {
  $current = $conf['donations_thermometer_current'];
  $target = $conf['donations_thermometer_target'];
  $prefix = $conf['donations_thermometer_prefix'];
  $postfix = $conf['donations_thermometer_postfix'];
  $size = $conf['donations_thermometer_size'];

  $block = new stdClass();
 
  // initial content is blank
  $block->title = t('Progress thermometer'); // This will be overridden by the user within the panel options.
  $block->content = theme('donations_thermometer', array('current' => $current, 'target' => $target, 'prefix' => $prefix, 'postfix' => $postfix, 'size' => $size));
 
  return $block;
}

/**
 * 'admin info' callback for panel pane.
 */
function donations_thermometer_panels_pane_custom_pane_admin_info($subtype, $conf, $contexts) {
  if (!empty($conf)) {
    $block = new stdClass;
    $block->title = $conf['override_title'] ? $conf['override_title_text'] : '';
    $block->content = $conf['text'];
    return $block;
  }
}