<div class="clearfix <?php print $classes . ' ' . $zebra;?>"<?php print $attributes;?>>

	<div class="user">
	<?php if ($picture) : print $picture; endif; ?>
    <?php if ($logged_in) : print render($content['links']); endif; ?>
	</div>
    
	<div class="comment-node">
		<div class="comment-content clearfix">
			<h3><?php print $author?><span class="comment-date"><?php print format_date($comment -> created, 'custom', 'F j, Y H:i')?></span></h3>

            <div class="content"<?php print $content_attributes; ?>>
            
            <?php print render($title_prefix); ?>
            <h3<?php print $title_attributes; ?> class="title"><?php print $title ?></h3>
            <?php print render($title_suffix); ?>
            
              <?php hide($content['links']); print render($content); ?>
              <?php if ($signature): ?>
              <div class="clearfix signature">
                <div>—</div>
                <?php print $signature ?>
              </div>
              <?php endif; ?>
            </div>
            
			<?php if ($new) : ?>
            <span class="new"><?php print drupal_ucfirst($new) ?></span>
            <?php endif; ?>

		</div>
	</div>
    
</div>
