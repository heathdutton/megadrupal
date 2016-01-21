<?php

/**
 * @file
 * Contains the angles_reaction subclass.
 */

/**
 * Expose angles app names as context reactions.
 */
class angles_application_reaction extends context_reaction {
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
      'app_name' => array(
        '#title' => t('App Name'),
        '#description' => t('The name of the AngularJS application to run'),
        '#type' => 'textfield',
        '#default_value' => isset($values['app_name']) ? $values['app_name'] : '',
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
        return $reaction['app_name'];
      }
    }
  }
}
