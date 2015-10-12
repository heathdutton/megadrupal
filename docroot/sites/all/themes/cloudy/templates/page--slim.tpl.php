<div id="page" class="page-slim meta-toggled">
  <?php if ($meta = render($page['meta'])): ?>
    <div id="meta" class="toggle meta">
      <?php print $meta; ?>
    </div>
  <?php endif; ?>

  <div class="main">
    <?php print $messages; ?>

    <?php if (isset($frame_url)): ?>
      <iframe class="page-slim-iframe" src="<?php print $frame_url; ?>" height="100%" width="100%" frameborder="no"></iframe>
    <?php else: ?>

      <div id="main-content" class="main-content">
        <?php print render($page['preface']); ?>

        <?php if ($features = render($page['featured'])): ?>
          <div id="featured">
            <?php print $features; ?>
          </div>
        <?php endif; ?>

        <a id="main-content-anchor"></a>
        <?php print render($title_prefix); ?>
        <?php if ($title): ?>
          <h1 class="title" id="page-title"><?php print $title; ?></h1>
        <?php endif; ?>
        <?php print render($title_suffix); ?>
        <?php print render($tabs); ?>
        <?php if ($action_links): ?>
          <ul class="action-links"><?php print render($action_links); ?></ul>
        <?php endif; ?>

        <?php print render($page['content']); ?>

        <?php print render($page['postscript']); ?>
      </div>
    <?php endif; ?>
  </div>
</div>
