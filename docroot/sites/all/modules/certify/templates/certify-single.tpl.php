<div class="certify_certificate">
  <?php if ($showtitle): ?>
    <p class="certify_title"><strong><?php print check_plain($certificate->title) ?></strong></p>
  <?php endif ?>
  <div class="certify_bar-border">
    <div class="certify_bar-fill">
      <?php if ($certificate->certificate->getStatus()->completed): ?>
        <div class="certify_bar-bar complete" style="width:100%">
          <span class="progress">100%</span>
          <span class="completed">
            <a href="<?php global $user; print url('user/' . $user->uid . '/certify/download/' . $certificate->nid) ?>">
              <img src="<?php print base_path() . drupal_get_path('module', 'certify') ?>/images/pdf-icon.gif" alt=""/>
              <?php print t('Download certificate') ?>
            </a>
          </span>
        </div>
      <?php else: ?>
        <div class="certify_bar-bar" style="width:<?php print $certificate->certificate->getStatus()->progress ?>%">
          <span class="progress"><?php print $certificate->certificate->getStatus()->progress ?>%</span>
        </div>
      <?php endif ?>
    </div>
  </div>
  <?php if (!$condensed): ?>
    <?php $certificate->body = field_view_field('node', $certificate, 'body', array('label' => 'hidden')); print render($certificate->body) ?>
  <?php endif ?>
  <?php if (user_access('view certificate')): ?>
    <div class="certify_details"><?php print l(t('Show details'), 'node/' . $certificate->nid) ?></div>
    <br style="clear:both"/>
  <?php endif ?>
</div>