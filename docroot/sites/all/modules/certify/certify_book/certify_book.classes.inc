<?php

/**
 * @file 
 * certify_book.classes.inc php file
 * Classes for the Drupal module Certify Book.
 */

class CertifyBook extends CertifyCondition {

  public $showpages; // !! Whether to list pages on output
  public $pages; // Array of pages
  public $numpages; // Number of pages that has to be read

  public $pagesread;
  public $unreadpages;

  /**
   * Return form elements for node form
   */
  public static function certify_form($node) {

    // Collect books from the book-module
    $optionsb = array();
    $books = book_get_books(); // !TODO organic groups filtering

    foreach ($books as $bookid => $book) {
      $optionsb[$bookid] = $book['title'];
    }

    // Books
    $form['selected_books'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Books to be part of this certificate'),
      '#options' => $optionsb,
      '#default_value' => Certify::getConditionsNidsFromNode($node, 'book'),
    );
    return $form;
  }

  /**
   * Process node form submit
   */
  public static function certify_insert($node) {
    foreach ($node->selected_books as $bookkey => $book) {
      if ($bookkey == $book) {
        $booknode = node_load($book);
        // @TODO Should probably be done using an API

        db_insert('certify_conditions')->fields(array(
          'cid' => $node->nid,
          'vid' => $node->vid,
          'condtype' => 'book',
          'condnode' => $book
        ))->execute();
      }
    }
  }

  /**
   * Handle deletion of certificate
   */
  public function certify_delete($node) {
    // @TODO purge booklog entries ONLY if this node is the last referencing the selected book id
  }

  /**
   * Render condition for view
   */
  public function certify_view($node) {
    global $user;
    
    $this->fillProgress($user->uid);
    return theme('certify_book_view', array('node' => $node, 'condition' => $this));
  }

  /**
   * Header string for output
   */
  public static function getViewHeader() {
    return t("Books");
  }

  /**
   * Calculate progress data for this condition
   */
  public function fillProgress($uid = 0) {

    $this->loadConditionNode();

    $bpages = book_toc($this->mynode->book['bid'], 999);
    $bpres = db_query('SELECT b.mlid, b.nid, n.title FROM {book} b INNER JOIN {node} n ON b.bid = :bid AND n.status = 1 AND n.nid = b.nid', array(':bid' => $this->mynode->book['bid']));
    $pages = array();
    foreach ($bpres as $page) {
      $pages[$page->nid] = array(
        'title' => check_plain($page->title),
        'mlid' => $page->mlid,
        'nid' => $page->nid,
        'bid' => $this->mynode->book['bid'],
        'read' => FALSE,
      );
    }
    $this->pages = $pages;
    $this->numpages = count($pages) - 1; // Excluding front page

    // Book calculations (assumes front page is shown)

    $this->pagesread = 0;
    $logres = db_query('SELECT nid FROM {certify_book_viewlog} WHERE uid = :uid AND bid = :bid', array(':uid' => $uid, ':bid' => $this->mynode->book['bid']));
    foreach ($logres as $page) {
      if ($this->mynode->book['bid'] != $page->nid) { // Do not count front page
        $this->pages[$page->nid]['read'] = TRUE;
        $this->pagesread++;
      }
    }

    $this->unreadpages = $this->numpages - $this->pagesread;
    if ($this->numpages == 0) {
      $bookpct = 100; // All read
    }
    else {
      $bookpct = ( (float) $this->pagesread / (float) $this->numpages ) * 100;
    }
    if ( ( $this->unreadpages < 10 ) || ( $bookpct > 90 ) ) {
      $this->showpages = TRUE;
    }
    else {
      $this->showpages = FALSE;
    }

    if ($this->pagesread) {
      $this->started = TRUE;
    }
    else {
      $this->started = FALSE;
    }

    if ($this->unreadpages > 0) {
      $this->completed = FALSE;
    }
    else {
      $this->completed = TRUE;
    }

    $this->gotpoints = $this->pagesread;
    $this->gotpointscapped = $this->pagesread;
    $this->totalpoints = $this->numpages;
    $this->requiredpoints = $this->numpages;
  }
}
