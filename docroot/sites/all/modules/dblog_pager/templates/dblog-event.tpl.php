<?php

/**
 * @file
 * DBLog Pager theme implementation for DBLog event.
 *
 * Available variables:
 * - $table : the event log, themed as a table.
 * - $prev : link process through l() to previous event.
 * - $next : link process through l() to next event.
 * - $first : link process through l() to first event.
 * - $last : link process through l() to last event.
 * - all link variables respect filtering on log main page.
 *
 * @see template_preprocess()
 * @see template_process()
 * @see dblog_pager_dblog_event()
 */
?>
<div class='dblog_links'>
    <?php print render($elements['first']);?>
    <?php print render($elements['prev']);?>
    <?php print render($elements['next']);?>
    <?php print render($elements['last']);?>
</div>
<?php print render($elements['table']);?>
