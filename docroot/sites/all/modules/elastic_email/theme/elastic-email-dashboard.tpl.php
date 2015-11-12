<?php
?>
<div class="ee-dashboard">
  <div class="ee-dashboard-box">
    <div>
      <span class="ee-dashboard-value"><?php print $data['credit']; ?> <?php print $data['currency']; ?></span>
      <?php print t('Current Credit Amount') ?>
    </div>
  </div>
  <div class="ee-dashboard-box">
    <div>
      <span class="ee-dashboard-value"><?php print $data['totalemailssent']; ?></span>
      <?php print t('Emails Sent') ?>
    </div>
  </div>
  <div class="ee-dashboard-box">
    <div>
      <span class="ee-dashboard-value"><?php print ($data['priceperemail'] * 1000); ?> <?php print $data['currency']; ?></span>
      <?php print t('Cost per thousand') ?>
    </div>
  </div>
  <div class="ee-dashboard-box">
    <div>
      <span class="ee-dashboard-value"><?php print $data['reputation']; ?> / 5</span>
      <?php print t('Reputation') ?>
    </div>
  </div>
</div>