<?php
  /*
Allie Micka
3145 Dean Ct Apt 401,
Minneapolis, MN 55416-4390
 ************4697
Exp. 07-2011
  */
?>
<div class="authorizenet-payment-profile-summary">
<?php if($first_name || $last_name):?>
<p><?php print $first_name ?> <?php print $last_name?></p>
<?php endif?>

<?php if($card_mask):?>
  <p><?php print ($card_mask)?></p>
<?php endif?>
<?php if($expiration):?>
  <p>Exp <?php print date('m-Y', $expiration)?></p>
<?php endif?>
</div>
