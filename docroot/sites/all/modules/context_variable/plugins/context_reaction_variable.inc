<?php
/**
 * @file
 * Context reaction plugin for Contextual variables.
 */

/**
 * Apply image styles to favicon as context reactions.
 */
class context_reaction_variable extends context_reaction {
  /**
   * Options form.
   */
  function options_form($context) {
    $form = array();

    $form['variables'] = array(
      '#type' => 'container',
      '#prefix' => '<div id="context-variables-ajax-wrapper">',
      '#suffix' => '</div>',
      '#theme' => 'context_variable_form',
    );

    $form['variables']['variable'] = array();
    if (!empty($context->reactions['variable'])) {
      $form['variables']['remove'] = array();
      foreach (array_keys($context->reactions['variable']) as $variable) {
        $form['variables']['remove'][$variable] = array(
          '#type' => 'button',
          '#value' => t('Remove'),
          '#name' => $variable,
          '#ajax' => array(
            'callback' => 'context_variable_options_form_ajax',
            'wrapper' => 'context-variables-ajax-wrapper',
          ),
          '#limit_validation_errors' => TRUE,
        );
        $form['variables']['variable'] += variable_edit_subform($variable);
      }
      $this->variables_set_default($form['variables']['variable'], $context->reactions['variable']);
    }

    // Add a variable.
    $form['variables']['add'] = array(
      '#type' => 'container',
    );
    $form['variables']['add']['name'] = array(
      '#type' => 'select',
      '#title' => t('Variable'),
      '#options' => $this->variables_get(array_keys($form['variables']['variable'])),
    );
    $form['variables']['add']['button'] = array(
      '#type' => 'button',
      '#value' => t('Add'),
      '#ajax' => array(
        'callback' => 'context_variable_options_form_ajax',
        'wrapper' => 'context-variables-ajax-wrapper',
      ),
      '#limit_validation_errors' => TRUE,
    );

    return $form;
  }

  /**
   * Recursively set variables default values.
   *
   * @param $form
   *   The Form element to be recursively iterrated over.
   *
   * @param $value
   *   The values to be used for the default values of the form elements.
   */
  function variables_set_default(&$form, $value) {
    foreach (element_children($form) as $child) {
      if (isset($value[$child])) {
        if (!in_array($form[$child]['#type'], array('container', 'fieldset'))) {
          $form[$child]['#default_value'] = $value[$child];
        }
        else {
          $this->variables_set_default($form[$child], $value[$child]);
        }
      }
    }
  }

  /**
   * Get a list of variables defined by hook_variable_info().
   *
   * @param $exclude
   *   An optional array of variables to exclude from the list of returned
   *   variables.
   *
   * @return
   *   A nested array of variables, grouped by their module.
   */
  function variables_get($exclude = array()) {
    $module_info = system_get_info('module');
    foreach (variable_get_info() as $name => $variable) {
      if (!in_array($name, $exclude)) {
        if (!isset($variables[$module_info[$variable['module']]['name']])) {
          $variables[$module_info[$variable['module']]['name']] = array();
        }
        $variables[$module_info[$variable['module']]['name']][$name] = $variable['title'];
        if (isset($variable['multiple'])) {
          unset($variables[$module_info[$variable['module']]['name']][$name]);
          $variable = variable_build($variable);
          foreach ($variable['children'] as $name => $child) {
            if (!in_array($name, $exclude)) {
              $variables[$module_info[$variable['module']]['name']][$name] = "{$variable['title']} [{$child['title']}]";
            }
          }
        }
      }
    }
    ksort($variables);
    foreach ($variables as &$group) {
      if (is_array($group)) {
        ksort($group);
      }
    }
    return $variables;
  }

  /**
   * Options form submit handler.
   */
  function options_form_submit($values) {
    $variables = array();

    foreach ($values['variables']['variable'] as $variable => $value) {
      $variables[$variable] = $value;
    }

    // AJAX handler for Add and Remove buttons.
    if (strstr(request_uri(), 'system/ajax')) {
      $form_state = array('submitted' => FALSE);
      $form_build_id = $_POST['form_build_id'];
      $form = form_get_cache($form_build_id, $form_state);

      $form_state['input'] = $_POST;
      $form_state['values'] = array();
      $form = form_builder($form['#form_id'], $form, $form_state);

      switch ($form_state['triggering_element']['#value']) {
        case t('Add'):
          $variables[$values['variables']['add']['name']] = variable_get($values['variables']['add']['name'], array());
          break;

        case t('Remove'):
          unset($variables[$form_state['triggering_element']['#name']]);
          break;
      }
    }

    return $variables;
  }

  /**
   * Execute.
   */
  function execute() {
    foreach ($this->get_contexts() as $context) {
      if (isset($context->reactions['variable'])) {
        foreach ($context->reactions['variable'] as $variable => $value) {
          $GLOBALS['conf'][$variable] = $value;
        }
      }
    }
  }
}

/**
 * AJAX callback for Context reaction options form.
 */
function context_variable_options_form_ajax($form, $form_state) {
  return $form['reactions']['plugins']['variable']['variables'];
}
