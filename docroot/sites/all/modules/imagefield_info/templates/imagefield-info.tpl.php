<?php
/**
 * @file
 * This is a template file for the list (table) of image URLs.
 *
 * Variables:
 * - $styles (array): An array containing label and the URL for each item.
 */
?>

<table class="path-info">
  <caption><?php print t('Image path you can use to insert image URL in wysiwyg editor'); ?></caption>
  <?php foreach ($styles as $style) : ?>
    <tr>
      <td><strong><?php print $style['label']; ?></strong></td>
      <td><?php print $style['url']; ?></td>
    </tr>
  <?php endforeach; ?>
</table>
