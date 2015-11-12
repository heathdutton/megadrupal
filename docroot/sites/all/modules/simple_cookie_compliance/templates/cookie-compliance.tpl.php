<div id="cookie-compliance" class="cookie-compliance clearfix">
  <div class="cookie-compliance__inner">
    <div class="cookie-compliance__text">
    <?php if ($message): ?>
      <p><?php print $message; ?></p>
    <?php endif ?>
    </div>
    <?php if ($form): ?>
      <?php print render($form); ?>
    <?php endif ?>
  </div>
</div>
