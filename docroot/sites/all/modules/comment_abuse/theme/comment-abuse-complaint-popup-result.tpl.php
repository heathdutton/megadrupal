<?php

/**
 * @file
 * Template for complaint result.
 */
?>

<div class="messages<?php print ($result == 0) ? ' error' : ' status'; ?>">
  <span class="abuse-notice"><?php print $message; ?></span>
</div>
