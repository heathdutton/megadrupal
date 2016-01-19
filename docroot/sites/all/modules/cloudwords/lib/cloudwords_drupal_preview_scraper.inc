<?php

class CloudwordsPreviewScraper {
  protected $zip_file; 
  /**
   * 
   * returns filepath of archive for api call
   */
  public function __construct($projectName, $languageCode, $sourceObjectId, $pathObjectId, $path) { 

    global $base_url;
  
    $upload_dir = 'private://cloudwords/static_preview/';

    $languages = language_list();
    $language = isset($languages[$languageCode]) ? $languages[$languageCode] : null;
    
    //Set an access token unique to this object id
    $token = md5(rand());
    $tokens = variable_get('cloudwords_node_view_access_bypass', array());
    $tokens[$pathObjectId] = $token;

    variable_set('cloudwords_node_view_access_bypass', $tokens);
    
    $absolute_url = urldecode(url($path, array(
      'absolute' => TRUE,
      'alias' => TRUE,
      'language' => $language,
      'query' => array('cwAccessToken' => $token)
    )));

    $request = drupal_http_request($absolute_url, array(
        'headers' => array('User-Agent' => 'Drupal Cloudwords (+http://drupal.org/project/cloudwords)'),
        'method' => 'GET',
        'max_redirects' => 1,
        'timeout' => '300',
      )
    );

    //delete token after http request    
    unset($tokens[$pathObjectId]);
    variable_set('cloudwords_node_view_access_bypass', $tokens);
    
    $status = $request->code;
    
    if($status < 200 && $status <= 400 && isset($request->error)){
      watchdog('cloudwords', 'Unable to retreive static content to prepare in context preview.  Error response: %error', array('%error' => $request->error));      
      return;
    }    
    
    $status_message = $request->status_message;
    $data = $request->data;

    //SET PRIMARY DIRECTORY FOR STORAGE OF STATIC PAGE DIRECTORIES
    //project name - source - target - object id
  
    if (!file_prepare_directory($upload_dir, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
      form_set_error('reference', t('Unable to create the upload directory.'));
      watchdog('cloudwords', t('Unable to create the upload directory.'));
    }
  
    $static_dir_name = $projectName.'-'.$languageCode.'-'.$sourceObjectId;
    $drupal_static_path_full = $upload_dir.$static_dir_name;
    if (!file_prepare_directory($drupal_static_path_full, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
      form_set_error('reference', t('Unable to create the upload directory.'));
      watchdog('cloudwords', t('Unable to create the upload directory.'));    
    }
      
    //handle linked assets -- css and js
    $linked_assets = $this->generate_static_linked_files($data);
  
    foreach($linked_assets as $linked_asset){
  
      $linked_asset_url = urldecode(url($linked_asset, array('absolute' => TRUE, 'alias' => TRUE)));
      $linked_asset_request = drupal_http_request($linked_asset_url);

      $linked_asset_parts = parse_url($linked_asset);
      $linked_asset_relative_filepath = $linked_asset_parts['path'];
      
      $static_linked_asset_relative_path = $drupal_static_path_full.'/'.dirname($linked_asset_relative_filepath);
      $static_linked_asset_filename = basename($linked_asset_relative_filepath);
  
      if (!file_prepare_directory($static_linked_asset_relative_path, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
        form_set_error('reference', t('Unable to create the upload directory.'));
      }
      if(isset($linked_asset_request->data)){
        file_put_contents($static_linked_asset_relative_path.'/'.$static_linked_asset_filename, $linked_asset_request->data);    
      }
      //Replace references to css and js files with relative references
      $relative_path = dirname($linked_asset_relative_filepath).'/'.$static_linked_asset_filename;
      
      //replace all instances of string references to files in html
      $data = str_replace($linked_asset, ltrim($relative_path,'/'), $data);
    }
  
    // put html of page with rewritten links into index file in folder
    file_put_contents($drupal_static_path_full.'/index.html', $data);
    
    // zip up entire static page directory
    $archiver = new ZipArchive();
    $zip_file = drupal_realpath($drupal_static_path_full . '.zip');
    $destination = drupal_realpath($drupal_static_path_full);
    if ($archiver->open($zip_file, ZIPARCHIVE::OVERWRITE) !== TRUE) {
      return FALSE;
    }
   
    $this->list_valid_files_for_archive($destination, $valid_files);
  
    foreach ($valid_files as $file) {
      $archiver->addFromString(str_replace($destination . '/', '', $file), file_get_contents($file));
    }
    $archiver->close();
    
    //remove directory created for archive
    $this->remove_files_for_archive($destination);
    rmdir($destination);
    
    $this->zip_file = $zip_file;
  }
  
  public function list_valid_files_for_archive($destination, &$valid_files){
    if ($items = @scandir($destination)) {
      foreach ($items as $item) {
        if (is_file("$destination/$item") && strpos($item, '.') !== 0) {
          $valid_files[] = "$destination/$item";
        }else if(is_dir("$destination/$item") && strpos($item, '.') !== 0) {
          $this->list_valid_files_for_archive("$destination/$item", $valid_files);
        }
      }
    }  
  }
  
  public function remove_files_for_archive($destination){
    if ($items = @scandir($destination)) {
      foreach ($items as $item) {
        if (is_file("$destination/$item") && strpos($item, '.') !== 0) {
          unlink("$destination/$item");
        }else if(is_dir("$destination/$item") && strpos($item, '.') !== 0) {
          $this->remove_files_for_archive("$destination/$item");
          rmdir("$destination/$item");
        }
      }
    }  
  }
  
  /**
   * Find any linked files within the page and copy them over to the destination.
   */
  public function generate_static_linked_files($data) {
  
    $dom = new domDocument;
    libxml_use_internal_errors(true);
    $dom->loadHTML($data);
    $dom->preserveWhiteSpace = false;
  
    //get linked files from source text via regex
    $matches = $this->list_static_linked_files($data);
  
    //get all images referenced in html to copy to relative directory
    $images = $dom->getElementsByTagName('img');
    foreach ($images as $image) {
      $image_src = $image->getAttribute('src');
      if(strlen($image_src) > 0){
        $image_hrefs[] = $image_src;
      }
    }
    $matches = array_merge($matches, $image_hrefs);
  
    $files = array();
    $matches = array_unique($matches);
  
    libxml_clear_errors();
  
    return $matches;
  }
  
  
  /**
   * Find any linked files within the file by regex matches
   */
  public function list_static_linked_files($data) {
  
    // checking common files.
    $matches = array();
    // Match include("/anything").
    $includes = array();
    preg_match_all('/include\(["\'][\/\.]([^"\']*)["\']\)/', $data, $includes);
    if (isset($includes[1])) {
      $matches += $includes[1];
    }
    // Match url("/anything").
    // Regex based on drupal_build_css_cache().
    // - Finds css files for "@import url()".
    // - Finds files within css files - url(images/button.png).
    // - Excludes data based images.
    $imports = array();
    preg_match_all('/url\(\s*[\'"]?(?!(?:data)+:)([^\'")]+)[\'"]?\s*\)/i', $data, $imports);
    if (isset($imports[1])) {
      $matches = array_merge($matches, $imports[1]);
    }
    // Match src="/{anything}".
    //TODO - what about files stored on cdns?
    $srcs = array();
    preg_match_all('/src=["\'][\/\.]([^"\']*)["\']/i', $data, $srcs);
    if (isset($srcs[1])) {
      $matches = array_merge($matches, $srcs[1]);
    }
    // Match href="/{anything}.{ico|css|pdf|doc}(?{anything})" () querystring is
    // optional.
    $hrefs = array();
    preg_match_all('/href="[\/\.]([^"]*\.(ico|css|pdf|doc|js)(\?[^"]*)?)"/i', $data, $hrefs);
    if (isset($hrefs[1])) {
      $matches = array_merge($matches, $hrefs[1]);
    }
    // Match href='/{anything}.{ico|css|pdf|doc}(?{anything})' () querystring is
    // optional.
    $hrefs = array();
    preg_match_all("/href=\'[\/\.]([^']*\.(ico|css|pdf|doc|js)(\?[^']*)?)\'/i", $data, $hrefs);
    if (isset($hrefs[1])) {
      $matches = array_merge($matches, $hrefs[1]);
    }
  
    $files = array();
      
    $matches = array_unique($matches);
  
    //remove extensions  
    $exclude_extensions = array('html');
    foreach($matches as $k => $v){
      $url = parse_url($v);
      $path = pathinfo($url['path']);
      if(isset($path['extension']) && in_array($path['extension'], $exclude_extensions)){
        unset($matches[$k]);
      }
    }
  
    return $matches;
  }
  public function get_zip_file(){
    return $this->zip_file;
  }
}