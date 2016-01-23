<?php if (isset($country)): ?>
  <p><?php echo t('Using language (%language) based on detected country (%country).', array('%language' => $language, '%country' => $country)); ?></p>
<?php else: ?>
  <p><?php echo t('Unable to obtain country data for this IP.'); ?></p>
<?php endif; ?>
