<article<?php print $attributes; ?>>

  <?php print render($title_prefix); ?>
  <?php if (!$page && $title): ?>
    <header>
      <h2<?php print $title_attributes; ?>><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
    </header>
  <?php endif; ?>
  <?php print render($title_suffix); ?>
  <?php if ($user_picture || $display_submitted): ?>
    <div class="incubator-user-submitted">
      <?php print $user_picture; ?>
      <?php if ($display_submitted): ?>
        <footer class="submitted">
          <span class="submitted-day"><?php print $submitted_day; ?></span>
          <span class="submitted-month"><?php print $submitted_month; ?></span>
          <span class="submitted-year"><?php print $submitted_year; ?></span>
        </footer>
      <?php endif; ?>
    </div>
  <?php endif; ?>


  <div<?php print $content_attributes; ?>>
    <?php if ($teaser) {
        if (!empty($content['field_main_image'])) {
          print render($content['field_main_image']);
        }
        if (!empty($content['field_media'])) {
          print render($content['field_media']);
        }
      }?>
    <?php
    // We hide the comments and links now so that we can render them later.
    hide($content['comments']);
    hide($content['links']);
    print render($content);
    ?>
  </div>

  <div class="clearfix">
    <?php if (!empty($content['links'])): ?>
      <nav class="links node-links clearfix"><?php print render($content['links']); ?></nav>
    <?php endif; ?>

    <?php print render($content['comments']); ?>
  </div>
</article>