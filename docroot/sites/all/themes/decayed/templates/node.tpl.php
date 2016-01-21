
<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <?php print render($title_prefix); ?>
  <?php if (!$page): ?>
    <?php if (!$page): ?>
      <h2<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
    <?php endif; ?>
  <?php endif; ?>
  <?php print render($title_suffix); ?>
  <?php if ($user_picture || $display_submitted):?>
    <div class="post-info">

        <?php if ($user_picture): ?>
          <?php print $user_picture; ?>
        <?php endif; ?>

      <?php if ($display_submitted): ?>
        <span class="submitted"><?php print $submitted ?></span>
      <?php endif; ?>
    </div>
  <?php endif; ?>

  <div class="content" <?php print $content_attributes; ?>>
    <?php hide($content['comments']); ?>
    <?php hide($content['links']); ?>
    <?php print render($content); ?>
  </div>
  <div class="terms-links">
    <?php if (!empty($content['field_tags']) && !$is_front): ?>
      <div class="tags">
        <?php print render($content['field_tags']) ?>
     </div>
    <?php endif; ?>
    <?php print render($content['links']); ?>
  </div>
  <?php print render($content['comments']); ?>

</div>