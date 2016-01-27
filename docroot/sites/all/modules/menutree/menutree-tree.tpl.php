<?php
/**
 * @file
 * Theme implementation to display a menutree tree.
 *
 * Available variables:
 * - $menu_name: The name of the tree's menu.
 * - $title: the (sanitized) title of the tree.
 * - $description: The descriptive intro text for this tree.
 * - $tree: The pre-rendered menu tree itself.
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the
 *   following:
 *   - menutree-tree: The current template type, i.e., "theming hook".
 *   - menutree-[name]: The name of the tree's menu.
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 *
 * Other variables:
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *   teaser listings.
 *
 *
 * @see template_preprocess()
 * @see template_preprocess_menutree_tree()
 * @see template_process()
 */
?>
<div class="<?php print $classes; ?>"<?php print $attributes; ?>>

  <?php print render($title_prefix); ?>
  <?php if ($title): ?>
    <h2<?php print $title_attributes; ?>><?php print $title; ?></a></h2>
  <?php endif; ?>
  <?php print render($title_suffix); ?>

  <?php if ($description): ?>
    <div class="menutree-description">
      <?php print $description; ?>
    </div>
  <?php endif; ?>

  <div class="tree"<?php print $content_attributes; ?>>
    <?php print render($tree); ?>
  </div>

</div>
