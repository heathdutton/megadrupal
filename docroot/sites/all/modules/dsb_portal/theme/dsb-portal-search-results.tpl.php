<?php

/**
 * @file
 * Template for rendering dsb Portal search results.
 *
 * Available variables (none are sanitized):
 * - $num_found: The total number of results available for the search query.
 * - $results: The search results to render. Each item is an instance of
 *   \Educa\DSB\Client\Lom\LomDescriptionSearchResult.
 *
 * @ingroup themeable
 */
?>
<div class="dsb-portal-search-results">
  <div class="dsb-portal-search-results__total-num-found">
    <h4><?php print t("@num results found", array('@num' => $num_found)); ?></h4>
  </div>

  <div class="dsb-portal-search-results__results">
    <?php foreach ($results as $result): ?>
      <div class="dsb-portal-search-results__results__result">
        <?php print theme('dsb_portal_search_result', array(
          'result' => $result,
        )); ?>
      </div>
    <?php endforeach; ?>
  </div>
</div>
