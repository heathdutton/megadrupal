<?php

/**
 * @file
 * Default theme implementation to display a reservation.
 *
 * Available variables:
 * - $content: An array of content items. Use render($content) to print them all, or
 *   print a subset such as render($content['field_example']). Use
 *   hide($content['field_example']) to temporarily suppress the printing of a
 *   given element.
 * - $title: The title of the reservation.
 * - $reservation_url: Direct url of the current node.
 * - $page: TRUE if this reservation is the current page.
 * - $classes_array: An array of CSS classes that can be modified in template
 *   preprocess functions.
 * - $classes: String of CSS classes flattened from $classes_array.
 *
 * @see template_preprocess()
 * @see template_preprocess_reservation()
 * @see template_process()
 */
?>
<div id="reservation-<?php print $reservation->reservation_id; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <?php if (!$page): ?>
    <h2<?php print $title_attributes; ?>>
        <a href="<?php print $reservation_url; ?>"><?php print $title; ?></a>
    </h2>
  <?php endif; ?>

  <div class="content"<?php print $content_attributes; ?>>
    <?php print render($content); ?>
  </div>
</div>
