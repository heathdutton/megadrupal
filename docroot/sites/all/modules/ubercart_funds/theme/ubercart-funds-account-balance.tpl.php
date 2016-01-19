<?php
/**
 * @file
 * definition of ubercart-funds-account-balance.tpl.php.
 */
$user = user_load($variables['uid']);

$balance = db_query("SELECT * FROM {ubercart_funds_user_funds} WHERE uid='$user->uid'")->fetchAssoc();

if ($balance) {
  ?>

  <h3>Balance: </h3><span><?php print uc_currency_format($balance['balance']/100); ?></span>

<?php }
else { ?>

  <h3>Balance: </h3><span><?php print uc_currency_format(0); ?></span>

<?php } ?>