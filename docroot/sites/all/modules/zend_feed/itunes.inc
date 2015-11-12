<?php

/**
 * @file
 */

/**
 * Construct iTunes extension data array.
 * 
 * @param $zend_itunes
 * @param array $feed
 * @return array
 */
function _parser_zend_feed_parse_itunes($zend_itunes, $feed) {
  return array(
    'author'       => 'Artist column', // optional, default to the main author value
    'owner'        => array(
      'name' => 'name of the owner', // optional, default to main author value
      'email' => 'email of the owner', // optional, default to main email value
    ), // Owner of the podcast // optional
    'image'        => 'album/podcast art', // optional, default to the main image value
    'subtitle'     => 'short description', // optional, default to the main description value
    'summary'      => 'longer description', // optional, default to the main description value
    'block'        => 'Prevent an episode from appearing (yes|no)', // optional
    'category'     => array(
      array(
        'main' => 'main category', // required
        'sub'  => 'sub category' // optional
      ),
      // up to 3 rows
    ), // 'Category column and in iTunes Music Store Browse' // required
    'explicit'     => 'parental advisory graphic (yes|no|clean)', // optional
    'keywords'     => 'a comma separated list of 12 keywords maximum', // optional
    'new-feed-url' => 'used to inform iTunes of new feed URL location', // optional
  ); // Itunes extension data // optional, ignored if atom is used
}
