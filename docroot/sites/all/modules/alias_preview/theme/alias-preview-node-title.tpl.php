<?php
/**
 *
 */
?>
<label>
  <?php echo t('URL alias') ?>:
</label>
<span class="alias-preview-base-url">
  <?php echo $base_url ?>/
</span>
<span class="alias-preview-alias">
  <?php echo $alias ?>
</span>
<?php if (isset($show_jump_link) && $show_jump_link): ?>
  <a class="alias-preview-jump-pathauto" href="#path[pathauto]">
    <?php echo t('edit') ?>
  </a>
<?php endif; ?>