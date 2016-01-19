<?php

/**
 * @file
 * Homebox packery admin file.
 */

/**
 * Homebox packery settings form to be add on the homebox settings page.
 */
function homebox_packery_form_homebox_configure_form_alter(&$form, &$form_state, $form_id) {

  // Save the actual form to add the packery form on top of it.
  $form_save = $form;

  // Get the arg page pass the homebox_configure_form.
  $page = $form_state['build_info']['args'][0];

  $form = array();
  $form['packery'] = array(
    '#type' => 'checkbox',
    '#title' => t('Do you want to use JS library Packery?'),
    '#description' => t('This will set the number of columns to 1 and no need to configure the column width. It will be 100% by default.'),
    '#default_value' => (isset($page->settings['packery']) ? $page->settings['packery'] : ''),
  );

  // Settings name by default.
  $name = 'packery_settings';
  // Get the settings for packery.
  $settings = (isset($page->settings[$name]) ? $page->settings[$name] : array());

  $form[$name] = array(
    '#type' => 'fieldset',
    '#title' => t('Packery settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form[$name]['container_style'] = array(
    '#type' => 'textfield',
    '#title' => t('Container style'),
    '#description' => t('CSS styles that are applied to the container element. To disable Packery from setting any CSS to the container element, set NULL'),
    '#default_value' => (isset($settings['container_style']) ? $settings['container_style'] : "{ position: 'relative' }"),
  );
  $form[$name]['column_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Column width'),
    '#description' => t('The width of a column of a horizontal grid. When set, Packery will align item elements horizontally to this grid.'),
    '#default_value' => (isset($settings['column_width']) ? $settings['column_width'] : 250),
  );
  $form[$name]['row_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Row height'),
    '#description' => t('Height of a row of a vertical grid. When set, Packery will align item elements vertically to this grid.'),
    '#default_value' => (isset($settings['row_height']) ? $settings['row_height'] : 125),
  );
  $form[$name]['gutter'] = array(
    '#type' => 'textfield',
    '#title' => t('Gutter'),
    '#description' => t('The space between item elements, both vertically and horizontally.'),
    '#default_value' => (isset($settings['gutter']) ? $settings['gutter'] : 0),
  );
  $form[$name]['is_horizontal'] = array(
    '#type' => 'select',
    '#title' => t('Is horizontal'),
    '#options' => array(
      1 => t('True'),
      0 => t('False'),
    ),
    '#description' => t('Arranges items horizontally instead of vertically.'),
    '#default_value' => array((isset($settings['is_horizontal']) ? $settings['is_horizontal'] : FALSE)),
  );
  $form[$name]['is_init_layout'] = array(
    '#type' => 'select',
    '#title' => t('Is init layout'),
    '#options' => array(
      1 => t('True'),
      0 => t('False'),
    ),
    '#description' => t('Enables layout on initialization. Set this to false to disable layout on initialization, so you can use methods or add events before the initial layout.'),
    '#default_value' => array((isset($settings['is_init_layout']) ? $settings['is_init_layout'] : TRUE)),
  );
  $form[$name]['is_origin_left'] = array(
    '#type' => 'select',
    '#title' => t('Is origin left'),
    '#options' => array(
      1 => t('True'),
      0 => t('False'),
    ),
    '#description' => t('Controls the horizontal flow of the layout. By default, item elements start positioning at the left. Set to false for right-to-left layouts.'),
    '#default_value' => array((isset($settings['is_origin_left']) ? $settings['is_origin_left'] : TRUE)),
  );
  $form[$name]['is_origin_top'] = array(
    '#type' => 'select',
    '#title' => t('Is origin top'),
    '#options' => array(
      1 => t('True'),
      0 => t('False'),
    ),
    '#description' => t('Controls the vertical flow of the layout. By default, item elements start positioning at the top. Set to false for bottom-up layouts. It’s like Tetris!'),
    '#default_value' => array((isset($settings['is_origin_top']) ? $settings['is_origin_top'] : TRUE)),
  );
  $form[$name]['is_resize_bound'] = array(
    '#type' => 'select',
    '#title' => t('Is resize bound'),
    '#options' => array(
      1 => t('True'),
      0 => t('False'),
    ),
    '#description' => t('isResizeBound binds layout only when the Packery instance is first initialized. You can bind and unbind resize layout afterwards with the bindResize and unbindResize methods.'),
    '#default_value' => array((isset($settings['is_resize_bound']) ? $settings['is_resize_bound'] : TRUE)),
  );
  $form[$name]['item_selector'] = array(
    '#type' => 'textfield',
    '#title' => t('Item selector'),
    '#description' => t('Specifies which child elements to be used as item elements. Setting itemSelector is always recommended. itemSelector is useful to exclude sizing elements.'),
    '#default_value' => array((isset($settings['item_selector']) ? $settings['item_selector'] : '.homebox-draggable')),
  );
  $form[$name]['stamp'] = array(
    '#type' => 'textfield',
    '#title' => t('Stamp'),
    '#description' => t('Specifies which elements are stamped within the layout. These are special layout elements which will not be laid out by Packery. Rather, Packery will layout item elements around stamped elements.'),
    '#default_value' => (isset($settings['stamp']) ? $settings['stamp'] : '.tile-stamp'),
  );
  $form[$name]['transition_duration'] = array(
    '#type' => 'textfield',
    '#title' => t('Transition duration'),
    '#description' => t('The time duration of transitions for item elements.'),
    '#default_value' => (isset($settings['transition_duration']) ? $settings['transition_duration'] : '0.25s'),
  );

  // Merge the 2 forms to add packery field on top.
  $form = array_merge($form, $form_save);

  // Add the validate homebox packery before the homebox validate form.
  array_unshift($form['#validate'], 'homebox_packery_settings_form_validate');
  // Add the submit homebox packery before the homebox submit form.
  array_unshift($form['#submit'], 'homebox_packery_settings_form_submit');

  return $form;
}

