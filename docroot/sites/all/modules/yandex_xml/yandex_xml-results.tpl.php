<?php

/**
 * @file
 * Default theme implementation for displaying search results.
 * 
 * @see template_preprocess_yandex_xml_results()
 *
 * @ingroup themeable
 */
?>
<div class="yandex-search-summary">
  <?php print $summary; ?>
</div>
<dl class="yandex-search-results">
  <?php foreach ($results as $result): ?>
      <dt>
        <a href="<?php print $result['url']; ?>" title="<?php echo $result['url']; ?>"><?php print $result['title']; ?></a>
      </dt>
      <dd>
        <?php if ($result['headline']): ?>
          <div class="headline">
            <?php print $result['headline']; ?>
          </div>
        <?php endif; ?>
        <?php if ($result['passages']): ?>
          <div class="passages">
            <?php print $result['passages']; ?>
          </div>
        <?php endif; ?>
        <a href="<?php print $result['url']; ?>" class="yandex-search-host"><?php print $result['url']; ?></a>
      </dd>
  <?php endforeach;?>
</dl>
