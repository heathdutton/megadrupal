<?php

/**
 * @file
 * Extends Simplenews by providing a visitor log by user.
 *
 * Date: 15-nov-2013
 * File: simplenews_visitor_preg_callback_wrap.inc
 * Author: Callegari Stefano
 */

/**
 * Changes the original url to the site target and sets the GET the newsletter
 * data.
 *
 * SC 19/mag/2011 09.44.52 stefano
 */
class SimplenewsVisitorPregCallbackWrap {
  /*
   * SC Thu Oct 10 16:57:35 CEST 2013 stefano
   *
   * Created a class to replace preg_replace (/e deprecated) with
   * preg_replace_callback.
   */

  /**
   * the first part regex
   *
   * SC Thu Oct 10 19:11:15 CEST 2013 stefano
   */
  protected $preUrl;

  /**
   * url regex
   *
   * SC Thu Oct 10 19:11:54 CEST 2013 stefano
   */
  protected $url;

  /**
   * the last part regex
   *
   * SC Thu Oct 10 19:12:20 CEST 2013 stefano
   */
  protected $endUrl;

  /**
   * the link regex
   *
   * SC Thu Oct 10 19:12:38 CEST 2013 stefano
   */
  protected $link;

  /**
   * the end link regex
   *
   * SC Thu Oct 10 19:13:05 CEST 2013 stefano
   */
  protected $endLink;

  /**
   * User email.
   *
   * SC Thu Oct 10 19:14:20 CEST 2013 stefano
   */
  protected $email;

  /**
   * Id node/newsletter.
   *
   * SC Thu Oct 10 19:14:38 CEST 2013 stefano
   */
  protected $nid;

  /**
   * The key url.
   *
   * SC Thu Oct 24 12:01:16 CEST 2013 stefano
   */
  protected $keyUrl;

  /**
   * Constructor.
   *
   * Take the email and the node from the newsletter.
   *
   * SC Thu Oct 10 18:15:45 CEST 2013 stefano
   *
   * @param string $to
   *   the user email
   * @param int $node_id
   *   the node id of the newsletter
   */
  public function __construct($to, $node_id) {
    $this->email = $to;
    $this->nid = $node_id;

    $this->keyUrl = $this->NewUrlKey();
  }

  /**
   * For link into html newsletter.
   *
   * SC Thu Oct 10 17:26:49 CEST 2013 stefano
   *
   * @param array $matches
   *   values matched from preg
   *
   * @return string
   *   the new url format to replaced into preg
   */
  public function NewUrl($matches) {
    $this->preUrl = $matches[1];
    $this->url = $matches[2];
    $this->endUrl = $matches[3];
    $this->link = $matches[4];
    $this->endLink = $matches[5];

    /* SC Tue Oct 22 19:54:58 CEST 2013 stefano
     *
     * Added a key to validate the URL.
     */
    return $this->preUrl . base_path() . "?q=redirect/" . $this->nid . "/" . rawurlencode($this->email) . "/" . base64_encode($this->url) . "/" . rawurlencode($this->link) . "/" . $this->keyUrl . $this->endUrl . $this->link . $this->endLink;
  }

  /**
   * For link into plain newsletter.
   *
   * SC Thu Oct 10 17:26:49 CEST 2013 stefano
   *
   * @param array $matches
   *   values matched from preg
   *
   * @return string
   *   the new url format to replaced into preg
   */
  public function NewUrlPlain($matches) {
    $this->link = $matches[1];
    $this->preUrl = $matches[2];
    $this->url = $matches[3];

    /* SC Tue Oct 22 19:54:58 CEST 2013 stefano
     *
     * Added a key to validate the URL.
     */
    return $this->link . $this->preUrl . base_path() . "?q=redirect/" . $this->nid . "/" . rawurlencode($this->email) . "/" . base64_encode($this->url) . "/" . rawurlencode($this->link) . "/" . $this->keyUrl;
  }

  /**
   * Generates a key to verify the url.
   *
   * SC Tue Oct 22 19:58:26 CEST 2013 stefano
   *
   * @return string
   *   the key
   */
  protected function NewUrlKey() {

    /* SC Tue Oct 22 19:27:38 CEST 2013 stefano
     *
     * Checks if has set the key.
     */
    $key = variable_get('simplenews_visitor_urlkey');

    if (empty($key)) {
      watchdog('simplenews_visitor', 'No url key defined.');
      drupal_set_message(t("You haven't set the url key."), 'warning');

      /* SC Tue Oct 22 19:41:29 CESTV 2013 stefano
       *
       * Redirects to configure page.
       */
      drupal_goto('admin/config/services/simplenews/visitor');
    }

    return md5($this->email . $this->nid . $key);
  }

  /**
   * Return the key for the URL.
   *
   * SC Thu Oct 24 12:22:12 CEST 2013 stefano
   *
   * @return string
   *   the url key
   */
  public function GetNewUrlKey() {
    return $this->keyUrl;
  }

}
