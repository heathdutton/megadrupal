<div 	class="comment<?php print ($comment->new) ? ' comment-new' : ''; print ' '. $status; print ' '. $zebra; ?>">
<img class="reply_img" src="<?php print base_path() . path_to_theme() ?>/images/reply.png" alt="Replied" title="Reply to above post" />

<div class="clearfix">
<div class="comment-paper">
<div class="comment-inner">
<div class="comment-inner2">
<?php if ($comment->new) : ?> <span
	class="new"><?php print drupal_ucfirst($new) ?></span> <?php endif; ?>

<?php print $picture ?>

  <?php print render($title_prefix); ?>
<h3><?php print $title ?></h3>
  <?php print render($title_suffix); ?>
  
<?php if ($submitted): ?> <span class="submitted"><?php print $submitted; ?></span>
<?php endif; ?>
<div class="content">       
    <?php
      hide($content['links']);
      print render($content);
    ?> 
<?php if ($signature): ?>
    
<div class="clearfix">
<div>—-</div>
<?php print $signature ?>
</div>
<?php endif; ?>
</div>


      <?php if ($content['links']): ?>
    <div class="links clearfix"><?php print render($content['links']) ?></div>
  <?php endif; ?>

</div>
</div>
</div>
</div>
</div>