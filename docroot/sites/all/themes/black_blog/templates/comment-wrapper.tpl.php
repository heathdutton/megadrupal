
<div id="comments" class="commenttitle <?php print $classes; ?>"<?php print $attributes; ?>>

  <?php if ($content['comments'] && $node->type != 'forum'): ?>
    <?php print render($title_prefix); ?>
    
    <?php print render($title_suffix); ?>
  <?php endif; ?>
  
  <?php if ($content['comment_form']): ?>
    <h2 class="commenttitlemain"><?php print t('Post your comments'); ?></h2>
    <?php print render($content['comment_form']); ?>
  <?php endif; ?>
  <div class="displaycomment">
   <h2 class="commenttitlemain"><?php print t('Recent comments'); ?></h2>
  <?php print render($content['comments']); ?>
 </div>
</div>