/**
 * Homebox packery settings form validation handler.
 */
function homebox_packery_settings_form_validate($form, &$form_state) {
  // If packery is Set, add default value to homebox settings.
  $packery = (int) $form_state['values']['packery'];
  if ($packery) {
    $form_state['values']['columns'] = 1;
    $form_state['values']['width_1'] = 100;
  }
}

/**
 * Homebox packery settings form submit handler.
 *
 * Add packery variables to the settings of the homebox page.
 */
function homebox_packery_settings_form_submit($form, &$form_state) {
  // Fetch page.
  $page = homebox_get_page($form_state['values']['name']);

  // Adjust settings.
  $settings = array();
  $settings['container_style'] = $form_state['values']['container_style'];
  $settings['column_width'] = (int) $form_state['values']['column_width'];
  $settings['row_height'] = (int) $form_state['values']['row_height'];
  $settings['gutter'] = (int) $form_state['values']['gutter'];
  $settings['is_horizontal'] = (boolean) $form_state['values']['is_horizontal'];
  $settings['is_init_layout'] = (boolean) $form_state['values']['is_init_layout'];
  $settings['is_origin_left'] = (boolean) $form_state['values']['is_origin_left'];
  $settings['is_origin_top'] = (boolean) $form_state['values']['is_origin_top'];
  $settings['is_resize_bound'] = (boolean) $form_state['values']['is_resize_bound'];
  $settings['item_selector'] = $form_state['values']['item_selector'];
  $settings['stamp'] = $form_state['values']['stamp'];
  $settings['transition_duration'] = $form_state['values']['transition_duration'];

  $page->settings['packery'] = (int) $form_state['values']['packery'];
  $page->settings['packery_settings'] = $settings;

  // Save settings.
  homebox_save_page($page);
}
