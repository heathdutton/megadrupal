<?php
/**
 * @file
 * Super light page template file.
 */
?>
    <div class="container">
      <div class="header">
        <a class="logo" href="http://getharmony.io" target="_blank">Harmony</a>
      </div>

      <div class="body">
        <?php if (!empty($page['highlighted'])): ?>
        <div class="highlighted jumbotron"><?php print render($page['highlighted']); ?></div>
        <?php endif; ?>
        <?php if (!empty($breadcrumb)): print $breadcrumb; endif;?>
        <a id="main-content"></a>
        <?php print render($title_prefix); ?>
        <?php if (!empty($title)): ?>
          <h1 class="page-header"><?php print $title; ?></h1>
        <?php endif; ?>
        <?php print render($title_suffix); ?>
        <?php print $messages; ?>
        <?php if (!empty($tabs)): ?>
          <?php print render($tabs); ?>
        <?php endif; ?>
        <?php if (!empty($page['help'])): ?>
          <?php print render($page['help']); ?>
        <?php endif; ?>
        <?php if (!empty($action_links)): ?>
          <ul class="action-links"><?php print render($action_links); ?></ul>
        <?php endif; ?>
        <?php print render($page['content']); ?>
      </div>

      <footer class="footer text-center text-muted">
        <p><small>Need some help? Come over to <a href="http://getharmony.io/installing" target="_blank">http://getharmony.io/installing</a>.</small></p>
      </footer>
    </div>
