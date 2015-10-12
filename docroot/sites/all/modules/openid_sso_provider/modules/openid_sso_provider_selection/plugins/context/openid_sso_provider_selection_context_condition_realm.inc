<?php

/**
 * Expose OpenID Single Sign-On relying party realms as a context condition.
 */
class openid_sso_provider_selection_context_condition_realm extends context_condition {


  /**
   * Defines values for context's configuration (form).
   */
  function condition_values() {
    $values = array();
    foreach (openid_sso_provider_get_relying_parties() as $relying_party) {
      $values[$relying_party->realm] = check_plain($relying_party->label . ' - ' . $relying_party->realm);
    }
    return $values;
  }


  /**
   * Executes the context's logic
   */
  function execute() {

    // Obtain realm
    $realm = openid_sso_provider_get_realm();

    // Check if realm was given back.
    if ($realm) {

      // Check condition
      $this->check_condition($realm);
    }
  }


  /**
   * Check a given realm for the occurency in the configured context
   *
   * @param string $realm
   *   The string of the OpenID relying party realm url.
   * @return
   *   Returns a valid url
   */
  protected function check_condition($realm) {

    // Check if realm is defined in any context's definition
    foreach ($this->get_contexts() as $context) {
      $realm_conditions = $this->fetch_from_context($context, 'values');
      foreach ($realm_conditions as $realm_condition) {
        if ($realm_condition == $realm) {

          // Set condition of context as met.
          $this->condition_met($context, $realm);

          // Give a positive feedback
          return TRUE;
        }
      }
    }
    // Return false in case nothing was positive
    return FALSE;
  }
}
