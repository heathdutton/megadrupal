<?php
?>
<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?>"<?php print $attributes; ?>>

  <?php if (!$page): ?>
    <h2 <?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
  <?php endif; ?>
	
  <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
      /*print render($content);*/
  ?>
	
	<?php if ($teaser) : ?>
		<div class="content"<?php print $content_attributes; ?>>
			<?php print render($content) ?>
		</div>
		<?php print '<div class="submitted">by <a href="' .url('user/'.$node->uid). '">' .$node->name. '</a> on ' .date('d M y',$node->created). '</div>';?>
			
	<?php else: ?>
	
		<?php print '<div class="submitted">by <a href="' .url('user/'.$node->uid). '">' .$node->name. '</a> on ' .date('d M y',$node->created). '</div>';?>
		<div class="content"<?php print $content_attributes; ?>>
			<?php print render($content) ?>
		</div>
		
	<?php endif; ?>
	
	
	<?php if (!empty($content['links'])): ?>
      <div class="links"><?php print render($content['links']); ?></div>
	<?php endif; ?>
	<?php print render($content['comments']); ?>

</div>
