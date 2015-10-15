<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clear"<?php print $attributes; ?>>
  <?php print render($title_prefix); ?>
  <?php if (!$page): ?>
    <h2<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
  <?php endif; ?>
  <?php print render($title_suffix); ?>

  <?php if ($display_submitted): ?>
    <div class="submitted">
      <?php if ($display_submitted): ?>
        
          <?php
            print t('by !username | !datetime',
              array('!username' => $name, '!datetime' => $date));
          ?>
        
      <?php endif; ?>
    </div>
  <?php endif; ?>

  <div class="content"<?php print $content_attributes; ?>>
    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['field_tags']);
      hide($content['links']);
      print render($content);
    ?>
  </div>
  <div class="node_links clear">
    <?php print render($content['field_tags']); ?>
    <?php print render($content['comments']); ?>
    <?php print render($content['links']); ?>
  </div>
</div> <!-- /.node -->