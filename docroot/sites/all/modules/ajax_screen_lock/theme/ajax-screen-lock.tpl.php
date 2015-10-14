<?php
/**
 * @file
 * Template for popup.
 */
?>
<div class="ajax-screen-lock-popup modal fade">
  <div class="modal-dialog">
    <div class="modal-content">
      <div class="modal-header">
        <?php if (!empty($title)): ?>
          <h4 class="modal-title"><?php print $title; ?></h4>
        <?php endif; ?>
      </div>
      <div class="modal-body">
        <div class="ajax-screen-lock-progress"></div>
      </div>
      <div class="modal-footer"></div>
    </div>
  </div>
</div>
