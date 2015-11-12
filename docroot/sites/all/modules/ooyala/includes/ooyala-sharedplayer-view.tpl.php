<?php

/**
 * @file
 * Template to display a set of Ooyala videos in a single player.
 *
 * - $player_id : The unique identifier of the player.
 * - $width : The width of the player.
 * - $height : The height of the player.
 * - $first_embed_code : The first embed code to view.
 * - $embed_codes : An array of embed codes that are included in the player.
 */
?>

<div id="<?php print $player_id; ?>" class="ooyala_shared_player">
  <div id="<?php print $player_id; ?>-wrapper">
    <script src="//player.ooyala.com/player.js?callback=receiveOoyalaEvent&playerId=<?php print $player_id; ?>-player&width=<?php print $width; ?>&height=<?php print $height; ?>&embedCode=<?php print $first_embed_code; ?>"></script>
  </div>
  <ul class="ooyala_shared_player_items">
    <?php foreach ($embed_codes as $embed_code => $title): ?>
      <li class="ooyala_shared_player_row" data-embed-code="<?php print $embed_code; ?>"><a href="#"><?php print $title; ?></a></li>
    <?php endforeach; ?>
  </ul>
</div>

