<?php

/**
 * @file
 * Context reaction that forces user's to authenticate via OneLogin.
 */

// Class existence check for soft dependency on Context.
if (class_exists('context_reaction')) {

  class context_reaction_require_onelogin_auth extends context_reaction {

    function options_form($context) {
      return array('require_onelogin_auth' => array('#type' => 'value', '#value' => TRUE));
    }

    function options_form_submit($values) {
      return array('require_onelogin_auth' => 1);
    }

    /**
     * Check if any contexts are set for this reaction. If so, attempt auth.
     */
    function execute() {
      if ($contexts = $this->get_contexts()) {
        onelogin_require_authentication();
      }

    }
  }

}
