<?php
/**
 * @file
 * Node node.tpl override from Bootstrap parent theme
 */
?>
<article id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>

<div class="article-content">
  <header>
    <?php print render($title_prefix); ?>
    <?php if (!$page && $title): ?>
      <h2<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
    <?php endif; ?>
    <?php print render($title_suffix); ?>

    <?php if ($display_submitted): ?>
      <span class="submitted">
        <?php print $user_picture; ?>
        <?php print $submitted; ?>
      </span>
    <?php endif; ?>
  </header>

  <?php
    // Hide comments, tags, and links now so that we can render them later.
    hide($content['comments']);
    hide($content['links']);
    hide($content['field_tags']);
    hide($content['field_image']);
  ?>
  <?php if (!empty($content['field_image'])): ?>
  <div class="pull-left">
    <?php print render($content['field_image']); ?>
 </div>
   <?php endif; ?>
    <?php print render($content); ?>

  <?php if (!empty($content['field_tags']) || !empty($content['links'])): ?>
    <footer>
      <?php print render($content['field_tags']); ?>
      <?php print render($content['links']); ?>
    </footer>
  <?php endif; ?>
</div> 
  <?php print render($content['comments']); ?>

</article> <!-- /.node -->
