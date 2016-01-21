<?php

/**
 * @file
 * Contains the angles_reaction subclass.
 */

/**
 * Expose angles templates as context reactions.
 */
class angles_ngtemplate_reaction extends context_reaction {
  /**
   * Editor form.
   */
  public function editor_form($context) {
    $form = $this->options_form($context);
    return $form;
  }

  /**
   * Submit handler for editor form.
   */
  public function editor_form_submit($context, $values) {
    return trim($values);
  }

  /**
   * Creates the configuration form for the Angles context reaction.
   */
  public function options_form($context) {
    $values = $this->fetch_from_context($context);
    $form = array(
      'template' => array(
        '#type' => 'select',
        '#title' => t('Choose the Angular JS template you wish to pre-load for a directive'),
        '#default_value' => isset($values['template']) ? $values['template'] : '',
        '#options' => _angles_get_options('template'),
        '#required' => FALSE,
      ),
    );
    return $form;
  }

  /**
   * Modifies the body tag.
   */
  public function execute(&$vars = NULL) {
    foreach ($this->get_contexts() as $v) {
      if (!empty($v->reactions[$this->plugin])) {
        // Build the settings array for the configured Angles reaction.
        $reaction = $v->reactions[$this->plugin];
        return $reaction['template'];
      }
    }
  }
}
