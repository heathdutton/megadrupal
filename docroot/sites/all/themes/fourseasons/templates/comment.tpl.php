<div class="comment<?php print ($comment->new) ? ' comment-new' : ''; print ($comment->status == COMMENT_NOT_PUBLISHED) ? ' comment-unpublished' : ''; print ' '. $zebra; ?>">
  <div class="clear-block">
<?php print_r(user_load(array('uid'=>$node->uid))); ?>
  <?php if ($comment->new) : ?>
    <a id="new"></a>
    <span class="new"><?php print drupal_ucfirst($new) ?></span>
  <?php endif; ?>
  <?php print $picture; ?>
  <?php print render($title_prefix); ?>
    <h3<?php print $title_attributes; ?>><?php print $title; ?></h3>
  <?php print render($title_suffix); ?>
  <div class="submitted">
    <?php print $created . " — " .$author;?>
  </div>
  
  <div class="comment-text">
    <div class="comment-arrow"></div>
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
      
      <div class="comment-links">
        <?php print render($content['links']); ?>
      </div> <!-- /.comment-links -->
      
    </div> <!-- /.comment-text -->
</div><!-- /.comment -->