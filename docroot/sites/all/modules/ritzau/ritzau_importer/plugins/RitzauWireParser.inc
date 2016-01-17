<?php
/**
 * @file
 * This file contains the Ritzau feeds parser
 *
 */

class RitzauWireParser extends FeedsParser {

  /**
   * Implements FeedsParser::parse().
   */
  public function parse(FeedsSource$source, FeedsFetcherResult$fetcher_result) {

    $wire   = array();
    $string = $fetcher_result->getRaw();
    $xml    = new DOMDocument("1.0", 'iso-8859-1');
    $xml->loadHTML($string);

    //Filtering of unwanted wires
    if ($xml->getElementsByTagName('urg')->item(0)->hasAttribute('prio')) {

      $cat = $xml->getElementsByTagName('cat')->item(0)->textContent;

      //If there is no category set - do nothing
      if (empty($cat)) {
        return new FeedsParserResult(array());
      }
      $threshold = $this->config['priority_' . $cat];
      $priority = $xml->getElementsByTagName('urg')->item(0)->getAttribute('prio');
      //
      if (empty($threshold) || $threshold == 0 || $threshold < $priority) {
        return new FeedsParserResult(array());
      }
    }


    $body_element = simplexml_import_dom($xml->getElementsByTagName('ord')->item(0));
    $body_string = '';
    foreach ($body_element->children() AS $child) {
      $body_string .= $child->asXML();
    }
    $logs = array(
      '01' => t('The wire has been canceled. The node is unpublished.'),
      '02' => t('The wire is updated.'),
    );

    $wire['headline'] = $xml->getElementsByTagName('headline')->item(0)->textContent;
    $wire['body'] = $body_string;
    $wire['ritzau_id'] = $xml->getElementsByTagName('uno')->item(0)->textContent;
    $wire['refering_ritzau_id'] = $xml->getElementsByTagName('refuno')->item(0)->textContent;
    $wire['city'] = $xml->getElementsByTagName('city')->item(0)->textContent;
    $wire['source'] = $xml->getElementsByTagName('source')->item(0)->textContent;

    $wire['category'] = $xml->getElementsByTagName('cat')->item(0)->textContent;
    $wire['category_tid'] = $this->getTaxonomyTid($wire['category']);

    $wire['genre'] = $xml->getElementsByTagName('objattr')->item(0)->textContent;
    $wire['genre_tid'] = $this->getTaxonomyTid($wire['genre']);

    $wire['iptc_subjects_string'] = $xml->getElementsByTagName('subrefno')->item(0)->textContent;
    $wire['iptc_subjects_array'] = explode('+', str_replace('IPTC:', '', $wire['iptc_subjects_string']));
    $wire['iptc_subjects_tids'] = $this->getTaxonomyTid($wire['iptc_subjects_array']);

    $wire['caption'] = $xml->getElementsByTagName('caption')->item(0)->textContent;
    $wire['urgency'] = $xml->getElementsByTagName('urg')->item(0)->getAttribute('prio');
    $wire['status'] = $xml->getElementsByTagName('actadv')->item(0)->textContent;
    $wire['published'] = (!empty($wire['status']) && $wire['status'] == '01') ? 0 : 1;
    $wire['log'] = $logs[$wire['status']];
    $wire['reference'] = $xml->getElementsByTagName('refuno')->item(0)->textContent;

    $wire['sent_date'] = $xml->getElementsByTagName('datesent')->item(0)->textContent;
    $wire['sent_time'] = $xml->getElementsByTagName('timesent')->item(0)->textContent;
    $sent_datetime = $this->convertDateAndTimeToTimestamp($wire['sent_date'], $wire['sent_time']);

    $date = new DateObject($sent_datetime);
    if (isset($wire['status']) && !empty($wire['status'])) {
      $wire['sent_datetime'] =  $date->format('c');
      $wire['is_updated'] = 1;
    }
    elseif (empty($wire['status'])) {
      $wire['sent_datetime'] =  $date->format('c');
      $wire['original_sent_datetime'] =  $date->format('c');
      $wire['is_updated'] = 0;
    }

    $wire['release_date'] = $xml->getElementsByTagName('releasedate')->item(0)->textContent;
    $wire['release_time'] = $xml->getElementsByTagName('releasetime')->item(0)->textContent;
    $wire['release_datetime'] = $wire['sent_datetime'];
    if (!empty($wire['release_date']) && !empty($wire['release_time'])) {
      $sent_datetime = $this->convertDateAndTimeToTimestamp($wire['release_date'], $wire['release_time']);
      $release_date = new DateObject($sent_datetime);
      $wire['release_datetime'] = $release_date->format('c');
    }


    /// Move the xml file.
    $pathinfo = pathinfo($fetcher_result->getFilePath());
    $destination_subpath = drupal_substr($wire['sent_date'], 0, 4) . '/' . drupal_substr($wire['sent_date'], 4, 2) . '/' . drupal_substr($wire['sent_date'], 6, 2);
    $full_path = 'private://' . $this->config['destination'] . '/' . $destination_subpath;
    $destination_folder = file_stream_wrapper_uri_normalize($full_path);
    file_prepare_directory($destination_folder, FILE_CREATE_DIRECTORY);

    $uri = file_unmanaged_move($pathinfo['dirname'] . '/' . $pathinfo['basename'], $destination_folder . '/' . $pathinfo['basename'], FILE_EXISTS_RENAME);
    if (!empty($uri)) {
      $article['xmlfile'] = $uri;
    }
    return new FeedsParserResult(array($wire));
  }

