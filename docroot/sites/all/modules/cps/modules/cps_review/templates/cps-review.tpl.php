<?php

/**
 * @file
 * Default theme implementation to display CPSReview entities.
 *
 * Variables:
 * - $css: An array of CSS files for the current page.
 *
 * @see template_preprocess()
 * @see template_preprocess_entity()
 * @see template_process()
 */

hide($content['submitted']);
$rendered_content = render($content);
?>
<?php if (!empty($rendered_content)): ?>
  <div class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
    <?php if ($title): ?>
      <h3><?php print $title; ?></h3>
    <?php endif; ?>
    <div class="submitted"><?php print render($content['submitted']); ?></div>
    <?php
      print $rendered_content;
    ?>
  </div>
<?php endif; ?>
