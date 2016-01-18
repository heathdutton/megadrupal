<?php print l($condition->mynode->title, 'node/' . $condition->mynode->nid) ?>
<?php if (user_access('view own certificate progress') && !$condition->completed): ?>
  <span class="certify_failed">(<?php print t('not passed') ?>)</span>
<?php elseif (user_access('view own certificate progress') && $condition->completed): ?>
  <span class="certify_passed">(<?php print t('passed') ?>)</span>
<?php endif ?>