<?php

/**
 * @file
 * Node specific options.
 */

/**
 * Implements hook_form_alter().
 *
 * Used by node type add and node edit forms.
 */
function bootstrap_elements_form_alter(&$form, &$form_state, $form_id) {

  // Deal with the node add and edit forms.
  if (isset($form['#node_edit_form'])) {

    // Find the name of the bundle so we can access the saved form settings.
    $bundle = $form['type']['#value'];

    // Load some saved settings.
    $title_grid = variable_get('title_grid_' . $bundle, array());
    $element_grid = variable_get('element_grid_' . $bundle, array());
    $tooltips = variable_get('tooltips_' . $bundle, array());
    $popovers = variable_get('popovers_' . $bundle, array());
    $addons = variable_get('addons_' . $bundle, array());
    $wrappers = variable_get('wrappers_' . $bundle, array());

    // Load the title specific settings.
    $form['title']['#description'] = variable_get('title_description_' . $bundle);
    $form['title']['#form_type'] = variable_get('form_type_' . $bundle, BOOTSTRAP_ELEMENTS_FORM_TYPE_DEFAULT);
    $form['title']['#title_display'] = variable_get('title_display_' . $bundle, BOOTSTRAP_ELEMENTS_TITLE_DISPLAY_DEFAULT);

    // Addons.
    $form['title']['#field_prefix'] = isset($addons['field_prefix']) ? $addons['field_prefix'] : BOOTSTRAP_ELEMENTS_FIELD_PREFIX_DEFAULT;
    $form['title']['#field_suffix'] = isset($addons['field_suffix']) ? $addons['field_suffix'] : BOOTSTRAP_ELEMENTS_FIELD_SUFFIX_DEFAULT;
    $form['title']['#input_group'] = isset($addons['input_group']) ? $addons['input_group'] : BOOTSTRAP_ELEMENTS_INPUT_GROUP_DEFAULT;

    // Tooltips.
    $form['title']['#title_tooltip'] = isset($tooltips['title_tooltip']) ? $tooltips['title_tooltip'] : array();
    $form['title']['#prefix_tooltip'] = isset($tooltips['prefix_tooltip']) ? $tooltips['prefix_tooltip'] : array();
    $form['title']['#suffix_tooltip'] = isset($tooltips['suffix_tooltip']) ? $tooltips['suffix_tooltip'] : array();

    // Popovers.
    $form['title']['#title_popover'] = isset($popovers['title_popover']) ? $popovers['title_popover'] : array();
    $form['title']['#prefix_popover'] = isset($popovers['prefix_popover']) ? $popovers['prefix_popover'] : array();
    $form['title']['#suffix_popover'] = isset($popovers['suffix_popover']) ? $popovers['suffix_popover'] : array();

    // Grid settings.
    $form['title']['#title_grid'] = $title_grid;
    $form['title']['#element_grid'] = $element_grid;

    // Load some defaults here.
    $form['title'] += array(
      '#form_type' => BOOTSTRAP_ELEMENTS_FORM_TYPE_DEFAULT,
      '#title_display' => BOOTSTRAP_ELEMENTS_TITLE_DISPLAY_DEFAULT,
      '#title_width' => BOOTSTRAP_ELEMENTS_TITLE_WIDTH_DEFAULT,
      '#field_prefix' => BOOTSTRAP_ELEMENTS_FIELD_PREFIX_DEFAULT,
      '#field_suffix' => BOOTSTRAP_ELEMENTS_FIELD_SUFFIX_DEFAULT,
      '#input_group' => BOOTSTRAP_ELEMENTS_INPUT_GROUP_DEFAULT,
      '#title_tooltip' => BOOTSTRAP_ELEMENTS_TITLE_TOOLTIP_DEFAULT,
      '#element_offset' => BOOTSTRAP_ELEMENTS_ELEMENT_OFFSET_DEFAULT,
    );

    // Add the actions data.
    $actions_element_grid = variable_get('actions_element_grid_' . $bundle, array());
    $form['actions']['#form_type'] = variable_get('actions_form_type_' . $bundle, BOOTSTRAP_ELEMENTS_FORM_TYPE_DEFAULT);
    $form['actions']['#element_grid'] = $actions_element_grid;

  }

  if (isset($form['#node_type'])) {

    // Find the name of the bundle so we can access the saved form settings.
    $bundle = $form['#node_type']->type;

    // Add our submit handler.
    $form['#submit'][] = '_bootstrap_elements_node_form_submit';

    // Add description text to the existing title field.
    $form['submission']['title_label']['#weight'] = -1;
    $form['submission']['title_description'] = array(
      '#title' => 'Description',
      '#type' => 'textfield',
      '#description' => t('Enter a description to use beneath the title on the add node form.'),
      '#default_value' => variable_get('title_description_' . $bundle, ''),
      '#weight' => -1,
    );

    // Wrap in a collapsible fieldset in an attempt to keep things pretty.
    $form['submission']['element_layout'] = array(
      '#type' => 'fieldset',
      '#title' => 'Title Layout',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    // Load some saved settings.
    $title_grid = variable_get('title_grid_' . $form['#node_type']->type, array());
    $element_grid = variable_get('element_grid_' . $form['#node_type']->type, array());
    $tooltips = variable_get('tooltips_' . $form['#node_type']->type, array());
    $popovers = variable_get('popovers_' . $form['#node_type']->type, array());
    $addons = variable_get('addons_' . $form['#node_type']->type, array());
    $wrappers = variable_get('wrappers_' . $form['#node_type']->type, array());

    // Build the defaults.
    $defaults = array(
      'form_type' => variable_get('form_type_' . $form['#node_type']->type, BOOTSTRAP_ELEMENTS_FORM_TYPE_DEFAULT),
      'title_display' => variable_get('title_display_' . $form['#node_type']->type, BOOTSTRAP_ELEMENTS_TITLE_DISPLAY_DEFAULT),
      'title_grid' => array(
        'columns' => array(
          'xs' => isset($title_grid['columns']['xs']) ? $title_grid['columns']['xs'] : '',
          'sm' => isset($title_grid['columns']['sm']) ? $title_grid['columns']['sm'] : '',
          'md' => isset($title_grid['columns']['md']) ? $title_grid['columns']['md'] : '',
          'lg' => isset($title_grid['columns']['lg']) ? $title_grid['columns']['lg'] : '',
        ),
        'offset' => array(
          'xs' => isset($title_grid['offset']['xs']) ? $title_grid['offset']['xs'] : '',
          'sm' => isset($title_grid['offset']['sm']) ? $title_grid['offset']['sm'] : '',
          'md' => isset($title_grid['offset']['md']) ? $title_grid['offset']['md'] : '',
          'lg' => isset($title_grid['offset']['lg']) ? $title_grid['offset']['lg'] : '',
        ),
        'push' => array(
          'xs' => isset($title_grid['push']['xs']) ? $title_grid['push']['xs'] : '',
          'sm' => isset($title_grid['push']['sm']) ? $title_grid['push']['sm'] : '',
          'md' => isset($title_grid['push']['md']) ? $title_grid['push']['md'] : '',
          'lg' => isset($title_grid['push']['lg']) ? $title_grid['push']['lg'] : '',
        ),
        'pull' => array(
          'xs' => isset($title_grid['pull']['xs']) ? $title_grid['pull']['xs'] : '',
          'sm' => isset($title_grid['pull']['sm']) ? $title_grid['pull']['sm'] : '',
          'md' => isset($title_grid['pull']['md']) ? $title_grid['pull']['md'] : '',
          'lg' => isset($title_grid['pull']['lg']) ? $title_grid['pull']['lg'] : '',
        ),
      ),
      'element_grid' => array(
        'columns' => array(
          'xs' => isset($element_grid['columns']['xs']) ? $element_grid['columns']['xs'] : '',
          'sm' => isset($element_grid['columns']['sm']) ? $element_grid['columns']['sm'] : '',
          'md' => isset($element_grid['columns']['md']) ? $element_grid['columns']['md'] : '',
          'lg' => isset($element_grid['columns']['lg']) ? $element_grid['columns']['lg'] : '',
        ),
        'offset' => array(
          'xs' => isset($element_grid['offset']['xs']) ? $element_grid['offset']['xs'] : '',
          'sm' => isset($element_grid['offset']['sm']) ? $element_grid['offset']['sm'] : '',
          'md' => isset($element_grid['offset']['md']) ? $element_grid['offset']['md'] : '',
          'lg' => isset($element_grid['offset']['lg']) ? $element_grid['offset']['lg'] : '',
        ),
        'push' => array(
          'xs' => isset($element_grid['push']['xs']) ? $element_grid['push']['xs'] : '',
          'sm' => isset($element_grid['push']['sm']) ? $element_grid['push']['sm'] : '',
          'md' => isset($element_grid['push']['md']) ? $element_grid['push']['md'] : '',
          'lg' => isset($element_grid['push']['lg']) ? $element_grid['push']['lg'] : '',
        ),
        'pull' => array(
          'xs' => isset($element_grid['pull']['xs']) ? $element_grid['pull']['xs'] : '',
          'sm' => isset($element_grid['pull']['sm']) ? $element_grid['pull']['sm'] : '',
          'md' => isset($element_grid['pull']['md']) ? $element_grid['pull']['md'] : '',
          'lg' => isset($element_grid['pull']['lg']) ? $element_grid['pull']['lg'] : '',
        ),
      ),
      'tooltips' => array(
        'title_tooltip' => array(
          'title' => isset($tooltips['title_tooltip']['title']) ? $tooltips['title_tooltip']['title'] : '',
          'position' => isset($tooltips['title_tooltip']['position']) ? $tooltips['title_tooltip']['position'] : '',
        ),
        'prefix_tooltip' => array(
          'title' => isset($tooltips['prefix_tooltip']['title']) ? $tooltips['prefix_tooltip']['title'] : '',
          'position' => isset($tooltips['prefix_tooltip']['position']) ? $tooltips['prefix_tooltip']['position'] : '',
        ),
        'suffix_tooltip' => array(
          'title' => isset($tooltips['suffix_tooltip']['title']) ? $tooltips['suffix_tooltip']['title'] : '',
          'position' => isset($tooltips['suffix_tooltip']['position']) ? $tooltips['suffix_tooltip']['position'] : '',
        ),
      ),
      'popovers' => array(
        'title_popover' => array(
          'title' => isset($popovers['title_popover']['title']) ? $popovers['title_popover']['title'] : '',
          'description' => isset($popovers['title_popover']['description']) ? $popovers['title_popover']['description'] : '',
          'position' => isset($popovers['title_popover']['position']) ? $popovers['title_popover']['position'] : '',
        ),
        'prefix_popover' => array(
          'title' => isset($popovers['prefix_popover']['title']) ? $popovers['prefix_popover']['title'] : '',
          'description' => isset($popovers['prefix_popover']['description']) ? $popovers['prefix_popover']['description'] : '',
          'position' => isset($popovers['prefix_popover']['position']) ? $popovers['prefix_popover']['position'] : '',
        ),
        'suffix_popover' => array(
          'title' => isset($popovers['suffix_popover']['title']) ? $popovers['suffix_popover']['title'] : '',
          'description' => isset($popovers['suffix_popover']['description']) ? $popovers['suffix_popover']['description'] : '',
          'position' => isset($popovers['suffix_popover']['position']) ? $popovers['suffix_popover']['position'] : '',
        ),
      ),
      'addons' => array(
        'field_prefix' => isset($addons['field_prefix']) ? $addons['field_prefix'] : BOOTSTRAP_ELEMENTS_FIELD_PREFIX_DEFAULT,
        'field_suffix' => isset($addons['field_suffix']) ? $addons['field_suffix'] : BOOTSTRAP_ELEMENTS_FIELD_SUFFIX_DEFAULT,
        'input_group' => isset($addons['input_group']) ? $addons['input_group'] : BOOTSTRAP_ELEMENTS_INPUT_GROUP_DEFAULT,
      ),
      'wrappers' => array(
        'form_type_wrapper' => isset($wrappers['form_type_wrapper']) ? $wrappers['form_type_wrapper'] : BOOTSTRAP_ELEMENTS_FORM_TYPE_WRAPPER_DEFAULT,
        'form_group_wrapper' => isset($wrappers['form_group_wrapper']) ? $wrappers['form_group_wrapper'] : BOOTSTRAP_ELEMENTS_FORM_GROUP_WRAPPER_DEFAULT,
        'form_field_wrapper' => isset($wrappers['form_field_wrapper']) ? $wrappers['form_field_wrapper'] : BOOTSTRAP_ELEMENTS_FORM_FIELD_WRAPPER_DEFAULT,
        'element_inline_wrapper' => isset($wrappers['element_inline_wrapper']) ? $wrappers['element_inline_wrapper'] : BOOTSTRAP_ELEMENTS_ELEMENT_INLINE_WRAPPER_DEFAULT,
      ),
    );

    // Add the form elements and apply the defaults.
    _bootstrap_elements_settings($form['submission']['element_layout'], $defaults);

    // Actions Layout options.
    $actions_element_grid = variable_get('actions_element_grid_' . $form['#node_type']->type, array());
    $defaults = array(
      'actions_form_type' => variable_get('actions_form_type_' . $form['#node_type']->type, BOOTSTRAP_ELEMENTS_FORM_TYPE_DEFAULT),
      'actions_element_grid' => array(
        'columns' => array(
          'xs' => isset($actions_element_grid['columns']['xs']) ? $actions_element_grid['columns']['xs'] : '',
          'sm' => isset($actions_element_grid['columns']['sm']) ? $actions_element_grid['columns']['sm'] : '',
          'md' => isset($actions_element_grid['columns']['md']) ? $actions_element_grid['columns']['md'] : '',
          'lg' => isset($actions_element_grid['columns']['lg']) ? $actions_element_grid['columns']['lg'] : '',
        ),
        'offset' => array(
          'xs' => isset($actions_element_grid['offset']['xs']) ? $actions_element_grid['offset']['xs'] : '',
          'sm' => isset($actions_element_grid['offset']['sm']) ? $actions_element_grid['offset']['sm'] : '',
          'md' => isset($actions_element_grid['offset']['md']) ? $actions_element_grid['offset']['md'] : '',
          'lg' => isset($actions_element_grid['offset']['lg']) ? $actions_element_grid['offset']['lg'] : '',
        ),
        'push' => array(
          'xs' => isset($actions_element_grid['push']['xs']) ? $actions_element_grid['push']['xs'] : '',
          'sm' => isset($actions_element_grid['push']['sm']) ? $actions_element_grid['push']['sm'] : '',
          'md' => isset($actions_element_grid['push']['md']) ? $actions_element_grid['push']['md'] : '',
          'lg' => isset($actions_element_grid['push']['lg']) ? $actions_element_grid['push']['lg'] : '',
        ),
        'pull' => array(
          'xs' => isset($actions_element_grid['pull']['xs']) ? $actions_element_grid['pull']['xs'] : '',
          'sm' => isset($actions_element_grid['pull']['sm']) ? $actions_element_grid['pull']['sm'] : '',
          'md' => isset($actions_element_grid['pull']['md']) ? $actions_element_grid['pull']['md'] : '',
          'lg' => isset($actions_element_grid['pull']['lg']) ? $actions_element_grid['pull']['lg'] : '',
        ),
      ),
    );

    $form['submission']['actions_layout'] = array(
      '#type' => 'fieldset',
      '#title' => t('Actions Layout'),
      '#description' => t('The following settings will apply to the action buttons at the bottom of the form.'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    // Form Type.
    $form['submission']['actions_layout']['actions_form_type'] = _bootstrap_elements_form_type($defaults['actions_form_type']);

    // Element grid fieldset.
    $form['submission']['actions_layout']['actions_element_grid'] = _bootstrap_elements_element_grid();
    $form['submission']['actions_layout']['actions_element_grid']['columns'] = _bootstrap_elements_columns($defaults['actions_element_grid']['columns']);
    $form['submission']['actions_layout']['actions_element_grid']['offset'] = _bootstrap_elements_offset($defaults['actions_element_grid']['offset']);
    $form['submission']['actions_layout']['actions_element_grid']['push'] = _bootstrap_elements_push($defaults['actions_element_grid']['push']);
    $form['submission']['actions_layout']['actions_element_grid']['pull'] = _bootstrap_elements_pull($defaults['actions_element_grid']['pull']);
  }
}

/**
 * Form submit handler.
 *
 * @param $form
 * @param $form_state
 */
function _bootstrap_elements_node_form_submit($form, &$form_state) {

  // Extract out tooltips.
  $variables = $form_state['values'];

  // List the arrays we want to save as a tree.
  $keys = array(
    'tooltips',
    'popovers',
    'addons',
    'wrappers',
    'title_grid',
    'element_grid',
    'actions_element_grid',
  );

  // Loop through the keys and save the arrays.
  foreach ($keys as $key) {
    variable_set($key . '_' . $variables['type'], $variables[$key]);
  }

}
