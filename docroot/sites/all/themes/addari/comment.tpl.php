
<div class="comment<?php print ($comment->new) ? ' comment-new' : ''; print ' '. $status; print ' '. $zebra; ?>">

  <div class="clear-block">
  <?php if ($submitted): ?>
    <span class="info"><?php print $submitted; ?></span>
  <?php endif; ?>

  <?php if ($comment->new) : ?>
    <span class="new"><?php print drupal_ucfirst($new) ?></span>
  <?php endif; ?>

  <?php print $picture ?>


  <?php print render($title_prefix); ?>
		<h3><?php print $title ?></h3>
  <?php print render($title_suffix); ?>
  
    <div class="content">
    <?php
      hide($content['links']);
      print render($content);
    ?> 
	  <?php if ($signature): ?>
      <div class="clear-block">
        <div>—</div>
        <?php print $signature ?>
      </div>
      <?php endif; ?>
    </div>
  </div>

      <?php if ($content['links']): ?>
    <div class="links clearfix"><?php print render($content['links']) ?></div>
  <?php endif; ?>
</div>
