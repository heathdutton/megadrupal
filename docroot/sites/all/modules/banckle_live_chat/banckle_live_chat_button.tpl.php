<?php

/**
 * @file
 * UI template for Banckle Live Chat button
 *
 * Available variables:
 * - $banckle_live_chat_deployment: The ID of deployment selected during
 *     activate process.
 *
 * @see banckle_live_chat_theme()
 *
 * @ingroup themeable
 */
?>

<div id="banckle-live-chat-button">
  <a href="#" onclick="blc_startChat(); return false;">
    <img id="blc_chatImg" src="https://apps.banckle.com/em/onlineImg.do?d=<?php echo $banckle_live_chat_deployment; ?>" alt="Banckle Chat" />
  </a>
</div>
