<?php
    $settings = variable_get('spotlight_settings', array());
?>
<div class="spotlight-wrapper" style="width:<?php print $settings['width']; ?>">
    <div class="spotlight-input-wrapper">
        <i class="spotlight-spin animate-spin"></i>
        <label for="spotlight-input"><?php print t('Spotlight') ?></label>
        <input type="text" name="spotlight-input" class="spotlight-input" autocomplete="off" />
        <i class="spotlight-search"></i>
    </div>

    <div style="clear:both;"></div>

    <div class="spotlight-result-wrapper"></div>
</div>