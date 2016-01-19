<?php

/**
 * @file
 * packery.admin.inc
 */

/**
 * Packery settings form.
 */
function packery_settings_form($form, $form_state) {
  $groups = packery_settings_callback_list();

  $form['settings'] = array(
    '#type' => 'vertical_tabs',
  );

  foreach ($groups as $group) {
    // Normalize setting variable name.
    $name = str_replace('_settings', '', $group);

    // Group names must be unique.
    $form[$name] = array(
      '#type' => 'fieldset',
      '#title' => $group,
      '#group' => 'settings',
      '#attributes' => array(
        'id' => $group,
      )
    );
    $form[$name][$name . '_imagesloaded'] = array(
      '#type' => 'select',
      '#title' => t('imagesLoaded'),
      '#options' => array(
        1 => t('True'),
        0 => t('False'),
      ),
      '#description' => t('Provides support for imagesLoaded plugin, read the readme.txt for more info.'),
      '#default_value' => variable_get($name . '_imagesloaded', 0)
    );
    $form[$name][$name . '_container_style'] = array(
      '#type' => 'textfield',
      '#title' => t('Container style'),
      '#description' => t('CSS styles that are applied to the container element. To disable Packery from setting any CSS to the container element, set NULL'),
      '#default_value' => variable_get($name . '_container_style', "{ position: 'relative' }")
    );
    $form[$name][$name . '_column_width'] = array(
      '#type' => 'textfield',
      '#title' => t('Column width'),
      '#description' => t('The width of a column of a horizontal grid. When set, Packery will align item elements horizontally to this grid.'),
      '#default_value' => variable_get($name . '_column_width', '')
    );
    $form[$name][$name . '_gutter'] = array(
      '#type' => 'textfield',
      '#title' => t('Gutter'),
      '#description' => t('The space between item elements, both vertically and horizontally.'),
      '#default_value' => variable_get($name . '_gutter', '')
    );
    $form[$name][$name . '_percent_position'] = array(
      '#type' => 'select',
      '#title' => t('Percent position'),
      '#options' => array(
        1 => t('True'),
        0 => t('False'),
      ),
      '#description' => t('Will set item position in percent values, rather than pixel values. percentPosition works well with percent-width items, as items will not transition their position on resize.'),
      '#default_value' => variable_get($name . '_percent_position', 0)
    );
    $form[$name][$name . '_is_horizontal'] = array(
      '#type' => 'select',
      '#title' => t('Is horizontal'),
      '#options' => array(
        1 => t('True'),
        0 => t('False'),
      ),
      '#description' => t('Arranges items horizontally instead of vertically.'),
      '#default_value' => variable_get($name . '_is_horizontal', 0)
    );
    $form[$name][$name . '_is_init_layout'] = array(
      '#type' => 'select',
      '#title' => t('Is init layout'),
      '#options' => array(
        1 => t('True'),
        0 => t('False'),
      ),
      '#description' => t('Enables layout on initialization. Set this to false to disable layout on initialization, so you can use methods or add events before the initial layout.'),
      '#default_value' => variable_get($name . '_is_init_layout', 1)
    );
    $form[$name][$name . '_is_origin_left'] = array(
      '#type' => 'select',
      '#title' => t('Is origin left'),
      '#options' => array(
        1 => t('True'),
        0 => t('False'),
      ),
      '#description' => t('Controls the horizontal flow of the layout. By default, item elements start positioning at the left. Set to false for right-to-left layouts.'),
      '#default_value' => variable_get($name . '_is_origin_left', 1)
    );
    $form[$name][$name . '_is_origin_top'] = array(
      '#type' => 'select',
      '#title' => t('Is origin top'),
      '#options' => array(
        1 => t('True'),
        0 => t('False'),
      ),
      '#description' => t('Controls the vertical flow of the layout. By default, item elements start positioning at the top. Set to false for bottom-up layouts. It’s like Tetris!'),
      '#default_value' => variable_get($name . '_is_origin_top', 1)
    );
    $form[$name][$name . '_is_resize_bound'] = array(
      '#type' => 'select',
      '#title' => t('Is resize bound'),
      '#options' => array(
        1 => t('True'),
        0 => t('False'),
      ),
      '#description' => t('isResizeBound binds layout only when the Packery instance is first initialized. You can bind and unbind resize layout afterwards with the bindResize and unbindResize methods.'),
      '#default_value' => variable_get($name . '_is_resize_bound', 1)
    );
    $form[$name][$name . '_item_selector'] = array(
      '#type' => 'textfield',
      '#title' => t('Item selector'),
      '#description' => t('Specifies which child elements to be used as item elements. Setting itemSelector is always recommended. itemSelector is useful to exclude sizing elements.'),
      '#default_value' => variable_get($name . '_item_selector', '.packery-item')
    );
    $form[$name][$name . '_row_height'] = array(
      '#type' => 'textfield',
      '#title' => t('Row height'),
      '#description' => t('Height of a row of a vertical grid. When set, Packery will align item elements vertically to this grid.'),
      '#default_value' => variable_get($name . '_row_height', 60)
    );
    $form[$name][$name . '_stamp'] = array(
      '#type' => 'textfield',
      '#title' => t('Stamp'),
      '#description' => t('Specifies which elements are stamped within the layout. These are special layout elements which will not be laid out by Packery. Rather, Packery will layout item elements around stamped elements.'),
      '#default_value' => variable_get($name . '_stamp', FALSE)
    );
    $form[$name][$name . '_transition_duration'] = array(
      '#type' => 'textfield',
      '#title' => t('Transition duration'),
      '#description' => t('The time duration of transitions for item elements.'),
      '#default_value' => variable_get($name . '_transition_duration', '0.4s')
    );

    // Global submit handler.
    $form[$name]['actions'] = array(
      '#type' => 'container',
    );
    $form[$name]['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save settings'),
      '#limit_validation_errors' => array(),
      '#validate' => array('packery_settings_form_validate'),
      '#submit' => array('packery_settings_form_submit'),
    );
  }

  // Include add settings group form.
  $form['group'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add settings group'),
    '#weight' => -10,
    '#group' => 'settings'
  );
  $form['group']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('Identifier for group of settings.'),
    '#required' => TRUE,
  );
  $form['group']['actions'] = array(
    '#type' => 'container',
  );
  $form['group']['actions']['add'] = array(
    '#type' => 'submit',
    '#value' => t('Add group'),
    '#limit_validation_errors' => array(
      array('name'),
    ),
    '#validate' => array('packery_settings_add_form_validate'),
    '#submit' => array('packery_settings_add_form_submit'),
  );

  return $form;
}

