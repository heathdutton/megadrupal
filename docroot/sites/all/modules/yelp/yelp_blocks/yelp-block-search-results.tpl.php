<?php
  /**
   * @file yelp_block_search_results.tpl.php
   *
   *  Default template for listing Yelp search results in a yelp block
   *  
   *  Block specific template can be created by copying this template file to your theme
   *  and renaming it yelp-block-search-results--YELP_BLOCK_MACHINE_NAME.tpl.php
   *
   *  Per Yelp terms of use, you must include a Yelp logo when display search results
   *  @see http://www.yelp.com/developers/getting_started/display_requirements
   *
   *  All of Yelps images are provided by the yelp_api module, in the 'yelp-images' directory . 
   *  To display one, use the drupal_get_path() function:
   *  <?php echo drupal_get_path('module', 'yelp_api'); ?>/yelp-images/IMAGE_NAME
   *
   *  Available variables:
   *
   *  $businesses (array)    : an array of rendered Yelp results
   *  $total (int)          : the total number of results returned
   *  $results (object)      : raw (unsafe) results returned from Yelp API . 
   *  $yelp_block (object)  : the yelp block being displayed
   *  $yelp_block->yelp_block_id (int) : id of the block
   *  $yelp_block->machine_name  (string)  : yelp block machine name
   *  $yelp_block->title (string)  : block title
   *  $yelp_block->admin_title (string) : Admin title
   *  $yelp_block->cache (string)  : cache settings
   *  $yelp_block->yelp_id (int)  : yelp API results id
   *  $yelp_block->yelp_settings (object) : Yelp API search settings
   *  $yelp_block->yelp_settings->yelp_id (int) : Yelp API results id
   *  $yelp_block->yelp_settings->location (string) : location to search
   *  $yelp_block->yelp_settings->radius (int)  : radius in miles to search
   *  $yelp_block->yelp_settings->max_results (int) : max number of results to return
   *  $yelp_block->yelp_settings->cc (string) : country code to search
   *  $yelp_block->yelp_settings->lang (string) : language code to search
   *   $yelp_block->yelp_settings->categories (array) : array of categories to search
   *
   * @ingroup themable
   *
   */
?>
<div class="yelp-block-results" id="<?php echo $yelp_block->machine_name; ?>">
  <?php if ($businesses): ?>
  <ul class="yelp-results-list">
  <?php foreach ($businesses as $id  => $business) { ?>
    <li id="yelp-result-<?php echo $id; ?>">
      <?php print $business; ?>
    </li>
  <?php } ?>
  </ul>
  <?php endif; ?>
  <br />
  <a href="http://www.yelp.com" target="_blank">
    <img src="/<?php echo drupal_get_path('module', 'yelp_api'); ?>/yelp-images/Powered_By_Yelp_Red.png" alt="Powered by Yelp!" />
  </a>
</div>