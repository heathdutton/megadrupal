<?php
/**
 * @file
 * CTools content_type plugin, displaying a Classified Ad expiration info.
 *
 * @copyright (c) 2011 Ouest Systemes Informatiques (OSInet)
 *
 * @license General Public License version 2 or later
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'single' => TRUE,
  'title' => t('Classified Ad expiration'),
  'icon' => 'icon_cck_field.png',
  'description' => t('The expiration of the referenced Classified Ad.'),
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'category' => t('Node'),
  'defaults' => array(
    'link' => 1,
    'format' => 'remaining',
  ),
);

/**
 * Helper to provide consistent naming of format options.
 *
 * @return array
 *   An array of expiration formats, keyed by machine name.
 */
function _classified_classified_expires_option_values() {
  $ret = array(
    'date' => t('Date'),
    'percent' => t('% remaining'),
    'duration' => t('Time remaining'),
    'formatted' => t('Themed expiration'),
  );
  return $ret;
}

/**
 * Implements hook_PLUGIN_content_type_render().
 *
 * Render the custom content type.
 */
function classified_classified_expires_content_type_render($subtype, $conf, $panel_args, $context) {
  if (empty($context) || empty($context->data)) {
    return;
  }

  // Get a shortcut to the node.
  $node = $context->data;
  if ($node->type != 'classified') {
    return;
  }

  // Build the content type block.
  $block = new stdClass();
  $block->module  = 'classified';
  $block->delta   = $node->nid;
  $variables = array(
    'node' => $node,
  );

  if (empty($conf['format'])) {
    $conf['format'] = 'formatted';
  }

  switch ($conf['format']) {
    case 'date':
      $block->title = t('Ad Expiration date');
      classified_preprocess_classified_expires($variables);
      $block->content = t('@date', array('@date' => $variables['expires']));
      break;

    case 'percent':
      $block->title = t('Ad Lifetime, remaining');
      classified_preprocess_classified_expires($variables);
      $block->content = t('@percent%', array('@percent' => $variables['remaining_ratio']));
      break;

    case 'duration':
      $block->title = t('Ad Lifetime, remaining');
      classified_preprocess_classified_expires($variables);
      $block->content = t('@remaining', array('@remaining' => $variables['remaining']));
      break;

    case 'formatted':
    default:
      $block->title = NULL;
      $block->content = theme('classified_expires', array('node' => $node));
      break;
  }

  if (!empty($conf['link'])) {
    $block->content = l($block->content, 'node/' . $node->nid, array('html' => TRUE));
  }
  return $block;
}

/**
 * Implements hook_PLUGIN_content_type_edit_form().
 *
 * Returns an edit form for custom type settings.
 */
function classified_classified_expires_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['link'] = array(
    '#title' => t('Link'),
    '#type' => 'checkbox',
    '#default_value' => $conf['link'],
    '#description' => t('Check here to make the Ad expiration link to the node.'),
  );

  $form['format'] = array(
    '#title' => t('Format'),
    '#type' => 'radios',
    '#default_value' => $conf['format'],
    '#options' => _classified_classified_expires_option_values(),
    '#description' => t('Choose an expiration format.'),
  );

  return $form;
}

/**
 * Submit handler for the custom type settings form.
 */
function classified_classified_expires_content_type_edit_form_submit(&$form, &$form_state) {
  // Copy everything from our defaults.
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/**
 * Implements hook_PLUGIN_content_type_admin_info().
 *
 * Provides a summary view of the current configuration.
 */
function classified_classified_expires_content_type_admin_info($subtype, $conf) {
  $values = _classified_classified_expires_option_values();
  $format = isset($values[$conf['format']])
    ? $values[$conf['format']]
    : $vales['formatted'];

  $ret = new stdClass();
  $ret->title = t('@format - @link', array(
    '@format' => $format,
    '@link' => $conf['link'] ? t('Linking') : t('Not linking'),
  ));
  $ret->content = NULL;
  return $ret;
}

/**
 * Implements hook_PLUGIN_content_type_admin_title().
 *
 * Returns the administrative title for an Ad expiration.
 */
function classified_classified_expires_content_type_admin_title($subtype, $conf, $context) {
  return t('"@s" expiration (Classified Ads)', array('@s' => $context->identifier));
}
