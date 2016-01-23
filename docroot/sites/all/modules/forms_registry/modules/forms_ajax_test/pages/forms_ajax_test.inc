<?php
/**
 * @file
 * Ajax form test page.
 */

/**
 * Displays the AJAX handled form in page.
 */
function forms_ajax_test_page() {
  return forms_get_form(FormsAjaxTestConfig::AJAX_SELECT_EXAMPLE);
}