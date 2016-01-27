<?php
/*
 * @file hotel_booking_search_results.tpl.php
 * template file for search results - makes theming easier
 * @copyright Copyright(c) 2010 Lee Rowlands
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Lee Rowlands leerowlands at rowlands-bcs dot com
 *
 * results are available are $rooms and keyed by nid (array)
 * as follows array('image', 'title', 'total', 'book', 'pricing', 'description', 'calendar')
 * no results text is $no_results
 * $book_caption is the caption for the button as per admin screens
 *
 * @see template_preprocess_hotel_booking_search_results
 *
 */
?>
<?php if (isset($rooms) && is_array($rooms) && count($rooms) > 0) :?>
  <div class="hotel-booking-search-results-wrap">
  <?php foreach ($rooms as $nid => $room) : ?>
    <div class="hotel-booking-search-result" id="hotel-booking-search-result-<?php print $nid?>">
      <div class="hotel-booking-search-result-text">
        <?php if (isset($room['node'])) : ?>
          <div class="hotel-booking-search-result-node"><?php print render($room['node']);?></div>
        <?php endif; ?>
      </div>
      <div class="hotel-booking-search-result-book">
        <div class="hotel-booking-search-result-book-price"><?php print $room['total'];?></div>
        <div>
          <div class="hotel-booking-search-result-book-button"><?php print $room['book'];?></div>
          <label for="edit-book_room_<?php print $nid?>"><?php print $book_caption?></label>
        </div>
      </div>
      <div class="hotel-booking-search-addtl-details">
        <?php print $room['pricing']?>
        <?php if($room['calendar']) : ?>
          <?php print $room['calendar'];?>
        <?php endif; ?>
      </div>
    </div>
  <?php endforeach; ?>
  </div>
<?php elseif ($no_results): ?>
  <div class="hotel-booking-no-results"><?php print $no_results; ?></div>
<?php endif; ?>