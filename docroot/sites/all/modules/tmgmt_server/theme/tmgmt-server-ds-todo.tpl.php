<?php
/**
 * @file
 * Template file to display TS points.
 */
?>
<dl class="ds-check-list">
  <dt class="ds-register <?php print $register_class; ?>"><span class="check">✓</span> <span class="cross">✗</span> <?php print t('Connect to the TMGMT Directory and make sure your service is reachable') ?></dt>
  <dd class="ds-register"><?php print $register_msg; ?></dd>
  <dt class="ds-finish-profile <?php print $finish_profile_class; ?>"><span class="check">✓</span> <span class="cross">✗</span> <?php print t('Fill out your Translation server profile at the TMGMT Directory') ?></dt>
  <dd class="ds-finish-profile"><?php print $finish_profile_msg; ?></dd>
  <dt class="ds-sync <?php print $sync_class; ?>"><span class="check">✓</span> <span class="cross">✗</span> <?php print t('Synchronise your language capabilities') ?></dt>
  <dd class="ds-sync"><?php print $sync_msg; ?></dd>
  <dt class="ds-go-live <?php print $go_live_class; ?>"><span class="check">✓</span> <span class="cross">✗</span> <?php print t('Go live with your Translation server') ?></dt>
  <dd class="ds-go-live"><?php print $go_live_msg; ?></dd>
  <dt class="ds-go-premium <?php print $go_premium_class; ?>"><span class="check">✓</span> <span class="cross">✗</span> <?php print t('Go Premium!') ?></dt>
  <dd class="ds-go-premium"><?php print $go_premium_msg; ?></dd>
</dl>
