<?php
/**
 * @file
 * Administrative pages for the contextual_elements module.
 */
/**
 * View the details of a contextual element instance.
 */
function contextual_elements_details_page($contextual_element) {
  return theme('contextual_elements_details', array('contextual_element' => $contextual_element));
}
