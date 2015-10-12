<?php
/**
 * @file
 *   Template file for the searchresults page (/amazon_store)
 *
 */

?>
<?php
  if (variable_get('amazon_store_show_searchform',TRUE)) {
    // Argument specifies how wide the keywords textfield should be, in chars.
    $form = drupal_get_form('amazon_store_search_form', 50);
    print drupal_render($form);
  }
?>
<div class="amazon-store-panel search-results"><!--
<a href='<?php print url(AMAZON_STORE_PATH . "/cart") ?>'> Go to your cart</a>
 -->


<?php if (!empty($results->Item)): ?>

<div class="change_sort"><?php
if (variable_get('amazon_store_show_sort_form',TRUE) && (int)$results->TotalResults > 1) {
  $form = drupal_get_form('amazon_store_sort_form');
  print drupal_render($form);
}?>
</div>
<div class="search-sets narrow-by"><?php
if (variable_get('amazon_store_show_narrowby_form',TRUE) && !empty($results->SearchBinSets) && (int)$results->TotalResults > 1) {
  $form = drupal_get_form('amazon_store_searchbin_sets_form', $results->SearchBinSets);
  print drupal_render($form);
}
?></div>


<div id="amazon-store-search-results" class="amazon-form"><?php if  (isset($Keywords) && isset($SearchIndex) && count($SearchIndex)) : ?>
<h3>Your search for <?php print "$Keywords in $SearchIndex" ?></h3>
<?php endif; ?>
<table>
	<tbody>

	<?php $i=0;
	foreach ($results->Item as $result):
	if ($result->OfferSummary->TotalNew == 0 && empty($result->Variations->Item)) {
	  continue;
	}
	$asin = (string)$result->ASIN;
	module_load_include('inc', 'amazon_store', 'amazon_store.pages');
	$form = drupal_get_form('amazon_store_addcart_form',(string)$result->ASIN);
	?>

		<!--  BEGIN ITEM PROCESSING -->
		<tr>
			<td><?php if (!empty($result->LargeImage)) : ?> <a rel="nofollow"
				href="<?php print $result->LargeImage->URL; ?>" class="colorbox-inline"
				title="<?php print $result->ItemAttributes->Title ?>"> <img
				src="<?php print $result->MediumImage->URL ?>"
				alt="Image of <?php print $result->ItemAttributes->Title ?>"
				class="product-image" /></a> <?php else: ?>
        <?php print theme('image', array('path' => "$directory/images/no_image_med.jpg")); ?>
       <?php endif; ?></td>
			<td>
			<p class="title"><a rel="nofollow"
				href="<?php print url(AMAZON_STORE_PATH ."/item/{$result->ASIN}") ?>"> <?php print $result->ItemAttributes->Title ?></a></p>
				<?php if (!empty($result->ItemAttributes->Manufacturer)){
				  print theme('amazon_store_search_results_manufacturer', array('manufacturer' => (string)$result->ItemAttributes->Manufacturer));
				}
				?>

			<div class="editorial">
        <?php if (!empty($result->EditorialReviews->EditorialReview[0]->Content)) { ?>
          <a href="javascript:void(null)"
          class="togglebtn">Show/hide full description</a> or
          <?php } ?>
          <a rel="nofollow"
				href="<?php print url(AMAZON_STORE_PATH ."/item/{$result->ASIN}"); ?>">See full
			details</a>
			<div class="toggle editorial">
			<?php
			if (!empty($result->EditorialReviews->EditorialReview[0]->Content)) {
			  print  _filter_htmlcorrector(filter_xss_admin($result->EditorialReviews->EditorialReview[0]->Content));
			}
			?>
      </div>
			</div>
			<?php print drupal_render($form); ?></td>

		</tr>
		<!--  END ITEM PROCESSING -->
		<?php endforeach; ?>


		<tr>
			<td colspan="3">
			<div class="greyrule"></div>
			</td>
		</tr>
	</tbody>
</table>

</div>
<!--  end mid_right_column_wrap -->
<div>

<?php print amazon_store_search_results_pager($results); ?>

</div>

		<?php endif; ?></div>
<?php print "<br />" . theme('amazon_store_link_button', array('text' => t("View Cart"), 'url' => AMAZON_STORE_PATH .'/cart'));
?>
