<?php
/**
 * @file
 * Fieldgroup specific options.
 */

/**
 * Implements hook_field_group_format_settings().
 *
 * Add some title layout options to the fieldset options for fieldgroups.
 */
function bootstrap_elements_field_group_format_settings($group) {

  // Assign the settings to a variable for ease of use.
  $instance_settings = $group->format_settings['instance_settings'];
  $layout_settings = isset($instance_settings['element_layout']) ? $instance_settings['element_layout'] : array();

  // Limit our new settings fields to only the fieldset formatter.
  switch ($group->format_type) {
    case "fieldset":

      // Add a collapsible fieldset in an attempt to keep things pretty.
      $form['instance_settings']['element_layout'] = array(
        '#type' => 'fieldset',
        '#title' => 'Element Layout',
        '#weight' => 5,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );

      // Build the defaults.
      $defaults = array(
        'form_type' => isset($layout_settings['form_type']) ? $layout_settings['form_type'] : BOOTSTRAP_ELEMENTS_FORM_TYPE_DEFAULT,
        'title_display' => isset($layout_settings['title_display']) ? $layout_settings['title_display'] : BOOTSTRAP_ELEMENTS_TITLE_DISPLAY_DEFAULT,
        'title_grid' => array(
          'columns' => array(
            'xs' => isset($layout_settings['grid']['title_grid']['columns']['xs']) ? $layout_settings['grid']['title_grid']['columns']['xs'] : '',
            'sm' => isset($layout_settings['grid']['title_grid']['columns']['sm']) ? $layout_settings['grid']['title_grid']['columns']['sm'] : '',
            'md' => isset($layout_settings['grid']['title_grid']['columns']['md']) ? $layout_settings['grid']['title_grid']['columns']['md'] : '',
            'lg' => isset($layout_settings['grid']['title_grid']['columns']['lg']) ? $layout_settings['grid']['title_grid']['columns']['lg'] : '',
          ),
          'offset' => array(
            'xs' => isset($layout_settings['grid']['title_grid']['offset']['xs']) ? $layout_settings['grid']['title_grid']['offset']['xs'] : '',
            'sm' => isset($layout_settings['grid']['title_grid']['offset']['sm']) ? $layout_settings['grid']['title_grid']['offset']['sm'] : '',
            'md' => isset($layout_settings['grid']['title_grid']['offset']['md']) ? $layout_settings['grid']['title_grid']['offset']['md'] : '',
            'lg' => isset($layout_settings['grid']['title_grid']['offset']['lg']) ? $layout_settings['grid']['title_grid']['offset']['lg'] : '',
          ),
          'push' => array(
            'xs' => isset($layout_settings['grid']['title_grid']['push']['xs']) ? $layout_settings['grid']['title_grid']['push']['xs'] : '',
            'sm' => isset($layout_settings['grid']['title_grid']['push']['sm']) ? $layout_settings['grid']['title_grid']['push']['sm'] : '',
            'md' => isset($layout_settings['grid']['title_grid']['push']['md']) ? $layout_settings['grid']['title_grid']['push']['md'] : '',
            'lg' => isset($layout_settings['grid']['title_grid']['push']['lg']) ? $layout_settings['grid']['title_grid']['push']['lg'] : '',
          ),
          'pull' => array(
            'xs' => isset($layout_settings['grid']['title_grid']['pull']['xs']) ? $layout_settings['grid']['title_grid']['pull']['xs'] : '',
            'sm' => isset($layout_settings['grid']['title_grid']['pull']['sm']) ? $layout_settings['grid']['title_grid']['pull']['sm'] : '',
            'md' => isset($layout_settings['grid']['title_grid']['pull']['md']) ? $layout_settings['grid']['title_grid']['pull']['md'] : '',
            'lg' => isset($layout_settings['grid']['title_grid']['pull']['lg']) ? $layout_settings['grid']['title_grid']['pull']['lg'] : '',
          ),
        ),
        'element_grid' => array(
          'columns' => array(
            'xs' => isset($layout_settings['grid']['element_grid']['columns']['xs']) ? $layout_settings['grid']['element_grid']['columns']['xs'] : '',
            'sm' => isset($layout_settings['grid']['element_grid']['columns']['sm']) ? $layout_settings['grid']['element_grid']['columns']['sm'] : '',
            'md' => isset($layout_settings['grid']['element_grid']['columns']['md']) ? $layout_settings['grid']['element_grid']['columns']['md'] : '',
            'lg' => isset($layout_settings['grid']['element_grid']['columns']['lg']) ? $layout_settings['grid']['element_grid']['columns']['lg'] : '',
          ),
          'offset' => array(
            'xs' => isset($layout_settings['grid']['element_grid']['offset']['xs']) ? $layout_settings['grid']['element_grid']['offset']['xs'] : '',
            'sm' => isset($layout_settings['grid']['element_grid']['offset']['sm']) ? $layout_settings['grid']['element_grid']['offset']['sm'] : '',
            'md' => isset($layout_settings['grid']['element_grid']['offset']['md']) ? $layout_settings['grid']['element_grid']['offset']['md'] : '',
            'lg' => isset($layout_settings['grid']['element_grid']['offset']['lg']) ? $layout_settings['grid']['element_grid']['offset']['lg'] : '',
          ),
          'push' => array(
            'xs' => isset($layout_settings['grid']['element_grid']['push']['xs']) ? $layout_settings['grid']['element_grid']['push']['xs'] : '',
            'sm' => isset($layout_settings['grid']['element_grid']['push']['sm']) ? $layout_settings['grid']['element_grid']['push']['sm'] : '',
            'md' => isset($layout_settings['grid']['element_grid']['push']['md']) ? $layout_settings['grid']['element_grid']['push']['md'] : '',
            'lg' => isset($layout_settings['grid']['element_grid']['push']['lg']) ? $layout_settings['grid']['element_grid']['push']['lg'] : '',
          ),
          'pull' => array(
            'xs' => isset($layout_settings['grid']['element_grid']['pull']['xs']) ? $layout_settings['grid']['element_grid']['pull']['xs'] : '',
            'sm' => isset($layout_settings['grid']['element_grid']['pull']['sm']) ? $layout_settings['grid']['element_grid']['pull']['sm'] : '',
            'md' => isset($layout_settings['grid']['element_grid']['pull']['md']) ? $layout_settings['grid']['element_grid']['pull']['md'] : '',
            'lg' => isset($layout_settings['grid']['element_grid']['pull']['lg']) ? $layout_settings['grid']['element_grid']['pull']['lg'] : '',
          ),
        ),
        'tooltips' => array(
          'title_tooltip' => array(
            'title' => isset($layout_settings['tooltips']['title_tooltip']['title']) ? $layout_settings['tooltips']['title_tooltip']['title'] : '',
            'position' => isset($layout_settings['tooltips']['title_tooltip']['position']) ? $layout_settings['tooltips']['title_tooltip']['position'] : '',
          ),
          'prefix_tooltip' => array(
            'title' => isset($layout_settings['tooltips']['prefix_tooltip']['title']) ? $layout_settings['tooltips']['prefix_tooltip']['title'] : '',
            'position' => isset($layout_settings['tooltips']['prefix_tooltip']['position']) ? $layout_settings['tooltips']['prefix_tooltip']['position'] : '',
          ),
          'suffix_tooltip' => array(
            'title' => isset($layout_settings['tooltips']['suffix_tooltip']['title']) ? $layout_settings['tooltips']['suffix_tooltip']['title'] : '',
            'position' => isset($layout_settings['tooltips']['suffix_tooltip']['position']) ? $layout_settings['tooltips']['suffix_tooltip']['position'] : '',
          ),
        ),
        'popovers' => array(
          'title_popover' => array(
            'title' => isset($layout_settings['popovers']['title_popover']['title']) ? $layout_settings['popovers']['title_popover']['title'] : '',
            'description' => isset($layout_settings['popovers']['title_popover']['description']) ? $layout_settings['popovers']['title_popover']['description'] : '',
            'position' => isset($layout_settings['popovers']['title_popover']['position']) ? $layout_settings['popovers']['title_popover']['position'] : '',
          ),
          'prefix_popover' => array(
            'title' => isset($layout_settings['popovers']['prefix_popover']['title']) ? $layout_settings['popovers']['prefix_popover']['title'] : '',
            'description' => isset($layout_settings['popovers']['prefix_popover']['description']) ? $layout_settings['popovers']['prefix_popover']['description'] : '',
            'position' => isset($layout_settings['popovers']['prefix_popover']['position']) ? $layout_settings['popovers']['prefix_popover']['position'] : '',
          ),
          'suffix_popover' => array(
            'title' => isset($layout_settings['popovers']['suffix_popover']['title']) ? $layout_settings['popovers']['suffix_popover']['title'] : '',
            'description' => isset($layout_settings['popovers']['suffix_popover']['description']) ? $layout_settings['popovers']['suffix_popover']['description'] : '',
            'position' => isset($layout_settings['popovers']['suffix_popover']['position']) ? $layout_settings['popovers']['suffix_popover']['position'] : '',
          ),
        ),
        'addons' => array(
          'field_prefix' => isset($layout_settings['addons']['field_prefix']) ? $layout_settings['addons']['field_prefix'] : BOOTSTRAP_ELEMENTS_FIELD_PREFIX_DEFAULT,
          'field_suffix' => isset($layout_settings['addons']['field_suffix']) ? $layout_settings['addons']['field_suffix'] : BOOTSTRAP_ELEMENTS_FIELD_SUFFIX_DEFAULT,
          'input_group' => isset($layout_settings['addons']['input_group']) ? $layout_settings['addons']['input_group'] : BOOTSTRAP_ELEMENTS_INPUT_GROUP_DEFAULT,
        ),
        'wrappers' => array(
          'form_type_wrapper' => isset($layout_settings['wrappers']['form_type_wrapper']) ? $layout_settings['wrappers']['form_type_wrapper'] : BOOTSTRAP_ELEMENTS_FORM_TYPE_WRAPPER_DEFAULT,
          'form_group_wrapper' => isset($layout_settings['wrappers']['form_group_wrapper']) ? $layout_settings['wrappers']['form_group_wrapper'] : BOOTSTRAP_ELEMENTS_FORM_GROUP_WRAPPER_DEFAULT,
          'form_field_wrapper' => isset($layout_settings['wrappers']['form_field_wrapper']) ? $layout_settings['wrappers']['form_field_wrapper'] : BOOTSTRAP_ELEMENTS_FORM_FIELD_WRAPPER_DEFAULT,
        ),
      );

      // Add the form elements and apply the defaults.
      _bootstrap_elements_settings($form['instance_settings']['element_layout'], $defaults);

      break;
  }

  // Return the form (this gets merged with any exiting form data
  // from other implementations of the hook).
  return $form;
}


