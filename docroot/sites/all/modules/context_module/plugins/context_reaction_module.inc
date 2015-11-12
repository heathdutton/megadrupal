<?php
/**
 * @file
 * Context reaction plugin for Contextual variables.
 */

/**
 * Apply image styles to favicon as context reactions.
 */
class context_reaction_module extends context_reaction {
  /**
   * Options form.
   */
  function options_form($context) {
    $form = array();

    $form['modules'] = array(
      '#type' => 'container',
      '#prefix' => '<div id="context-modules-ajax-wrapper">',
      '#suffix' => '</div>',
      '#theme' => 'context_module_form',
    );

    $form['modules']['module'] = array();
    if (!empty($context->reactions['module'])) {
      $form['modules']['remove'] = array();
      foreach ($context->reactions['module'] as $module => $status) {
        $form['modules']['remove'][$module] = array(
          '#type' => 'button',
          '#value' => t('Remove'),
          '#name' => $module,
          '#ajax' => array(
            'callback' => 'context_module_options_form_ajax',
            'wrapper' => 'context-modules-ajax-wrapper',
          ),
          '#limit_validation_errors' => TRUE,
        );
        $form['modules']['module'][$module] = array(
          '#type' => 'hidden',
          '#value' => $status,
        );
      }
    }

    // Add a module.
    $form['modules']['add'] = array(
      '#type' => 'container',
    );
    $form['modules']['add']['name'] = array(
      '#type' => 'select',
      '#title' => t('Module'),
      '#options' => $this->modules_get(array_keys($form['modules']['module'])),
    );
    $form['modules']['add']['enable'] = array(
      '#type' => 'button',
      '#value' => t('Enable'),
      '#ajax' => array(
        'callback' => 'context_module_options_form_ajax',
        'wrapper' => 'context-modules-ajax-wrapper',
      ),
      '#limit_validation_errors' => TRUE,
    );
    $form['modules']['add']['disable'] = array(
      '#type' => 'button',
      '#value' => t('Disable'),
      '#ajax' => array(
        'callback' => 'context_module_options_form_ajax',
        'wrapper' => 'context-modules-ajax-wrapper',
      ),
      '#limit_validation_errors' => TRUE,
    );

    return $form;
  }

  /**
   * Get a list of modules.
   *
   * @param $exclude
   *   An optional array of modules to exclude from the list of returned
   *   modules.
   *
   * @return
   *   A nested array of modules, grouped by their packages.
   */
  function modules_get($exclude = array()) {
    $modules = array();
    foreach (context_module_modules_get() as $name => $module) {
      if (!isset($modules[$module->info['package']])) {
        $modules[$module->info['package']] = array();
      }
      if (!in_array($name, $exclude)) {
        $modules[$module->info['package']][$name] = $module->info['name'];
      }
    }
    ksort($modules);
    foreach ($modules as &$group) {
      if (is_array($group)) {
        ksort($group);
      }
    }
    return $modules;
  }

  /**
   * Options form submit handler.
   */
  function options_form_submit($values) {
    $modules = isset($values['modules']['module']) ? $values['modules']['module'] : array();

    // AJAX handler for Add and Remove buttons.
    if (strstr(request_uri(), 'system/ajax')) {
      $form_state = array('submitted' => FALSE);
      $form_build_id = $_POST['form_build_id'];
      $form = form_get_cache($form_build_id, $form_state);

      $form_state['input'] = $_POST;
      $form_state['values'] = array();
      $form = form_builder($form['#form_id'], $form, $form_state);

      switch ($form_state['triggering_element']['#value']) {
        case t('Enable'):
        case t('Disable'):
          $modules[$values['modules']['add']['name']] = $form_state['triggering_element']['#value'] == t('Enable') ? 'enable' : 'disable';
          break;

        case t('Remove'):
          unset($modules[$form_state['triggering_element']['#name']]);
          break;
      }
    }

    return $modules;
  }

  /**
   * Execute.
   */
  function execute() {
    foreach ($this->get_contexts() as $context) {
      if (isset($context->reactions['module'])) {
        // Build list of actionable modules.
        $enable = $disable = array();
        foreach ($context->reactions['module'] as $module => $state) {
          if ((!module_exists($module) && 'enable' == $state) || (module_exists($module) && 'disable' == $state)) {
            ${$state}[] = $module;
          }
        }

        // Action modules.
        if (!empty($enable) || !empty($disable)) {
          module_enable($enable);
          module_disable($disable);
          drupal_goto();
        }
      }
    }
  }
}

/**
 * AJAX callback for Context reaction options form.
 */
function context_module_options_form_ajax($form, $form_state) {
  return $form['reactions']['plugins']['module']['modules'];
}
