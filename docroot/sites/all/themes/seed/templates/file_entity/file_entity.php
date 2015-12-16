<?php

/**
 * @file
 * HTML5 version of this tenplate, original is located in file_entity module.
 *
 * @see template_preprocess()
 * @see template_preprocess_file()
 * @see template_process()
 */
?>
<article id="file-<?php print $file->fid ?>" class="<?php print $classes ?>"<?php print $attributes; ?>>

  <?php if (!$page || $display_submitted): ?>
    <header>
      <?php if (!$page): ?>
        <?php print render($title_prefix); ?>
        <h2<?php print $title_attributes; ?>><a href="<?php print $file_url; ?>"><?php print $label; ?></a></h2>
        <?php print render($title_suffix); ?>
      <?php endif; ?>

      <?php if ($display_submitted): ?>
        <div class="submitted">
          <?php print $submitted; ?>
        </div>
      <?php endif; ?>
    </header>
  <?php endif; ?>

  <div class="content"<?php print $content_attributes; ?>>
    <?php
      // We hide the links now so that we can render them later.
      hide($content['links']);
      print render($content);
    ?>
  </div>

  <?php if ($links = render($content['links'])): ?>
    <footer>
      <?php print $links; ?>
    </footer>
  <?php endif; ?>

</article>
