<div class="<?php print $classes . ' ' . $zebra; ?> clearfix"<?php print $attributes; ?>>

  <?php if ($picture): ?>
    <div class = "comment-picture">
      <?php print $picture ?>
    </div>
  <?php endif; ?>

  <div class = "comment-right <?php if ($picture) print 'with-picture'; ?>">
    <div class="content"<?php print $content_attributes; ?>>

      <div class = "comment-permalink">
        <?php print $permalink; ?>
      </div>

      <div class = "comment-author">
        <?php print ($author); ?>
      </div>      

      <?php
        hide($content['links']);
        print render($content);
      ?>

    </div> <!-- /.content -->
  </div> <!-- /.comment-right -->

  <div class = "comment-bottom clearfix">

    <?php if ($created): ?>
      <div class = "comment-created">
        <?php print $created; ?>
      </div>
    <?php endif; ?>

    <?php if ($content['links']): ?>
      <div class = "comment-links">

	      <?php print render($content['links']) ?>
      </div>
    <?php endif; ?>

  </div> <!-- /.comment-bottom -->

</div> <!-- /.comment -->