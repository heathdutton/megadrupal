<div class="comment<?php print ($new) ? ' comment-new' : ''; print ' '. $status; print ' '. $zebra; ?>">
  <div class="date">
    <div class="textdate">
      <div class="day"><?php print format_date($comment->created, 'custom', 'j'); ?></div>
      <div class="month"><?php print format_date($comment->created, 'custom', 'M'); ?></div>
  	</div>
  </div>
  
  <?php print render($title_prefix); ?>
    <h3<?php print $title_attributes; ?>><?php print $title ?></h3>
  <?php print render($title_suffix); ?>
    
  <span class="submitted"><?php print t('Submitted by !username on !datetime', array('!username' => $author, '!datetime' => $created)); ?></span>
	<div class="clear-block">
    <?php if ($new) : ?>
      <span class="new"><?php print $new ?></span>
    <?php endif; ?>

  	<?php if($picture) { ?>
  		  <?php print $picture ?>
  	<?php } ?>
	
		<div id="commentcontent">
      <div class="content">
        <?php
        hide($content['links']);
        print render($content);
        ?>
        <?php if($signature): ?>
          <div class="clear-block">
            <div>—</div>
            <?php print $signature ?>
          </div>
        <?php endif; ?>
      </div>
    </div>
  	
  	<div class="permalink-comment">
      <?php print l('#'. $comment->cid, 'node/'. $comment->nid, array('fragment' => 'comment-'. $comment->cid)); ?>
    </div>
    
    <?php if($content['links']): ?>
      <div class="links"><?php print render($content['links']) ?></div>
    <?php endif; ?>
	</div>
</div>
