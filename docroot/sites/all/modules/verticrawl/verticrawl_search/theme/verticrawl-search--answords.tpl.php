<?php
/**
 * @file
 * Template file for display answords campaign results.
 */

?>
<div class="verticrawl-search-answords">
<?php foreach ($answords as $key => $answord) : ?>
  <div class="verticrawl-search-item verticrawl-search-item-<?php print ($key % 2 == 0) ? 'even' : 'odd'; ?> verticrawl-search-item-<?php print $key; ?>">
    <?php if (isset($answord['URL_IMAGE']) && !empty($answord['URL_IMAGE'])) : ?>
    <div class="verticrawl-search-field verticrawl-search-field--url_image">
      <img src="<?php print $answord['URL_IMAGE'] ?>"/>
    </div>
    <?php endif; ?>
    <div class="verticrawl-search-field verticrawl-search-field--title">
      <a class="verticrawl-answord-link" href="http://<?php print $answord['URL_SHOW']; ?>" data-redirect="<?php print ($answord['URL_TRACK']) ?>" target="_blank">
        <?php print ($answord['TITLE']) ?>
      </a>
    </div>
    <div class="verticrawl-search-field verticrawl-search-field--description">
      <?php print $answord['DESCRIPTION']; ?>
    </div>
    <div class="verticrawl-search-field verticrawl-search-field--url">
      <a class="verticrawl-answord-link" href="http://<?php print $answord['URL_SHOW']; ?>" data-redirect="<?php print ($answord['URL_TRACK']) ?>" target="_blank">
        <?php print $answord['URL_SHOW']; ?></a>
    </div>
  </div>
<?php endforeach; ?>
</div>
