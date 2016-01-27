<?php

/**
 * @file
 * Theme implementation for comments.
 */
?>
<div class="<?php print $classes . ' ' . $zebra; ?>"<?php print $attributes; ?>>

  <div class="clearfix">
    
  <?php if ($picture): ?>
    <div class="user-picture">
      <?php print $picture; ?>
    </div>
  <?php endif; ?>

  <?php if ($new): ?>
    <span class="new"><?php print $new; ?></span>
  <?php endif; ?>

    <?php print render($title_prefix); ?>
    <h3<?php print $title_attributes; ?>><?php print $title; ?></h3>
    <?php print render($title_suffix); ?>
    
    <span class="submitted"><?php print $submitted; ?></span>

    <div class="content"<?php print $content_attributes; ?>>
      <?php 

        print render($content); ?>
      <?php if ($signature): ?>
      <div class="clearfix">
        <hr />
        <?php print $signature; ?>
      </div>
      <?php endif; ?>
    </div>
  </div>
</div>