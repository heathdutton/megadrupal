<?php
/**
 * @file
 * Form helper functions.
 */

/**
 * Helper function to build the settings form.
 *
 * @param $form
 */
function _bootstrap_elements_settings(&$form, $defaults = array()) {

  // Form Type.
  $form['form_type'] = _bootstrap_elements_form_type($defaults['form_type']);

  // Title Display.
  $form['title_display'] = _bootstrap_elements_title_display($defaults['title_display']);

  // Grid settings wrapper.
  $form['grid'] = _bootstrap_elements_grid('#edit-form-type');

  // Title grid fieldset.
  $form['grid']['title_grid'] = _bootstrap_elements_title_grid();
  $form['grid']['title_grid']['columns'] = _bootstrap_elements_columns($defaults['title_grid']['columns']);
  $form['grid']['title_grid']['offset'] = _bootstrap_elements_offset($defaults['title_grid']['offset']);
  $form['grid']['title_grid']['push'] = _bootstrap_elements_push($defaults['title_grid']['push']);
  $form['grid']['title_grid']['pull'] = _bootstrap_elements_pull($defaults['title_grid']['pull']);

  // Element grid fieldset.
  $form['grid']['element_grid'] = _bootstrap_elements_element_grid();
  $form['grid']['element_grid']['columns'] = _bootstrap_elements_columns($defaults['element_grid']['columns']);
  $form['grid']['element_grid']['offset'] = _bootstrap_elements_offset($defaults['element_grid']['offset']);
  $form['grid']['element_grid']['push'] = _bootstrap_elements_push($defaults['element_grid']['push']);
  $form['grid']['element_grid']['pull'] = _bootstrap_elements_pull($defaults['element_grid']['pull']);

  // Element columns fieldset.
  $form['grid']['element_columns'] = _bootstrap_elements_element_columns();
  $form['grid']['element_columns']['columns'] = _bootstrap_elements_columns($defaults['element_columns']['columns']);
  $form['grid']['element_columns']['offset'] = _bootstrap_elements_offset($defaults['element_columns']['offset']);
  $form['grid']['element_columns']['push'] = _bootstrap_elements_push($defaults['element_columns']['push']);
  $form['grid']['element_columns']['pull'] = _bootstrap_elements_pull($defaults['element_columns']['pull']);

  $form['tooltips'] = _bootstrap_elements_tooltips();
  $form['tooltips']['title_tooltip'] = _bootstrap_elements_title_tooltip();
  $form['tooltips']['title_tooltip'] += _bootstrap_elements_tooltip_options($defaults['tooltips']['title_tooltip']);
  $form['tooltips']['prefix_tooltip'] = _bootstrap_elements_prefix_tooltip();
  $form['tooltips']['prefix_tooltip'] += _bootstrap_elements_tooltip_options($defaults['tooltips']['prefix_tooltip']);
  $form['tooltips']['suffix_tooltip'] = _bootstrap_elements_suffix_tooltip();
  $form['tooltips']['suffix_tooltip'] += _bootstrap_elements_tooltip_options($defaults['tooltips']['suffix_tooltip']);

  $form['popovers'] = _bootstrap_elements_popovers();
  $form['popovers']['title_popover'] = _bootstrap_elements_title_popover();
  $form['popovers']['title_popover'] += _bootstrap_elements_popover_options($defaults['popovers']['title_popover']);
  $form['popovers']['prefix_popover'] = _bootstrap_elements_prefix_popover();
  $form['popovers']['prefix_popover'] += _bootstrap_elements_popover_options($defaults['popovers']['prefix_popover']);
  $form['popovers']['suffix_popover'] = _bootstrap_elements_suffix_popover();
  $form['popovers']['suffix_popover'] += _bootstrap_elements_popover_options($defaults['popovers']['suffix_popover']);

  // Addons.
  $form['addons'] = _bootstrap_elements_addons();
  $form['addons']['field_prefix'] = _bootstrap_elements_field_prefix($defaults['addons']['field_prefix']);
  $form['addons']['field_suffix'] = _bootstrap_elements_field_suffix($defaults['addons']['field_suffix']);
  $form['addons']['input_group'] = _bootstrap_elements_input_group($defaults['addons']['input_group']);

  // Wrappers.
  $form['wrappers'] = _bootstrap_elements_wrappers();
  $form['wrappers']['form_type_wrapper'] = _bootstrap_elements_form_type_wrapper($defaults['wrappers']['form_type_wrapper']);
  $form['wrappers']['form_group_wrapper'] = _bootstrap_elements_form_group_wrapper($defaults['wrappers']['form_group_wrapper']);
  $form['wrappers']['form_field_wrapper'] = _bootstrap_elements_form_field_wrapper($defaults['wrappers']['form_field_wrapper']);
  $form['wrappers']['element_inline_wrapper'] = _bootstrap_elements_element_inline_wrapper($defaults['wrappers']['element_inline_wrapper']);

  // Remove the element columns from everything except checkboxes and radios.
  if (!in_array($defaults['type'], array('checkboxes', 'radios'))) {
    unset($form['grid']['element_columns']);
    unset($form['wrappers']['element_inline_wrapper']);
  }

  // Remove element_grid for checkboxes and radios as the title_grid is used.
  if (in_array($defaults['type'], array('list_boolean'))) {
    unset($form['grid']['element_grid']);
  }

  // Remove suffix tooltips and popovers from some elements.
  if (in_array($defaults['type'], array('list_boolean'))) {
    unset($form['popovers']['suffix_popover']);
    unset($form['tooltips']['suffix_tooltip']);
  }

  // Remove prefix tooltips and popovers from some elements.
  if (in_array($defaults['type'], array('list_boolean'))) {
    unset($form['popovers']['prefix_popover']);
    unset($form['tooltips']['prefix_tooltip']);
  }

  // Remove input groups from some elements.
  if (in_array($defaults['type'], array('list_boolean'))) {
    unset($form['addons']['input_group']);
  }

}


