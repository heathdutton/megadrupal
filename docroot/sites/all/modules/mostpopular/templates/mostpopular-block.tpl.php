<?php
// $Id$
/**
* @file
* HTML for the most popular block.
*
* Available variables:
* - $classes: A list of classes generated from $classes_array.
* - $attributes: A list of tag attributes generated from $attributes_array.
* - $services: Tabs for the available services, with the current one selected.
* - $intervals: Tabs for the available intervals, with the current one selected.
*/
?>
<div <?php print $attributes; ?> class="<?php print $classes; ?>">
  <?php print render($title_suffix); ?>
  
  <?php print render($services); ?>
  <?php print render($intervals); ?>
  
  <div class="mostpopular--content"></div>
</div>