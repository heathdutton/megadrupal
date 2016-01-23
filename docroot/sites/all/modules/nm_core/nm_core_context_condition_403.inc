<?php

/**
 * Expose whether page is a 403 as a context condition.
 */
class nm_core_context_condition_403 extends context_condition {
  function condition_values() {
    return array(
      'is_403' => t('Is a 403'),
    );
  }
 
  function editor_form($context = NULL) {
    $form = parent::editor_form($context);
    $form['is_403']['#title'] = t('Is a 403');
    return $form;
  }

  function execute() {
    if ($this->condition_used()) {
      foreach ($this->get_contexts() as $context) {
        //get which option was selected in the form
        $boolean = $this->fetch_from_context($context, 'values');
        //if they want to set contexts on 403
        if (in_array('is_403', $boolean)) {
          //get header
          $header = drupal_get_http_header();
          //if this is a 403
          if (isset($header['status']) && $header['status'] == '403 Forbidden') {
            $this->condition_met($context);
          }
        }
      }
    }
  }

}