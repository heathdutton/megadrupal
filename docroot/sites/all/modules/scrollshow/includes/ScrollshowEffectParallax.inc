<?php

define('SCROLLSHOW_EFFECT_PARALLAX_LAYERS', 5);

class ScrollshowEffectParallax extends ScrollshowEffect {
  function getSettingsDefaults() {
    $defaults = array();

    for ($i = 0; $i < SCROLLSHOW_EFFECT_PARALLAX_LAYERS; $i++) {
      $defaults["layer_{$i}_field"] = '';
      $defaults["layer_{$i}_direction"] = 'up';
      $defaults["layer_{$i}_speed"] = '100';
      $defaults["layer_{$i}_z_index"] = '10';
      $defaults["layer_{$i}_x"] = '0';
      $defaults["layer_{$i}_y"] = '100';
    }

    return $defaults;
  }

  function getSettingsForm($settings) {
    $field_options = array('' => t('none'));
    foreach (field_info_field_map() as $name => $info) {
      if ($info['type'] == 'image' && $info['bundles']['node']) {
        $field_options[$name] = t('!name (used on !types)',
          array('!name' => $name, '!types' => implode(', ', $info['bundles']['node'])));
      }
    }

    $form = array();
    for ($i = 0; $i < SCROLLSHOW_EFFECT_PARALLAX_LAYERS; $i++) {
      $di = $i + 1;
      $form["layer_{$i}_field"] = array(
        '#type' => 'select',
        '#title' => t('Layer !num field', array('!num' => $di)),
        '#description' => t('The name of the field to use for this layer.'),
        '#options' => $field_options,
        '#default_value' => $settings["layer_{$i}_field"],
      );
      $form["layer_{$i}_direction"] = array(
        '#type' => 'select',
        '#options' => array(
          'up' => t('Up'),
          'down' => t('Down'),
        ),
        '#title' => t('Layer !num direction', array('!num' => $di)),
        '#description' => t('Which direction the layer should move when the page scrolls.'),
        '#default_value' => $settings["layer_{$i}_direction"],
      );
      $form["layer_{$i}_z_index"] = array(
        '#type' => 'select',
        '#title' => t('Layer !num z-index', array('!num' => $di)),
        '#options' => array(
          '10'  => t('Above'),
          '-10' => t('Below'),
        ),
        '#description' => t('Determines if this layer appear above or below the slide content.'),
        '#default_value' => $settings["layer_{$i}_z_index"],
      );
      $form["layer_{$i}_speed"] = array(
        '#type' => 'textfield',
        '#title' => t('Layer !num speed', array('!num' => $di)),
        '#field_suffix' => '%',
        '#description' => t('A percentage denoting how fast to move the layer in relation to the scroll movement. For example, 100% means move the layer 1 pixel for every 1 pixel of scroll movement.'),
        '#default_value' => $settings["layer_{$i}_speed"],
      );
      $form["layer_{$i}_x"] = array(
        '#type' => 'textfield',
        '#title' => t('Layer !num X position', array('!num' => $di)),
        '#field_suffix' => '%',
        '#description' => t('A percentage denoting the layer\'s starting X position in relation to the slide. 0% is the top, 100% is the bottom.'),
        '#default_value' => $settings["layer_{$i}_x"],
      );
      $form["layer_{$i}_y"] = array(
        '#type' => 'textfield',
        '#title' => t('Layer !num Y position', array('!num' => $di)),
        '#field_suffix' => '%',
        '#description' => t('A percentage denoting the layer\'s starting Y position in relation to the slide. 0% is the left, 100% is the right.'),
        '#default_value' => $settings["layer_{$i}_y"],
      );
    }
    return $form;
  }

  function prepareSettingsForJavascript($settings, $items) {
    $settings_fields = array('field', 'x', 'y', 'z_index', 'direction', 'speed');

    // make the settings 'deep' so they're easier to work with
    $settings['layers'] = array();
    for ($i = 0; $i < SCROLLSHOW_EFFECT_PARALLAX_LAYERS; $i++) {
      foreach ($settings_fields as $item) {
        if (in_array($item, array('x', 'y', 'speed', 'z_index'))) {
          $settings['layers'][$i][$item] = (int)$settings["layer_{$i}_{$item}"];
        }
        else {
          $settings['layers'][$i][$item] = $settings["layer_{$i}_{$item}"];
        }
      }
    }

    // get the image URLs from the actual entities
    $settings['slides'] = array();
    $slide_index = 0;
    foreach ($items as $item) {
      if (!empty($item['content'])) {
        if (!empty($item['entity'])) {
          $entity = $item['entity'];
          for ($i = 0; $i < SCROLLSHOW_EFFECT_PARALLAX_LAYERS; $i++) {
            $field_name = $settings["layer_{$i}_field"];
            if (!empty($field_name) && !empty($entity->$field_name)) {
              $field = $entity->$field_name;
              $settings['slides'][$slide_index]['layers'][$i] =
                file_create_url($field[LANGUAGE_NONE][0]['uri']);
            }
          }
        }
        $slide_index++;
      }
    }

    $settings['max_layers'] = SCROLLSHOW_EFFECT_PARALLAX_LAYERS;

    return $settings;
  }

  function getJS($settings) {
    return array(
      drupal_get_path('module', 'scrollshow') . '/js/scrollshow.parallax.js',
    );
  }
}

