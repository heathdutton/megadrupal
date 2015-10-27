<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?>"<?php print $attributes; ?>>

	<?php print render($title_prefix); ?>
	<?php if (!$page): ?>
		<h2<?php print $title_attributes; ?>><?php if(isset($title_additional)) print $title_additional; ?><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
	<?php endif; ?>
	<?php print render($title_suffix); ?>

	<div class="content clearfix"<?php print $content_attributes; ?>>	

    <?php if ($node_top): ?>
      <div class = "node-top">
        <?php print render($node_top); ?>
      </div>
    <?php endif; ?>
    
    <?php print $user_picture; ?>

		<?php
			hide($content['comments']);
			hide($content['links']);
			print render($content);
		?>

    <?php if ($node_bottom): ?>
      <div class = "node-bottom">
        <?php print render($node_bottom); ?>
      </div>
    <?php endif; ?>

	</div>

  <div class = "bottom clearfix">

    <?php if (isset($bottom_links) && $bottom_links): ?>
      <div class = "bottom-links">
        <?php print $bottom_links; ?>
      </div>
    <?php endif; ?>

    <?php if (isset($bottom_right_links) && $bottom_right_links): ?>
      <div class = "bottom-right-links">
        <?php print $bottom_right_links; ?>
      </div>
    <?php endif; ?>

  </div>

</div>

<?php if (isset($social_buttons)): ?>
  <div class = "social-links clearfix">
    <div class = "social-text"><?php print $social_text; ?></div>
    <?php print $social_buttons; ?>
  </div>
<?php endif; ?>

<?php if (isset($previous_node) || isset($next_node)): ?>
  <div class = "node-navigation clearfix">

    <div class = "node-prev">
      <?php if ($previous_node): ?>
        <div class = "node-navigation-bg">
          <?php print $previous_node; ?>
        </div>
      <?php endif; ?>
    </div>

    <div class = "nav"><?php print t('Navigation'); ?></div>

    <div class = "node-next">
      <?php if ($next_node): ?>
        <div class = "node-navigation-bg">
          <?php print $next_node; ?>
        </div>
      <?php endif; ?>    
    </div>

  </div>
<?php endif; ?>

<?php print render($content['comments']); ?>