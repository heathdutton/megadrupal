<?php
/**
 * @file
 * Context reaction overriding the default blocks reaction.
 */
class felix_reaction_block extends context_reaction_block {

  /**
   * An alternative version of block_list() that provides any context enabled
   * blocks plus blocks from Felix.
   *
   * @see context_reaction_block::block_list()
   */
  function block_list($region) {
    $blocks = parent::block_list($region);

    $felix_regions = _felix_regions_by_context($region);

    foreach ($felix_regions as $felix_region) {
      if (felix_block_access('add', $felix_region->region)) {
        $block = new stdClass;
        $block->module = 'felix';
        $block->delta = 'add-block';
        $block->status = 1;
        $block->weight = $felix_region->weight;
        $block->weight_felix = 0;
        $block->region = $region;
        $block->subject = $felix_region->title;
        $block->content = array(
          '#contextual_links' => array(
            'felix' => array(
              'felix-blocks/add',
              array($felix_region->name),
            )
          )
        );
        $blocks[] = $block;
      }

      $region_blocks = _felix_get_blocks_by_region($felix_region);
      $item_number = 0;
      foreach ($region_blocks as $block) {
        ++$item_number;
        $block->status = 1;
        $block->weight = $felix_region->weight + ($item_number / 1000);
        $block->weight_felix = $item_number;
        $block->region = $region;
        $block->felix_region = $felix_region->name;

        // Add the block controls if we may manage this region.
        // Note that there is no need to use if felix_block_access() here.
        // Finer permission checks are done by the menu system per contextual link.
        if (user_access('manage felix blocks')) {
          $static_info = &drupal_static('felix_' . $felix_region->name . $block->module . $block->delta . $block->fbid);
          $static_info = array(
            'region' => $felix_region,
            'block' => $block,
          );

          $block->content['#contextual_links']['felix'] = array(
            'felix-blocks/move-up',  // This is kind of hacky; for contextual links to even get to hook_menu_contextual_links_alter(), this needs to be an existing path. It won't actually result in a link
            array($block->fbid, $felix_region->name, $block->module, $block->delta, $item_number, count($region_blocks)),
          );
        }

        $blocks[] = $block;
      }
    }

    // Sort blocks by weight and weight_felix.
    $blocks = $this->sort_blocks($blocks);

    return $blocks;
  }

  /**
   * Reorder blocks
   *
   * Two steps are required to reorder the blocks. The array is sorted by the
   * uasort() function. That works in some cases, but themes may reorder the
   * blocks again based on the weight. Since all blocks from felix have the
   * same weight, this may result in different ordering. Hence we renumber
   * the weight numbers for all blocks.
   *
   * @param array $blocks
   */
  protected function sort_blocks($blocks) {
    uasort($blocks, 'felix_sort_blocks');
    foreach ($blocks as $i => $block) {
      if (isset($blocks[$i]->weight_felix)) {
        $blocks[$i]->weight += $i;
      }
    }
    return $blocks;
  }
}
