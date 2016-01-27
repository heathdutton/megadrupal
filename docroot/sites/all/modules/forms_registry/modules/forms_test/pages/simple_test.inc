<?php
/**
 * @file
 * Provides a simple forms test callback.
 */

/**
 * Renders the simple test page.
 */
function forms_test_simple_test_page() {
  return forms_get_form('forms_test_simple_form');
}