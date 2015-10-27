<?php
/**
 * @file
 * Administration form definition for Mapplic.
 */

/**
 * Mapplic settings form builder.
 */
function mapplic_admin_settings($form, &$form_state) {

  $form = array('#attributes' => array('enctype' => 'multipart/form-data'));

  $form['mapplic_height'] = array(
    '#title' => t('Floorplan height'),
    '#description' => t('Height of the application in pixels. The width will take up the available space.'),
    '#required' => TRUE,
    '#default_value' => variable_get('mapplic_height', 400),
    '#type' => 'textfield',
  );

  $form['mapplic_map_height'] = array(
    '#title' => t('Map height'),
    '#description' => t('Height of the map in pixels.'),
    '#required' => TRUE,
    '#default_value' => variable_get('mapplic_map_height', 200),
    '#type' => 'textfield',
  );

  $form['mapplic_map_width'] = array(
    '#title' => t('Map width'),
    '#description' => t('Width of the map in pixels.'),
    '#required' => TRUE,
    '#default_value' => variable_get('mapplic_map_width', 130),
    '#type' => 'textfield',
  );

  $form['mapplic_sidebar'] = array(
    '#title' => t('Show sidebar'),
    '#description' => t('Whether to display the sidebar, which contains a search form and a list with locations.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('mapplic_sidebar', TRUE),
  );

  $form['mapplic_minimap'] = array(
    '#title' => t('Show minimap'),
    '#description' => t('Whether to display the minimap.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('mapplic_minimap', TRUE),
  );

  $form['mapplic_locations'] = array(
    '#title' => t('Show locations'),
    '#description' => t('Whether to display the locations on the map. This should be set to false in case we have an interactive SVG as map, because the overlaying locations layer may block the interactivity of the SVG.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('mapplic_locations', FALSE),
  );

  $form['mapplic_fullscreen'] = array(
    '#title' => t('Allow fullscreen'),
    '#description' => t('Enable or disable the fullscreen option.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('mapplic_fullscreen', TRUE),
  );

  $form['mapplic_hovertip'] = array(
    '#title' => t('Show hovertip'),
    '#description' => t("Show or hide the hover tooltip containing the landmark's title"),
    '#type' => 'checkbox',
    '#default_value' => variable_get('mapplic_hovertip', TRUE),
  );

  $form['mapplic_search'] = array(
    '#title' => t('Enable search'),
    '#description' => t("in case there's a small number of locations, the search form can be disabled."),
    '#type' => 'checkbox',
    '#default_value' => variable_get('mapplic_search', TRUE),
  );

  $form['mapplic_animate'] = array(
    '#title' => t('Animate'),
    '#description' => t('Enable or disable pin animations.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('mapplic_animate', TRUE),
  );

  $form['mapplic_mapfill'] = array(
    '#title' => t('Fill container'),
    '#description' => t('To make the map fill the container, set this to false. Otherwise the map will fit into the container, as the default behavior.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('mapplic_mapfill', TRUE),
  );

  $form['mapplic_zoombuttons'] = array(
    '#title' => t('Emable zoom buttons'),
    '#description' => t('Show or hide the +/- zoom buttons.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('mapplic_zoombuttons', TRUE),
  );

  $form['mapplic_clearbutton'] = array(
    '#title' => t('Emable clear button'),
    '#description' => t('Whether to display the clear button.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('mapplic_clearbutton', TRUE),
  );

  $form['mapplic_developer_mode'] = array(
    '#title' => t('Activate developer mode'),
    '#description' => t('Enable or disable the developer option (displaying coordinates of the cursor).'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('mapplic_developer_mode', FALSE),
  );

  $form['mapplic_zoom'] = array(
    '#title' => t('Enable zoom'),
    '#description' => t('enable or disable the zoom feature.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('mapplic_zoom', TRUE),
  );

  $scales = array_combine(range(1, 10), range(1, 10));
  $form['mapplic_max_scale'] = array(
    '#title' => t('Mapplic max scale'),
    '#description' => t('The zoom-in limit of the map. For example, if we have a file with 600x400 dimensions when it fits, and the limit is set to 2, the maximum zoom will be 1200x800.'),
    '#type' => 'select',
    '#options' => $scales,
    '#default_value' => variable_get('mapplic_max_scale', 10),
  );

  return system_settings_form($form);
}
