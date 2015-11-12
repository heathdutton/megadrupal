<?php

/**
 * @file
 * Handler which extends the views_handler_area_text to add roles for global text field.
 */

class views_role_based_global_text_handler_extender extends views_handler_area_text {

  public function option_definition() {
    $options = parent::option_definition();
    $options['roles_fieldset']['roles'] = array('default' => NULL);
    return $options;
  }

  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['roles_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('Roles'),
      '#weight' => 10,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $roles_default_value = $this->options['roles_fieldset']['roles'];
    $form['roles_fieldset']['roles'] = array(
      '#type' => 'checkboxes',
      '#options' => drupal_map_assoc(user_roles()),
      '#title' => t('Select Role'),
      '#default_value' => isset($roles_default_value) ? $roles_default_value : array(),
      '#description' => t('Only the checked roles will be able to access this value. If no role is selected, available to all.')
    );
  }

  function options_submit(&$form, &$form_state) {
    $form_state['values']['options']['roles_fieldset']['roles'] = $form_state['values']['options']['roles_fieldset']['roles'];
    parent::options_submit($form, $form_state);
  }

  /**
   * Render a text area, using the proper format.
   */
  function render($empty = FALSE) {
    global $user;
    $roles_selected = $this->options['roles_fieldset']['roles'];
    if (!is_array($roles_selected)) {
      $roles_selected = array();
    }
    $any_role_selected = array_filter($roles_selected);
    //If user has the one of the selected role or no role is selected.
    if (array_intersect($user->roles, $roles_selected) || empty($any_role_selected)) {
      return parent::render($empty);
    }
    return '';
  }
}
