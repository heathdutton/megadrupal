<?php

/**
 * User's Line speed as a Context condition.
 */
class quova_context_linespeed_condition extends context_condition {

  function condition_values() {
    return array(
      'low' => "Low",
      'medium' => "Medium",
      'high' => "High",
    );
  }

  function execute() {
    if ($this->condition_used()) {
      $user_linespeed = quova_context_user_linespeed();
      foreach ($this->get_contexts() as $context) {
        $linespeeds = $this->fetch_from_context($context, 'values');
        if (in_array($user_linespeed, $linespeeds)) {
          $this->condition_met($context);
        }
      }
    }
  }
}
