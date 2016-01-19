<?php

class VGWort_Message {
  public $parties ;
  public $messagetext ;
  public $webranges ;
  public $privateidentificationid ;

  public function __construct($id) {
    $this->privateidentificationid = $id ;
    $this->webranges->webrange = array();
    $this->parties->authors = array();
    $this->messagetext->text = new stdClass();
    $this->messagetext->lyric = FALSE ;
  }

  public function addAuthor($firstName, $surName, $number = FALSE) {
    $author = new stdClass() ;
    $author->firstName = truncate_utf8($firstName, 39) ;
    $author->surName = $lastName ;
    if ( empty($lastName) ) {
      $author->surName = $firstName ;
    }
    if ( FALSE !== $number ) {
      $author->cardNumber = $number ;
    }
    $this->parties->authors[] = $author ;
  }

  public function addWebrange($url, $surl = FALSE) {
    $range = new stdClass();
    if ( $surl !== FALSE ) {
      $range->url = array($url, $surl) ;
    }
    else {
      $range->url = array($url) ;
    }
    $this->webranges->webrange[0]->url[] = $url ;
  }

  public function setShortText($text) {
    $this->messagetext->shorttext = $text ;
  }

  public function setPlainText($text) {
    $this->messagetext->text->plainText = $text ;
  }

  public function setLyric($bool) {
    $this->messagetext->lyric = $bool ;
  }
}
