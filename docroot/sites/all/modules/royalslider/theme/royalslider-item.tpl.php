<?php
/**
 * @file
 * RoyalSlider item template.
 *
 * Each item represents a slide.
 *
 * Available variables:
 *
 * - $tag string The HTML5 element to use. One of "a" or "img".
 * - $url string The absolute URL of the main stage image.
 * - $path string The URL that the image links to.
 * - $attributes array An array of attributes to apply to the element.
 * - $row array Render array containing the view result row.
 */
?>
<?php if (isset($row)):?>
  <?php print $row; ?>
<?php else: ?>
  <?php if ($tag === 'a'): ?>
    <a href="<?php print $url; ?>"<?php print drupal_attributes($attributes_array);?>></a>
  <?php else: ?>
  <?php if(isset($path)): ?>
    <a href="<?php print $path; ?>">
  <?php endif; ?>
    <img src="<?php print $url; ?>"<?php print drupal_attributes($attributes_array);?> />
  <?php if(isset($path)): ?>
    </a>
  <?php endif; ?>
  <?php endif; ?>

  <?php if (!empty($caption)): ?>
    <span class="rsCaption"><?php print $caption; ?></span>
  <?php endif; ?>
<?php endif; ?>