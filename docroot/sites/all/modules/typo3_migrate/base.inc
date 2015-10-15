<?php

/**
 * @file
 * Defines the base class for all the migrations.
 */

/**
 *
 * All migration settings are defined here.
 *
 */
abstract class BaseMigration extends Migration {
    public $queryParams = array();

    public function __construct() {
      // Always call the parent constructor first for basic setup
      parent::__construct();
      $this->team = array(
        new MigrateTeamMember('Test', 'test@srijan.in', t('Product Owner')),
      );

    $this->issuePattern = 'http://drupal.org/node/:id';

    /* Pages specific conditions to be plaed here */
    // the tt_content ctype to migrate
    $this->queryParams['pages']['destination_type'] = 'typo3_page';
    $this->queryParams['pages']['cType'] = array('text', 'textpic', 'image');
    $this->queryParams['pages']['cType_operator'] = 'IN';
    // merge the bodytext of tt_content elements belonging to a page
    $this->queryParams['pages']['expBody'] = "GROUP_CONCAT(concat(IF(STRCMP(tt.header,''),CONCAT('<div class=\"header-layout-',tt.header_layout,'\">'),''),tt.header,IF(STRCMP(tt.header,''),'</div>',''),
      tt.bodytext,
      IF(STRCMP(tt.header,'') , concat('<img src=\"/sites/default/files/' , tt.image , '\"/><p>' , tt.imagecaption , '</p>') , '')
      ) ORDER BY tt.sorting ASC separator '<p style=\"display:none;\"></p>')";

    // doktype : 1|Standard Pages;2|Advanced;3|External Url;4|Shortcuts;5|Not in Menu;6|Back End user section;254|Sys folder
    $this->queryParams['pages']['dokType'] = array(1);
    $this->queryParams['pages']['dokType_operator'] = 'IN';
    $this->queryParams['pages']['hidden'] = array(0);
    $this->queryParams['pages']['hidden_operator'] = 'IN';
    $this->queryParams['pages']['teaser'] = 0;
    $this->queryParams['pages']['orderField'] = 'tstamp';
    $this->queryParams['pages']['orderField_order'] = 'ASC';
    $this->queryParams['pages']['path_perform_alias'] = '1';

     // News settings
    $this->queryParams['newsCat']['hidden'] = array(0);
    $this->queryParams['newsCat']['hidden_operator'] = 'IN';
    // Check if news folder setings have been set. By default all the news content is migrated.
    $pids = variable_get('typo3_pids', '0');
    if ($pids != '0') {
      $this->queryParams['news']['pid'] = explode(",", $pids);
      $this->queryParams['news']['pid_operator'] = 'IN';
    }
    else {
      $this->queryParams['news']['pid'] = 'ignore';
    }
    $this->queryParams['news']['fields'] = array('uid', 'crdate', 'tstamp', 'pid',  'title',  'hidden',  'short',
                       'bodytext',  'author',  'author_email', 'image', 'imagetitletext', 'imagealttext', 'imagecaption', 'links', 'ext_url', 'news_files');
    $this->queryParams['news']['orderField'] = 'tstamp';
    $this->queryParams['news']['orderField_order'] = 'ASC';
    $this->queryParams['news']['related_article_field'] = 'field_news_related_articles'; // enable later on
    $this->queryParams['news']['default_language'] = '';


    //BeUser settings
    $this->queryParams['beuser']['fields'] = array('uid', 'disable', 'username', 'admin', 'usergroup', 'email', 'crdate', 'realname', 'password');
    $this->queryParams['beuser']['order_field'] = 'tstamp';
    $this->queryParams['beuser']['order_field_operator'] = 'desc';
    $this->queryParams['beuser']['disable'] = array(0);
    $this->queryParams['beuser']['disable_operator'] = 'IN';

    // FeUser settings
    $this->queryParams['feuser']['fields'] = array('uid', 'pid', 'disable', 'tstamp', 'username', 'password', 'email', 'crdate');
    $this->queryParams['feuser']['order_field'] = 'tstamp';
    $this->queryParams['feuser']['order_field_operator'] = 'desc';
    $this->queryParams['feuser']['disable'] = array(0);
    $this->queryParams['feuser']['disable_operator'] = 'IN';
  }

  /**
   *
   * Handle the LINK tag breakage
   * @param String $word
   */
  public function typo3_handle_link_breakage($word) {
    $word = str_ireplace("<link ", " <link ", $word);
    $word = str_ireplace(">", "> ", $word);
    $wordArr = explode(" ", $word);
    for ($i=0;$i<count($wordArr);$i++) {
      if ($wordArr[$i] == "<link") {
        $this->remove_words_after_position($wordArr, $i, ">");
      }
    }
    $finalArr =  $this->handle_space_word($wordArr);
    $word = implode(" ", $finalArr);
    $word = str_ireplace("<link ", '<a href="', $word);
    $word = str_ireplace("</link>", '</a>', $word);
    $word = str_ireplace(' ">', '">', $word);
    return $word;
  }

  /**
   *
   * Spaces are left out considering they are not neccessary
   * @param Array $words
   */
  public function handle_space_word(&$words) {
    $output = array();
    for ($i=0;$i<count($words);$i++) {
      if ($words[$i] == '') {
        continue;
      }
      $output[] = $words[$i];
    }
    return $output;
  }

  /**
   *
   * This will remove unwanted data from the word after the link tag is found
   * @param Array $words
   * @param Integer $pos
   * @param string $find
   */
  public function remove_words_after_position(&$words, $pos, $find=">") {
    $fWord = $words[$pos+1];
    $fWordChanged = str_replace(">", "", $fWord);
    if ($fWord != $fWordChanged) {
      $fWordChanged = ltrim(rtrim($fWordChanged));
      $words[$pos+1] = $fWordChanged . '">';
      $wordChanged = str_ireplace("@", "", $fWordChanged . '">');
      if ($wordChanged != $words[$pos+1] ) {
        $words[$pos+1] = "mailto:" . $words[$pos+1];
      }
      else{
        if (intval($words[$pos+1])) {
          $words[$pos+1] = "/nid/" . $words[$pos+1];
        }
        else{
          $words[$pos+1] = "/" . $words[$pos+1];
          $words[$pos+1] = str_ireplace("/http", "http", $words[$pos+1]);
        }
      }
      return '';
    }
    else{
      if (intval($words[$pos+1])) {    // if link is integer
          $words[$pos+1] = "/nid/" . $words[$pos+1];
        }
        else{                          // if there is @ present
          $wordChanged = str_ireplace("@", "", $words[$pos+1]);
          if ($wordChanged != $words[$pos+1] ) {
            $words[$pos+1] = "mailto:" . $words[$pos+1];
          }
          else{
            $words[$pos+1] = "/" . $words[$pos+1];
            $words[$pos+1] = str_ireplace("/http", "http", $words[$pos+1]);
          }
        }
    }

    $startPos = $pos + 2;
    for ($j=$startPos;$j<count($words);$j++) {
      $str = trim($words[$j]);
      $strChanged = str_replace($find, "", $str);
      if ($str == $strChanged) {
        $words[$j] = '';
        continue;
      }
      else{
        $words[$j] = '">';
        return '';
      }
    }
  }

  /**
   *
   * Handle the missing mailto where A tag is used.
   * It is left in the above Script, so tackling this separately
   * @param String $word
   */
  public function typo3_handle_missing_mailto($word) {
    $wordArr = explode('"', $word);
    for ($wordCtr=0;$wordCtr<count($wordArr);$wordCtr++) {
        $str = $wordArr[$wordCtr];
        $strChanged = str_ireplace("@", "", $str);
        if ($str != $strChanged) {
          $strChanged1 = str_ireplace("mailto:", "", $str);
          if ($str == $strChanged1) {
            $wordArr[$wordCtr] = "mailto:" . $wordArr[$wordCtr];
          }
        }
    }
    $word = implode('"', $wordArr);
    return $word;
  }

}
