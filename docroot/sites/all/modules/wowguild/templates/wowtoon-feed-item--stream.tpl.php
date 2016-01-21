<?php

/**
 * @file
 * A basic template for swortoon entities
 *
 * Available variables:
 * - $content: An array of comment items. Use render($content) to print them all, or
 *   print a subset such as render($content['field_example']). Use
 *   hide($content['field_example']) to temporarily suppress the printing of a
 *   given element.
 * - $title: The name of the swortoon
 * - $url: The standard URL for viewing a swortoon entity
 * - $page: TRUE if this is the main view page $url points too.
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. By default the following classes are available, where
 *   the parts enclosed by {} are replaced by the appropriate values:
 *   - entity-profile
 *   - swortoon-{TYPE}
 *
 * Other variables:
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 *
 * @see template_preprocess()
 * @see template_preprocess_entity()
 * @see template_process()
 */

?>


<div class="<?php print $classes; ?> social-circles-node-stream clearfix"<?php print $attributes; ?>>
  <div class="user-picture"><?php print $user_picture; ?></div>

  <div class="social-circles-node-stream-content content"<?php print $content_attributes; ?>>
    <?php
      print $icon;
      print $description . '<br />';
      print $datecompleted_formatted;
      print render($content);
    ?>
  </div>
</div>
