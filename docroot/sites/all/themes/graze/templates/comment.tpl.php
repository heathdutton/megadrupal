<?php // comment template ?>
<?php hide($content['links']); ?>
<div class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
    <p>
      <?php if ($new): ?><span class="new"><?php print $new; ?></span><?php endif; ?>
      <?php print render($title_prefix); ?>
      <h3<?php print $title_attributes; ?>><?php print $title; ?> <span><?php print $permalink; ?></span></h3>
      <?php print render($title_suffix); ?>
      <p class="meta"><?php print $submitted; ?></p>
      <?php print $picture ?>
    </p>
    <div class="content"<?php print $content_attributes; ?>>
      <?php print render($content); ?>
    </div>
		<?php
		  $links = render($content['links']);
		  if ($links):
		?>
	    <div class="links">
	    	<?php print $links; ?>
	    </div>
	  <?php endif; ?>
</div>