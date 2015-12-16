<?php
/**
 * @file
 * Default simple view template to display a list of rows.
 *
 * Available variables:
 * - $title: The title of this group of rows.  May be empty.
 * - $options['type'] will either be ul or ol.
 *
 * @ingroup views_templates
 */
?>
<div id="wrapper">
  <section class="block">
    <div id="circleslider3">
      <div class="viewport">
        <ul class="overview">
        <?php foreach ($items as $id => $row):
          ?>
          <li><?php print $row; ?></li>
        <?php endforeach; ?>
      </ul>
    </div>
    <div class="dot"></div>
    <div class="overlay"></div>
    <div class="thumb"></div>
  </div>
</section>
</div>