/**
 * Implements hook_field_group_pre_render().
 *
 * Copy the fieldset options to the actual FAPI element for rendering.
 */
function bootstrap_elements_field_group_pre_render(& $element, $group, $form) {

  // Add our fields to the fieldset definition.
  if ($group->format_type == 'fieldset' && isset($group->format_settings['instance_settings']['element_layout'])) {

    // Save the settings to a variable for ease of use.
    $element_layout = $group->format_settings['instance_settings']['element_layout'];

    // Define the array of fields.
    $fields = array(
      'form_type',
      'title_display',
      'grid' => array(
        'title_grid',
        'element_grid',
      ),
      'tooltips' => array(
        'title_tooltip',
        'prefix_tooltip',
        'suffix_tooltip',
      ),
      'popovers' => array(
        'title_popover',
        'prefix_popover',
        'suffix_popover',
      ),
      'addons' => array(
        'field_prefix',
        'field_suffix',
        'input_group',
      ),
      'wrappers' => array(
        'form_type_wrapper',
        'form_field_wrapper',
        'form_group_wrapper',
      ),
    );

    // Copy the settings to the FAPI elements.
    foreach ($fields as $key => $value) {

      // If the value is an array then go through the children.
      if (is_array($value)) {
        foreach ($value as $value_key => $value_value) {
          if (isset($element_layout[$key][$value_value])) {
            $element['#' . $value_value] = $element_layout[$key][$value_value];
          }
        }
      }
      else {
        if (isset($element_layout[$value])) {
          $element['#' . $value] = $element_layout[$value];
        }
      }
    }
  }
}
