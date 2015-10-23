<?php

/**
 * @file
 * Provide views data and handlers for amazon.module
 */

/**
 * @defgroup views_amazon_module amazon.module handlers
 *
 * Includes the tables 'amazon_book', 'amazon_dvd', 'amazon_music',
 * and 'amazon_software'.
 * @{
 */

/**
 * Implementation of hook_views_data()
 */
function amazon_media_views_data() {
  // Define the base group of this table. Fields that don't
  // have a group defined will go into this field by default.
  $data['amazon_book']['table']['group']  = t('Amazon');
  $data['amazon_book']['table']['join'] = array(
    // participant links to amazon_item directly via asin.
    'amazon_item' => array(
      'left_field' => 'asin',
      'field' => 'asin',
    ),
  );

  _amazon_make_simple_text_field($data, 'amazon_book', 'deweydecimalnumber', 'Dewey decimal number', 'Dewey decimal number (used by libraries)');
  _amazon_make_simple_text_field($data, 'amazon_book', 'isbn', 'ISBN number (10-digit)', 'Library of Congress ISBN number (10-digit)');
  _amazon_make_simple_text_field($data, 'amazon_book', 'ean', 'EAN (13-digit ISBN number)', 'The newer, longer 13-digit EAN or 13-digit ISBN number');
  _amazon_make_simple_text_field($data, 'amazon_book', 'edition', 'Edition', 'The edition of an Amazon book product.');
  _amazon_make_simple_number_field($data, 'amazon_book', 'numberofpages', 'Pages', 'The number of pages in an Amazon book product.');
  _amazon_make_simple_date_field($data, 'amazon_book', 'publicationdate', 'Publication date', 'The publication date of the item');

  // Define the base group of this table. Fields that don't
  // have a group defined will go into this field by default.
  $data['amazon_dvd']['table']['group']  = t('Amazon');
  $data['amazon_dvd']['table']['join'] = array(
    // participant links to amazon_item directly via asin.
    'amazon_item' => array(
      'left_field' => 'asin',
      'field' => 'asin',
    ),
  );

  _amazon_make_simple_text_field($data, 'amazon_dvd', 'aspectratio', 'Aspect ratio', 'The aspect ratio of an Amazon video product.');
  _amazon_make_simple_text_field($data, 'amazon_dvd', 'audiencerating', 'Audience rating', 'The Audience rating of an Amazon video product (G, PG, etc).');
  _amazon_make_simple_number_field($data, 'amazon_dvd', 'regioncode', 'Region code', 'The region code of an Amazon DVD product.');
  _amazon_make_simple_number_field($data, 'amazon_dvd', 'runningtime', 'Running time', 'The length of an Amazon video product, in minutes.');

  // Define the base group of this table. Fields that don't
  // have a group defined will go into this field by default.
  $data['amazon_music']['table']['group']  = t('Amazon');
  $data['amazon_music']['table']['join'] = array(
    // participant links to amazon_item directly via asin.
    'amazon_item' => array(
      'left_field' => 'asin',
      'field' => 'asin',
    ),
  );

  // This appears to be a dupe of core data. We'll just ignore.
  // _amazon_make_simple_text_field($data, 'amazon_music', 'artist', 'Artist', 'The label that produced an Amazon music product.');
  _amazon_make_simple_text_field($data, 'amazon_music', 'label', 'Label', 'The label that published an Amazon music product.');
  _amazon_make_simple_text_field($data, 'amazon_music', 'studio', 'Studio', 'The studio that published an Amazon music product.');
  _amazon_make_simple_number_field($data, 'amazon_music', 'numberofdiscs', 'Number of discs', 'The number of discs included in an Amazon music product.');

  // Define the base group of this table. Fields that don't
  // have a group defined will go into this field by default.
  $data['amazon_software']['table']['group']  = t('Amazon');
  $data['amazon_software']['table']['join'] = array(
    // participant links to amazon_item directly via asin.
    'amazon_item' => array(
      'left_field' => 'asin',
      'field' => 'asin',
    ),
  );

  _amazon_make_simple_text_field($data, 'amazon_software', 'esrbagerating', 'ESRB age rating', 'The age rating of an Amazon software product.');
  _amazon_make_simple_text_field($data, 'amazon_software', 'operatingsystem', 'Operating system', 'The operating systems required by an Amazon software product.');
  _amazon_make_simple_text_field($data, 'amazon_software', 'hardwareplatform', 'Hardware platform', 'The hardware platform required by an Amazon software product.');
  _amazon_make_simple_text_field($data, 'amazon_software', 'platform', 'Platform', 'The platform required by an Amazon software product.');
  _amazon_make_simple_text_field($data, 'amazon_software', 'genre', 'Genre', 'The genre of an Amazon software product.');

  return $data;
}

/**
 * @}
 */
