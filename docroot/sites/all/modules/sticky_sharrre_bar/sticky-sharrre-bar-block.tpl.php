<?php
/**
 * @file
 * Default theme implementation to Sticky Sharrre Bar block.
 *
 * Available variables:
 * - $providers: An array of providers.
 * - $url: URL to the page or node.
 * - $title: Title of the page or node.
 *
 * @ingroup themeable
 */
?>

<div class="sticky_sharrre_bar stickable">
  <h2 class="title"><?php print $title; ?></h2>
  <ul class="share_list">
    <?php foreach ($providers as $provider): ?>
      <li>
        <div class="share_on <?php print $provider; ?>"
             id="<?php print $provider; ?>" data-title="<?php print $title; ?>"
             data-url="<?php print $url; ?>"></div>
      </li>
    <?php endforeach; ?>
  </ul>
</div>
