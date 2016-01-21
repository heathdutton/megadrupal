<?php
/**
 * @file
 * Webform specific options.
 */

/**
 * Implements HOOK_form_FORM_ID_alter().
 */
function bootstrap_elements_form_webform_component_edit_form_alter(&$form, $form_state) {

  // Save a reference to the component for ease of use.
  $component = $form_state['build_info']['args'][1];

  if (!$component['extra']['aslist'] && ($component['type'] == 'select')) {
    if ($component['extra']['multiple']) {
      $type = 'checkboxes';
    }
    else {
      $type = 'radios';
    }
  }
  else {
    $type = $component['type'];
  }
  // Build the defaults.
  $defaults = array(
    'type' => $type,
    'form_type' => isset($component['extra']['form_type']) ? $component['extra']['form_type'] : BOOTSTRAP_ELEMENTS_FORM_TYPE_DEFAULT,
    'title_display' => isset($component['extra']['title_display']) ? $component['extra']['title_display'] : BOOTSTRAP_ELEMENTS_TITLE_DISPLAY_DEFAULT,
    'title_grid' => array(
      'columns' => array(
        'xs' => isset($component['extra']['grid']['title_grid']['columns']['xs']) ? $component['extra']['grid']['title_grid']['columns']['xs'] : '',
        'sm' => isset($component['extra']['grid']['title_grid']['columns']['sm']) ? $component['extra']['grid']['title_grid']['columns']['sm'] : '',
        'md' => isset($component['extra']['grid']['title_grid']['columns']['md']) ? $component['extra']['grid']['title_grid']['columns']['md'] : '',
        'lg' => isset($component['extra']['grid']['title_grid']['columns']['lg']) ? $component['extra']['grid']['title_grid']['columns']['lg'] : '',
      ),
      'offset' => array(
        'xs' => isset($component['extra']['grid']['title_grid']['offset']['xs']) ? $component['extra']['grid']['title_grid']['offset']['xs'] : '',
        'sm' => isset($component['extra']['grid']['title_grid']['offset']['sm']) ? $component['extra']['grid']['title_grid']['offset']['sm'] : '',
        'md' => isset($component['extra']['grid']['title_grid']['offset']['md']) ? $component['extra']['grid']['title_grid']['offset']['md'] : '',
        'lg' => isset($component['extra']['grid']['title_grid']['offset']['lg']) ? $component['extra']['grid']['title_grid']['offset']['lg'] : '',
      ),
      'push' => array(
        'xs' => isset($component['extra']['grid']['title_grid']['push']['xs']) ? $component['extra']['grid']['title_grid']['push']['xs'] : '',
        'sm' => isset($component['extra']['grid']['title_grid']['push']['sm']) ? $component['extra']['grid']['title_grid']['push']['sm'] : '',
        'md' => isset($component['extra']['grid']['title_grid']['push']['md']) ? $component['extra']['grid']['title_grid']['push']['md'] : '',
        'lg' => isset($component['extra']['grid']['title_grid']['push']['lg']) ? $component['extra']['grid']['title_grid']['push']['lg'] : '',
      ),
      'pull' => array(
        'xs' => isset($component['extra']['grid']['title_grid']['pull']['xs']) ? $component['extra']['grid']['title_grid']['pull']['xs'] : '',
        'sm' => isset($component['extra']['grid']['title_grid']['pull']['sm']) ? $component['extra']['grid']['title_grid']['pull']['sm'] : '',
        'md' => isset($component['extra']['grid']['title_grid']['pull']['md']) ? $component['extra']['grid']['title_grid']['pull']['md'] : '',
        'lg' => isset($component['extra']['grid']['title_grid']['pull']['lg']) ? $component['extra']['grid']['title_grid']['pull']['lg'] : '',
      ),
    ),
    'element_grid' => array(
      'columns' => array(
        'xs' => isset($component['extra']['grid']['element_grid']['columns']['xs']) ? $component['extra']['grid']['element_grid']['columns']['xs'] : '',
        'sm' => isset($component['extra']['grid']['element_grid']['columns']['sm']) ? $component['extra']['grid']['element_grid']['columns']['sm'] : '',
        'md' => isset($component['extra']['grid']['element_grid']['columns']['md']) ? $component['extra']['grid']['element_grid']['columns']['md'] : '',
        'lg' => isset($component['extra']['grid']['element_grid']['columns']['lg']) ? $component['extra']['grid']['element_grid']['columns']['lg'] : '',
      ),
      'offset' => array(
        'xs' => isset($component['extra']['grid']['element_grid']['offset']['xs']) ? $component['extra']['grid']['element_grid']['offset']['xs'] : '',
        'sm' => isset($component['extra']['grid']['element_grid']['offset']['sm']) ? $component['extra']['grid']['element_grid']['offset']['sm'] : '',
        'md' => isset($component['extra']['grid']['element_grid']['offset']['md']) ? $component['extra']['grid']['element_grid']['offset']['md'] : '',
        'lg' => isset($component['extra']['grid']['element_grid']['offset']['lg']) ? $component['extra']['grid']['element_grid']['offset']['lg'] : '',
      ),
      'push' => array(
        'xs' => isset($component['extra']['grid']['element_grid']['push']['xs']) ? $component['extra']['grid']['element_grid']['push']['xs'] : '',
        'sm' => isset($component['extra']['grid']['element_grid']['push']['sm']) ? $component['extra']['grid']['element_grid']['push']['sm'] : '',
        'md' => isset($component['extra']['grid']['element_grid']['push']['md']) ? $component['extra']['grid']['element_grid']['push']['md'] : '',
        'lg' => isset($component['extra']['grid']['element_grid']['push']['lg']) ? $component['extra']['grid']['element_grid']['push']['lg'] : '',
      ),
      'pull' => array(
        'xs' => isset($component['extra']['grid']['element_grid']['pull']['xs']) ? $component['extra']['grid']['element_grid']['pull']['xs'] : '',
        'sm' => isset($component['extra']['grid']['element_grid']['pull']['sm']) ? $component['extra']['grid']['element_grid']['pull']['sm'] : '',
        'md' => isset($component['extra']['grid']['element_grid']['pull']['md']) ? $component['extra']['grid']['element_grid']['pull']['md'] : '',
        'lg' => isset($component['extra']['grid']['element_grid']['pull']['lg']) ? $component['extra']['grid']['element_grid']['pull']['lg'] : '',
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
        'title' => isset($component['extra']['tooltips']['title_tooltip']['title']) ? $component['extra']['tooltips']['title_tooltip']['title'] : '',
        'position' => isset($component['extra']['tooltips']['title_tooltip']['position']) ? $component['extra']['tooltips']['title_tooltip']['position'] : '',
      ),
      'prefix_tooltip' => array(
        'title' => isset($component['extra']['tooltips']['prefix_tooltip']['title']) ? $component['extra']['tooltips']['prefix_tooltip']['title'] : '',
        'position' => isset($component['extra']['tooltips']['prefix_tooltip']['position']) ? $component['extra']['tooltips']['prefix_tooltip']['position'] : '',
      ),
      'suffix_tooltip' => array(
        'title' => isset($component['extra']['tooltips']['suffix_tooltip']['title']) ? $component['extra']['tooltips']['suffix_tooltip']['title'] : '',
        'position' => isset($component['extra']['tooltips']['suffix_tooltip']['position']) ? $component['extra']['tooltips']['suffix_tooltip']['position'] : '',
      ),
    ),
    'popovers' => array(
      'title_popover' => array(
        'title' => isset($component['extra']['popovers']['title_popover']['title']) ? $component['extra']['popovers']['title_popover']['title'] : '',
        'description' => isset($component['extra']['popovers']['title_popover']['description']) ? $component['extra']['popovers']['title_popover']['description'] : '',
        'position' => isset($component['extra']['popovers']['title_popover']['position']) ? $component['extra']['popovers']['title_popover']['position'] : '',
      ),
      'prefix_popover' => array(
        'title' => isset($component['extra']['popovers']['prefix_popover']['title']) ? $component['extra']['popovers']['prefix_popover']['title'] : '',
        'description' => isset($component['extra']['popovers']['prefix_popover']['description']) ? $component['extra']['popovers']['prefix_popover']['description'] : '',
        'position' => isset($component['extra']['popovers']['prefix_popover']['position']) ? $component['extra']['popovers']['prefix_popover']['position'] : '',
      ),
      'suffix_popover' => array(
        'title' => isset($component['extra']['popovers']['suffix_popover']['title']) ? $component['extra']['popovers']['suffix_popover']['title'] : '',
        'description' => isset($component['extra']['popovers']['suffix_popover']['description']) ? $component['extra']['popovers']['suffix_popover']['description'] : '',
        'position' => isset($component['extra']['popovers']['suffix_popover']['position']) ? $component['extra']['popovers']['suffix_popover']['position'] : '',
      ),
    ),
    'addons' => array(
      'field_prefix' => isset($component['extra']['addons']['field_prefix']) ? $component['extra']['addons']['field_prefix'] : BOOTSTRAP_ELEMENTS_FIELD_PREFIX_DEFAULT,
      'field_suffix' => isset($component['extra']['addons']['field_suffix']) ? $component['extra']['addons']['field_suffix'] : BOOTSTRAP_ELEMENTS_FIELD_SUFFIX_DEFAULT,
      'input_group' => isset($component['extra']['addons']['input_group']) ? $component['extra']['addons']['input_group'] : BOOTSTRAP_ELEMENTS_INPUT_GROUP_DEFAULT,
    ),
    'wrappers' => array(
      'form_type_wrapper' => isset($component['extra']['wrappers']['form_type_wrapper']) ? $component['extra']['wrappers']['form_type_wrapper'] : BOOTSTRAP_ELEMENTS_FORM_TYPE_WRAPPER_DEFAULT,
      'form_group_wrapper' => isset($component['extra']['wrappers']['form_group_wrapper']) ? $component['extra']['wrappers']['form_group_wrapper'] : BOOTSTRAP_ELEMENTS_FORM_GROUP_WRAPPER_DEFAULT,
      'form_field_wrapper' => isset($component['extra']['wrappers']['form_field_wrapper']) ? $component['extra']['wrappers']['form_field_wrapper'] : BOOTSTRAP_ELEMENTS_FORM_FIELD_WRAPPER_DEFAULT,
      'element_inline_wrapper' => isset($component['extra']['wrappers']['element_inline_wrapper']) ? $component['extra']['wrappers']['element_inline_wrapper'] : BOOTSTRAP_ELEMENTS_ELEMENT_INLINE_WRAPPER_DEFAULT,
    ),
  );

  // Add the form elements and apply the defaults.
  _bootstrap_elements_settings($form['display'], $defaults);

}

