<div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) {print ' node-unpublished'; } ?><?php if($teaser) {print 'node-teaser';} ?>">

<?php if($teaser): ?>
  <h2><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
<?php endif; ?>

  <?php if ($display_submitted): ?>
		<div class="submitted">
			<?php print $submitted ?>
		</div>
	<?php endif; ?>

 <div class="content"<?php print $content_attributes; ?>>
    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
      print render($content);
    ?>
  </div><!--/content -->
  
  <?php if($content['links']) { ?>
		<div class="node-links">
			<?php print render($content['links']); ?>
		</div> <!--/node-links -->
	<?php } ?>

  <?php print render($content['comments']); ?>



</div>