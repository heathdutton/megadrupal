<?php
/**
 * @file
 * Field specific options.
 */

/**
 * Implements hook_form_FORM_ID_form_alter().
 *
 * Add the layout options to the field's edit form.
 * These are then added to the elements FAPI array via a form alter.
 */
function bootstrap_elements_form_field_ui_field_edit_form_alter(&$form, &$form_state, $form_id) {

  // Create a new fieldset to contain the new options.
  $form['instance']['widget']['settings']['layout'] = array(
    '#type' => 'fieldset',
    '#title' => 'Layout',
  );
  $layout_settings = isset($form['#instance']['widget']['settings']['layout']) ? $form['#instance']['widget']['settings']['layout'] : array();

  // Build the defaults.
  $defaults = array(
    'type' => $form['#field']['type'],
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
    'element_columns' => array(
      'columns' => array(
        'xs' => isset($component['extra']['grid']['element_columns']['columns']['xs']) ? $component['extra']['grid']['element_columns']['columns']['xs'] : '',
        'sm' => isset($component['extra']['grid']['element_columns']['columns']['sm']) ? $component['extra']['grid']['element_columns']['columns']['sm'] : '',
        'md' => isset($component['extra']['grid']['element_columns']['columns']['md']) ? $component['extra']['grid']['element_columns']['columns']['md'] : '',
        'lg' => isset($component['extra']['grid']['element_columns']['columns']['lg']) ? $component['extra']['grid']['element_columns']['columns']['lg'] : '',
      ),
      'offset' => array(
        'xs' => isset($component['extra']['grid']['element_columns']['offset']['xs']) ? $component['extra']['grid']['element_columns']['offset']['xs'] : '',
        'sm' => isset($component['extra']['grid']['element_columns']['offset']['sm']) ? $component['extra']['grid']['element_columns']['offset']['sm'] : '',
        'md' => isset($component['extra']['grid']['element_columns']['offset']['md']) ? $component['extra']['grid']['element_columns']['offset']['md'] : '',
        'lg' => isset($component['extra']['grid']['element_columns']['offset']['lg']) ? $component['extra']['grid']['element_columns']['offset']['lg'] : '',
      ),
      'push' => array(
        'xs' => isset($component['extra']['grid']['element_columns']['push']['xs']) ? $component['extra']['grid']['element_columns']['push']['xs'] : '',
        'sm' => isset($component['extra']['grid']['element_columns']['push']['sm']) ? $component['extra']['grid']['element_columns']['push']['sm'] : '',
        'md' => isset($component['extra']['grid']['element_columns']['push']['md']) ? $component['extra']['grid']['element_columns']['push']['md'] : '',
        'lg' => isset($component['extra']['grid']['element_columns']['push']['lg']) ? $component['extra']['grid']['element_columns']['push']['lg'] : '',
      ),
      'pull' => array(
        'xs' => isset($component['extra']['grid']['element_columns']['pull']['xs']) ? $component['extra']['grid']['element_columns']['pull']['xs'] : '',
        'sm' => isset($component['extra']['grid']['element_columns']['pull']['sm']) ? $component['extra']['grid']['element_columns']['pull']['sm'] : '',
        'md' => isset($component['extra']['grid']['element_columns']['pull']['md']) ? $component['extra']['grid']['element_columns']['pull']['md'] : '',
        'lg' => isset($component['extra']['grid']['element_columns']['pull']['lg']) ? $component['extra']['grid']['element_columns']['pull']['lg'] : '',
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
      'element_inline_wrapper' => isset($layout_settings['wrappers']['element_inline_wrapper']) ? $layout_settings['wrappers']['element_inline_wrapper'] : BOOTSTRAP_ELEMENTS_ELEMENT_INLINE_WRAPPER_DEFAULT,
    ),
  );

  // Add the form elements and apply the defaults.
  _bootstrap_elements_settings($form['instance']['widget']['settings']['layout'], $defaults);

}


/**
 * Implements hook_form_alter().
 */
function bootstrap_elements_field_widget_form_alter(&$element, &$form_state, $context) {
  // @Todo should _bootstrap_elements_merge_properties exisit in bootrasp elemenst or strapped?
  if (isset($context['instance']['widget']['settings']['layout'])) {
    if (isset($element['value']) && $element['#delta'] == 0) {
      _bootstrap_elements_merge_properties($element['value'], $context['instance']['widget']['settings']['layout']);
    }
    elseif (isset($element['email'])) {
      _bootstrap_elements_merge_properties($element['email'], $context['instance']['widget']['settings']['layout']);
    }
    elseif (isset($element['target_id']) && $context['field']['cardinality'] == 1) {
      // Managed files have a key of 0 @todo this wont be enough.
      _bootstrap_elements_merge_properties($element['target_id'], $context['instance']['widget']['settings']['layout']);
    }
    elseif (isset($element[0])) {
      // Managed files have a key of 0 @todo this wont be enough.
      _bootstrap_elements_merge_properties($element[0], $context['instance']['widget']['settings']['layout']);
    }
    else {
      _bootstrap_elements_merge_properties($element, $context['instance']['widget']['settings']['layout']);
    }
  }

}
