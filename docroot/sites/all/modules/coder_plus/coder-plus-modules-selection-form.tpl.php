<?php

/**
 * @file
 * Customize the display of a complete search form and results.
 *
 * Available variables:
 * - $form: The complete form array.
 */
?>
<?php
print drupal_render($form['modules']);
print drupal_render($form['submit']);
print drupal_render($form['search_results']);

// Always print out the entire $form. This renders the remaining pieces of the
// form that haven't yet been rendered above.
print drupal_render_children($form);
unset($_SESSION['data']);
