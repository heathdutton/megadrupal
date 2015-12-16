<div id="node-<?php print $node->nid ?>" class="node node-<?php print $node->type ?>">

<?php if (!$page): ?>
  <h2 class="teaser-title"><a href="<?php print $node_url ?>" title="<?php print $title ?>">
    <?php print $title ?></a></h2>
<?php endif; ?>

	<?php if ($submitted): ?>
      	<div class="post-date"><span class="post-month"><?php print (format_date($node->created, 'custom', 'F')) ?> <?php print (format_date($node->created, 'custom', 'd')) ?>, 
		<?php print (format_date($node->created, 'custom', 'Y')) ?></span> -- Posted by: <a href="/user/<?php print($node->name) ?>"><?php print($node->name) ?></a> <?php if ($terms): ?>in <?php print $terms ?><?php endif;?></div>
	<?php endif; ?>

  <div class="content clear-block">
    <?php print $user_picture; ?>
    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
      print render($content);
    ?>
  </div>

<?php if (render($content['links'])):?>
  <div class="node-links"><?php print render($content['links']); ?></div>
<?php endif; ?>
  <span class="comments"><?php print $comment_count ?></span>
  <span class="date">
    <?php print (format_date($node->created, 'custom', 'F')) ?> <?php print (format_date($node->created, 'custom', 'd')) ?>, 
	<?php print (format_date($node->created, 'custom', 'Y')) ?>
  </span>

<?php print render($content['comments']); ?>

</div>

