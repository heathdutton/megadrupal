<?php 
// node template 

hide($content['comments']);
hide($content['links']);
?>
<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>

  <?php print render($title_prefix); ?>
  <?php if (!$page): ?>
    <h2<?php print $title_attributes; ?>>
      <a href="<?php print $node_url; ?>"><?php print $title; ?></a>
    </h2>
  <?php endif; ?>
  <?php print render($title_suffix); ?>

  <?php if ($display_submitted): ?>
    <div class="meta submitted">
      <?php print $submitted; ?>
    </div>
  <?php endif; ?>

  <div class="content clearfix"<?php print $content_attributes; ?>>
<?php
      print render($content);
    ?>
  </div>

	<?php
		// Remove the "Add new comment" link on the teaser page or if the comment
		// form is being displayed on the same page (taken from Bartik).
		if ($teaser || !empty($content['comments']['comment_form'])) {
    	unset($content['links']['comment']['#links']['comment-add']);
  	}

	  $links = render($content['links']);
	  if ($links):
	?>
    <div class="links">
    	<?php print $links; ?>
    </div>
  <?php endif; ?>
	
	<?php if ($content['comments'] && $page): ?>
  	<?php print render($content['comments']); ?>
	<?php endif; ?>

</div>