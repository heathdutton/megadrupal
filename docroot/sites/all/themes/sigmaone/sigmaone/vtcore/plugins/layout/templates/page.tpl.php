<?php
/**
 * page.tpl.php
 *
 * This page.tpl.php will just simply render the
 * $page['regions'] array that has been built by
 * layout.plugin file.
 *
 * You can still manipulate the content array
 * by editing the $page['regions'] but the children
 * content of it has already been rendered as
 * an html entities. To edit the children content
 * array before it is rendered by drupal_render()
 * function you will need to edit section.tpl.php
 * instead.
 *
 * It is still possible to do the normal Drupal
 * way of rendering page.tpl.php, You will need
 * to examine the $page and print the html tags
 * in this page along with the $page varialbes
 * that you wish to print using render() function
 *
 */
?>
<div <?php print drupal_attributes($page['#attributes']);?>>
  <?php print drupal_render_children($page); ?>
</div>