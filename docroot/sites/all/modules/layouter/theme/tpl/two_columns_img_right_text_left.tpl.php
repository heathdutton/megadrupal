<div style="overflow: hidden;">
  <div style="width: 49%; float: left;">
    <p><?php print $text; ?></p>
  </div>
  <div style="width: 49%; float: right;">
    <div><?php print $image; ?></div>
    <?php if ($caption): ?>
    <p style="font-size: 0.8em;"><?php print $caption; ?></p>
    <?php endif; ?>
  </div>
</div>
