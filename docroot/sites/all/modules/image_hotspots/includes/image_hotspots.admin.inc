<?php

/**
 * Builds Hotspots-style configuration form.
 */
function image_hotspots_style_config($form, &$form_state) {
  $default_settings = variable_get('image_hotspots_settings', image_hotspots_default());
  $presets = count($default_settings);
  $form_state['storage']['hotspot_style'] = isset($form_state['storage']['hotspot_style']) ? $form_state['storage']['hotspot_style'] : 0;

  $form['color_example'] = array(
    '#prefix' => '<div class="hotspots_example">',
    '#suffix' => '</div>',
    '#attached' => array(
      'css' => array(drupal_get_path('module', 'image_hotspots') . '/themes/image_hotspots.css'),
      'js' => array(drupal_get_path('module', 'image_hotspots') . '/themes/hotspots_color_adm.js'),
    ),
  );
  $form['old_style'] = array(
   '#type' => 'container',
    '#tree' => TRUE,
    '#prefix' => '<div id="old_style">',
    '#suffix' => '</div>',
  );
  $form['hotspot_style'] = array(
   '#type' => 'container',
    '#tree' => TRUE,
    '#prefix' => '<div id="hotspot_style">',
    '#suffix' => '</div>',
  );
  if (count($presets) > 0) {
    foreach ($default_settings as $setting => $values) {
      $form['old_style'][$setting] = image_hotspots_build_row();
      $form['old_style'][$setting]['#states'] = array(
        'disabled' => array(':input[name="old_style[' . $setting . '][remove]"]' => array('checked' => TRUE)),
        'visible' => array(':input[name="old_style[' . $setting . '][remove]"]' => array('checked' => FALSE)),
      );
      $form['old_style'][$setting]['border']['width']['#states'] = array(
        'disabled' => array(':input[name="old_style[' . $setting . '][border][style]"]' => array('value' => 'none')),
      );
      $form['old_style'][$setting]['border']['color']['#states'] = $form['old_style'][$setting]['border']['width']['#states'];

      // @todo: Make it more cleanly.
      $form['old_style'][$setting]['color']['#value'] = (isset($values['background-color'])) ? $values['background-color'] : '';
      $form['old_style'][$setting]['opacity']['#value'] = (isset($values['opacity'])) ? $values['opacity'] : '';
      $form['old_style'][$setting]['border']['color']['#value'] = (isset($values['border']['color'])) ? $values['border']['color'] : '';
      $form['old_style'][$setting]['border']['width']['#value'] = (isset($values['border']['width'])) ? $values['border']['width'] : '';
      $form['old_style'][$setting]['border']['style']['#default_value'] = (isset($values['border']['style'])) ? $values['border']['style'] : '';
    }
  }
  if ($form_state['storage']['hotspot_style']) {
    for ($i = 1; $i <= $form_state['storage']['hotspot_style']; $i++) {
      $form['hotspot_style'][$i] = image_hotspots_build_row();
      $form['hotspot_style'][$i]['#states'] = array(
        'disabled' => array(':input[name="hotspot_style[' . $i . '][remove]"]' => array('checked' => TRUE)),
        'visible' => array(':input[name="hotspot_style[' . $i . '][remove]"]' => array('checked' => FALSE)),
      );
      $form['hotspot_style'][$i]['border']['width']['#states'] = array(
        'disabled' => array(':input[name="hotspot_style[' . $i . '][border][style]"]' => array('value' => 'none')),
      );
      $form['hotspot_style'][$i]['border']['color']['#states'] = $form['hotspot_style'][$i]['border']['width']['#states'];
    }
  }
  $form['add_participant'] = array(
    '#type' => 'button',
    '#value' => t('+'),
    '#href' => '',
    '#ajax' => array(
      'callback' => 'image_hotspots_add_row',
      'wrapper' => 'hotspot_style',
    ),
  );
  $form['submit'] = array(
    '#value' => t('Save'),
    '#type' => 'submit', 
  );
  $form['default'] = array(
    '#value' => t('Default'),
    '#type' => 'submit', 
  );

  $form_state['storage']['hotspot_style']++;

  return $form;
}

