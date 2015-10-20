<?php

class olebs_behavior_blockswitcher extends openlayers_behavior {
  /**
   * Override of options_init().
   */
  function options_init() {
    $options = array(
      'baselayers_element_type' => 'radios'
    );
    return $options;
  }

  /**
   * Override of options_form().
   */
  function options_form($defaults = array()) {
    $form = parent::options_form();

    $form['baselayers_element_type'] = array(
      '#type' => 'select',
      '#options' => array(
        'select' => 'Select element',
        'radios' => 'Radios elements'
      ),
      '#default_value' => isset($defaults['baselayers_element_type']) ? $defaults['baselayers_element_type'] : 'radios',
      '#description' => t('Select the element type that the layerswitcher block will use.')
    );

    return $form;
  }

  /**
   * Render.
   */
  function render(&$map) {
    return $this->options;
  }
}
