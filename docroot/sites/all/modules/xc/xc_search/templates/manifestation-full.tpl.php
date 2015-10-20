<?php
/**
 * @file
 * Template for manifestation's full record display
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */
?>

<div id="xc-search-full">

  <?php /*  THE RIGHT SIDEBAR */ ?>
  <div id="xc-search-full-right">
    <div id="coverart-<?php print $node_id; ?>" class="coverart">
      <?php if ($image_url_large): ?>
        <a href="<?php print $image_url_large ?>" rel="lightbox"
           title="<?php if ($title): ?><?php print $title; ?><?php endif?><?php if ($creator): ?> by <?php print $creator; ?><?php endif?>">
          <img src="<?php print $image_url_medium; ?>" alt="" />
        </a>
      <?php elseif ($image_url_medium): ?>
        <img src="<?php print $image_url_medium; ?>" alt="" />
      <?php endif; ?>
    </div>

    <div id="buttons-<?php print $node_id; ?>" class="buttons">
      <?php print $action_buttons; ?>
    </div>
  </div>

  <div id="xc-search-full-left">

    <table class="xc-search-full-table">
      <?php if ($title): ?>
        <tr>
          <td class="xc-label"><?php print t('Title'); ?>:</td>
          <td class="xc-title"><?php print $title; ?></td>
        </tr>
      <?php endif; ?>

      <?php if (isset($top_author_links)): ?>
        <tr>
          <td class="xc-label"><?php print t('Creators/Authors'); ?>:</td>
          <td><?php print $top_author_links; ?></td>
        </tr>
      <?php endif; ?>

      <?php if (isset($publisher) || isset($placeOfProduction) || isset($date)): ?>
        <tr>
          <td class="xc-label"><?php print t('Publication info'); ?>:</td>
          <td>
          <?php if (isset($placeOfProduction)): ?>
            <?php print $placeOfProduction; ?>
          <?php endif; ?>

          <?php if ($publisher): ?>
            <?php print $publisher; ?>
          <?php endif; ?>

          <?php if ($date): ?>
            <?php print $date; ?>
          <?php endif; ?>
          </td>
        </tr>
      <?php endif ?>

      <?php if ($format || $language): ?>
        <tr>
          <td class="xc-label"><?php print t('Edition/Format'); ?>:</td>
          <td>
            <?php if ($format): ?><span class="xc-format"><?php print $format; ?></span><?php endif; ?>
            <?php if ($format && ($language || isset($editionStatement))) : ?>; <?php endif; ?>
            <?php if ($language): ?><?php print $language; ?><?php endif; ?>
            <?php if ($language && $editionStatement): ?>; <?php endif; ?>
            <?php if (isset($editionStatement)): ?><?php print $editionStatement; ?><?php endif; ?>
          </td>
        </tr>
      <?php endif; ?>

      <?php if (isset($holdings_summary)): ?>
        <tr>
          <td class="xc-label"><?php print t('Holdings Summary'); ?>:</td>
          <td>
            <?php if ($xc_record['holdings_use_table'] == TRUE): ?>
              <?php print theme('table', array('headers' => array(t('Location'), t('Call number'), t('Textual holdings')), 'rows' => $holdings_summary)); ?>
            <?php else: ?>
              <?php print theme('item_list', array('items' => $holdings_summary)); ?>
            <?php endif; ?>
          </td>
        </tr>
      <?php endif; ?>

      <?php if (!empty($url_identifiers)): ?>
        <tr>
          <td class="xc-label"><?php print t('Location'); ?>:</td>
          <td><?php print theme('item_list', array('items' => $url_identifiers)); ?></td>
        </tr>
      <?php else : ?>
        <tr class="xc-availability">
          <td class="xc-label"><?php print t('Location') ?>:</td>
          <td id="xc-availability-<?php print $node_id; ?>">
            <img src="<?php print drupal_get_path('module', 'xc_search') . '/images/ajax-loader.gif' ?>"
                 alt="<?php print t('Loading availability information'); ?>"
                 title="<?php print t('Loading availability information'); ?>" />
            <?php /* print theme('manifestation_full_locations', $locations) */ ?>
          </td>
        </tr>
      <?php endif; ?>

      <?php // has version /dcterms:hasVersion ?>
      <?php if (!empty($has_version)): ?>
        <tr>
          <td class="xc-label"><?php print t('Has Version(s)'); ?>:</td>
          <td><?php print theme('item_list', array('items' => $has_version)); ?></td>
        </tr>
      <?php endif; ?>

      <?php if (isset($genre)):?>
        <tr class="xc-genre">
          <td class="xc-label"><?php print t('Genre/Form'); ?>:</td>
          <td class="xc-genre"><?php print $genre; ?></td>
        </tr>
      <?php endif; ?>

      <?php if (isset($natureOfContent)): ?>
        <tr>
          <td class="xc-label"><?php print t('Material Type'); ?>:</td>
          <td><?php print $natureOfContent; ?></td>
        </tr>
      <?php endif; ?>

      <?php if (isset($format)):?>
        <tr>
          <td class="xc-label"><?php print t('Document Type'); ?>:</td>
          <td><?php print $format; ?></td>
        </tr>
      <?php endif; ?>

      <?php if (isset($all_author_links) && !empty($all_author_links)): ?>
        <tr class="xc-other-contributors">
          <td class="xc-label"><?php print t('Other contributors'); ?>:</td>
          <td><?php print theme('item_list', array('items' => $all_author_links)); ?></td>
        </tr>
      <?php endif; ?>

      <?php if (isset($thesisAdvisor)):?>
        <tr>
          <td class="xc-label"><?php print t('Thesis Advisor'); ?>:</td>
          <td><?php print $thesisAdvisor; ?></td>
        </tr>
      <?php endif; ?>

      <?php if (isset($ISBN) && !empty($ISBN)): ?>
        <tr>
          <td class="xc-label"><?php print t('ISBN'); ?>:</td>
          <td><?php print $ISBN; ?></td>
        </tr>
      <?php endif; ?>

      <?php if (isset($ISSN) && $ISSN != null): ?>
        <tr>
          <td class="xc-label"><?php print t('ISSN'); ?>:</td>
          <td>9999<?php print $ISSN; ?></td>
        </tr>
      <?php endif; ?>

      <?php if (isset($OCLC)): ?>
        <tr>
          <td class="xc-label"><?php print t('OCLC'); ?>:</td>
          <td><?php print $OCLC; ?></td>
        </tr>
      <?php endif; ?>

      <?php if (isset($dissertationOrTheses) || isset($description)): ?>
        <tr>
          <td class="xc-label"><?php print t('Notes'); ?>:</td>
          <td>
          <?php if (isset($dissertationOrTheses)): ?>
            <?php print $dissertationOrTheses; ?>
          <?php endif; ?>
          <?php if ($description): ?>
            <?php print $description; ?>
          <?php endif; ?>
          </td>
        </tr>
      <?php endif; ?>

      <?php if (isset($audience)): ?>
        <tr>
          <td class="xc-label"><?php print t('Target Audience'); ?>:</td>
          <td><?php print $audience; ?></td>
        </tr>
      <?php endif; ?>

      <?php if (isset($extent) || isset($otherPhisicalDetails) || isset($dimensions)): ?>
        <tr>
          <td class="xc-label"><?php print t('Description'); ?>:</td>
          <td>
            <?php if (isset($extent)): ?>
              <?php print $extent; ?>
            <?php endif; ?>
            <?php if (isset($otherPhisicalDetails)): ?>
              <?php print $otherPhisicalDetails; ?>
            <?php endif; ?>
            <?php if (isset($dimensions)): ?>
              <?php print $dimensions; ?>
            <?php endif; ?>
          </td>
        </tr>
      <?php endif; ?>

      <?php if (isset($requires)): ?>
        <tr>
          <td class="xc-label"><?php print t('Details'); ?>:</td>
          <td><?php print $requires; ?></td>
        </tr>
      <?php endif; ?>

      <?php if (isset($tableOfContents)): ?>
        <tr>
          <td class="xc-label"><?php print t('Contents'); ?>:</td>
          <td><?php print preg_replace('/\.$/','<br/>',str_replace(' -- ','<br />', $tableOfContents)); ?></td>
        </tr>
      <?php endif; ?>

      <?php if (isset($titleOfWork)): ?>
        <tr>
          <td class="xc-label"><?php print t('Other titles'); ?>:</td>
          <td><?php print $titleOfWork; ?></td>
        </tr>
      <?php endif; ?>

      <?php if (isset($responsibility) && !empty($responsibility)): ?>
        <tr>
          <td class="xc-label"><?php print t('Responsibility'); ?>:</td>
          <td><?php print $responsibility; ?></td>
        </tr>
      <?php endif; ?>

      <?php if (isset($isPartOf) && !empty($isPartOf)): ?>
        <tr>
          <td class="xc-label"><?php print t('Series Title'); ?>:</td>
          <td><?php print $isPartOf; ?></td>
        </tr>
      <?php endif; ?>

      <?php if (isset($abstract) && !empty($abstract)): ?>
        <tr>
          <td class="xc-label"><?php print t('Abstract'); ?>:</td>
          <td><?php print $abstract; ?></td>
        </tr>
      <?php endif ?>

      <?php if (isset($summary) && !empty($summary)): ?>
        <tr>
          <td class="xc-label"><?php print t('Reviews'); ?>:</td>
          <td><?php print $summary; ?></td>
        </tr>
      <?php endif ?>

      <?php if (isset($relations) && !empty($relations)): ?>
        <tr id="xc-search-related-resources">
          <td class="xc-label"><?php print t('Related Resources'); ?>:</td>
          <td>
          <?php if (count($relations) == 1) : ?>
            <?php print $relations[0]; ?>
          <?php else: ?>
            <?php print theme('item_list', array('items' => $relations,
              'title' => NULL,
              'type' => 'ul',
              'attributes' => array('class' => array('xc-relations')))); ?>
          <?php endif; ?>
          </td>
        </tr>
      <?php endif; ?>

      <?php if (isset($subject_topic) && !empty($subject_topic)): ?>
        <tr id="subject-topic" class="xc-search-subjects">
          <td class="xc-label"><?php print t('Topic'); ?>:</td>
          <td><?php print theme('item_list', array('items' => $subject_topic)); ?></td>
        </tr>
      <?php endif; ?>

      <?php if (isset($subject_spatial) && !empty($subject_spatial)): ?>
        <tr id="subject-spatial" class="xc-search-subjects">
          <td class="xc-label"><?php print t('Region'); ?>:</td>
          <td><?php print theme('item_list', array('items' => $subject_spatial)); ?></td>
        </tr>
      <?php endif; ?>

      <?php if (isset($subject_temporal) && !empty($subject_temporal)): ?>
        <tr id="subject-temporal" class="xc-search-subjects">
          <td class="xc-label"><?php print t('Time period'); ?>:</td>
          <td><?php print theme('item_list', array('items' => $subject_temporal)); ?></td>
        </tr>
      <?php endif; ?>

      <?php if (!empty($enhancements) && !empty($enhancements)): ?>
        <?php foreach ($enhancements as $module => $enhancement): ?>
          <tr class="enhancement-title" <?php if (isset($enhancement['meta']['id'])): ?>id="<?php print $enhancement['meta']['id']; ?>"<?php endif; ?>>
            <td class="xc-label"></td>
            <td class="enhancement-title">
              <?php if (isset($enhancement['meta']['title'])): ?>
                <?php print $enhancement['meta']['title']; ?>
              <?php endif; ?>
            </td>
          </tr>
          <?php if (isset($enhancement['content'])): ?>
            <?php foreach ($enhancement['content'] as $key => $value) : ?>
              <tr class="enhancement-content">
                <td class="xc-label"><?php print $key; ?>:</td>
                <td><?php print $value; ?></td>
              </tr>
            <?php endforeach; ?>
          <?php endif; ?>
        <?php endforeach; ?>
      <?php endif ?>

      <?php

        // processed fields for admin viewing only
        $processed_fields = array(
          'id',
          'node_id',
          'timestamp',
          'type',
          'node_type',
          'processed',
          'metadata_id',
          'metadata_type',
          'syndetics_ids',
          'image_url',
          'dcterms:tableOfContents',
          'rdvocab:titleOfWork',
          'dcterms:description',
          'xc:relation',
          'dcterms:relation',
          'rdvocab:placeOfPublication',
          'xc:workExpressed',
          'xc:expressionManifested',
          'xc:recordID_OCoLC',
          'dcterms:extent',
          'mlt',
          'xc:subject',
          'dcterms:subject',
          'rdvocab:natureOfContent',
          'dcterms:identifier_ISBN',
          'summary',
          'language',
          'xc:isPartOf',
          'dcterms:isPartOf',
          'rdvocab:placeOfProduction',
          'dcterms:identifier',
          'rdvocab:editionStatement',
          'mlts',
          'holdings_summary',
          'rdvocab:statementOfResponsibilityRelatingToTitle',
          'dcterms:audience',

          // display template elements
          'dcterms:language',
          'xc:spacial',
          'xc:spatial',

          // language

          // special fields
          'image_url_large',
          'image_url_medium',
          'status',
          'serialized_tsn',
          'locations',

          // facets
          'format',
          'classification',
          'topic',
          'date',
          'genre',
          'subject',
          'authors_contributors',
          'other_contributors',
          'related_resources',
          'subject_temporal',
          'subject_topic',
        );

      ?>

      <?php if (isset($has_admin_rights) && $has_admin_rights == TRUE) foreach ($xc_record as $key => $values): ?>
        <?php if (!in_array($key, $processed_fields)): ?>
          <tr>
            <td class="xc-label"><?php print t($key); ?>:</td>
            <td>
              <?php if (is_array($values)) : ?>
                <?php if (count($values) > 1) : ?>
                  <ul class="xc-search-subject">
                    <?php foreach ($values as $value) : ?>
                      <li><?php print $value['#value']; ?></li>
                    <?php endforeach; ?>
                  </ul>
                <?php else: ?>
                  <?php print $values[0]['#value']; ?>
                <?php endif; ?>
              <?php else: ?>
                <?php print $values; ?>
              <?php endif; ?>
            </td>
          </tr>
        <?php endif; ?>
      <?php endforeach; ?>
    </table>

  </div>

