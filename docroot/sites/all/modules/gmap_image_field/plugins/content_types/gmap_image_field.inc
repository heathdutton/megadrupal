<?php

/**
 * @file
 * Pane which allow to embed gmap_image_field map to panel page.
 */

$plugin = array(
  'title' => t('GMap Image field'),
  'description' => t('Show just a map from a node.'),
  'render callback' => 'gmap_image_field_pane_render',
  'edit form' => 'gmap_image_field_pane_settings_form',
  'admin info' => 'gmap_image_field_pane_admin_info',
  'category' => t('Widgets'),
  'hook theme' => 'ga_node_report_content_type_theme',
  'defaults' => array(),
);

/**
 * Implementing the pane render callback, which render the pane by the settings.
 *
 * @param array $subtype
 *   subtype
 * @param array $conf
 *   conf array
 * @param mixed $context
 *   context
 *
 * @return stdClass
 *   block
 */
function gmap_image_field_pane_render($subtype, $conf, $context = NULL) {

  $block = new stdClass();

  $block->title = empty($conf['override_title']) ? NULL : $conf['override_title_text'];
  $block->content = NULL;

  if (!empty($conf['image']['nid']) && !empty($conf['image']['field_name'])) {
    $pane_config = array_merge($conf['settings'], $conf['image']);
    $map = _gmap_image_field_get_map_settings($conf['image']['nid'], $conf['image']['field_name'], NULL, $pane_config);
    $block->content = theme('gmap_image_field_show_map', array('map' => $map));
  }

  return $block;
}

/**
 * Implementing pane settings cb, that simulate field formatter settings form.
 *
 * @param array $form
 *   form element
 * @param array $form_state
 *   current form state
 *
 * @return array
 *   form element
 */
function gmap_image_field_pane_settings_form($form, &$form_state) {

  // Create dummy instance with pane settings.
  $instance = array(
    'display' => array('pane' => $form_state['conf']),
  );

  $form['image'] = array(
    '#type' => 'fieldset',
    '#title' => t('Image'),
    '#tree' => TRUE,
    '#prefix' => '<div id="gmap-image-field-pane-preview">',
    '#suffix' => '</div>',
  );
  $form['image']['title'] = array(
    '#title' => 'Image',
    '#type' => 'textfield',
    '#autocomplete_path' => 'admin/ajax/gmap-image-field/autocomplete/0/',
    '#default_value' => $form_state['conf']['image']['title'],
    '#ajax' => array(
      'callback' => 'gmap_image_field_pane_settings_form_image_changed',
      'wrapper' => 'gmap-image-field-pane-preview',
      'method' => 'replace',
    ),
  );

  $image_title = empty($form_state['triggering_element']) ? $form['image']['title']['#default_value'] : $form_state['triggering_element']['#value'];

  $preview_image_markup = '';
  $image_nid = NULL;
  $image_field_name = NULL;

  if (preg_match('#\[([\w\-\_]+)\: (\d+)\]$#', $image_title, $image_data)) {
    $map = _gmap_image_field_get_map_settings($image_data[2], $image_data[1]);
    if ($map) {
      $preview_image_markup = theme('image_style', array('style_name' => 'medium', 'path' => $map['uri']));
      $image_field_name = $image_data[1];
      $image_nid = $image_data[2];
    }
  }
  $form['image']['nid'] = array(
    '#type' => 'value',
    '#value' => $image_nid,
  );
  $form['image']['field_name'] = array(
    '#type' => 'value',
    '#value' => $image_field_name,
  );
  $form['image']['preview'] = array(
    '#type' => 'markup',
    '#markup' => $preview_image_markup,
  );

  $form['image']['minzoomlevel'] = array(
    '#type' => 'select',
    '#title' => t('Minimum zoomlevel'),
    '#options' => drupal_map_assoc(range($map['minzoomlevel'], $map['maxzoomlevel'])),
    '#default_value' => empty($form_state['triggering_element']) ? ((empty($form_state['conf']['image']['minzoomlevel']) ? $map['minzoomlevel'] : $form_state['conf']['image']['minzoomlevel'])) : $map['minzoomlevel'],
  );

  $form['image']['maxzoomlevel'] = array(
    '#type' => 'select',
    '#title' => t('Maximum zoomlevel'),
    '#options' => drupal_map_assoc(range($map['minzoomlevel'], $map['maxzoomlevel'])),
    '#default_value' => empty($form_state['triggering_element']) ? ((empty($form_state['conf']['image']['maxzoomlevel']) ? $map['maxzoomlevel'] : $form_state['conf']['image']['maxzoomlevel'])) : $map['maxzoomlevel'],
  );

  $form['image']['zoom'] = array(
    '#type' => 'select',
    '#title' => t('Initial zoomlevel'),
    '#options' => drupal_map_assoc(range($map['minzoomlevel'], $map['maxzoomlevel'])),
    '#default_value' => empty($form_state['triggering_element']) ? ((empty($form_state['conf']['image']['zoom']) ? $map['zoom'] : $form_state['conf']['image']['zoom'])) : $map['zoom'],
  );

  // Simulate the field formatter, which can be used as the pane.
  $form['settings'] = gmap_image_field_field_formatter_settings_form(array(), $instance, 'pane', array(), $form_state);
  $form['settings']['#tree'] = TRUE;
  $form['settings']['#type'] = 'fieldset';
  $form['settings']['#title'] = t('Formatter settings');

  return $form;
}

/**
 * Implements FORM_ID_submit().
 */
function gmap_image_field_pane_settings_form_submit($form, &$form_state) {
  $form_state['conf'] = $form_state['values'];
}
