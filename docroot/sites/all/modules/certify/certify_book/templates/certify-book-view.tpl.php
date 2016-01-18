<?php print l($condition->mynode->title, 'node/' . $condition->mynode->nid) ?>
<?php if (user_access('view own certificate progress') && $condition->completed): ?>
  <span class="certify_passed">(<?php print t('read') ?>)</span>
<?php elseif (user_access('view own certificate progress') && !$condition->completed): ?>
  <?php if ($condition->showpages): ?>
    <span class="certify_failed"><?php print t('Unread pages:') ?></span>
    <ul>
      <?php foreach ($condition->pages as $page): ?>
        <?php if ($page['bid'] != $page['nid'] && !$page['read']): ?>
          <li><?php print l($page['title'], 'node/' . $page['nid']) ?></li>
        <?php endif ?>
      <?php endforeach ?>
    </ul>
  <?php else: ?>
    <span class="certify_failed">(<?php print t('too many unread pages to show') ?>)</span>
  <?php endif ?>
<?php endif ?>