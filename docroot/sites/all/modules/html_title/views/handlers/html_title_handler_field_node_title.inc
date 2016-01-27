<?php

/**
 * @file
 * The html_title_handler_field_node_title field handler re-renders title fields to enable allowed markup that would
 * otherwise be transformed or stripped.
 *
 * @ingroup html_title
 */

/**
 * Override the standard node title rendering to enable select HTML tags.
 */
class html_title_handler_field_node_title extends views_handler_field_node {

  /**
  * Renders the field handler.
  */
  function render($values) {
    $output = parent::render($values);

    $elements = variable_get('html_title_allowed_elements', array('em', 'sub', 'sup'));
    if (count($elements)) {
      static $done = FALSE;

      // Ensure this block executes only once
      if (!$done) {

        // Add permitted elements to options so they are not stripped later
        $tags = array();
        foreach ($elements as $element) {
          $tags[] = '<'. $element .'>';
        }
        $this->options['alter']['preserve_tags'] .= ' '. implode(' ', $tags);
        $done = TRUE;
      }

      // Decode permitted HTML elements
      $pattern = "/&lt;(\/?)(". implode('|', $elements) .")&gt;/i";
      $output = preg_replace($pattern, '<$1$2>', $output);

      // Decode HTML character entities
      $pattern = "/&amp;([a-z0-9#]+);/";
      $output = preg_replace($pattern, '&$1;', $output);
    }

    return $output;
  }
}