/**
 * @param $element
 * @param $settings
 */
function _bootstrap_elements_merge_properties(&$element, $settings) {

  // Form Type.
  if (isset($settings['form_type'])) {
    $element['#form_type'] = $settings['form_type'];
  }

  if (isset($settings['title_display'])) {
    $element['#title_display'] = $settings['title_display'];
  }

  // Title grid.
  if (isset($settings['grid']['title_grid'])) {
    $element['#title_grid'] = $settings['grid']['title_grid'];
  }

  // Element Grid.
  if (isset($settings['grid']['element_grid'])) {
    $element['#element_grid'] = $settings['grid']['element_grid'];
  }

  // Element Columns.
  if (isset($settings['element_columns'])) {
    $element['#element_columns'] = $settings['element_columns'];
  }

  // Tooltips.
  if (isset($settings['tooltips'])) {
    if (isset($settings['tooltips']['title_tooltip'])) {
      $element['#title_tooltip'] = $settings['tooltips']['title_tooltip'];
    }
    if (isset($settings['tooltips']['prefix_tooltip'])) {
      $element['#prefix_tooltip'] = $settings['tooltips']['prefix_tooltip'];
    }
    if (isset($settings['tooltips']['suffix_tooltip'])) {
      $element['#suffix_tooltip'] = $settings['tooltips']['suffix_tooltip'];
    }
  }

  // Popovers.
  if (isset($settings['popovers'])) {
    if (isset($settings['popovers']['title_popover'])) {
      $element['#title_popover'] = $settings['popovers']['title_popover'];
    }
    if (isset($settings['popovers']['prefix_tooltip'])) {
      $element['#prefix_popover'] = $settings['popovers']['prefix_popover'];
    }
    if (isset($settings['popovers']['suffix_tooltip'])) {
      $element['#suffix_popover'] = $settings['popovers']['suffix_popover'];
    }
  }

  // Addons - Prefix/Suffixes
  if (isset($settings['addons'])) {
    if (isset($settings['addons']['field_prefix'])) {
      $element['#field_prefix'] = $settings['addons']['field_prefix'];
    }
    if (isset($settings['addons']['field_suffix'])) {
      $element['#field_suffix'] = $settings['addons']['field_suffix'];
    }
    if (isset($settings['addons']['input_group'])) {
      $element['#input_group'] = $settings['addons']['input_group'];
    }
  }

  // Wrappers.
  if (isset($settings['wrappers'])) {
    if (isset($settings['wrappers']['form_field_wrapper'])) {
      $element['#form_field_wrapper'] = $settings['wrappers']['form_field_wrapper'];
    }
    if (isset($settings['wrappers']['form_type_wrapper'])) {
      $element['#form_type_wrapper'] = $settings['wrappers']['form_type_wrapper'];
    }
    if (isset($settings['wrappers']['form_group_wrapper'])) {
      $element['#form_group_wrapper'] = $settings['wrappers']['form_group_wrapper'];
    }
  }

  // Merge in the actions.
}