  private function convertDateAndTimeToTimestamp($date, $time) {
    return mktime(
      drupal_substr($time, 0, 2), //Hour
      drupal_substr($time, 2, 2), //Minute
      drupal_substr($time, 4, 2), //Second
      drupal_substr($date, 4, 2), //Month
      drupal_substr($date, 6, 2), //Day
      drupal_substr($date, 0, 4) //Year
    );
  }


  private function getCategories() {
    return array(
      'U' => 'Udland',
      'I' => 'Indland',
      'E' => 'Erhverv',
      'P' => 'Politik',
      'K' => 'Kriminal',
      'S' => 'Sport',
      'B' => 'Børs',
      'H' => 'Handel',
      'L' => 'Lotteri',
      'T' => 'Tid og sted',
      'A' => 'Kultur & Underholdning',
      'O' => 'Pressemeddelelser',
      'X' => 'Citat',
    );
  }

  public function configDefaults() {
    $categories = $this->getCategories();
    $config = array();

    foreach ($categories as $key => $name) {
      $config['priority_' . $key] = 6;
    }
    $config['destination'] = 'ritzau_archive';
    return $config;
  }

  /**
   * Return configuration form for this object. The keys of the configuration
   * form must match the keys of the array returned by configDefaults().
   *
   * @return
   *   FormAPI style form definition.
   */
  public function configForm(&$form_state) {
    $categories = $this->getCategories();
    $form       = array();
    $options    = range(0, 6);
    $options[0] .= ' - ' . t('None');
    $options[1] .= ' - ' . t('Only top priority');
    $options[6] .= ' - ' . t('All');
    foreach ($categories as $key => $name) {
      $form['priority_' . $key] = array(
        '#type' => 'select',
        '#title' => t('Priority: !category', array('!category' => $name)),
        '#multiple' => FALSE,
        '#description' => t('Select the minimal priority should be imported for this category.'),
        '#options' => $options,
        '#default_value' => $this->config['priority_' . $key],
      );
    }
    $desc = t('Name of the folder in private:// where files should be put after import.');
    $form['destination'] = array(
      '#type' => 'textfield',
      '#title' => t('Destination folder'),
      '#default_value' => $this->config['destination'],
      '#description' => $desc,
    );
    return $form;
  }

