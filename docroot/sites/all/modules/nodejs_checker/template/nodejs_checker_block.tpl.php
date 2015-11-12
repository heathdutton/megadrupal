<?php

/**
 * @file
 * nodejs_checker_block theme implementation 
 *
 * BE CAREFUL some class attribute are used by JAVASCRIPT.
 *
 * @param object $state
 *    Server state.
 *
 * @see nodejs_checker.js
 */
?>
<div id='nodejs_checker_server_state_disconnected' class='nodejs_checker_server_state
  <?php if ($state == TRUE): ?>
    <?php print 'nodejs_checker_hidden'; ?>
  <?php endif; ?>
'>   
  <?php print t('Nodejs Server'); ?>
</div>

<div id='nodejs_checker_server_state_connected' class='nodejs_checker_server_state 
  <?php if ($state == FALSE): ?>
    <?php print 'nodejs_checker_hidden'; ?>
  <?php endif; ?>
'>
  <?php print t('Nodejs Server'); ?>
</div>

<div id='nodejs_checker_client_connected' class='nodejs_checker_client_state nodejs_checker_hidden'> 
  <?php print t("You're connected to Nodejs"); ?>
</div>
<div id='nodejs_checker_client_disconnected' class='nodejs_checker_client_state'>
  <?php print t("You're disconnected from Nodejs"); ?>
</div>

<div id='nodejs_checker_transport' class='nodejs_checker_hidden'>
</div>
