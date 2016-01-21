<?php

/**
 * @file
 * Views handler for field Fortissimmo title
 */
class views_handler_field_fortissimmo_title extends views_handler_field {

  function init(&$view, &$options) {
    parent::init($view, $options);

    // Retrieve Fortissimmo ID if we need to make this a link
    if ($options['fortissimmo_title_make_link'] == 1) {
      $this->additional_fields['fortissimmo'] = array('table' => $view->base_table, 'field' => 'fortissimmo');
    }

    if ($options['fortissimmo_title_reference_fallback'] == 1) {
      // Add reference additional field for fallback reasons
      $this->additional_fields['reference'] = array('table' => $view->base_table, 'field' => 'reference');
    }
  }

  /**
   * Options definition
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['fortissimmo_title_make_link'] = array('default' => TRUE, 'bool' => TRUE);
    $options['fortissimmo_title_reference_fallback'] = array('default' => TRUE, 'bool' => TRUE);
    return $options;
  }

  /**
   * Provide options for handler
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $options = $this->options;

    $form['fortissimmo_title_make_link'] = array(
      '#type' => 'checkbox',
      '#title' => t('Link to property detail page'),
      '#default_value' => isset($options['fortissimmo_title_make_link']) ? $options['fortissimmo_title_make_link'] : TRUE,
    );

    $form['fortissimmo_title_reference_fallback'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use "reference" field as fallback if title is not present'),
      '#default_value' => isset($options['fortissimmo_title_reference_fallback']) ? $options['fortissimmo_title_reference_fallback'] : TRUE,
    );
  }

  /**
   * Render title
   */
  function render($values) {
    $options = $this->options;
    $make_link = $options['fortissimmo_title_make_link'];
    $ref_fallback = $options['fortissimmo_title_reference_fallback'];
    $title = $values->{$this->field_alias};

    // Use reference as title if enabled and available
    if (empty($title) && $ref_fallback && isset($this->aliases['reference'])) {
      $title = $values->{$this->aliases['reference']};
    }

    // Link to property detail page if enabled
    if (isset($this->aliases['fortissimmo']) && $make_link) {
      $fortissimmo_id = $values->{$this->aliases['fortissimmo']};
      $title = fortissimmo_generate_link($title, $fortissimmo_id);
    }
    return $title;
  }
}