<?php
/**
 * @file node.tpl.php

 *
 * Theme implementation to display a node.
 *
 * Available variables:
 * - $title: the (sanitized) title of the node.
 * - $content: Node body or teaser depending on $teaser flag.
 * - $user_picture: The authors picture of the node output from
 *   theme_user_picture().
 * - $date: Formatted creation date (use $created to reformat with
 *   format_date()).
 * - $links: Themed links like "Read more", "Add new comment", etc. output
 *   from theme_links().
 * - $name: Themed username of node author output from theme_user().
 * - $node_url: Direct url of the current node.
 * - $terms: the themed list of taxonomy term links output from theme_links().
 * - $submitted: themed submission information output from
 *   theme_node_submitted().
 */
?>
<?php phptemplate_comment_wrapper(NULL, $node->type); ?>

<div class="node<?php if ($sticky) { print " sticky"; } ?><?php if (!$status) { print " node-unpublished"; } ?>" id="node-<?php print $node->nid; ?>">
  <?php print render($title_prefix); ?>
  <?php if (!$page): ?> 
    <h2 class="title"> <a href="<?php print $node_url; ?>"><?php print $title; ?></a> </h2>
  <?php endif; ?>
  <?php print render($title_suffix); ?>

  <?php if ($user_picture): ?>
    <?php print $user_picture; ?>
  <?php endif; ?>

	<?php if ($submitted): ?>
    <div class="meta submitted">
			<div class="post-date">
				<span class="post-year"><?php print (format_date($node->created, 'custom', 'd')); ?>
                                <?php print (format_date($node->created, 'custom', 'M')); ?>
				<?php print (format_date($node->created, 'custom', 'Y')); ?></span>
			</div>
			<span class="author"><?php print $name; ?></span>  
    </div>

  <?php endif; ?>

  <div class="content">
    <?php hide($content['links']); ?>
    <?php print render($content); ?>
  </div>

  <?php $links = render($content['links']); ?>
  <?php if ($links): ?>
    <div class="links"><?php print $links; ?></div>
  <?php endif; ?>

</div>

