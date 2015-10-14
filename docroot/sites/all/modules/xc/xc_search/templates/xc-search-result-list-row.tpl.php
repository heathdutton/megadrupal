<?php
/**
 * @file templates/xc-search-result-list-row.tpl.php
 * Default theme implementation of a navigation bar on the browse admin UI
 *
 * Available variables:
 * - $counter: the record counter
 * - $node_id: the node id
 * - $result: the result (from Search API)
 * - $xc_record: the full XC record
 * - $attributes: attributes for TR element
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */?>
<tr <?php print $attributes; ?>>

  <!-- counter, selector -->
  <td class="num">
    <?php print $counter; ?>.
  </td>

  <!-- checkbox -->
  <td class="checkbox">
    <input class="num-checkbox form-checkbox" type="checkbox" name="items"
       value="<?php print $node_id; ?>|<?php print $result['extra']['id']; ?>" />
  </td>

  <!-- main data -->
  <td class="result<?php if ($result['extra']['highlight']): ?> has-highlight<?php endif; ?><?php if ($xc_record['node_type'] == 'xc_manifestation' && !empty($result['extra']['element']['abstract'])): ?> has-abstract<?php endif; ?>">
    <?php print $result['title']; ?>

    <?php if ($xc_record['node_type'] == 'xc_manifestation'
              && $result['extra']['element']['do_display_abstract'] == TRUE
              && !empty($result['extra']['element']['abstract'])) : ?>
      <div class="xc-abstract-container">
        <div class="xc-abstract">
          <span class="snippet-label">abstract:</span>
          <span><?php print $result['extra']['element']['abstract']; ?></span>
          <?php if (!empty($result['extra']['element']['abstract_more'])): ?>
            <span id="text_exposed_less_<?php print $counter; ?>" class="text_exposed_show">&hellip;</span>
            <span id="text_exposed_more_<?php print $counter; ?>" class="text_exposed_hide">
              <?php print $result['extra']['element']['abstract_more']; ?>
            </span>
            <a href="#" onclick="return XCSearch.showMore(this, <?php print $counter; ?>);"><?php print t('Read more')?></a>
          <?php endif; ?>
        </div>
      </div>
    <?php elseif ($result['extra']['highlight']): ?>
      <div class="highlight-container">
        <div class="highlight"><?php print $result['extra']['highlight'] ?></div>
      </div>
    <?php endif; ?>

    <?php print $result['snippet']; ?>
  </td>

  <!-- image -->
  <td class="coverart" id="coverart-<?php print $node_id; ?>">
    <a href="<?php print $result['link']; ?>"
      title="<?php print $result['extra']['cover_title']; ?>">
      <?php if ($result['extra']['image_url']): ?>
        <img src="<?php print $result['extra']['image_url']; ?>"
          alt="<?php print $result['extra']['cover_title']; ?>" title="<?php print $result['extra']['cover_title']; ?>" />
      <?php endif; ?>
    </a>
  </td>

</tr>