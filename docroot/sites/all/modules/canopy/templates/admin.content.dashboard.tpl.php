<?php
/**
 * @file
 * TPL file to render the osCaddie Alfresco Dashboard view.
 */
?>
<?php if (!empty($sections)): ?>
<div class="dashboard">
  <ul>
    <?php foreach ($sections as $section): ?>
    <li>
      <?php if ($section->enabled): ?>
        <?php print l(check_plain($section->title), $section->path, array('attributes' => array('class' => array('icon', $section->name), 'title' => check_plain($section->title)))); ?>
      <?php else: ?>
        <span class="<?php print 'icon ' . $section->name . ' disabled'; ?>"><?php print check_plain($section->title); ?></span>
      <?php endif; ?>
    </li>
    <?php endforeach; ?>
  </ul>
</div>
<?php endif; ?>
