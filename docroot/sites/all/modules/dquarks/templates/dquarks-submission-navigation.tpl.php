<?php
/**
 * @file
 * Customize the navigation shown when editing or viewing submissions.
 *
 * Available variables:
 * - $node: The node object for this dquarks.
 * - $mode: Either "form" or "display". May be other modes provided by other
 *          modules, such as "print" or "pdf".
 * - $submission: The contents of the dquarks submission.
 * - $previous: The previous submission ID.
 * - $next: The next submission ID.
 * - $previous_url: The URL of the previous submission.
 * - $next_url: The URL of the next submission.
 */
?>
<div class="dquarks-submission-navigation">
  <?php if ($previous): ?>
    <?php print l(t('Previous submission'), $previous_url, array('attributes' => array('class' => array('dquarks-submission-previous')), 'query' => ($mode == 'form' ? array('destination' => $previous_url) : NULL))); ?>
  <?php else: ?>
    <span class="dquarks-submission-previous"><?php print t('Previous submission'); ?></span>
  <?php endif; ?>

  <?php if ($next): ?>
    <?php print l(t('Next submission'), $next_url, array('attributes' => array('class' => array('dquarks-submission-next')), 'query' => ($mode == 'form' ? array('destination' => $next_url) : NULL))); ?>
  <?php else: ?>
    <span class="dquarks-submission-next"><?php print t('Next submission'); ?></span>
  <?php endif; ?>
</div>
