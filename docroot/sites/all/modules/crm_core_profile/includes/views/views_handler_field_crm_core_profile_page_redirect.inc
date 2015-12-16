<?php

/**
 * @file
 * Handler for crm_core_profile path and redirect fields.
 */

/**
 * Field handler to present the path to the profile page.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_crm_core_profile_page_redirect extends views_handler_field_crm_core_profile_page_path {

  function construct() {
    parent::construct();
    $this->additional_fields['page_redirect'] = 'page_redirect';
  }

  function render($values) {
    $path = $this->get_value($values, 'page_redirect');
    if ($this->options['link']) {
      return l($path, $path, array('absolute' => $this->options['absolute']));
    }
    else {
      return $path;
    }
  }
}
