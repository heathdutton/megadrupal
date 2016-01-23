<?php

/**
 * @file
 * Controller class definition file for newsletter_newsletter entity.
 */

/**
 * Newsletter Newsletter Controller
 */
class NewsletterNewsletterController extends DrupalDefaultEntityController {

  public function __construct($entityType) {
    parent::__construct('newsletter_newsletter');
  }

  /**
   *
   */
  public function create($title, $ntid, $custom = FALSE) {
    $newsletter = new stdClass();

    $newsletter->title = $title;
    $newsletter->ntid = $ntid;
    $newsletter->custom = $custom ? 1 : 0;
    $newsletter->send_id = 0;
    $this->saveNew($newsletter);

    return $newsletter;
  }

  /**
   * Saves a new custom newsletter to database.
   *
   * @param $newsletter
   *   A full newsletter object.
   */
  protected function saveNew($newsletter) {
    $max_send_id = db_query('SELECT MAX(send_id) as send_id
      FROM {newsletter_newsletter}
      WHERE title = :title AND ntid = :ntid',
      array(':title' => $newsletter->title, ':ntid' => $newsletter->ntid))
      ->fetchField();
    if ($max_send_id != '') {
      $newsletter->send_id = ++$max_send_id;
    }
    drupal_write_record('newsletter_newsletter', $newsletter);
    module_invoke_all('entity_insert', $newsletter, 'newsletter_newsletter');
  }

  /**
   * Saves a newsletter to database.
   *
   * @param $newsletter
   *   A full newsletter object.
   */
  public function save($newsletter) {
    if (empty($newsletter->nnid)) {
      return isset($newsletter->title) && isset($newsletter->ntid) ? $this->saveNew($newsletter) : FALSE;
    }
    drupal_write_record('newsletter_newsletter', $newsletter, 'nnid');
    field_attach_update('newsletter_newsletter', $newsletter);
    module_invoke_all('entity_update', $newsletter, 'newsletter_newsletter');
  }

  /**
   *
   */
  public function saveDraft($subscribers, $draft) {
    $drafts = variable_get('newsletter_custom_drafts', array());
    $drafts[$draft->nnid] = array(
      'subscribers' => $subscribers,
      'ntid' => $draft->ntid,
    );
    variable_set('newsletter_custom_drafts', $drafts);
  }

  public function delete($nnid) {

    $newsletter = newsletter_newsletter_load($nnid);
    $deleted = db_delete('newsletter_newsletter')
      ->condition('nnid', $nnid)
      ->execute();

    module_invoke_all('entity_delete', $newsletter, 'newsletter_newsletter');

    // It's necessary to clear the caches when an entity is deleted.
    cache_clear_all();
    $this->resetCache();
    return $deleted ? TRUE : FALSE;
  }

  public function deleteDraft($nnid) {
    $drafts = variable_get('newsletter_custom_drafts', array());
    if (!empty($drafts[$nnid])) {
      unset($drafts[$nnid]);
      variable_set('newsletter_custom_drafts', $drafts);
      // Also delete from database.
      return $this->delete($nnid);
    }
  }
}
