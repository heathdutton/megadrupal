<?php

/**
 * @file
 * Ultimate Cron configuration for FlickrCollection.
 */

/**
 * Implements hook_cronapi().
 *
 * Provide more info on cron handling if ultimate_cron.module is used.
 */
function flickrcollection_cronapi() {
  return array(
    'flickrcollection_cron' => array(
      'title' => t('Update the list of sets in the Flickr Collection'),
      'scheduler' => array(
        'simple' => array(
          'rules' => array('0+@ */6 * * *'),
        ),
        'crontab' => array(
          'rules' => array('0+@ */6 * * *'),
        ),
      ),
    ),
  );
}