/*******************************************************************************
 * FORM HELPER FUNCTIONS
 */

/**
 * Helper function to create the 'form_type_wrapper' field.
 *
 * @param null $defaults
 *
 * @return array
 */
function _bootstrap_elements_form_type_wrapper($defaults = NULL) {
  $element = array(
    '#type' => 'checkbox',
    '#title' => 'Form type wrapper',
  );
  if (!is_null($defaults)) {
    $element['#default_value'] = $defaults;
  }
  return $element;
}

/**
 * Helper function to create the 'form_group_wrapper' field.
 *
 * @param null $defaults
 *
 * @return array
 */
function _bootstrap_elements_form_group_wrapper($defaults = NULL) {
  $element = array(
    '#type' => 'checkbox',
    '#title' => 'Form group wrapper',
  );
  if (!is_null($defaults)) {
    $element['#default_value'] = $defaults;
  }
  return $element;
}

/**
 * Helper function to create the 'form_field_wrapper' field.
 *
 * @param null $defaults
 *
 * @return array
 */
function _bootstrap_elements_form_field_wrapper($defaults = NULL) {
  $element = array(
    '#type' => 'checkbox',
    '#title' => 'Form field wrapper',
  );
  if (!is_null($defaults)) {
    $element['#default_value'] = $defaults;
  }
  return $element;
}

/**
 * Helper function to create the 'form_field_wrapper' field.
 *
 * @param null $defaults
 *
 * @return array
 */
function _bootstrap_elements_element_inline_wrapper($defaults = NULL) {
  $element = array(
    '#type' => 'checkbox',
    '#title' => 'Element inline wrappers',
    '#description' => 'Render radios/checkboxes inline as per http://getbootstrap.com/css/#inline-checkboxes-and-radios',
  );
  if (!is_null($defaults)) {
    $element['#default_value'] = $defaults;
  }
  return $element;
}




/**
 * Render element for selecting 'form_type'.
 *
 * @return array
 */
function _bootstrap_elements_form_type($default_value = NULL) {
  $element = array(
    '#type' => 'select',
    '#title' => 'Form Type',
    '#options' => array(
      'basic' => 'Basic',
      'inline' => 'Inline',
      'horizontal' => 'Horizontal',
    ),
    '#description' => 'Select a Bootstrap form layout to use. http://getbootstrap.com/css/#forms',
    '#weight' => -100,
  );

  if (!is_null($default_value)) {
    $element['#default_value'] = $default_value;
  }

  return $element;
}

/**
 * Render element for selecting 'title_display'.
 *
 * @return array
 */
function _bootstrap_elements_title_display($default_value = NULL) {
  $element = array(
    '#type' => 'select',
    '#title' => 'Title display',
    '#options' => array(
      'before' => 'Above',
      'inline' => 'Inline',
      'none' => 'None',
    ),
    '#description' => 'Choose a title/label display option.',

  );

  if (!is_null($default_value)) {
    $element['#default_value'] = $default_value;
  }
  return $element;
}

/**
 * Render element for selecting 'title_width'.
 *
 * @return array
 */
function _bootstrap_elements_title_width() {

  return array(
    '#type' => 'select',
    '#title' => t('Label width'),
    '#options' => array(
      '1' => '1 column',
      '2' => '2 columns',
      '3' => '3 columns',
      '4' => '4 columns',
      '5' => '5 columns',
      '6' => '6 columns',
      '7' => '7 columns',
      '8' => '8 columns',
      '9' => '9 columns',
      '10' => '10 columns',
      '11' => '11 columns',
      '12' => '12 columns',
    ),
    '#description' => 'Select the column width of the components label (only used with horizontal forms).',

  );
}

/**
 * Render element for selecting 'element_offset'.
 *
 * @return array
 */
function _bootstrap_elements_element_offset() {

  return array(
    '#type' => 'select',
    '#title' => t('Element offset'),
    '#options' => array(
      '0' => 'None',
      '1' => '1 column',
      '2' => '2 columns',
      '3' => '3 columns',
      '4' => '4 columns',
      '5' => '5 columns',
      '6' => '6 columns',
      '7' => '7 columns',
      '8' => '8 columns',
      '9' => '9 columns',
      '10' => '10 columns',
      '11' => '11 columns',
      '12' => '12 columns',
    ),
    '#description' => 'Move elements to the right using this option. These options increase the left margin of an element by the selected amount of columns.',

  );
}


