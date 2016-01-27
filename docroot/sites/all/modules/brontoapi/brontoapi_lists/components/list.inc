<?php

/**
 * @file
 * Webform module Bronto API list component.
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_brontoapi_list() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'required' => 0,
    'pid' => 0,
    'weight' => 0,
    'extra' => array(
      'description' => '',
      'brontoapi_list' => '',
      'private' => FALSE,
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_brontoapi_list($component) {

  if ($lists = brontoapi_lists_read_lists()) {
    $options = array('' => t('None'));
    foreach ($lists as $list) {
      $options[$list->id] = $list->label;
    }

    $form = array();

    $form['extra']['brontoapi_list'] = array(
      '#title' => t('Select from Bronto API lists'),
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => $component['extra']['brontoapi_list'],
      '#weight' => 1,
      '#description' => t('Here you can select which particular Bronto API list you want this component to send to.'),
    );
  }

  return $form;
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_brontoapi_list($component, $value = NULL, $filter = TRUE) {
  $list = $component['extra']['brontoapi_list'];
  $element = drupal_get_form("brontoapi_lists_sign_up_form", $list, "webform");
  $element['brontoapi_list_sign_up_' . $list]['#title'] = $component['name'];
  $element['brontoapi_list_sign_up_' . $list]['#title_display'] = $component['extra']['title_display'];
  return $element;
}

/**
 * Implements _webform_submit_component().
 */
function _webform_submit_brontoapi_list($component, $value) {
  global $user;
  $list = $component['extra']['brontoapi_list'];
  if (reset($value) == 1) {
    brontoapi_lists_add_to_lists($user->mail, array($list));
  }
  elseif (reset($value) == 0) {
    brontoapi_lists_remove_from_lists($email, array($list));
  }
  return reset($value);
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_brontoapi_list($component, $value, $format = 'html') {
  return array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#theme' => 'webform_display_brontoapi_list',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#format' => $format,
    '#value' => $value,
  );
}

/**
 * Format the text output for this component.
 */
function theme_webform_display_brontoapi_list($variables) {
  $output = '';
  if (isset($variables['element']['#value'])) {
    $value = reset($variables['element']['#value']);
  }
  else {
    $value = NULL;
  }
  if ($value == 1) {
    $output = "Opt-in";
  }
  elseif ($value == 0) {
    $output = "Opt-out";
  }

  return $output;
}

/**
 * Implements _webform_theme_component().
 */
function _webform_theme_brontoapi_list() {
  return array(
    'webform_display_brontoapi_list' => array(
      'render element' => 'element',
    ),
  );
}
