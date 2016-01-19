<?php

	/**
	* @file
	* Business Yellow Theme
	* Created by Zyxware Technologies
	*/

?>
<div class="comment-text-top"></div>
  <div class="comment-text">
    <div class="comment-arrow">
    <div class="profile_picture">
    <?php if ($picture): ?>
				<?php print $picture; ?>	
			<?php else: ?>
				<?php print $default_photo; ?>	
			<?php endif; ?>	
			</div>	
      <div class="submitted">
      <span class="commenter-name">
        <?php print $author; ?>
      </span>
      </div>
    <?php if ($new): ?>
      <span class="new"><?php print $new; ?></span>
    <?php endif; ?>

    <?php print render($title_prefix); ?>
    <h2<?php print $title_attributes; ?>><?php print $title; ?></h2>
    <?php print render($title_suffix); ?>
    
    <div class="content"<?php print $content_attributes; ?>>
      
      <?php
        // We hide the comments and links now so that we can render them later.
        hide($content['links']);
        print render($content);
      ?>
      
      <?php if ($signature): ?>
      <div class="user-signature clearfix">
        <?php print $signature; ?>
      </div>
      <?php endif; ?>
    </div> <!-- /.content -->
       <div class="submitted_date">
      <span class="comment-time">
        <?php print ago($comment_created); ?>
      </span></div>

<div class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>


    <?php print render($content['links']); ?>
  </div> <!-- /.comment-text -->


</div>
</div>
<div class="comment-text-bottom"></div>

