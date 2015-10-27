<?php print t('To avoid duplicates, please search before submitting a new issue.'); ?>

<?php if ($view_issues): ?>
  <?php print drupal_render($form); ?>
  <div class="issue-cockpit-categories">
    <?php foreach ($categories as $key => $category): ?>
      <div class="issue-cockpit-<?php print $key; ?>">
        <h3><?php print $categories[$key]['name']; ?></h3>
        <div class="issue-cockpit-totals">
          <?php print l(t('!open open', array('!open' => $categories[$key]['open'])), $path, array('query' => array('categories' => $key))); ?>,
          <?php print l(t('!total total', array('!total' => $categories[$key]['total'])), $path, array('query' => array('status' => 'All', 'categories' => $key))); ?>
        </div>
      </div>
    <?php endforeach; ?>
  </div>

  <div class="issue-cockpit-subscribe">
    <?php print $issue_subscribe; ?>
  </div>

  <?php if (isset($issue_metrics) && !empty($issue_metrics)): ?>
    <div class="issue-cockpit-metrics">
      <h3><?php print t('Statistics'); ?></h3>
      <?php print $issue_metrics; ?>
      <div><small>2 year graph, updates weekly</small></div>
    </div>
  <?php endif; ?>
<?php endif; ?>
