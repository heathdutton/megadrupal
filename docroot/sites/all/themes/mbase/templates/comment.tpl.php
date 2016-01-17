<?php
/**
 * @file
 * Adds Bootstrap markup and removes some default markup cruft.
 */
?>
<article class="<?php print $classes . ' ' . $zebra; ?> panel panel-default"<?php print $attributes; ?>>
  
  <header class = "panel-heading">
    <div class = "row">

    <div class = "col-xs-12 col-sm-9 col-md-10">
    
    <?php print render($title_prefix); ?>
    <h3<?php print $title_attributes; ?>><?php print $title ?></h3>
    <?php print render($title_suffix); ?>
    <?php if ($new): ?>
      <span class="new"><?php print $new ?></span>
    <?php endif; ?>
    <span class="submitted"><?php print $author; ?> - <?php print $created; ?></span>

    </div>
    
    <?php if ($picture): ?>
    <div class = "col-xs-12 col-sm-3 col-md-2 text-right"><?php print $picture ?></div>
    <?php endif; ?>
    </div>
    
  </header>

  <div class="content panel-body"<?php print $content_attributes; ?>>
    <?php hide($content['links']); print render($content); ?>
    <?php if ($signature): ?>
    <blockquote class="user-signature clearfix">
      <?php print $signature ?>
    </blockquote>
    <?php endif; ?>    
  </div>
  <?php if (!empty($content['links']['comment'])): ?>
    <footer class = "panel-footer">
      <?php print render($content['links']) ?>
    </footer>
  <?php endif; ?>

</article> <!-- /.comment -->
