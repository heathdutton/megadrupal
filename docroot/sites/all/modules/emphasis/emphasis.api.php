<?php

/**
 * @file
 * API documentation for emphasis module.
 */

/**
 * Allow custom field formatters to be emphasis enabled.
 */
function hook_emphasis_field_formatters() {
  return array(
    // Machine name of custom formatter.
    'my_custom_formatter',
  );
}

/**
 * Modify which field formatters may be emphasis enabled.
 */
function hook_emphasis_field_formatters_alter(&$formatters) {
  unset($formatters['text_summary_or_trimmed']);
}
