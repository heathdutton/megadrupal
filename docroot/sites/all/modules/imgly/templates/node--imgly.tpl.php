<?php
/**
 * @file
 * Imgly's theme implementation to display an image node.
 */
?>
<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>

  <?php if ($page): ?>

    <div class="imgly-image">
      <?php print render($content['field_imgly_image']); ?>
    </div>

    <div class="imgly-title"><?php print $title; ?></div>

    <div class="imgly-footer clearfix">

      <div class="imgly-title-description">

        <div class="imgly-description">
          <?php print render($content['field_imgly_description']); ?>
        </div>

        <?php // Hide image and description, we've rendered these ourselves. ?>
        <?php hide($content['field_imgly_image']); ?>
        <?php hide($content['field_imgly_description']); ?>

        <?php // Render any other fields added by the user. ?>
        <?php print render($content); ?>

      </div> <!-- /.imgly-title-description -->

      <div class="imgly-meta">

        <?php if ($display_submitted): ?>
          <div class="meta submitted">
            <?php print $submitted; ?>
          </div>
        <?php endif; ?>

        <?php if (module_exists('imgly_stats')): ?>
          <?php // Create placeholder element for stats to be added to via AJAX.  ?>
          <div class="meta imgly-stats"></div>
        <?php endif; ?>

        <?php if (isset($imgly_powered_by)): ?>
          <div class="meta poweredby">
            <?php print $imgly_powered_by; ?>
          </div>
        <?php endif; ?>

      </div> <!-- /.imgly-meta -->

    </div> <!-- /.imgly-footer -->

  <?php else: ?>
    <?php print render($title_prefix); ?>
    <h2<?php print $title_attributes; ?>>
      <a href="<?php print $node_url; ?>"><?php print $title; ?></a>
    </h2>
    <?php print render($title_suffix); ?>

    <?php if ($display_submitted): ?>
      <div class="meta submitted">
        <?php print $submitted; ?>
      </div>
    <?php endif; ?>

    <div class="content clearfix"<?php print $content_attributes; ?>>
      <?php
      // We hide the comments and links in teaser mode.
      hide($content['comments']);
      hide($content['links']);
      print render($content);
      ?>
    </div>

  <?php endif; ?>

</div>