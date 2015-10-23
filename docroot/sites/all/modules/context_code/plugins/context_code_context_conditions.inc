<?php
/**
 * @file
 * Context condition plugin to provide a trigger for content callbacks.
 */

/**
 * Expose the content callback as the context conditions.
 */
class context_code_context_condition extends context_condition {

  function condition_values() {
    return context_code_list();
  }

  /**
   * Condition form.
   */
  function options_form($context) {
    $defaults = $this->fetch_from_context($context, 'options');
    return array(
      'negate' => array(
        '#type' => 'checkbox',
        '#title' => t('Negate'),
        '#default_value' => isset($defaults['negate']) ? $defaults['negate'] : 0,
      ),
      'require_all' => array(
        '#type' => 'checkbox',
        '#title' => t('Require all'),
        '#default_value' => isset($defaults['require_all']) ? $defaults['require_all'] : 0,
      ),
    );
  }

  /**
   * Execute.
   */
  function execute() {
    if ($this->condition_used()) {
      foreach ($this->get_contexts() as $context) {
        $values = $this->fetch_from_context($context, 'values');
        $options = $this->fetch_from_context($context, 'options');

        $data = context_code_get();
        $met = $options['negate'] ? TRUE : FALSE;
        $i = 0;
        $count = count($values);
        foreach ($values as $value) {
          // Check if the
          if (in_array($value, $data)) {
            $i++;
          }

          if (($options['require_all'] && $i = $count) || $i > 0) {
            $met = $options['negate'] ? FALSE : TRUE;
          }
        }

        if ($met) {
          $this->condition_met($context);
        }
      }
    }
  }
}
