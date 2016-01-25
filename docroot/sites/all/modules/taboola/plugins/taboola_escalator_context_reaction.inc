<?php
/**
 * @file
 * Context reaction plugin for Taboola escalator.
 */

/**
 * Provides Taboola Escalator context reaction.
 */
class taboola_escalator_context_reaction extends context_reaction {
  /**
   * Form options.
   */
  public function options_form($context) {
    $form = array();
    $values = array();

    // Get existing values for this form.
    $settings = $this->fetch_from_context($context);
    if (!empty($settings['values'])) {
      $values = $settings['values'];
    }

    $form['values'] = array(
      '#type' => 'fieldset',
    );
    $form['values']['mode'] = array(
      '#type' => 'textfield',
      '#title' => t('Mode'),
      '#description' => t('Enter a mode.'),
      '#default_value' => isset($values['mode']) ? $values['mode'] : 'alternating-thumbnails-a-escalator',
    );
    $form['values']['slider'] = array(
      '#type' => 'checkbox',
      '#title' => t('Slider'),
      '#description' => t('Slider.'),
      '#default_value' => isset($values['slider']) ? $values['slider'] : FALSE,
    );
    $form['values']['target_type'] = array(
      '#type' => 'textfield',
      '#title' => t('Target Type'),
      '#description' => t('Enter a target type.'),
      '#default_value' => isset($values['target_type']) ? $values['target_type'] : 'mix',
    );
    $form['values']['placement'] = array(
      '#type' => 'textfield',
      '#title' => t('Placement'),
      '#description' => t('Enter a placement.'),
      '#default_value' => isset($values['placement']) ? $values['placement'] : 'Mobile Escalator Thumbnails',
    );

    return $form;
  }

  /**
   * Execute context.
   */
  public function execute() {
    foreach ($this->get_contexts() as $context) {
      if (isset($context->reactions['taboola_escalator']['values'])) {
        if (!path_is_admin(current_path()) && taboola_get_setting('taboola_enabled') && taboola_get_setting('taboola_service_path')) {
          // Set the global taboola variable.
          $js[] = "window._taboola = window._taboola || [];";

          // Clean and add the JS params.
          $js_params = array();
          foreach ($context->reactions['taboola_escalator']['values'] as $key => $value) {
            $js_params[] = sprintf("%s: %s", check_plain($key), $this->cleanJsValue($key, $value));
          }
          $js[] = "_taboola.push({" . implode(', ', $js_params) . "});";

          // Add inline header JS.
          drupal_add_js(implode(PHP_EOL, $js), array(
            'scope' => 'footer',
            'type' => 'inline',
            'weight' => 1,
          ));
        }
      }
    }
  }

  /**
   * Clean the specified variable.
   *
   * @param string $key
   *   Variable key.
   * @param string $value
   *   Variable value.
   */
  protected function cleanJsValue($key, $value) {
    if ($key == 'slider') {
      return (bool) $value ? 'true' : 'false';
    }
    else {
      return "'" . check_plain($value) . "'";
    }
  }

}
