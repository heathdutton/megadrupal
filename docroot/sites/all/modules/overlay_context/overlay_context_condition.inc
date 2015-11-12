<?php
/**
 * @file
 * Overlay context condition.
 */

/**
 * Detect if the current page is displayed as an overlay parent or child.
 */
class overlay_context_condition extends context_condition {
  /**
   * {@inheritdoc}
   */
  public function condition_values() {
    return array(
      'none' => t('Page is not shown in overlay'),
      'parent' => t('Page is the parent page of an overlay'),
      'child' => t('Page is loaded within the overlay'),
    );
  }

  /**
   * {@inheritdoc}
   */
  public function execute() {
    if ($this->condition_used()) {
      $overlay_mode = overlay_get_mode();

      foreach ($this->get_contexts() as $context) {
        $condition_values = $this->fetch_from_context($context, 'values');

        if (array_key_exists($overlay_mode, $condition_values)) {
          $this->condition_met($context);
        }
      }
    }
  }
}
