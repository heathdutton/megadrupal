<?php
/**
 * Social feed block.
 */
?>
<div id="<?php print $content_id; ?>-wrapper"  class="campaign-social-container">
  <div id="<?php print $content_id; ?>-header" class="campaign-social-header"><?php if (isset($header)) print $header; ?></div>
  <div id="<?php print $content_id; ?>" class="campaign-social-content"><?php if (isset($content)) print $content; ?></div>
  <div id="<?php print $content_id; ?>-footer" class="campaign-social-footer"><?php if (isset($footer)) print $footer; ?></div>
</div>