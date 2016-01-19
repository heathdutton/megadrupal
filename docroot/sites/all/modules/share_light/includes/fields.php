<?php

/**
 * @file
 * Field related functions.
 */

use \Drupal\share_light\Loader;
use \Drupal\share_light\Block;

/**
 * Implements hook_field_info().
 */
function share_light_field_info() {
  $info['share_light'] = array(
    'label' => t('Share light'),
    'description' => t('Allows you to display a share block.'),
    'settings' => array('style' => NULL),
    'default_widget' => 'share_light',
    'default_formatter' => 'share_light',
  );
  return $info;
}


/**
 * Implements hook_field_presave().
 */
function share_light_field_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
  if ($field['type'] == 'share_light') {
    foreach ($items as &$item) {
      $item['options'] = serialize($item['options']);
    }
  }
}

/**
 * Implements hook_field_load().
 */
function share_light_field_load($entity_type, $entities, $field, $instances, $langcode, &$items, $age) {
  if ($field['type'] == 'share_light') {
    foreach ($entities as $id => $entity) {
      foreach ($items[$id] as &$item) {
        $item['options'] = unserialize($item['options']);
      }
    }
  }
}

/**
 * Implements hook_field_is_empty().
 */
function share_light_field_is_empty($item, $field) {
  return FALSE;
}

/**
 * Implements hook_field_widget_info().
 */
function share_light_field_widget_info() {
  $info['share_light'] = array(
    'label' => t('Share light'),
    'field types' => array('share_light'),
    'settings' => array('size' => 60),
    'behaviors' => array(
      'multiple values' => FIELD_BEHAVIOR_DEFAULT,
      'default values' => FIELD_BEHAVIOR_DEFAULT,
    ),
  );
  return $info;
}

/**
 * Implements hook_field_widget_form().
 */
function share_light_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $item = isset($items[$delta]) ? $items[$delta] : array();
  if (isset($instance['default_value'][$delta]) && !isset($items[$delta])) {
    $item = $instance['default_value'][$delta];
  }
  $item = drupal_array_merge_deep(array(
    'toggle' => variable_get('share_light_default_toggle', 1),
    'options' => Block::defaults(),
  ), $item);

  $toggle_id = drupal_html_id('share-light-widget-toggle');
  $element['#element_validate'][] = '_share_light_transform_options';
  $element['toggle'] = array(
    '#title' => t('Display a share block.'),
    '#description' => t('Display a share block.'),
    '#type' => 'checkbox',
    '#default_value' => $item['toggle'],
    '#attributes' => array('id' => $toggle_id),
  );

  $element['options'] = array(
    '#type' => 'container',
    '#states' => array(
      'visible' => array(
        '#' . $toggle_id => array('checked' => TRUE),
      ),
    ),
  );

  $element['options']['subject'] = array(
    '#title' => t('Title of the share-box.'),
    '#description' => t('The title is typically displayed right above the share buttons.'),
    '#type' => 'textfield',
    '#default_value' => $item['options']['subject'],
  );

  $element['options']['share_url'] = array(
    '#title' => t('URL to be shared.'),
    '#description' => t('URL to be shared. Leave this empty to share the current page.'),
    '#type' => 'textfield',
    '#size' => 60,
    '#default_value' => $item['options']['link']['path'],
  );

  $element['options']['image'] = array(
    '#title' => t('Image to be shared.'),
    '#description' => t('Image to be shared.'),
    '#type' => 'media',
    '#default_value' => $item['options']['image'],
    '#access' => FALSE, // TODO
  );

  $element['options']['counter_toggle'] = array(
    '#title' => t('Show counter (number of shares) along with share options'),
    '#description' => t('Show counter (number of shares) along with share options'),
    '#type' => 'checkbox',
    '#default_value' => $item['options']['counter_toggle'],
    '#access' => FALSE, // TODO
);

  $element['options']['channels'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced share options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $loader = Loader::instance();
  $available_channels = $loader->channelOptions();
  foreach ($available_channels as $channel_name => $title) {
    $ctoggle_id = drupal_html_id('share-light-channel-' . $channel_name . '-toggle');
    $element['options']['channels']['toggle_' . $channel_name] = array(
      '#title' => t('Show '.$title.' share button.'),
      '#description' => t('Enable '.$title.' on this page.'),
      '#type' => 'checkbox',
      '#default_value' => $item['options']['channels'][$channel_name]['toggle'],
      '#attributes' => array('id' => $ctoggle_id),
    );

    $element['options']['channels']['options_' . $channel_name] = array(
      '#type' => 'container',
      '#states' => array(
        'visible' => array(
          '#' . $ctoggle_id => array('checked' => TRUE),
        ),
      ),
    );
    $channel_options = &$element['options']['channels']['options_' . $channel_name];

    $class = $loader->channelClass($channel_name);
    $class::optionsWidget($channel_options, $item['options']['channels'][$channel_name]);
  }

  return $element;
}

function _share_light_transform_options($element, &$form_state, $form) {
  $values = &drupal_array_get_nested_value($form_state['values'], $element['#parents']);
  $loader = Loader::instance();
  $available_channels = $loader->channelOptions();

  $options = array();
  foreach ($available_channels as $name => $title) {
    $options[$name]['toggle'] = !empty($values['options']['channels']['toggle_' . $name]);
    if (isset($values['options']['channels']['options_' . $name])) {
      $options[$name] += $values['options']['channels']['options_' . $name];
    }
  }
  $values['options']['channels'] = $options;
  $values['options']['link']['path'] = $values['options']['share_url'];
  unset($values['options']['share_url']);
}


/**
 * Implements hook_field_formatter_info().
 */
function share_light_field_formatter_info() {
  $info['share_light'] = array(
    'label' => 'Share light',
    'field types' => array('share_light'),
  );
  return $info;
}
