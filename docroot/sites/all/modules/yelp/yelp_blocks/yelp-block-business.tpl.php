<?php
  /**
   * @file yelp-block-business.tpl.php
   *
   *  Default template file used to display a yelp block business . 
   *  
   *  Block specific template can be created by copying this template file to your theme
   *  and renaming it yelp-block-business--YELP_BLOCK_MACHINE_NAME.tpl.php
   *
   *  Per Yelp's terms of use, if you display any photos, THEY MUST link to Yelp
   *  Per Yelp's terms of use, YOU MUST display the rating and number of reviews . 
   *
   *  Available variables:
   *
   *  $business (object)              : The Yelp search result for a business,    
   *  $business->id (string)          : business Yelp ID
   *  $business->name (string)        : Name of business
   *  $business->url (string)          : Url to Yelp business
   *  $business->phone (string)        : phone number of business
   *  $business->display_phone (string)  : display version of the business phone number
   *  $business->image_url (string)    : url to image for business
   *  $business->location (object)    : location information about this business
   *  $business->location->cross_streets (string)
   *  $business->location->cross_streets (string)
   *  $business->location->city (string)
   *  $business->location->address (string)
   *  $business->location->display_address (array)
   *  $business->location->geo_accuracy(int)
   *  $business->location->neighborhoods (array)
   *  $business->location->postal_code (string)
   *  $business->location->country_code (string)
   *  $business->location->coordinate (object)
   *  $business->location->coordinate->latitude (float)
   *  $business->location->coordinate->longitude (float)
   *  $business->location->state_code (string)
   *  $business->mobile_url (string)  : mobile url to Yelp business
   *  $business->rating (int)          : Yelp rating of the business
   *  $business->rating_img_url (string)   : url of rating (stars) image
   *  $business->rating_img_url_small (string) : url to small version of rating (stars) image
   *  $business->rating_image_url_large (string) : url to large version of rating image
   *  $business->review_count (int)    : number of reviews business has on Yelp
   *  $business->snippet_text (string): snippet of text from latest review
   *  $business->snippet_image_url (string) : url to user photo for snippet
   *  $business->categories (array)    : associative array of yelp categories this business is in (display name  => category_id)
   *
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
   *  $yelp_block->yelp_settings->categories (array) : array of categories to search
   *
   *   For a complete break down of the business object
   *   @see http://www. yelp.com/developers/documentation/v2/search_api
   *
   * @ingroup themable
   */
?>
  <div class="yelp-block-business" id="<?php $business->id; ?>">
    <span class="business-name">
      <?php if (isset($business->image_url)): ?>
        <?php print l('<img src="' . $business->image_url . '" alt="' . $business->name . '" />', $business->url, array('html' => true, 'absolute' => true, 'attributes' => array('target' => '_blank'))); ?>
      <?php endif; ?>
      <br />
      <?php echo l($business->name, $business->url, array('absolute' => true, 'attributes' => array('target' => '_blank', 'class' => 'yelp-block-business-link'))); ?>
    </span>
    <br />
    <span class="business-rating">
      <?php print l('<img src="' . $business->rating_img_url . '" alt="' . $business->rating . '" />', $business->url, array('html' => true, 'absolute' => true, 'attributes' => array('target' => '_blank'))); ?>
      <br />
      <?php echo $business->review_count; ?> reviews . 
    </span>
    <br />
    <span class="business-address">
      <?php
        $address = array();
        if (count($business->location->address)>0) {
          $address[] = implode('<br />', $business->location->address);
        }
        $address[] = $business->location->city . ', ' . $business->location->state_code . ' ' . $business->location->postal_code;
        if (isset($business->display_phone)) {
          $address[] = $business->display_phone;
        }
        print implode('<br />', $address);
      ?>
    </span>
  </div>
  
  
  