/**
 *
 */
function _bootstrap_elements_columns($defaults = array()) {

  $fieldset = array(
    '#type' => 'fieldset',
    '#title' => 'Columns',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  return $fieldset += _bootstrap_elements_column_grid($defaults);

}

/**
 *
 */
function _bootstrap_elements_offset($defaults = array()) {

  $fieldset = array(
    '#type' => 'fieldset',
    '#title' => 'Offset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  return $fieldset += _bootstrap_elements_column_grid($defaults);

}

/**
 *
 */
function _bootstrap_elements_push($defaults = array()) {

  $fieldset = array(
    '#type' => 'fieldset',
    '#title' => 'Push',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  return $fieldset += _bootstrap_elements_column_grid($defaults);

}

/**
 *
 */
function _bootstrap_elements_pull($defaults = array()) {

  $fieldset = array(
    '#type' => 'fieldset',
    '#title' => 'Pull',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  return $fieldset += _bootstrap_elements_column_grid($defaults);

}

/**
 * Render element for selecting 'field_prefix'.
 *
 * @return array
 */
function _bootstrap_elements_field_prefix($defaults) {
  $element = array(
    '#type' => 'textfield',
    '#title' => t('Field prefix'),
    '#description' => 'Enter a prefix to appear before the field.',
  );
  if (!is_null($defaults)) {
    $element['#default_value'] = $defaults;
  }
  return $element;
}

/**
 * Render element for selecting 'field_suffix'.
 *
 * @return array
 */
function _bootstrap_elements_field_suffix($defaults) {
  $element = array(
    '#type' => 'textfield',
    '#title' => t('Field suffix'),
    '#description' => 'Enter a suffix to appear after the field.',
  );
  if (!is_null($defaults)) {
    $element['#default_value'] = $defaults;
  }
  return $element;
}

/**
 * Render element for selecting 'input_group'.
 *
 * @return array
 */
function  _bootstrap_elements_input_group($defaults = NULL) {
  $element = array(
    '#type' => 'checkbox',
    '#title' => t('Show prefix and suffix in input groups?'),
    '#description' => 'Checking this box will make the field prefix and suffix appear as an input group.',
  );
  if (!is_null($defaults)) {
    $element['#default_value'] = $defaults;
  }
  return $element;
}


/**
 * Render element for selecting 'element_columns'.
 *
 * @return array
 */
function _bootstrap_elements_element_columns() {

  $fieldset = array(
    '#type' => 'fieldset',
    '#title' => 'Checkbox/Radio Grid',
    '#description' => 'Organise the checkboxes into columns',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  return $fieldset;
}


/**
 *
 */
function _bootstrap_elements_column_options() {
  return array(
    '' => 'Disabled',
    '0' => '0 Columns',
    '1' => '1 column',
    '2' => '2 columns',
    '3' => '3 columns',
    '4' => '4 columns',
    '5' => '5 columns',
    '6' => '6 columns',
    '7' => '7 columns',
    '8' => '8 columns',
    '9' => '9 columns',
    '10' => '10 columns',
    '11' => '11 columns',
    '12' => '12 columns',
  );
}

/**
 *
 */
function _bootstrap_elements_column_grid($defaults = array()) {
  $element = array(
    'xs' => array(
      '#type' => 'select',
      '#title' => 'Extra Small (xs)',
      '#options' => _bootstrap_elements_column_options(),
    ),
    'sm' => array(
      '#type' => 'select',
      '#title' => 'Small (sm)',
      '#options' => _bootstrap_elements_column_options(),
    ),
    'md' => array(
      '#type' => 'select',
      '#title' => 'Medium (md)',
      '#options' => _bootstrap_elements_column_options(),
    ),
    'lg' => array(
      '#type' => 'select',
      '#title' => 'Large (lg)',
      '#options' => _bootstrap_elements_column_options(),
    ),
  );

  if (isset($defaults['xs'])) {
    $element['xs']['#default_value'] = $defaults['xs'];
  }
  if (isset($defaults['sm'])) {
    $element['sm']['#default_value'] = $defaults['sm'];
  }
  if (isset($defaults['md'])) {
    $element['md']['#default_value'] = $defaults['md'];
  }
  if (isset($defaults['lg'])) {
    $element['lg']['#default_value'] = $defaults['lg'];
  }

  return $element;
}


/**
 *
 */
function _bootstrap_elements_popover_options($defaults = array()) {
  $element = array(
    '#tree' => TRUE,
    'title' => array(
      '#type' => 'textfield',
      '#title' => 'Title',
    ),
    'description' => array(
      '#type' => 'textarea',
      '#title' => 'Description',
    ),
    'position' => array(
      '#type' => 'select',
      '#title' => 'Position',
      '#options' => array(
        'top' => 'Top',
        'right' => 'Right',
        'bottom' => 'Bottom',
        'left' => 'Left',
      ),
    ),
  );

  if (isset($defaults['title'])) {
    $element['title']['#default_value'] = $defaults['title'];
  }

  if (isset($defaults['description'])) {
    $element['description']['#default_value'] = $defaults['description'];
  }

  if (isset($defaults['position'])) {
    $element['position']['#default_value'] = $defaults['position'];
  }

  return $element;

}


/**
 *
 */
function _bootstrap_elements_tooltip_options($defaults = array()) {

  $element = array(
    '#tree' => TRUE,
    'title' => array(
      '#type' => 'textfield',
      '#title' => 'Tooltip',
    ),
    'position' => array(
      '#type' => 'select',
      '#title' => 'Position',
      '#options' => array(
        'top' => 'Top',
        'right' => 'Right',
        'bottom' => 'Bottom',
        'left' => 'Left',
      ),
    ),
  );

  if (isset($defaults['title'])) {
    $element['title']['#default_value'] = $defaults['title'];
  }

  if (isset($defaults['position'])) {
    $element['position']['#default_value'] = $defaults['position'];
  }

  return $element;
}


/**
 *
 */
function _bootstrap_elements_addons() {
  return array(
    '#type' => 'fieldset',
    '#title' => 'Field Addons',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
  );
}

/**
 *
 */
function _bootstrap_elements_popovers() {
  return array(
    '#type' => 'fieldset',
    '#title' => 'Popovers',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
  );
}

/**
 *
 */
function _bootstrap_elements_wrappers() {
  return array(
    '#type' => 'fieldset',
    '#title' => 'Field Wrappers',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
  );
}

/**
 *
 */
function _bootstrap_elements_title_popover() {
  return array(
    '#type' => 'fieldset',
    '#title' => 'Title popover',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
}

/**
 *
 */
function _bootstrap_elements_suffix_popover() {
  return array(
    '#type' => 'fieldset',
    '#title' => 'Suffix popover',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
}

/**
 *
 */
function _bootstrap_elements_prefix_popover() {
  return array(
    '#type' => 'fieldset',
    '#title' => 'Prefix popover',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
}

/**
 *
 */
function _bootstrap_elements_title_tooltip() {
  return array(
    '#type' => 'fieldset',
    '#title' => 'Title tooltip',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
}

/**
 *
 */
function _bootstrap_elements_suffix_tooltip() {
  return array(
    '#type' => 'fieldset',
    '#title' => 'Suffix tooltip',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
}

/**
 *
 */
function _bootstrap_elements_prefix_tooltip() {
  return array(
    '#type' => 'fieldset',
    '#title' => 'Prefix tooltip',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
}


/**
 *
 */
function _bootstrap_elements_title_grid() {
  return array(
    '#type' => 'fieldset',
    '#title' => 'Title Grid',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
  );
}


/**
 *
 */
function  _bootstrap_elements_element_grid() {
  return array(
    '#type' => 'fieldset',
    '#title' => 'Element Grid',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
  );
}


/**
 *
 */
function _bootstrap_elements_form_attributes() {
  return array(
    '#type' => 'fieldset',
    '#title' => 'Form Attributes',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
  );
}

/**
 *
 */
function _bootstrap_elements_tooltips() {
  return array(
    '#type' => 'fieldset',
    '#title' => 'Tooltips',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
    // '#weight' => '-95'
  );
}

/**
 *
 */
function _bootstrap_elements_grid($id) {
  return array(
    '#type' => 'fieldset',
    '#title' => 'Grid Settings',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#states' => array(
      'visible' => array(
  // Action to take.
        $id => array('value' => 'horizontal'),
      ),
    ),
  );
}
