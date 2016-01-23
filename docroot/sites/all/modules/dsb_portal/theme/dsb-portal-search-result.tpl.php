<?php

/**
 * @file
 * Template for rendering a single dsb Portal search result.
 *
 * Available variables (none are sanitized):
 * - $result: The \Educa\DSB\Client\Lom\LomDescriptionSearchResult search result
 *   object to render.
 *
 * @ingroup themeable
 */
?>
<div class="dsb-portal-search-result">
  <div class="dsb-portal-search-result__partner-data">
    <?php print dsb_portal_theme_owner_filter_link($result, TRUE); ?>
  </div>

  <?php if ($result->getPreviewImage()): ?>
    <div class="dsb-portal-search-result__preview-image">
      <a href="<?php print url("dsb-portal/description/{$result->getLomId()}"); ?>">
        <?php print theme('image', array(
          'path' => $result->getPreviewImage(),
          'alt' => t("Preview image for LOM object @title", array(
            '@title' => $result->getTitle(),
          )),
        )); ?>
      </a>
    </div>
  <?php endif; ?>

  <div class="dsb-portal-search-result__description">
    <h3 class="dsb-portal-search-result__description__title">
      <?php print l($result->getTitle(), "dsb-portal/description/{$result->getLomId()}"); ?>
    </h3>

    <div class="dsb-portal-search-result__description__teaser">
      <?php print check_plain($result->getTeaser()); ?>
    </div>
  </div>
</div>
