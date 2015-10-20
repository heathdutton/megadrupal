<?php

/**
 * @file
 * Customize the display of a complete dquarks.
 *
 * This file may be renamed "dquarks-form-[nid].tpl.php" to target a specific
 * dquarks on your site. Or you can leave it "dquarks-form.tpl.php" to affect
 * all dquarkss on your site.
 *
 * Available variables:
 * - $form: The complete form array.
 * - $nid: The node ID of the dquarks.
 *
 * The $form array contains two main pieces:
 * - $form['submitted']: The main content of the user-created form.
 * - $form['details']: Internal information stored by dquarks.
 */
?>
<?php

// If editing or viewing submissions, display the navigation at the top.
if (isset($form['submission_info']) || isset($form['navigation'])) :  ?> 
  <?php print drupal_render_children($form['navigation']);
endif;
// Print out the main part of the form.
// Feel free to break this up and move the pieces within the array.
print drupal_render($form['submitted']);
// Always print out the entire $form. This renders the remaining pieces of the
// form that haven't yet been rendered above.
print drupal_render_children($form);

// Print out the navigation again at the bottom.
if (isset($form['submission_info']) || isset($form['navigation'])): ?>
  <?php unset($form['navigation']['#printed']);
  print drupal_render($form['navigation']);
endif;
