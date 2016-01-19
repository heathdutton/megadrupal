<div id="header">
  <div class="wrapper">
    <?php print $breadcrumb; ?>
    <?php print render($title_prefix); ?>
      <?php if ($title): ?>
        <h1><?php print $title; ?></h1>
      <?php endif; ?>
    <?php print render($title_suffix); ?>
  </div><!-- /.wrapper -->
</div><!-- /#header -->

<?php if ($tabs): ?>
  <div id="tabs"><?php print render($tabs); ?></div>
<?php endif; ?>

<div id="content" class="clearfix">

  <?php if ($action_links): ?>
    <div class="wrapper">
      <div id="main_actions">
        <ul class="action-links">
          <?php print render($action_links); ?>
        </ul>
      </div>
    </div>
  <?php endif; ?>

  <div class="wrapper">
    <div id="system">
      <?php if ($messages): ?>
        <div id="messages">
          <?php print render($messages); ?>
        </div>
      <?php endif; ?>
      <?php print render($page['system']); ?>
    </div>
    <div id="main_content">
      <?php print render($page['content']); ?>
    </div><!-- /#main_content -->
    <div id="sidebar">
      <?php print render($page['sidebar_first']); ?>
    </div><!-- /#sidebar -->
  </div>

</div><!-- /#content -->

<div id="footer">
  <?php if ($page['footer']): ?>
    <div class="wrapper">
      <?php print render($page['footer']); ?>
    </div>
  <?php endif; ?>
</div><!-- /#footer -->