  private function getTaxonomyTid($ritzau_id) {
    if (is_array($ritzau_id)) {
      $tids = array();
      foreach ($ritzau_id as $value) {
        $tids[] = $this->getTaxonomyTidFromRitzauId($value);
      }
      return $tids;
    }
    else {
      return $this->getTaxonomyTidFromRitzauId($ritzau_id);
    }
  }

  private function getTaxonomyTidFromRitzauId($ritzau_id) {
    return db_query("SELECT tid FROM {ritzau_alternative_keys} r WHERE r.ritzau_id = :ritzau_id", array(':ritzau_id' => $ritzau_id))->fetchField();
  }


  /**
   * Return mapping sources.
   *
   * At a future point, we could expose data type information here,
   * storage systems like Data module could use this information to store
   * parsed data automatically in fields with a correct field type.
   */
  public function getMappingSources() {
    return array(
      'headline' => array(
        'name' => t('Headline'),
        'description' => t('Title of the wire'),
      ),
      'body' => array(
        'name' => t('Body'),
        'description' => t('Body'),
      ),
      'ritzau_id' => array(
        'name' => t('Ritzau ID'),
        'description' => t('Unique ID for the imported content.'),
      ),
      'refering_ritzau_id' => array(
        'name' => t('Refering Ritzau ID'),
        'description' => t('Refering Unique ID for the imported content.'),
      ),
      'city' => array(
        'name' => t('City'),
        'description' => t('City'),
      ),
      'source' => array(
        'name' => t('Source'),
        'description' => t('Source'),
      ),
      'category' => array(
        'name' => t('Category'),
        'description' => t('Category'),
      ),
      'category_tid' => array(
        'name' => t('Category tid'),
        'description' => t('Category tid'),
      ),
      'genre' => array(
        'name' => t('Genre'),
        'description' => t('Genre'),
      ),
      'genre_tid' => array(
        'name' => t('Genre tid'),
        'description' => t('Genre tid'),
      ),
      'iptc_subjects_string' => array(
        'name' => t('IPTC Subjects string'),
        'description' => t('IPTC Subjects string'),
      ),
      'iptc_subjects_array' => array(
        'name' => t('IPTC Subjects array'),
        'description' => t('IPTC Subjects array'),
      ),
      'iptc_subjects_tids' => array(
        'name' => t('IPTC Subjects tids'),
        'description' => t('IPTC Subjects tids'),
      ),
      'caption' => array(
        'name' => t('Caption'),
        'description' => t('Caption'),
      ),
      'urgency' => array(
        'name' => t('Urgency'),
        'description' => t('Urgency'),
      ),
      'status' => array(
        'name' => t('Status'),
        'description' => t('Status'),
      ),
      'published' => array(
        'name' => t('Published'),
        'description' => t('Published'),
      ),
      'log' => array(
        'name' => t('Log'),
        'description' => t('Log message'),
      ),
      'reference' => array(
        'name' => t('Reference'),
        'description' => t('Reference'),
      ),
      'sent_date' => array(
        'name' => t('Sent date'),
        'description' => t('Sent date'),
      ),
      'sent_time' => array(
        'name' => t('Sent time'),
        'description' => t('Sent time'),
      ),
      'sent_datetime' => array(
        'name' => t('Sent date & time'),
        'description' => t('Sent date & time'),
      ),
      'update_datetime' => array(
        'name' => t('Update date & time'),
        'description' => t('Update date & time'),
      ),
      'release_date' => array(
        'name' => t('Release date'),
        'description' => t('Release date'),
      ),
      'release_time' => array(
        'name' => t('Release time'),
        'description' => t('Release time'),
      ),
      'release_datetime' => array(
        'name' => t('Release date & time'),
        'description' => t('Release date & time'),
      ),
      'xmlfile' => array(
        'name' => t('Ritzau xml file'),
        'description' => t('Ritzau xml file'),
      ),
    ) + parent::getMappingSources();
  }
}

