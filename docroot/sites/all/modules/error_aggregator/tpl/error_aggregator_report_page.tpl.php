<?php
/**
 * @file
 * Template file for a page with the list of php errors.
 *
 * Variables available:
 * - $variables['error_list']:  Contains the list of errors
 * - $message:  Contains the message that will show up for each of the errors.
 */
?>

<p><?php print (t('On this page you can see all php errors on the portal aggregated'));?></p>
<ul>
<?php  foreach ($variables['error_list'] as $message): ?>
  <li> <?php print (format_string($message->message, unserialize($message->variables))); ?></li>
<?php endforeach; ?>
</ul>
<div  class="actions">
  <a class="button clear" href="/admin/reports/error-aggregator/clear-all" title="<?php print t('Clear messages'); ?>"><?php print t('Clear messages'); ?></a>
</div>
