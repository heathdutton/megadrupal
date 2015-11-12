<?php

$plugin = array(
  'title' => t('My example filter'),
  'settings callback' => '', // Defaults to my_module_example_filter_settings
  'process callback' => '', // Defaults to my_module_example_filter_process
  'prepare callback' => '', // Defaults to my_module_example_filter_prepare
  'tips callback' => '', // Defaults to my_module_example_filter_tips
  // ... plus any other values you want to pass to `hook_filter_info()`
);

/**
 * Process callback `My example filter`
 * @see `hook_filter_info()` for documentation on custom filters.
 */
function my_module_example_filter_process($text, $filter) {
  return $text; // Just an example.
}

// .. the other callbacks for the filter in the same way as the process callback
// but with arguments matching the type of callback.