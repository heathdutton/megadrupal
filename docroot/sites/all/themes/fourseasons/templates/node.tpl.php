<div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?><?php if ($page == 0) { print ' teaser'; } ?>">

<?php if($user_picture): ?>
	<div class="user-picture"><?php print $user_picture; ?></div>
<?php endif; ?>

<?php if (!$page): ?>
  <h2><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
<?php endif; ?>

  <div class="meta">
  <?php if ($display_submitted): ?>
    <div class="submitted"><?php print $submitted ?></div>
  <?php endif; ?>

  <?php if (isset($terms)): ?>
    <div class="terms terms-inline"><?php print $terms ?></div>
  <?php endif;?>
  </div>

  <div class="content">
    <?php print render($content); ?>
  </div>

  <?php if (isset($links)): ?>
	<?php print $links; ?>
  <?php endif; ?>	
</div>