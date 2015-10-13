<?php

/**
 * @file
 * Bartik's theme implementation to display a single Drupal page.
 */
?>
<div id="entermedia-page-wrapper"><div id="entermedia-page">
  <?php if ($messages): ?>
    <div id="messages"><div class="section clearfix">
      <?php print $messages; ?>
    </div></div> <!-- /.section, /#messages -->
  <?php endif; ?>
  <div id="main-wrapper" class="clearfix"><div id="main" class="clearfix">
    <div id="content" class="column"><div class="section">
      <a id="main-content"></a>
      <?php print render($title_prefix); ?>
      <?php if ($title): ?>
        <h1 class="title" id="page-title">
          <?php print $title; ?>
        </h1>
      <?php endif; ?>
      <?php print render($page['content']); ?>

    </div></div> <!-- /.section, /#content -->
  </div></div> <!-- /#main, /#main-wrapper -->
</div></div> <!-- /#page, /#page-wrapper -->