/**
 * Implements hook_.
 *
 * @param $form
 * @param $form_state
 */
function bootstrap_elements_form_webform_configure_form_alter(&$form, $form_state) {

  $node = node_load($form['nid']['#value']);

  // Add a custom submit handlers to save the fields we added in 'bootstrap_elements_form_webform_component_edit_form_alter'
  array_unshift($form['#submit'], '_bootstrap_elements_webform_configure_form_submit');

  // Grab the element grid settings from the webform object.
  $form_actions = unserialize($node->webform['form_actions']);

  // Element grid fieldset.
  $form['advanced']['form_actions'] = _bootstrap_elements_element_grid();
  $form['advanced']['form_actions']['#title'] = t('Actions Grid');
  $form['advanced']['form_actions']['#description'] = t('Control the layout of the actions/buttons at the foot of the webform.');
  $form['advanced']['form_actions']['columns'] = _bootstrap_elements_columns($form_actions['columns']);
  $form['advanced']['form_actions']['offset'] = _bootstrap_elements_offset($form_actions['offset']);
  $form['advanced']['form_actions']['push'] = _bootstrap_elements_push($form_actions['push']);
  $form['advanced']['form_actions']['pull'] = _bootstrap_elements_pull($form_actions['pull']);

  $form['advanced']['form_attributes'] = _bootstrap_elements_form_attributes();


  $form_attributes = unserialize($node->webform['form_attributes']);

  $form['advanced']['form_attributes']['novalidate'] = array(
    '#type' => 'checkbox',
    '#default_value' => isset($form_attributes['novalidate']) ? $form_attributes['novalidate'] : 0,
    '#title' => t('Add a novalidate attribute to the form element.'),
    '#description' => t('Add the novalidate attribute to the form'),
  );

}


