<div id="orghunter-charity-search-result-charity-ein-<?php print $ein; ?>" class="orghunter-charity-search-result-charity">
  <h2 class="orghunter-charity-search-result-charity-name"><?php print $title; ?></h2>
  <addr class="orghunter-charity-search-result-charity-address">
    <strong class="label"><?php print t('Address'); ?>:</strong>
    <span class="value">
      <strong class="label"><?php print t('City'); ?>:</strong>
      <span class="value"><?php print $city; ?></span><span class="sep">,</span>
      <strong class="label"><?php print t('State'); ?>:</strong>
      <span class="value"><?php print $state; ?></span>
      <strong class="label"><?php print t('Zip Code'); ?>:</strong>
      <span class="value"><?php print $zip; ?></span>
    </span>
  </addr>
  <div class="orghunter-charity-search-result-charity-category">
    <strong class="label"><?php print t('Category'); ?>:</strong>
    <span class="value"><?php print $category; ?></span>
  </div>
  <div class="orghunter-charity-search-result-charity-status orghunter-charity-search-result-charity-status-<?php print ($status); ?>">
    <?php if (isset($status_message)): ?>
      <?php print $status_message; ?>
    <?php endif; ?>
  </div>
  <div class="orghunter-charity-search-result-charity-deductible orghunter-charity-search-result-charity-deductibility-<?php print ($deductibility); ?>">
    <strong class="label"><?php print t('Deductibility'); ?>:</strong>
    <span class="value"><?php print $deductible; ?></span>
  </div>
  <div class="orghunter-charity-search-result-charity-donation orghunter-charity-search-result-charity-donation-<?php print ($eligible); ?>">
    <?php if (isset($donation_link)): ?>
      <?php print $donation_link; ?>
    <?php endif; ?>
  </div>
</div>

