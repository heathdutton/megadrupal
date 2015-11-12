<?php

/**
 * @file
 * The theme system, which controls the output of the Skype status links and buttons.
 */

/**
 * Implements hook_theme().
 */
function skype_status_theme_theme() {
  $path = drupal_get_path('module', 'skype_status') . '/includes';
  return array(
    'skype_status' => array(
      'variables' => array('id' => NULL, 'type' => NULL, 'action' => NULL),
      'file' => 'skype_status.theme.inc',
      'path' => $path,
    ),
    'skype_status_image' => array(
      'variables' => array('id' => NULL, 'status' => array(), 'style' => NULL),
      'file' => 'skype_status.theme.inc',
      'path' => $path,
    ),
    'skype_status_block' => array(
      'variables' => array('id' => NULL, 'message' => NULL, 'action' => NULL),
      'file' => 'skype_status.theme.inc',
      'path' => $path,
    ),
  );
}

/**
 * Returns HTML for a Skype status link.
 *
 * @param $variables
 *   An associative array containing:
 *   - id: The Skype ID of the user.
 *   - type: The type of the requested link (user or block).
 *   - action: The action to perform when clicking on the link.
  *
 * @ingroup themeable
 */
function theme_skype_status($variables) {
  $id = $variables['id'];
  $type = $variables['type'];
  $action = $variables['action'];
  $encoded_id = rawurlencode($id);
  $class = 'skype-status-' . $type . '-link';
  // Don't use the more correct l() because our pseudo-protocol 'skype:' will get stripped.
  // Do not change rel and class attribute or it will break the status indicator.
  return '<a href="skype:' . $encoded_id . '?' . $action . '" class="' . $class . '" rel="' . $encoded_id . '">' . $id . '</a>';
}

/**
 * Returns HTML for a Skype status button.
 *
 * @param $variables
 *   An associative array containing:
 *   - id: The Skype ID of the user.
 *   - status: The status of the given Skype user.
 *   - style: The style of the Skype button.
 *
 * @ingroup themeable
 */
function theme_skype_status_image($variables) {
  $id = $variables['id'];
  $status = $variables['status'];
  $style = $variables['style'];

  $path = SKYPE_STATUS_IMAGE_PATH . '/' . $style . '/' . $status['num'] . '.png';
  $alt = t('Skype Status for @id', array('@id' => rawurldecode($id)));
  $title = (in_array($style, array('mediumicon', 'smallicon'))) ? $status['text'] : '';
  $attributes = array('style' => 'border:none;');

  return theme('image', array('path' => $path, 'alt' => $alt, 'title' => $title, 'attributes' => $attributes));
}

/**
 * Returns HTML for a Skype status block.
 *
 * @param $variables
 *   An associative array containing:
 *   - id: The Skype ID of the user.
 *   - message: Message to display with the status.
 *   - action: The action to perform when clicking on the link.
  *
 * @ingroup themeable
 */
function theme_skype_status_block($variables) {
  global $language;

  $id = $variables['id'];
  $message = $variables['message'];
  $action = $variables['action'];
  $skype_status = theme('skype_status', array('id' => $id, 'type' => 'block', 'action' => $action));

  $output = $skype_status;
  $output .= check_markup($message, NULL, $language->language);

  return $output;
}