<?php
/**
 * @file iframe.content_migrate.iframe.inc
 * Code to implement Content Migrate hooks on behalf of the Iframe module.
 */

/**
 * Implements hook_content_migrate_field_alter().
 *
 * Use this to tweak the conversion of field settings
 * from the D6 style to the D7 style for specific
 * situations not handled by basic conversion,
 * as when field types or settings are changed.
 */
function iframe_content_migrate_field_alter(&$field_value, $instance_value) {
  // deal only with iframe fields
  if ($field_value['module'] == 'iframe') {
    #iframe_debug(0, 'iframe_content_migrate_field_alter', array($field_value, $instance_value));

    #$field_value['messages'][] = t("Changed field type: The '@field' field type will be changed from '@type' to 'iframe'.", array('@type' => $field_value['type'], '@field' => $field_value['field_name'], '@widget' => $instance_value['widget']['type']));
    $field_value['module'] = 'iframe';
    $field_value['type'] = 'iframe';
    // settings are now a field settings.
    if (isset($field_value['settings']) && is_array($field_value['settings']['attributes'])) {
      $default_attributes = array();
      foreach(array('class', 'width', 'height', 'frameborder', 'scrolling', 'transparency', 'tokensupport') as $attr) {
        if (isset($field_value['settings']['attributes'][$attr])) {
          $field_value['settings'][$attr] = $field_value['settings']['attributes'][$attr];
          $default_attributes[$attr] = $field_value['settings']['attributes'][$attr];
        }
      }
      unset($field_value['settings']['attributes']);
      $field_value['messages'][] = t("Transfer of default attributes: The '@field' will get following default attributes: @attributes", array('@field' => $field_value['field_name'], '@widget' => $instance_value['widget']['type'], '@attributes' => print_r($default_attributes, TRUE)));
    }
  }
}

/**
 * Implements hook_content_migrate_instance_alter().
 *
 * Use this to tweak the conversion of instance or widget settings
 * from the D6 style to the D7 style for specific
 * situations not handled by basic conversion, as when
 * formatter or widget names or settings are changed.
 */
function iframe_content_migrate_instance_alter(&$instance_value, $field_value) {
  // Translate formatters.
  if ($instance_value['widget']['type'] == 'iframe') {
    #iframe_debug(0, 'iframe_content_migrate_instance_alter', array($instance_value, $field_value));

    foreach ($instance_value['display'] as $context => $settings) {
      switch ($settings['type']) {
        case 'asurl':
          $instance_value['display'][$context]['type'] = 'iframe_formatter_asurl';
          break;

        case 'asurlwithuri':
        case 'asurl_withuri':
          $instance_value['display'][$context]['type'] = 'iframe_formatter_asurlwithuri';
          break;

        case 'iframeonly':
        case 'only':
          $instance_value['display'][$context]['type'] = 'iframe_formatter_only';
          break;

        case 'default':
          $instance_value['display'][$context]['type'] = 'iframe_formatter_default';
          break;

        case 'hidden':
          $instance_value['display'][$context]['type'] = 'iframe_formatter_hidden';
          $instance_value['display'][$context]['label'] = 'hidden';
          break;

        default:
          // Handle formatters using imagecache. D7 does not store them in the same way.
          $instance_value['display'][$context]['type'] = 'iframe_formatter_only';
      }
    }
  }

  // Translate the original widgets.
  $instance_value['widget']['type'] = 'iframe_widget_urlwidthheight';

  if (isset($instance_value['default_value'][0]) && is_array($instance_value['default_value'][0]['attributes'])) {
    // settings are now a instance settings.
    $settings =& $instance_value['default_value'][0];
    foreach(array('class', 'width', 'height', 'frameborder', 'scrolling', 'transparency', 'tokensupport') as $attr) {
      if (isset($settings['attributes'][$attr])) {
        $field_value['settings'][$attr] = $settings['attributes'][$attr];
        $default_attributes[$attr] = $settings['attributes'][$attr];
      }
    }
    unset($instance_value['default_value'][0]['attributes']);
  } 
}


/**
 * Implements hook_content_migrate_data_record_alter().
 *
 * Tweaks individual records in a field.
 */
function iframe_content_migrate_data_record_alter(&$record, $field, $instance) {
  switch($field['type']) {
    case 'iframe':
      #drupal_set_message(print_r($record, TRUE), 'status');
      #return;

      // Map D6 attributes to D7 iframe columns.
      if (!empty($record[$field['field_name'] . '_attributes'])) {
        try {
          $attributes = unserialize($record[$field['field_name'] . '_attributes']);
        }
        catch (Exception $e) {
          $attributes = array();
        }
      }
      foreach(array('class', 'width', 'height', 'frameborder', 'scrolling', 'transparency', 'tokensupport') as $attr) {
        if (isset($attributes[$attr])) {
          $record[$field['field_name'] . '_' . $attr] = (string)$attributes[$attr];
        }
        else {
          if (isset($record[$field['field_name'] . '_' . $attr])) {
            unset($record[$field['field_name'] . '_' . $attr]);
          }
        }
      }
      if (isset($record[$field['field_name'] . '_attributes'])) {
        unset($record[$field['field_name'] . '_attributes']);
      }
      break;
  }
}

