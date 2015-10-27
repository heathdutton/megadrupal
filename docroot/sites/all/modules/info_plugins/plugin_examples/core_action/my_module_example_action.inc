<?php

$plugin = array(
  // Values you would normally set in hook_action_info().
  'label' => 'Example action',
  'type' => 'entity',
  'configurable' => TRUE,
  'triggers' => array('any'),
  'behaviour' => array('changes_property'),

  // Views Bulk Operations (VBO) configuration.
  'vbo_configurable' => FALSE,
  'aggregate' => FALSE,
  'pass rows' => FALSE,
  // An array of permissions user must have (all of them) in order to execute the action.
  'permissions' => array(),
);

/**
 * Action callback. Keep in mind that inside hook_action_info() we cannot set a
 * callback function and it defaults to the machine name of the action (in this
 * case of the plugin). It is highly recommended that your action plugin names
 * are prefixed with the module name.
 *
 * @param $object
 *   The object that the action will act on: a node, user, or comment object.
 * @param $context
 *   Associative array containing extra information about what triggered
 *   the action call, with $context['hook'] giving the name of the hook
 *   that resulted in this call to actions_do().
 * @param $a1
 *   Possibly passed along to the callback as well
 * @param $a2
 *   Possibly passed along to the callback.
 */
function my_module_example_action($object, $context, $a1, $a2) {
  // Does my action.
}

/**
 * Form callback if the action is configurable. Named based on the action
 * machine name suffixed by '_form'.
 *
 * @param $context
 *   Associative array containing extra information about the action.
 *
 * @return array
 */
function my_module_example_action_form($context) {
  return array(
    'name' => array(
      '#type' => 'textfield',
      '#title' => 'Name',
    ),
  );
}

/**
 * Form validation callback if the action is configurable. Named based on the action
 * machine name suffixed by '_validate'.
 *
 * @param $form
 * @param $form_state
 *
 * @return array
 */
function my_module_example_action_validate($form, &$form_state) {
  // Perform validation.
}

/**
 * Form submit callback if the action is configurable. Named based on the action
 * machine name suffixed by '_submit'.
 *
 * @param $form
 * @param $form_state
 *
 * @return array of key|values pairs of data from the form you want to keep in the
 * context.
 */
function my_module_example_action_submit($form, &$form_state) {
  return array(
    'name' => $form_state['values']['name']
  );
}

/**
 * Form callback for the VBO configuration form if you set the action plugin
 * to work with VBO. Named based on the action machine name suffixed by
 * 'views_bulk_operations_form'.
 *
 * @param $options
 *
 * @return array
 */
function my_module_example_action_views_bulk_operations_form($options) {
  return array(
    'name_vbo' => array(
      '#type' => 'textfield',
      '#title' => 'Name VBO',
      '#default_value' => isset($options['name_vbo']) && $options['name_vbo'] ? $options['name_vbo'] : ''
    ),
  );
}

/**
 * Validation callback for the VBO configuration form if you set the action plugin
 * to work with VBO. Named based on the action machine name suffixed by
 * 'views_bulk_operations_form_validate'.
 *
 * @param $options
 * @param $form_state
 *
 * @return array
 */
function my_module_example_action_views_bulk_operations_form_validate($options, &$form_state) {
  // Perform validation.
}

/**
 * Submit callback for the VBO configuration form if you set the action plugin
 * to work with VBO. Named based on the action machine name suffixed by
 * 'views_bulk_operations_form_submit'.
 *
 * @param $options
 * @param $form_state
 *
 * @return array
 */
function my_module_example_action_views_bulk_operations_form_submit($options, &$form_state) {
  return array(
    'name_vbo' => $form_state['values']['settings']['name_vbo'],
  );
}