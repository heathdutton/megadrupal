<?php

/**
 * @file
 * Yandex Maps Admin UI file.
 */

/**
 * Form builder for admin settings form.
 */
function yamaps_settings_page($form, &$form_state) {
  // Set amount of blocks admin can create.
  $blocks_count = range(0, 20);
  unset($blocks_count[0]);
  $form['yamaps_blocks_amount'] = array(
    '#title' => t('Set amount of Yandex Maps blocks'),
    '#description' => t('Amount of Yandex Maps blocks available on !blocks_page.',
      array('!blocks_page' => l(t('Blocks page'), 'admin/structure/block'))),
    '#element_validate' => array('element_validate_integer_positive'),
    '#type' => 'select',
    '#options' => $blocks_count,
    '#default_value' => variable_get('yamaps_blocks_amount', YAMAPS_DEFAULT_BLOCK_AMOUNT),
    '#required' => TRUE,
  );

  $display_options = variable_get('yamaps_block_edit_display_options', array());

  // Container for display options.
  $form['yamaps_block_edit_display_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display options'),
    '#tree' => TRUE,
  );

  // Display options.
  $form['yamaps_block_edit_display_options']['display_type'] = array(
    '#type' => 'radios',
    '#title' => t('Map display style in admin UI for blocks'),
    '#options' => array(
      'map' => t('Map'),
      'map_button' => t('Map opened by button click'),
    ),
    '#default_value' => (isset($display_options['display_type'])) ? $display_options['display_type'] : 'map',
    '#required' => FALSE,
    '#description' => t('Configure how to display "Yandex Map" field while creating new / updating blocks.
    This setting applies to admin UI only. To display map opening in block by button click for end users select
    appropriate option in "Map display style for end users" section of the appropriate block.'),
  );

  // Text on button that opens map by click.
  $form['yamaps_block_edit_display_options']['open_button_text'] = array(
    '#type' => 'textfield',
    '#title' => t('"Open" button text'),
    '#default_value' => isset($display_options['open_button_text']) ? t($display_options['open_button_text']) : t(YAMAPS_DEFAULT_OPEN_MAP_TEXT),
    '#required' => FALSE,
    '#description' => t('Specify text to display on the button opening map in case of "Map opened by button click" option.
    This setting applies to admin UI only. To configure text on the button opening map for end users select appropriate
    option in "Manage Display" section of the appropriate block.'),
    '#states' => array(
      'visible' => array(
        ':input[name="yamaps_block_edit_display_options[display_type]"]' => array(
          'value' => 'map_button',
        ),
      ),
    ),
  );

  // Text on button that closes map by click.
  $form['yamaps_block_edit_display_options']['close_button_text'] = array(
    '#type' => 'textfield',
    '#title' => t('"Close" button text'),
    '#default_value' => isset($display_options['close_button_text']) ? t($display_options['close_button_text']) : t(YAMAPS_DEFAULT_CLOSE_MAP_TEXT),
    '#required' => FALSE,
    '#description' => t('Specify text to display on the button closing map in case of "Map opened by button click" option.
    This setting applies to admin UI only. To configure text on the button closing map for end users select appropriate
    option in "Manage Display" section of the appropriate block.'),
    '#states' => array(
      'visible' => array(
        ':input[name="yamaps_block_edit_display_options[display_type]"]' => array(
          'value' => 'map_button',
        ),
      ),
    ),
  );

  $form['yamaps_block_edit_display_options']['width'] = array(
    '#title' => t('Map width in admin UI'),
    '#field_suffix' => ' ' . t('in pixels (px) or percentage (%).'),
    '#type' => 'textfield',
    '#default_value' => isset($display_options['width']) ? $display_options['width'] : YAMAPS_DEFAULT_ADMIN_UI_MAP_WIDTH,
    '#size' => 5,
    '#element_validate' => array('yamaps_field_validate_pixels_percentage'),
    '#required' => TRUE,
    '#description' => t('Set width for the map in "block edit" form.'),
  );

  $form['yamaps_block_edit_display_options']['height'] = array(
    '#title' => t('Map height in admin UI'),
    '#field_suffix' => ' ' . t('in pixels (px) or percentage (%).'),
    '#type' => 'textfield',
    '#default_value' => isset($display_options['height']) ? $display_options['height'] : YAMAPS_DEFAULT_ADMIN_UI_MAP_HEIGHT,
    '#size' => 5,
    '#element_validate' => array('yamaps_field_validate_pixels_percentage'),
    '#required' => TRUE,
    '#description' => t('Set height for the map in "block edit" form.'),
  );

  return system_settings_form($form);
}
