<?php
/**
 * @file
 * Default simple template to render provided menu tree.
 *
 * - $variables['menu_name']: The machine name of the menu.
 * - $variables['menu_tree']: The menu tree to be rendered.
 *   drupal_render() can be used to simply create the output.
 */

?>

<div class="inline-menu <?php print $variables['menu_name']; ?>">
  <?php print drupal_render($variables['menu_tree']); ?>
</div>