/**
 * Packery settings form validation handler.
 */
function packery_settings_form_validate($form, $form_state) {
  // Failed selector? This should never happen.
  if (empty($form_state['input']['settings__active_tab'])) {
    form_set_error('settings__active_tab', t('Can not find active settings group.'));
  }
}

/**
 * Packery settings form submit handler.
 */
function packery_settings_form_submit($form, $form_state) {
  // Active group name.
  $group = $form_state['input']['settings__active_tab'];

  // Recreate variable ident.
  $name = str_replace('_settings', '', $group);

  // Update variables.
  variable_set($name . '_imagesloaded', $form_state['input'][$name . '_imagesloaded']);
  variable_set($name . '_container_style', $form_state['input'][$name . '_container_style']);
  variable_set($name . '_column_width', $form_state['input'][$name . '_column_width']);
  variable_set($name . '_gutter', $form_state['input'][$name . '_gutter']);
  variable_set($name . '_percent_position', $form_state['input'][$name . '_percent_position']);
  variable_set($name . '_is_horizontal', $form_state['input'][$name . '_is_horizontal']);
  variable_set($name . '_is_init_layout', $form_state['input'][$name . '_is_init_layout']);
  variable_set($name . '_is_origin_left', $form_state['input'][$name . '_is_origin_left']);
  variable_set($name . '_is_origin_top', $form_state['input'][$name . '_is_origin_top']);
  variable_set($name . '_is_resize_bound', $form_state['input'][$name . '_is_resize_bound']);
  variable_set($name . '_item_selector', $form_state['input'][$name . '_item_selector']);
  variable_set($name . '_row_height', $form_state['input'][$name . '_row_height']);
  variable_set($name . '_stamp', $form_state['input'][$name . '_stamp']);
  variable_set($name . '_transition_duration', $form_state['input'][$name . '_transition_duration']);
}

/**
 * Additional validation handler.
 */
function packery_settings_add_form_validate($form, $form_state) {
  $name = $form_state['values']['name'];

  $check = variable_get('packery_settings_' . $name, FALSE);
  if ($check) {
    form_set_error('name', t('Group name already exists'));
  }
}

/**
 * Additional submit handler.
 */
function packery_settings_add_form_submit($form, $form_state) {
  $name = $form_state['values']['name'];
  variable_set('packery_settings_' . $name, $name);
}
