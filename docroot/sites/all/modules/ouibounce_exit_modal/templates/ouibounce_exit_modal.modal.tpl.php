<!-- Ouibounce Modal -->
<div id="ouibounce-exit-modal">
  <div class="underlay"></div>
  <div class="modal">
    <?php if (isset($title) && !empty($title)): ?>
      <div class="modal-title">
        <h3><?php print $title; ?></h3>
      </div>
    <?php endif; ?>

    <div class="modal-body">
      <?php if (isset($body) && !empty($body)): ?>
        <?php print $body; ?>
      <?php endif; ?>
    </div>

    <div class="modal-footer">
      <?php if (isset($footer) && !empty($footer)): ?>
        <p><?php print $footer; ?></p>
      <?php endif; ?>
    </div>
  </div>
</div>
