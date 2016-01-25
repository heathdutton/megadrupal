<div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?> post">
<?php print $user_picture ?>
<?php if ($submitted): ?>
    <div class="post-date">
	<?php print format_date($node->created, 'custom', "M y");?>
	<div class="post-day"><?php print format_date($node->created, 'custom', "d");?></div>
	</div>
  <?php endif; ?>
<?php if ($page == 0): ?>
<div class="post-title">
  <h2 class="entry-title"><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
  <?php if ($submitted): ?>
    <span class="post-author meta-link-post"><?php print $name; ?></span>
  <?php endif; ?>
  </div>
  <?php endif; ?>
<div class="meta-link-post post-comments"><?php
  print $node->comment_count . ' comment';
  if ($node->comment_count != 1) { print 's'; }
?>
</div> 
  <div class="content inline-block">
    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
	  hide($content['field_tags']);
      print render($content);
    ?>
  </div>
  <div class="clear-block">
    <div class="meta meta-link-post meta-entry">
    <?php if (!empty($content['field_tags'])): ?>
      <div class="post-tags"><?php print render($content['field_tags']); ?></div>
    <?php endif;?>
	<?php print render($content['comments']); ?>
    <div class="links controls"><?php print render($content['links']); ?></div>
  </div>
	</div>
  <div class="divider2"></div>
</div>