<?php
drupal_add_css(drupal_get_path('module', 'lingwo_pron') . '/lingwo_pron.css');
?>
<div class="lingwo-pron-list">
<?php foreach ($pron_list as $pron): ?>
  <div class="lingwo-pron-item">
  <?php if (!empty($pron['tag'])): ?>
    <span class="lingwo-pron-tag">(<?php print check_plain($pron['tag']); ?>)</span>
  <?php endif; ?>
  <?php if (!empty($pron['ipa'])): ?>
    <span class="lingwo-pron-ipa">/<?php print check_plain($pron['ipa']); ?>/</span>
  <?php endif; ?>
  <?php if (!empty($pron['audio'])): ?>
    <?php print $pron['audio_widget']; ?>
  <?php endif; ?>
  </div>
<?php endforeach; ?>
</div>
