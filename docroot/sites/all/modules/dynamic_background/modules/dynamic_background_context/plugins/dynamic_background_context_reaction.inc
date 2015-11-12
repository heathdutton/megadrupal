<?php

class DynamicBackgroundReaction extends context_reaction {
  
  function options_form($context) {
    $form = array();
    $form['dynamic_background'] = array(
      '#type' => 'fieldset',
      '#title' => t('Dynamic background'),
      '#description' => t('Select the image that you want for the current context.'),
      '#collapsed' => FALSE,
      '#collapsible' => TRUE,
      '#tree' => TRUE,
    );

    // Add the image selection part of the form.
    $form['dynamic_background'] += dynamic_background_image_selector_form($this->find_selected_image($context));

    return $form;
  }

  /**
   * Options form submit handler.
   */
  function options_form_submit($values) {
    // Remove the extra level, that the field set above creates.
    return array_pop($values);
  }

  /**
   * Find the selected image and return its id.
   */
  function execute() {
    $image_id = NULL;
    foreach ($this->get_contexts() as $context) {
      $image_id = $this->find_selected_image($context);
    }
    return $image_id;
  }

  function find_selected_image($context) {
    // Find the currently selected image
    if (isset($context->reactions['dynamic_background'])) {
      foreach ($context->reactions['dynamic_background'] as $id => $value) {
        if (isset($value['selected']) && $value['selected']) {
          return $id;
        }
      }
    }
    return NULL;
  }
}