</div>

<!------------------------------------------------------------------------------

  NOTE: Enhancements, related subject topics, regions, time periods, and
  resources are disabled in the default XC theme

<?php /*********************** DISABLED IN THEME *******************************


*****************************************************************************/?>
------------------------------------------------------------------------------->

<?php if (isset($mlts)): ?>
  <?php $count = 1; $image_mlts = ''; ?>
  <?php foreach ($mlts as $mlt): ?>
    <?php $image_mlts .= trim(theme('manifestation_full_mlt', array('mlt' => $mlt, 'count' => $count, 'show' => 'image'))); ?>
    <?php $count++; ?>
  <?php endforeach; ?>

  <?php $count = 1; $title_mlts = array(); ?>
  <?php foreach ($mlts as $mlt): ?>
    <?php $title_mlts[] = trim(theme('manifestation_full_mlt', array('mlt' => $mlt, 'count' => $count, 'show' => 'title'))); ?>
    <?php $count++; ?>
  <?php endforeach; ?>

  <div id="xc-search-similar-items">
    <h3 class="title"><?php print t('Similar Items'); ?>:</h3>

    <?php /* print out images */ ?>
    <?php if ($image_mlts): ?>
      <div id="xc-search-similar-items-images">
        <?php print $image_mlts; ?>
        <div style="clear:both;"></div>
      </div>
    <?php endif; ?>

    <?php /* print out list */ ?>
    <?php if ($title_mlts): ?>
      <div id="xc-search-similar-items-titles">
        <?php print theme('xc_multicolumn_table', array('links' => $title_mlts, 'colnum' => 2)); ?>
        <div style="clear:both;"></div>
      </div>
    <?php endif; ?>
  </div>
<?php endif; ?>