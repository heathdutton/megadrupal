<div class="comment<?php print ($comment->new) ? ' comment-new' : ''; print ' '. $status ?> <?php print $zebra ?> clearfix">

  <div class="comment-body">

  <?php if ($new): ?>
    <div class="new"><?php print $new; ?></div>
  <?php endif; ?>

  <div class="submitted"><div class="submitted-inner">
    <?php print $picture; ?> Posted by <?php print $author; ?> on <?php print $created; ?>  <?php print render($content['links']) ?>
  </div></div>

  <div class="content"<?php print $content_attributes; ?>>
		<?php
    // We hide the comments and links now so that we can render them later.
    hide($content['links']);
    print render($content);
    ?>
	  <?php print $permalink; ?>
      <?php if ($signature): ?>
        <div class="user-signature">
          <?php print $signature ?>
        </div>
      <?php endif; ?>
  </div>


  
  </div>

</div> 

