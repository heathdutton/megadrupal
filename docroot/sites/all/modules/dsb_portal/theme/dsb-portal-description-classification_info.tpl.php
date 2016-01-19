<?php

/**
 * @file
 * Template for rendering a the educational classification information for a
 * single dsb Portal LOM description.
 *
 * Available variables (none are sanitized):
 * - $description: The \Educa\DSB\Client\Lom\LomDescriptionInterface object to
 *   render.
 * - $trees: A list of trees representing the below classification information.
 *   Each tree has the following keys:
 *   - source: A human-readable string representing the source of the tree.
 *     Example: "Plan d'études romand (PER)", "Default curriculum", etc. This
 *     variable is already sanitized.
 *   - educational_level_tree: (optional) A pre-rendered HTML tree containing
 *     all the below educational level information, if available.
 *   - discipline_tree: (optional) A pre-rendered HTML tree containing all the
 *     below discipline information, if available.
 * - $classification_information: An array of information about classification.
 *   Each item is part of a curriculum ("Plan d'étude", or "Lehrplan"). Each
 *   item is can have the following keys:
 *   - source: The source of the curriculum. "educa" is the default and
 *     standard curriculum. "per" is the PER curriculum, etc.
 *   - educational level: (optional) The educational level data. Not all
 *     curricula provide this information.
 *   - discipline: (optional) The disciplines covered, with hierarchical
 *     information based on the educational level. Not all curricula
 *     provide this information.
 *   Each entry in the above trees has the following keys:
 *   - entry: A list of human-readable data, keyed by language key. In some
 *     cases, the data is an array with meta-data, but in most cases it is
 *     simply a string.
 *   - children: An array of sub-entries, which have the same keys.
 *
 * @see dsb_portal_preprocess_dsb_portal_description_classification_info()
 *
 * @ingroup themeable
 */
?>
<div class="dsb-portal-description-classification-information">
  <?php foreach ($trees as $data): ?>
    <div class="dsb-portal-description-classification-information__trees">
      <h3><?php print $data['source']; ?></h3>

      <?php if (!empty($data['discipline_tree'])): ?>
        <div class="dsb-portal-description-classification-information__trees__tree dsb-portal-description-classification-information__discipline-level">
          <?php print $data['discipline_tree'] ?>
        </div>
      <?php endif; ?>
    </div>
  <?php endforeach; ?>
</div>
