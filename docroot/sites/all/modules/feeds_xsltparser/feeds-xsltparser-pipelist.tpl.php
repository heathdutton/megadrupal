<?php
/**
 * @file
 * Display XSLT pipelines along with the stylessheets.
 */
?>
<?php if (is_array($pipes)): ?>
  <?php  foreach ($pipes as $name => $files): ?>
    <dl class="xsltparser-pipelist">
      <dt><?php print $name; ?></dt>
      <dd><?php print $files; ?></dd>
    </dl>
  <?php endforeach; ?>
<?php endif; ?>
