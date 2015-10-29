
<div class="comment<?php print ($comment->new) ? ' comment-new' : ''; print ' '. $status; print ' '. $zebra; ?>">

  <div class="clearfix">


  <?php if ($comment->new) : ?>
    <span class="new"><?php print drupal_ucfirst($new) ?></span>
  <?php endif; ?>
  
<div class="comment_info">
  <?php print $picture ?>
  <?php if ($submitted): ?>
    <span class="submitted"><?php print $submitted; ?></span>
  <?php endif; ?>
 </div>
 
 <div class="comment_right">
    
  <?php print render($title_prefix); ?>
  <h3<?php print $title_attributes; ?> class="title"><?php print $title ?></h3>
  <?php print render($title_suffix); ?>
  
    <div class="content">
    <?php
      hide($content['links']);
      print render($content);
    ?>
      <?php if ($signature): ?>
      <div class="clearfix">
        <div>—</div>
        <?php print $signature ?>
      </div>
      <?php endif; ?>
    </div>
      <?php if ($content['links']): ?>
    <div class="links"><?php print render($content['links']) ?></div>
  <?php endif; ?>
</div>
  </div>


</div>
