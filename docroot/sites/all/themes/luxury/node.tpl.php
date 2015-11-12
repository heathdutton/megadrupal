<?php
?>
<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?>"<?php print $attributes; ?>>

  <?php print $user_picture; ?>
  <?php if (!$page): ?>
    <h2 <?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
  <?php endif; ?>
<div class="submitted"><?php print $submitted ?></div>
  <div class="content"<?php print $content_attributes; ?>>
    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
      print render($content);
    ?>
  </div>

	<?php if (!empty($content['links'])): ?>
		<div class="links"><?php print render($content['links']); ?></div>
	<?php endif; ?> 

</div>

<?php print render($content['comments']); ?>

