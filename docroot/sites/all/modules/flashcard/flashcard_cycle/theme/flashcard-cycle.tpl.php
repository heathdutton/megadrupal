<?php
/**
 * @file
 * Display the Flashcard set.
 */
?>

<div class="flashcard-cycle clearfix">
  <div class="flashcard-cycle-buttons flashcard-cycle-toolbar clearfix">
    <?php if ($restart = render($restart)): ?>
      <?php print $restart; ?>
    <?php endif; ?>
    <?php if ($edit = render($edit)): ?>
      <?php print $edit; ?>
    <?php endif; ?>
  </div>
  <div class="flashcard-cycle-cards">
    <?php foreach ($items as $item): ?>
    <div class="flashcard-cycle-card">
      <?php print render($item); ?>
    </div>
    <?php endforeach; ?>
  </div>
  <?php if ($pager = render($pager)): ?>
  <div class="flashcard-cycle-buttons flashcard-cycle-pager clearfix">
    <?php print $pager; ?>
  </div>
  <?php endif; ?>
  <?php if ($mark = render($mark)): ?>
  <div class="flashcard-cycle-mark clearfix">
    <?php print $mark; ?>
  </div>
  <?php endif; ?>
  <?php if ($mode = render($mode)): ?>
  <div class="flashcard-cycle-buttons flashcard-cycle-mode clearfix">
    <?php print $mode; ?>
  </div>
  <?php endif; ?>
</div>