/**
 * Submit handler for webform_configure_form().
 */
function _bootstrap_elements_webform_configure_form_submit($form, &$form_state) {
  $webform = &$form['#node']->webform;
  $webform['form_actions'] = serialize($form_state['values']['form_actions']);
  $webform['form_attributes'] = serialize($form_state['values']['form_attributes']);
}


/**
 * Implements hook_webform_component_presave().
 *
 * @param $component
 */
function bootstrap_elements_webform_component_presave(&$component) {

  if (!empty($component['display'])) {
    $component['extra'] = array_merge($component['extra'], $component['display']);
    unset($component['display']);
  }

  // Save the title as we need it for markup elements.
  $component['extra']['title'] = $component['name'];
}


/**
 * Implements hook_form_alter().
 */
function bootstrap_elements_form_webform_client_form_alter(&$form, $form_state) {

  // Pull the indent option from the webform and apply it to the actions wrapper.
  if (isset($form['#node']->webform['form_actions'])) {
    $form_actions = unserialize($form['#node']->webform['form_actions']);
    $form['actions']['#form_type'] = 'horizontal';
    $form['actions']['#title_display'] = 'inline';
    $form['actions']['#element_grid'] = $form_actions;
  }

  if (isset($form['#node']->webform['form_attributes'])) {
    $form_attributes = unserialize($form['#node']->webform['form_attributes']);
    if (isset($form_attributes['novalidate']) && $form_attributes['novalidate']) {
      $form['#attributes']['novalidate'] = 'novalidate';
    }

  }
}
