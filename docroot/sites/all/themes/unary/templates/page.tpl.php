<div id="main-wrapper" class="container-fluid">
  <?php if ($messages): ?>
    <div id="messages" class="clearfix"><?php print $messages; ?></div>
  <?php endif; ?>
  <div id="branding" class="clearfix">
    <?php print $breadcrumb; ?>
    <div class="title-wrapper clearfix">
      <?php print render($title_prefix); ?>
      <?php if ($title): ?>
        <h1 class="page-title pull-left"><?php print $title; ?></h1>
      <?php endif; ?>
      <?php print render($title_suffix); ?>
      <?php if ($action_links): ?>
        <ul class="nav nav-pills action-links pull-right">
          <?php print render($action_links); ?>
        </ul>
      <?php endif; ?>
    </div>
    <?php if ($tabs): ?>
      <div class="tabs">
        <?php print render($tabs); ?>
      </div>
    <?php endif; ?>
  </div>

  <div id="page">
    <div id="content" class="clearfix">
      <div class="element-invisible"><a id="main-content"></a></div>
      <?php if ($page['help']): ?>
        <div id="help">
          <?php print render($page['help']); ?>
        </div>
      <?php endif; ?>
      <?php print render($page['content']); ?>
    </div>

    <div id="footer">
      <?php print $feed_icons; ?>
    </div>

  </div>
</div>