/**
 * Adds new color row into form.
 */
function image_hotspots_add_row($form, $form_state) {
  foreach ($form_state['values']['hotspot_style'] as $row => $value) {
    if (empty($value['color'])
      && (empty($value['border']['color'])
      || intval($value['border']['width']) == 0)
      && $row != count($form_state['values']['hotspot_style'])
    ) {
      unset($form['hotspot_style'][$row]);
    }
  }
  return $form['hotspot_style'];
}

/**
 * Hotspots-style configuration submit.
 */
function image_hotspots_style_config_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#value'] == 'Default') {
    $style_settings = image_hotspots_default();
  }
  else {
    $style_settings = array();
    $old = (isset($form_state['input']['old_style'])) ? $form_state['input']['old_style'] : array();
    $new = (isset($form_state['input']['hotspot_style'])) ? $form_state['input']['hotspot_style'] : array();
    $form_state['input']['hotspot_style'] = array_merge($old, $new);
    foreach ($form_state['input']['hotspot_style'] as $row => $value) {
      if ($value['remove'] != TRUE) {
        $row_settings = array();
        if (!empty($value['color'])) {
          $row_settings['background-color'] = str_replace('#', '', $value['color']);
        }
        if (!empty($value['border']['color']) && intval($value['border']['width']) > 0) {
          $value['border']['width'] = intval($value['border']['width']);
          $value['border']['color'] = str_replace('#', '', $value['border']['color']);
          $row_settings['border'] = $value['border'];
        }
        if (count($row_settings) != 0) {
          $row_settings['opacity'] = floatval($value['opacity']);
          $style_settings[] = $row_settings;
        }
      }
    }
  }
  variable_set('image_hotspots_settings', $style_settings); 
}

/**
 * Pattern to build a row with the style elements.
 */
function image_hotspots_build_row() {
  $row = array(
    '#attributes' => array('class' => array('container-inline')),
    '#type' => 'fieldset',
  ); 
  $row['color'] = array(
    '#type' => 'textfield',
    '#size' => 8,
    '#maxlength' => 6,
    '#attributes' => array('placeholder' => t('color'), 'class' => array('hotspot_color_field')),
  );
  $row['border'] = array(
    '#prefix' => '<div class="hotspots_border_settings">',
    '#suffix' => '</div>',
  );
  $row['border']['style'] = array(
    '#type' => 'select',
    '#options' => drupal_map_assoc(array('none', 'dotted', 'dashed', 'solid', 'double', 'groove', 'ridge', 'inset', 'outset')),
    '#attributes' => array('class' => array('hotspot_border_style_field')),
  );
  $row['border']['width'] = array( 
    '#type' => 'textfield',
    '#size' => 5,
    '#attributes' => array('placeholder' => t('width'), 'class' => array('hotspot_border_width_field')),
    '#suffix' => 'px',
  );
  $row['border']['color'] = array(
    '#type' => 'textfield',
    '#size' => 8,
    '#maxlength' => 6,
    '#attributes' => array('placeholder' => t('border color'), 'class' => array('hotspot_border_color_field')),
  );
  $row['opacity'] = array(
    '#type' => 'textfield',
    '#number_type' => 'decimal',
    '#size' => 5,
    '#maxlength' => 7,
    '#attributes' => array('placeholder' => t('opacity'), 'class' => array('hotspot_opacity_field')),
    '#value' => '0.3',
  );
  $row['remove'] = array(
    '#title' => t('Remove'),
    '#type' => 'checkbox',
  );

  return $row;
}

/**
 * Default style settings array.
 */
function image_hotspots_default() {
  $default_opacity = '0.3';
  $default_colors = array('6666FF', '66CCCC', 'FF3366', 'FF3300', 'FFFF66', '33FF66', '99FF66', 'FFCC99', '9966FF', '33CCFF', );
  $default_hotspots = array();
  foreach ($default_colors as $color) {
    $default_hotspots[] = array(
      'background-color' => $color,
      'opacity' => $default_opacity,
    );
  }
  return $default_hotspots;
}
