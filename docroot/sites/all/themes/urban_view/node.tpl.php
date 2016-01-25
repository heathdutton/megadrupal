<?php
// $Id: node.tpl.php $
?>
<div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?> post">
<div class="post-c">
<div class="post-cnt">
  <?php print $user_picture; ?>
  <?php if ($display_submitted): ?>
    <small class="date">
	  <?php print format_date($node->created, 'custom', "M");?>
	  <div class="post-day"><?php print format_date($node->created, 'custom', "d");?></div>
	</small>
  <?php endif; ?>
  <?php print render($title_prefix); ?>
  <?php if (!$page): ?>
    <h2<?php print $title_attributes; ?>><span><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></span></h2>
  <?php endif; ?>
  <?php print render($title_suffix); ?>
  <div class="content"<?php print $content_attributes; ?>>
    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
	  hide($content['field_tags']);
      print render($content);
    ?>
  </div>
</div>
</div>
  <div class="clear-block">
  <div class="post-b">
    <div class="meta post-meta post-cnt">
    <?php if (!empty($content['field_tags'])): ?>
      <div class="terms"><?php print render($content['field_tags']); ?></div>
    <?php endif;?>
    </div>
    <?php if (!empty($content['links'])): ?>
      <div class="links controls"><?php print render($content['links']); ?></div>
    <?php endif; ?>
    <div class="num-comments">
      <?php
      print $node->comment_count . ' comment';
      if ($node->comment_count != 1) { print 's'; } ?>
    </div>
  </div>
  </div>
  <div class="divider2"></div>

</div>
