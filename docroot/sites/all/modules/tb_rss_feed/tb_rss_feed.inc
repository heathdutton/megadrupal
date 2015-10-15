<?php

module_load_include('inc', 'tb_rss_feed', 'generator/field_generator');
module_load_include('inc', 'tb_rss_feed', 'generator/node_generator');

class TBRSSFeed {
  protected $configs;

  public function setConfig($cfg) {
    $this->configs = $cfg;
  }

  public function getConfig() {
    return $this->configs;
  }

  public function crawlPage($url) {
    if (function_exists('curl_init')) {
      $curl = curl_init();
      curl_setopt($curl, CURLOPT_URL, $url);
      curl_setopt($curl, CURLOPT_HEADER, 0);
      curl_setopt($curl, CURLOPT_TIMEOUT, 600);
      curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
      curl_setopt($curl, CURLOPT_USERAGENT, 'Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.7.5) Gecko/20041107 Firefox/1.0');
      $content = curl_exec($curl);
      curl_close($curl);
    }
    else {
      $content = @file_get_contents($url);
    }
    return $content;
  }

  public function feed() {    
    set_time_limit(0);
    $results = $this->getContent();
    $nodes = array();
    if (!is_array($results))
      return $nodes;
    foreach ($results as $row) {
      $existed = db_select('tb_rss_feed_contents', 'c')
        ->fields("c")
        ->condition('feed_id', $row['feed_id'])
        ->condition('account', $row['account'])
        ->execute()
        ->fetchObject();
      if ($existed) {
        if ($row['changed'] > $existed->changed) {
          $node = tb_rss_feed_generate_node($row, $existed->node_id);
          $row_contents = array(
            'feed_id' => $row['feed_id'],
            'data' => serialize($row),
            'account' => $row['account'],
            'created' => $row['created'],
            'changed' => $row['changed'],
            'url' => $row['url'],
            'reference_url' => $row['reference_url'],
            'latest_feed' => time(),
            'node_id' => $node->nid,
          );
          db_update('tb_rss_feed_contents')
            ->fields($row_contents)
            ->condition('id', $existed->id)
            ->execute();
          $nodes[] = $node;
        }
      }
      else {        
        $node = tb_rss_feed_generate_node($row);
        $row_contents = array(
          'feed_id' => $row['feed_id'],
          'data' => serialize($row),
          'account' => $row['account'],
          'created' => $row['created'],
          'changed' => $row['changed'],
          'url' => $row['url'],
          'reference_url' => $row['reference_url'],
          'latest_feed' => time(),
          'node_id' => $node->nid,
        );
        db_insert('tb_rss_feed_contents')->fields($row_contents)->execute();
        $nodes[] = $node;
      }
    }
    return $nodes;
  }

  function createForm(&$form) {
    $form = array();

    $form["tb_rss_feed_auto_feed"] = array(
      '#type' => 'checkbox',
      '#title' => t("Auto feed"),
      '#value' => variable_get("tb_rss_feed_auto_feed"),
      '#description' => 'Automatically feed items every configured time of the drupal cron. Click <a href="' . base_path() . '/admin/config/system/cron">here</a> to configure cron ',
    );
    $form["tb_rss_feed_limit"] = array(
      '#type' => 'textfield',
      '#title' => t("Feed Limit"),
      '#value' => variable_get("tb_rss_feed_limit"),
      '#description' => t('Please enter a valid number, the number of feed items returned for each feed request.Enter 0 if you want to get the maximum feed items.'),
    );    
    $accounts = db_select('tb_rss_feed_accounts', 'a')
        ->fields('a')
        ->condition('deleted', 0)
        ->orderBy('weight')
        ->execute()->fetchAll();
    $accounts_form = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#title' => t("RSS Sources"),
    );
    $n = count($accounts) ? count($accounts) : 1;
    $sub_form = array();
    for ($i = 0; $i < $n; $i++) {
      $account = isset($accounts[$i]) ? $accounts[$i] : new stdClass();
      $account->form_order = $i;
      $account->account_name = isset($account->account_name) ? $account->account_name : "";
      $account->account_term = isset($account->account_term) ? $account->account_term : "";
      $this->createAccountForm($account, $sub_form);
      if ($sub_form) {
        $accounts_form["tb_rss_feed_account_wrapper_$i"] = $sub_form;
      }
    }

