<?php
/**
 * @file
 * The login content pane
 */

$plugin = array(
  'title' => t('WAYF.dk login'),
  'single' => TRUE,
  'category' => array(t('Widgets'), -9),
  'edit form' => 'wayf_dk_login_content_type_edit_form',
  'render callback' => 'wayf_dk_login_content_type_render',
  'defaults' => array('icon' => 'UK_01.png'),
);

/**
 * Run-time rendering of the body of the login block.
 */
function wayf_dk_login_content_type_render($subtype, $conf, $context = NULL) {

  // Don't show the login icon if a user is already logged in.
  global $user;
  if ($user->uid) {
    return;
  }

  $block = new stdClass();
  $icon_path = drupal_get_path('module', 'wayf_dk_login') . '/icons/';
  $icon_size = wayf_dk_login__icon_size(variable_get('wayf_dk_login_icon'));
  $block->content = theme('wayf_dk_login', array(
    'icon' => theme('image', array(
      'path' => $icon_path . $conf['icon'],
      'attributes' => array('width' => $icon_size->width, 'height' => $icon_size->height),
  ))));
  return $block;
}

/**
 * Edit form callback for the content type.
 */
function wayf_dk_login_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $icon_path = drupal_get_path('module', 'wayf_dk_login') . '/icons/';

  $icons = array();
  foreach (wayf_dk_login__icons() as $icon) {
    $icons[$icon] = theme('image', array('path' => $icon_path . $icon, 'attributes' => array('class' => 'wayf_dk_login-be-img')));
  }

  $form['settings']['icon'] = array(
    '#type' => 'radios',
    '#title' => t('Icon'),
    '#description' => t('Select icon.'),
    '#options' => $icons,
    '#default_value' => !empty($conf['icon']) ? $conf['icon'] : 'UK_01.png',
  );

  $form['#attached']['css'] = array(drupal_get_path('module', 'wayf_dk_login') . '/admin.css');

  return $form;
}


/**
 * Submit function.
 */
function wayf_dk_login_content_type_edit_form_submit(&$form, &$form_state) {
  $form_state['conf']['icon'] = $form_state['values']['icon'];
}
