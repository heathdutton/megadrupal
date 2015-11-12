<?php

class context_omega_reaction_omega_layout extends context_reaction_block {

  /**
   * Options form.
   */
  function options_form($context) {
    $form = parent::options_form($context);

    if ($layouts = context_omega_get_layouts()) {
      $options = $regions = array();
      foreach ($layouts as $key => $layout) {
        $regions[$key] = array_keys($layout['info']['regions']);
        $options[$key] = $layout['info']['name'];
      }
      asort($layouts);

      $values = $this->fetch_from_context($context);
      $form['layout'] = array(
        '#title' => t('Choose an Omega layout'),
        '#description' => t('Choose the Omega layout to use.'),
        '#type' => 'select',
        '#options' => $options,
        '#default_value' => isset($values['layout']) ? $values['layout'] : NULL,
        '#weight' => -10,
        '#empty_value' => '_default',
        '#empty_option' => t('Default'),
        '#attributes' => array(
          'class' => array('context-omega-layout-select'),
        ),
      );

      drupal_add_js(drupal_get_path('module', 'context_omega') . '/plugins/context_omega_reaction_omega_layout.js');
      drupal_add_js(array('contextOmega' => array('regions' => $regions)), 'setting');
    }

    return $form;
  }

  /**
   * Override of submit handler.
   */
  function options_form_submit($values) {
    $options = parent::options_form_submit($values);

    // Check whether a valid layout was selected.
    $layouts = context_omega_get_layouts();
    if (!empty($values['layout']) && isset($layouts[$values['layout']])) {
      $options['layout'] = $values['layout'];

      // Unset blocks attached to invalid regions.
      foreach ($options['blocks'] as $key => $block) {
        if (!isset($layouts[$options['layout']]['info']['regions'][$block['region']])) {
          unset($options['blocks'][$key]);
        }
      }
    }

    return $options;
  }

  /**
   * Retrieves the first layout specified by one of the active contexts.
   */
  function get_active_layout() {
    foreach ($this->get_contexts() as $context) {
      $values = $this->fetch_from_context($context);
      if (isset($values['layout'])) {
        return $values['layout'];
      }
    }

    // None of the active contexts specified a layout.
    return FALSE;
  }

}
