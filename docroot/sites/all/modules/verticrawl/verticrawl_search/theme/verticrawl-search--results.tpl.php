<?php
/**
 * @file
 * Template file for display results.
 */

?>
<div class="verticrawl-search-results">
<?php foreach ($urls as $key => $url) : ?>
  <div class="verticrawl-search-item verticrawl-search-item-<?php print ($key % 2 == 0) ? 'even' : 'odd'; ?> verticrawl-search-item-<?php print $key; ?>">
    <?php if (isset($screenshot_url) && isset($url['sshot']) && $url['sshot'] == 1): ?>
      <div class="verticrawl-search-field verticrawl-search-field--screenshot">
        <a href="<?php print ($url['url']) ?>" target="_blank">
          <img rel="nofollow" src="<?php print ($screenshot_url . '&rec_id=' . $url['doc_id']); ?>" />
        </a>
      </div>
    <?php endif; ?>
    <div class="verticrawl-search-field verticrawl-search-field--url-title">
      <a href="<?php print ($url['url']) ?>" target="_blank"><?php print ($url['url-title']) ?></a>
    </div>
    <div class="verticrawl-search-field verticrawl-search-field--body">
      <?php print $url['body']; ?>
    </div>
    <div class="verticrawl-search-field verticrawl-search-field--url">
      <a href=<?php print $url['url']; ?> target="_blank">
        <?php print VerticrawlSearchApi::getDisplayUrl($url['url']); ?></a>
    </div>
    <?php if (isset($url['last-modified-text'])) : ?>
      <div class="verticrawl-search-field verticrawl-search-field--last-modified-text">
        <?php print ($url['last-modified-text']); ?>
      </div>
    <?php endif; ?>
    <?php if ($group_by_site && isset($url['persite-query'])) : ?>
      <div class="verticrawl-search-field verticrawl-search-field--site-results">
        <a href="<?php print (str_replace('&group_by_site=1', '', $current_url) . substr($url['persite-query'], 1)); ?>">
          <?php print (t('See all results for this website only')); ?>
        </a>
      </div>
    <?php endif; ?>
  </div>
<?php endforeach; ?>
</div>