    if ($sub_form) {
      $settings_form["tb_rss_feed_accounts_wrapper"] = $accounts_form;
      $settings_form["tb_rss_feed_accounts_info"] = array('#type' => 'hidden', '#value' => "");
      $settings_form["tb_rss_feed_add_account_wrapper"] = array(
        '#type' => 'container',
        '#attributes' => array('class' => array('form-wrapper', 'tb-feed-add-account-wrapper')),
      );
      $settings_form["tb_rss_feed_add_account_wrapper"]['add-account-button'] = array(
        '#markup' => '<div class="add-account-button-wrapper"><a href="javascript:void(0)" class="btn btn-add-account" onclick="Drupal.TBRSSFeed.addAccount()">Add Account</a></div>',
      );
      $account = new stdClass();
      $account->form_order = 0;
      $account->account_name = "";
      $account->account_term = "";
      $this->createAccountForm($account, $sub_form);
      $settings_form["tb_rss_feed_add_account_wrapper"]["tb_rss_feed_account_wrapper"] = $sub_form;
    }
    $form['tb_rss_feed_settings_wrapper'] = $settings_form;
  }

  public function rssToArray($tags, $array, $url) {
    $doc = new DOMdocument();
    @$doc->load($url);
    $rss_array = array();
    foreach ($tags as $tag) {
      if ($doc->getElementsByTagName($tag)) {
        foreach ($doc->getElementsByTagName($tag) AS $node) {
          $items = array();
          foreach ($array AS $key => $values) {
            $items[$key] = array();
            foreach ($values as $value) {
              if ($itemsCheck = $node->getElementsByTagName($value)) {
                for ($j = 0; $j < $itemsCheck->length; $j++) {
                  if (($attribute = $itemsCheck->item($j)->nodeValue) != "") {
                    $items[$key][] = $attribute;
                  }
                  elseif ($attribute = $itemsCheck->item($j)->getAttribute('term')) {
                    $items[$key][] = $attribute;
                  }
                  elseif ($itemsCheck->item($j)->getAttribute('rel') == 'alternate') {
                    $items[$key][] = $itemsCheck->item($j)->getAttribute('href');
                  }
                }
              }
            }
          }
          array_push($rss_array, $items);
        }
      }
    }
    return $rss_array;
  }

  /**
   * Return feed limit based on number of feeds & user-defined feed limit
   * @param $items array list of items
   * @return $limit number 
   */
  public function getFeedLimit($items) {
    $length = count($items);
    $limit = $this->configs['limit'];
    if ($limit == 0 || $limit > $length)
      $limit = $length;
    return $limit;
  }

  function getFeedType() {
    return "RSSFeed";
  }

  public function getBundle() {
    return 'tb_rss_feed';
  }
  function __construct($cfg = NULL) {
    $this->configs = array(
      'account' => $cfg ? $cfg['account'] : '',
      'account_term' => $cfg ? $cfg['account_term'] : '',
      'limit' => $cfg['feed_limit']
    );
  }

  function getType() {
    return "RSS";
  }

  private function getImageSize($img){
    $result = array(0,0);
    if(!file_exists($img)){
      return $result;
    }    
    $result = getimagesize($img);
    return $result;
  }
  
  private function getBigestImage($list){
    $biggestImg = $list[0];
    foreach($list as $img){
      list($widthImg, $heightImg) = $this->getImageSize($img);
      list($widthBiggestImg, $heightBiggestImg) = $this->getImageSize($biggestImg);
      if($widthBiggestImg < $widthImg) $biggestImg = $img;
    }
    return $biggestImg;
  }
  
  public function getContent() {
    $url = $this->configs['account'];
    $rss_item_tags = array('item', 'entry', 'feed');
    $rss_tags = array(
      'title' => array('title'),
      'description' => array('description', 'content', 'summary'),
      'image' => array('url'),
      'link' => array('link'),
      'category' => array('category'),
    );
    $content = $this->rssToArray($rss_item_tags, $rss_tags, $url);
    $results = array();
    $limit = $this->getFeedLimit($content);
    for ($i = $limit-1; $i >= 0; $i--) {
      $row = $content[$i];
      $result = array();
      $result['title'] = $row['title'][0];
      $result['body'] = $row['description'][0];
      $pattern = '/<img[^>]+src\s*=\s*"([^"]+)"/i';
      preg_match_all($pattern, $row['description'][0], $matches);
      if (isset($matches[1][0])) {
        $result['field_image'] = $this->getBigestImage($matches[1]);
      }
      $result['field_rss_feed_terms'] = $this->configs['account_term'];
      $result['node_type'] = $this->getBundle();
      $result['feed_id'] = $result['field_reference_url'] = $result['reference_url'] = $row['link'][0];
      $result['created'] = $result['changed'] = 0;
      $result['url'] = $url;
      $result['account'] = $this->configs['account'];
      $results[] = $result;
    }
    return $results;
  }

  function createAccountForm($account, &$form) {
    $form = array(
      '#type' => 'fieldset',
      '#title' => t("Source")
    );
    $form['tb_rss_feed_account_name[]'] = array(
      '#type' => 'hidden',
      '#size' => 8,
      '#title' => t("Account Name"),
      '#default_value' => isset($account->account_name) ? $account->account_name : ""
    );
    $form['tb_rss_feed_account[]'] = array(
      '#type' => 'textfield',
      '#size' => 8,
      '#title' => t("RSS Url:"),
      '#default_value' => isset($account->account) ? $account->account : "",
      '#description' => t("Please enter a valid RSS url. Ex: http://engadget.com/rss.xml"),
      '#attributes' => array('class' => array('account')),
    );
    $form['tb_rss_feed_current_account[]'] = array(
      '#type' => 'hidden',
      '#value' => isset($account->account) ? $account->account : ""
    );
    $form['tb_rss_feed_account_term[]'] = array(
      '#type' => 'textfield',
      '#size' => 8,
      '#title' => t("Term:"),
      '#description' => t("Term to manage feed from the above source."),
      '#default_value' => isset($account->account_term) ? $account->account_term : ""
    );
    $form['tb_rss_feed_account_verify'] = array(
      '#markup' => '<a href="javascript:void(0)" class="form-submit btn-verify-account" onclick="Drupal.TBRSSFeed.verifyAccount(\'\', this)">' . t('Check feed') . '</a><br/>',
    );
    $form['tb_rss_feed_account_delete'] = array(
      '#markup' => '<a href="javascript:void(0)" class="btn btn-delete-account" onclick="Drupal.TBRSSFeed.deleteAccount(this)">' . t('Delete') . '</a>',
    );
  }
}
