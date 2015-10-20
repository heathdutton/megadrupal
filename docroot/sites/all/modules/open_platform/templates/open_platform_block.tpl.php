<?php
/**
 * @file
 * Displays a block.
 *
 * Available variables:
 * - $content_array: contains useful block data.
 * (list the other variables here)
 *
 * @see open_platform_theme()
 * @see open_platform_format_data()
 */
?>
<?php foreach ($content_array as $item => $content_value): ?>
  <div class="open_platform_ct">
    <?php if (isset($links[$item])) : ?>
      <h2> <?php print $links[$item]; ?> </h2>
    <?php endif; ?>
    <?php if (isset($thumbnail[$item])): ?>
      <img src='<?php print $thumbnail[$item]; ?>' class="open-platform-thumb" />
    <?php endif; ?>
    <?php if (isset($trail_text[$item])) : ?>
      <div>
        <?php print $trail_text[$item]; ?>
      </div>
    <?php endif; ?>
    <?php if (isset($body[$item])) : ?>
      <div>
        <?php print $body[$item]; ?>
      </div>
    <?php endif; ?>
  </div>  
<?php endforeach; ?>
<div> <?php print $logo; ?>   </div>
