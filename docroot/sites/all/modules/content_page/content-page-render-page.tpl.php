<div id="page" class="clearfix">
  <?php if ($messages): ?>
  <div id="messages-wrapper">
    <?php print $messages; ?>
  </div>
  <?php endif; ?>
  <?php print render($page['content-page']); ?>
</div>