<?php
?>
<div class="comment<?php print($comment->new) ? ' comment-new' : ''; print ' '. $status; print ' '. $zebra; ?>">


  <div class="clear-block">

  <?php if ($comment->new) : ?>
    <span class="new"><?php print drupal_ucfirst($new) ?></span>
  <?php endif; ?>

  <?php print $picture ?>

    <h3><?php print $title ?></h3>
      <?php if ($submitted): ?>
        <span class="submitted"><?php print $submitted; ?></span>
      <?php endif; ?>
    <div class="content">
      <?php print render($content); ?>
      <?php if ($signature): ?>
      <div class="clear-block">
        <div>—</div>
        <?php print $signature; ?>
      </div>
      <?php endif; ?>
    </div>
  </div>

  
</div>
