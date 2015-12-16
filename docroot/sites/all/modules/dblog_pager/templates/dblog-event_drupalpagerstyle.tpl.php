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
  <ul class="links pager pager-list">
    <li class="pager-item"><?php print render($elements['first']);?></li>
    <li class="pager-item"><?php print render($elements['prev']);?></li>
    <li class="pager-item"><?php print render($elements['next']);?></li>
    <li class="pager-item"><?php print render($elements['last']);?></li>
  </ul>
</div>
<?php print render($elements['table']);?>
