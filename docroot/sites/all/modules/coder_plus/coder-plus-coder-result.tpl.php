<?php

/**
 * @file
 * Customize the display of search results.
 */
?>
<?php
print drupal_render($build['search_form']);

if (isset($build['search_results']) && count($build['search_results']) > 0) {
  foreach ($build['search_results'] as $value) {
    print $value;
  }
}
