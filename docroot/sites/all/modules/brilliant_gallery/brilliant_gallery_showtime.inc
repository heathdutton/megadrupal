<?php

function render_brilliant_gallery($paramarray = array()) {

  $thisfolder = '';
  if (isset($paramarray[1])) {
    $thisfolder = $paramarray[1];
  }

  $colcountoverride = @$paramarray[2];

  $widthoverride = @$paramarray[3];
  #drupal_set_message('was set '.$widthoverride);

  $sortoverride = @$paramarray[4];

  $maximumnumbertoshow = @$paramarray[5];
  #if (arg(1)==264544){
  #watchdog('Brilliant Gal', 'aftera: '.vacilando_echo_array($paramarray));
  #watchdog('Brilliant Gal', 'afterb: '.$maximumnumbertoshow);
  #}

  $colouroverride = @$paramarray[6];

  $beginfromoverride = @$paramarray[7];
  if ($beginfromoverride == '') {
    $beginfromoverride = 1;
  }

  $captionyesnotext = @$paramarray[8];

  // Test whether 'thumbsquared' is set.
  @$paramarray[10] = strtolower(@$paramarray[10]);
  if ($paramarray[10] == 'yes' or $paramarray[10] == 'no') {
    $imagecrop = $paramarray[10];
    #drupal_set_message('was set '.$imagecrop);
  }
  else {
    $imagecrop = variable_get('brilliant_gallery_crop', 'no');
    #drupal_set_message('was NOT set '.$imagecrop);
  }

  if (@$paramarray[12] == '') {
    $paramarray[12] = 'no';
  }
  $bgcachexpire = brilliant_gallery_get_days_in_seconds(variable_get('brilliant_gallery_cache_duration', 90));

  $thumbalignincell = strtolower(@$paramarray[15]);
  if ($thumbalignincell <> 'center' and $thumbalignincell <> 'left' and $thumbalignincell <> 'right') {
    $thumbalignincell = 'center';
  }

  // Is this a Google+ Photos album page?
  // E.g. https://plus.google.com/photos/104031880678382567653/albums/5591422169328270129
  // E.g. https://plus.google.com/photos/104031880678382567653/albums/5414337846751631057?authkey=CLa-o8OXyqC-kAE
  // E.g. https://plus.google.com/u/0/photos/104031880678382567653/albums/5641519066949458241?authkey=CMiq-vWZvIvaaA
  $picasawebsignature = 'http://plus.google.com/';
  $picasawebsignaturesecure = 'https://plus.google.com/';
  if (substr($thisfolder, 0, strlen($picasawebsignature)) == $picasawebsignature OR substr($thisfolder, 0, strlen($picasawebsignaturesecure)) == $picasawebsignaturesecure) {
    // Get the authkey, if any, from the URL query
    $urlquery = parse_url($thisfolder,PHP_URL_QUERY);
    parse_str($urlquery, $parsedquery);
    $authkey = '';
    if (@$parsedquery['authkey'] <> ''){
      $authkey = 'Gv1sRg'.$parsedquery['authkey'];
      //dpm($authkey);
    }
    // Get the IDs from URL path
    $urlpath = parse_url($thisfolder,PHP_URL_PATH);
    $urlpath = explode('/',$urlpath);
    //if ($urlpath[1] == 'photos' AND $urlpath[3] == 'albums' AND is_numeric($urlpath[2]) AND is_numeric($urlpath[4])){
      $picasauserid = '';
      $picasaalbumid = '';
      foreach($urlpath as $key=>$val){
        if (is_numeric($val) AND strlen($val) > 10 AND $picasauserid == ''){
          $picasauserid = $val;
        }
        else if (is_numeric($val) AND strlen($val) > 10 AND $picasauserid <> ''){
          $picasaalbumid = $val;
        }
      }
      // OK we've got all needed to construct a picasaweb feed URL
      $thisfolder = 'https://picasaweb.google.com/data/feed/base/user/'.$picasauserid.'/albumid/'.$picasaalbumid.'?alt=rss&kind=photo';
      if ($authkey <> ''){
        $thisfolder .= '&authkey='.$authkey;
      }
      //dpm('>>> '.$thisfolder);
    //}
  }
  // Is this a Picasa gallery?
  $picasafolder = false;
  $picasawebsignature = 'http://picasaweb.google.';
  $picasawebsignaturesecure = 'https://picasaweb.google.';
  if (substr($thisfolder, 0, strlen($picasawebsignature)) == $picasawebsignature OR substr($thisfolder, 0, strlen($picasawebsignaturesecure)) == $picasawebsignaturesecure) {
    // Must work for all variants - http://picasaweb.google.com, http://picasaweb.google.co.uk, etc.
    // Replace '&amp;' (happens if tags are edited in rich text editors) by '&'.
    $thisfolder = str_replace("&amp;", "&", $thisfolder);
    $picasafolder = true;
  }

  // See if one or more particular images were asked to be shown.
  $thumbshowbyname = array();
  $tmp = explode(',', @$paramarray[13]);
  if (is_array($tmp)) {
    foreach ($tmp as $val) {
      $val = trim($val);
      if ($val == '') {
        continue;
      }
      $thumbshowbyname[] = $val;
      // Had an idea to update the column count to match the number of accepted images - but that does not work in case some images don't have a match anyway. So did not do it and users have to set the column override to whatever number they want. Too much intelligence in the gadget is bad anyways :-)
    }
  }
  #watchdog('Brilliant Gal', 's1: '.implode(' ... ',$thumbshowbyname));
  #drupal_add_css(drupal_get_path('module', 'brilliant_gallery') .'/brilliant_gallery.css');
  #$result = '</p>';
  $result = '';
  $galleryfolder = variable_get('brilliant_gallery_folder', '');
  /*
    if (substr($galleryfolder, strlen($galleryfolder) - 1, 1) == '/' or substr($galleryfolder, 0, 1) == '/') {
    #watchdog('error', 'failed to notify "weblogs.com" (site)');
    #form_set_error('yemail', t('Header injection attempt detected.  Do not enter line feed characters into the from field!'));
    $bgmsg = 'Main gallery folder path must not begin or end with a slash; please fix it at /admin/settings/brilliant_gallery';
    drupal_set_message($bgmsg);
    watchdog('Brilliant Gal',$bgmsg);
    return;
    }
   */
  /*
    if ($thisfolder <> '') {
    $galleryfolder .= '/'. $thisfolder;
    }
   */
  if ($thisfolder <> '') {
    if ($galleryfolder <> '') {
      #$galleryfolder .= '/' . $thisfolder;
      $galleryfolder .= ( substr($thisfolder, 0, 1) == '/' ? '' : '/') . $thisfolder; // See http://drupal.org/node/176939#comment-1494648
    }
    else {
      $galleryfolder = $thisfolder;
    }
  }
  if ($colcountoverride == '') {
    $columns = variable_get('brilliant_gallery_maxcol', 3);
  }
  else {
    $columns = $colcountoverride;
  }
  if ($widthoverride == '') {
    $imagewidth = variable_get('brilliant_gallery_maximagewidth', 150);
  }
  else {
    $imagewidth = $widthoverride;
  }
  if ($sortoverride == '' or strtolower($sortoverride) == 'sort') {
    $brilliant_gallery_sort = variable_get('brilliant_gallery_sort', '1');
  }
  else {
    $brilliant_gallery_sort = $sortoverride;
  }

  if ($colouroverride == '') {
    $bgcolour = variable_get('brilliant_gallery_bcgcolour_textfield', '#000000');
  }
  else {
    $bgcolour = $colouroverride;
  }
  #if ($captionyesornot == 'yes' or $captionyesornot == '' or (variable_get('brilliant_gallery_caption', '') <> '' and $captionyesornot <> 'no')) {
  if (($captionyesnotext == 'yes' or $captionyesnotext == '') and variable_get('brilliant_gallery_caption', '') <> '') {
    $showcaption = 'filename';
  }
  #else if ($captionyesornot == 'no') {
  else if ($captionyesnotext == 'no' or (variable_get('brilliant_gallery_caption', '') == '' and ($captionyesnotext == 'yes' or $captionyesnotext == 'no'))) {
    $showcaption = '';
  }
  else {
    #$showcaption = $captionyesornot;
    $showcaption = $captionyesnotext;
  }
  $padding = variable_get('brilliant_gallery_padding', 3);
  $overbrowser = variable_get('brilliant_gallery_overbrowser', 'colorbox');
  // Totally full resolution display would be impractical, so this is the maximum width of "full" resolution.
  $fullresolutionmaxwidth = variable_get('brilliant_gallery_maxwidth', '1000');
  $path = url(FILE_DIRECTORY_PATH . '/' . $galleryfolder, array('absolute' => TRUE));
  // url() ads i18n codes to the URL ... we need to remove them here...
  if (BG_LANGCODE <> '') {
    $path = str_replace('/' . BG_LANGCODE . '/', '/', $path);
  }

  // Non-clean URLs need removing ?q=
  $path = str_replace("?q=", "", $path);

  if ($paramarray[12] == 'yes' and $picasafolder) {
    // Display a Picasa slide show - but only if this is Picasa data and not a local gallery.
    $slideshowurl = str_replace('/base/', '/api/', $thisfolder); // The only difference between RSS and slideshow feeds, apparently.
    $result .= '<embed type="application/x-shockwave-flash" src="https://picasaweb.google.com/s/c/bin/slideshow.swf" width="' . $widthoverride . '" height="' . ($widthoverride * 0.75) . '" flashvars="host=picasaweb.google.com&hl=en_US&feat=flashalbum&interval=20&RGB=0x000000&feed=' . urlencode($slideshowurl) . '" pluginspage="http://www.macromedia.com/go/getflashplayer"></embed>';
  }
  else { // If this is a gallery and not a slideshow.
    // Get absolute path
    if ($picasafolder) {
      $url_to_fetch = $thisfolder;
      #echo '.--..'.$url_to_fetch;
      $mgalleryurl = md5($url_to_fetch);
      $pcachetemp = variable_get('brilliant_gallery_pcache', BRILLIANT_GALLERY_DEFAULT_CACHE_DIR);
      //brilliant_gallery_check_or_create_dir($pcachetemp);
      if ($pcachetemp == '' or $pcachetemp == file_directory_temp()) {
        // If there is no cache directory in the files folder, then we need to use the default temp dir
        $pcachetemp = file_directory_temp();
        $beg_realpcachetemp = file_directory_temp();
        $slashpcachetemp = '';
      }
      else {
        $slashpcachetemp = '/' . $pcachetemp;
        $beg_realpcachetemp = realpath(FILE_DIRECTORY_PATH) . $slashpcachetemp;
      }
      #watchdog('Brilliant Gal','sakr: '.$beg_realpcachetemp);
      #$mkdirek = $pcachetemp . '/bg_picasa_'. $mgalleryurl;
      $mkdirek = $beg_realpcachetemp . '/bg_picasa_orig_' . $mgalleryurl;
      // We go and look at the images every time we render BG, because it sometimes happens that Picasa does not provide them all at the time of the initial fetching. So we will fetch any missing ones, and also replace the expired ones.
      // Vacilando 20091016: It is ABSOLUTELY CRUCIAL that this uses include_ONCE, otherwise picasa.inc was called twice in some cases.
      //include_once ('./'. drupal_get_path('module', 'brilliant_gallery') ."/picasa.inc");
      #$url_to_fetch = "http://picasaweb.google.com/data/feed/base/user/tjfulopp/albumid/5288246472868572145?alt=rss&kind=photo&hl=en_US";
      if ($url_to_fetch == '') {
        return;
      }

      // Create a special directory in temp dir for this Picasa album, then save the images there.
      #$mgalleryurl = md5($url_to_fetch);
      #$mkdirek = '/tmp/bg_picasa_'. $mgalleryurl;
      //exec('mkdir '.$mkdirek);
      #watchdog('Brilliant Gal', mt_rand(1000,9999) . ' ' . arg(1).' pinc '.microtime());
      /*
        ob_start();
        debug_print_backtrace();
        $btrace = ob_get_contents();
        ob_end_clean();
       */
      #watchdog('Brilliant Gal', mt_rand(1000,9999) . ' ' . arg(1).' pinc '.$btrace);

      @mkdir($mkdirek); // See http://drupal.org/node/368583#comment-1243641
      // Get it over http, not https
      $url_to_fetch = str_replace('https://', 'http://', $url_to_fetch);

      // Get the Picasa gallery feed XML file.
      $cacheid_url_to_fetch = 'bg_gallery_picasa_xml_' . md5($url_to_fetch);
      if ($cache = cache_get($cacheid_url_to_fetch) and !empty($cache->data)) {
        $html = $cache->data;
        /*
          global $user; $userId = $user->uid;
          if ($userId == 1){
          drupal_set_message('got it from cache '.$url_to_fetch);
          }
         */
      }
      else {
        // Do your expensive calculations here...
        $timenow = REQUEST_TIME;
        $htmlobj = drupal_http_request($url_to_fetch);
        $html = $htmlobj->data; // Get just the data from the returned object.
        cache_set($cacheid_url_to_fetch, $html, 'cache', brilliant_gallery_get_time_this_many_seconds_from_now($bgcachexpire));
        #drupal_set_message('...2'.$html);
        watchdog('Brilliant Gal', 'Fetched XML from ' . $url_to_fetch . ' (' . strlen($html) . ' bytes). Elapsed time: ' . (REQUEST_TIME - $timenow) . ' seconds.');
        /*
          global $user; $userId = $user->uid;
          if ($userId == 1){
          drupal_set_message('again fetched '.$url_to_fetch);
          }
         */
      }

      # if ($http_result->code == 200) {
      #$headers    = $hc->getHeaders();
      #$header        = $hc->getHeader();
      #$inf        = $hc->getInfo();
      // Links to full images are in this format: <enclosure type='image/jpeg' url='http://lh4.ggpht.com/_HkgjhlSfbZA/SWOgKhb6LyI/AAAAAAAABZ4/5AEu8AF01sw/DSC03173.JPG' length='0'/>
      $imgchunks = explode("' url='", $html);
      $fetchingnow = 0;
      $timenow = REQUEST_TIME;
      foreach ($imgchunks as $imgchunk) {

        // Make sure the script execution does not expire in case many images need to be fetched from remote server.
        drupal_set_time_limit(360);

        // Get the file name
        $chunk2 = explode("' length='", $imgchunk);
        $imgurl = trim($chunk2[0]);
        if (substr($imgurl, 0, 4) <> 'http') {
          continue;
        }
        $imagename = substr($imgurl, strrpos($imgurl, '/') + 1);
        #echo $imagename . '<br>';
        $foqen = $mkdirek . '/' . $imagename;

        $lastchanged = @filemtime($foqen); // Last file modification time, or FALSE on error.
        /*
          global $user; $userId = $user->uid;
          if ($userId == 1){
          drupal_set_message('pic '.($timenow - $lastchanged).' > '.$bgcachexpire.' ... '.$foqen);
          }
         */
        #if ($_GET['fetchnow']==1){ $lastchanged = FALSE; } // Debugging!
        if ($lastchanged === FALSE OR ($timenow - $lastchanged > $bgcachexpire)) {
          // If the image is expired, we need to actively delete it, for the case that it was removed / hidden by the owner.
          @unlink($foqen);
          /*
            global $user; $userId = $user->uid;
            if ($userId == 1){
            drupal_set_message('in!');
            }
           */
          ///*
          $imager = drupal_http_request($imgurl);
          $image = $imager->data;
          //*/
          #watchdog('bg', $imager->code.' ... '.$imgurl.' =imgurl');
          // It happens that the file size is 0 (image not fetched). In such case, don't write it.
          if (strlen($image) > 0) {
            $fp = fopen($foqen, 'w');
            #drupal_set_message($foqen.'<br>');
            watchdog('bg', $foqen.' ...ok<br>');
            fwrite($fp, $image);
            fclose($fp);
            $fetchingnow++;
          }
        }
      }

      if ($fetchingnow <> 0) {
        watchdog('Brilliant Gal', 'Fetched ' . $fetchingnow . ' images from ' . $url_to_fetch . '. Elapsed time: ' . (REQUEST_TIME - $timenow) . ' seconds.');
      }

      $absolpath = $slashpcachetemp . '/bg_picasa_orig_' . $mgalleryurl;
      #echo $absolpath;
      #drupal_set_message(t('absol: '.$absolpath));
    }
    else {
      $absolpath = '/' . $galleryfolder;
    }
    #$result .= $absolpath;
    #watchdog('Brilliant Gal','absol: '.$absolpath);
    # Make an array with images in this folder, and their properties.
    #$temp              = load_dir_to_array($absolpath, $imagewidth, $fullresolutionmaxwidth, $brilliant_gallery_sort);
    $temp = load_dir_to_array($absolpath, $imagewidth, $fullresolutionmaxwidth, $brilliant_gallery_sort, $imagecrop, $maximumnumbertoshow);
    $retval_dimensions = $temp[0];
    $imagemaxh = $temp[1];
    $maxpoct = count($retval_dimensions);
    #if (arg(2) == 'edit') {
    #print_r( $retval_dimensions );
    #}

    if (!$picasafolder) {
      # Get a list of images that are forbidden from display.
      #$uid=$user->uid;
      #echo $user->uid;
      $excludedimgs = array();
      // any checkbox id that starts with user- we remember the current user's settings
      // any other id is global and we use user=0
      /*
        $result=db_query("select qid,state from {brilliant_gallery_checklist} ".
        " where nid='$nid' and qid not like 'user-%' and user=0 ".
        " union ".
        "select qid,state from {brilliant_gallery_checklist} ".
        "where nid='$nid' and qid like 'user-%' and user='$uid' ");
       */
      $dbresult = db_query("select qid from {brilliant_gallery_checklist} where qid like 'user-%' and state='1' ");
      foreach ($dbresult as $data) {
        $excludedimgs[] = $data->qid;
      }
      #print_r($excludedimgs);
    }

    // If we need to show just 1 image, then it is useless to calculate the UL width. Use width of the image.
    if ($maximumnumbertoshow == 1) {
      $columns = 0;
    }

    // If $columns is 0, columns will fill up the available space.
    if ($columns <> 0) {
      // Total 'width' controls the number of thumbnails in each row. The number, in pixels, should be calculated thusly: [maximum width desired for thumbnails] + [padding - if any, x 2] + [margin - if any, x 2] + [border - if any, x 2] x [number of thumbnails desired in each row]
      $style_ul = "font-size: 0px; margin: 0px; padding: 0px; width: " . (($imagewidth + 2 * $padding) * $columns) . "px;";
    }
    else {
      $style_ul = "font-size: 0; margin: 0px; padding: 0px;";
    }
    $column_count = 0;
    #$pocetobr = 0;
    //$setname = mt_rand(1, 9999999);
    $setname = md5($absolpath);
    #$result .= '<!-- Brilliant Gallery Table Beginning --><table border="0" rules="none" frame="box" cellpadding="0" cellspacing="0" bgcolor="' . $bgcolour . '"';
    #$result .= '<!-- Brilliant Gallery Table Beginning -->';
    #$result .= '<ul class="brilliant_gallery" style="' . $style_ul . '">';
    $result .= '<ul style="' . $style_ul . '">';
    #if ( $thisfolder <> '' ) {
    #$result .= ' align="center"';
    #   }
    #$result .= ' style="width:' . $columns*$imagewidth . 'px;">'; #cell-spacing: 5px;
    #$result .= ' style="width:' . $columns*$imagewidth . 'px;"';
    #$result .= '>' . "\n"; #cell-spacing: 5px;
    $maxpoct_show = '';
    if ($maximumnumbertoshow <> '' and is_numeric($maximumnumbertoshow)) {
      $maxpoct_show = $maximumnumbertoshow;
    }
    $retval = array();
    $cnt = 0;
    for ($poct = 1; $poct <= $maxpoct; $poct++) {

      if (!$picasafolder) {
        # Skip any images that were excluded from display.
        # md5() must be made of the main gallery path etc. ending with the file name (unique identifier). E.g. albums/2008/20080321-25_belgicko_zasypane_snehom/dsc02784_w1000.jpg
        if (array_search('user-' . md5($galleryfolder . '/' . $retval_dimensions[$poct - 1]['file']), $excludedimgs) !== false) {
          continue;
        }
      }

      // If we are to display only images specified in $thumbshowbyname, skip the others here:
      if (!empty($thumbshowbyname)) {
        #watchdog('Brilliant Gal', 's2: '.$retval_dimensions[$poct-1]['file'].' // '.implode('---',$thumbshowbyname));
        if (array_search($retval_dimensions[$poct - 1]['file'], $thumbshowbyname) === FALSE) {
          continue;
        }
        #watchdog('Brilliant Gal', '->sb: '.$retval_dimensions[$poct-1]['file'].' // '.implode('---',$thumbshowbyname));
      }

      // Begin only from $beginfromoverride image.
      if ($poct < $beginfromoverride) {
        continue;
      }

      $cnt += 1;
      if ($cnt > $maxpoct_show and $maxpoct_show <> '') {
        // Do not show more than $maxpoct_show (if defined).
        continue;
      }
      $retval[$poct - 1] = $retval_dimensions[$poct - 1]['file'];
      $fullimgpath = $path . '/' . $retval[$poct - 1];
      #if ($column_count == 0) { $result .= ('<tr>'); }
      #$result .= ('<td align="center" bgcolor="' . $bgcolour . '" style="padding: ' . $padding . 'px ' . $padding . 'px ' . $padding . 'px ' . $padding . 'px;vertical-align: middle;">');
      #$result .= ('<td align="center" bgcolor="' . $bgcolour . '" style="padding: ' . $padding . 'px ' . $padding . 'px ' . $padding . 'px ' . $padding . 'px;vertical-align: middle;">' . "\n");
      #$pocetobr += 1;
      if (brilliant_gallery_testext($retval[$poct - 1])) {
        $caption = str_replace(array('.', '_'), ' ', basename($retval[$poct - 1], strrchr($retval[$poct - 1], '.')));
        #$smallenough = false;
        $imgw = $retval_dimensions[$poct - 1]['imgw'];
        $imgh = $retval_dimensions[$poct - 1]['imgh'];
        $imgwbig = $retval_dimensions[$poct - 1]['imgwbig'];
        $imghbig = $retval_dimensions[$poct - 1]['imghbig'];
        $imgcrop = $retval_dimensions[$poct - 1]['imgcrop'];
        #@$smallenough = $retval_dimensions[$poct - 1]['smallenough'];
        #$style_li = "float: left; list-style: none; background: #000; width: 44px; height: 33px; padding: 4px; text-align: center; margin: 0; border: none;";
        // If we need to show just 1 image, then it is useless to use the maximum image height for the LI. Use height of the image. Analogous for the width.
        // Cropping has no influence on single images.
        $imagemaxh_here = $imagemaxh;
        $imagewidth_here = $imagewidth;
        if ($maximumnumbertoshow == 1) {
          $imagemaxh_here = $imgh;
          $imagewidth_here = $imgw;
        }

        $style_li = "font-size: 0px; float: left; width: " . $imagewidth_here . "px; list-style: none; background: " . $bgcolour . "; height: " . $imagemaxh_here . "px; padding: " . $padding . "px; text-align: " . $thumbalignincell . "; margin: 0px; border: none;";
        $result .= ( '<li style="' . $style_li . '">' . "\n");
        # Get this module's path:
        //$modulepath = url(drupal_get_path('module', 'brilliant_gallery'), array('absolute' => TRUE));
        $modulepath = drupal_get_path('module', 'brilliant_gallery');
        # url() ads i18n codes to the URL ... we need to remove them here...
        if (BG_LANGCODE <> '') {
          $modulepath = str_replace('/' . BG_LANGCODE . '/', '/', $modulepath);
        }
        # Non-clean URLs need removing ?q=
        $modulepath = str_replace("?q=", "", $modulepath);
        #if ($smallenough === true) {
        #  $result .= '<a href="'. $fullimgpath .'"';
        #}
        #else {
        # Important to begin with the "/" otherwise thumbnails in non-root folders fail. See http://drupal.org/node/175292
        #$result .= '<a href="' . $modulepath .'/image.php?imgp=' . base64_encode( $absolpath . '/' . $retval[$poct-1] ) . '&amp;imgw=' . $imgwbig . '&amp;imgh=' . $imghbig . '"'; #&dummy=.jpg
        #&dummy=.jpg
        //$imgsrc = file_create_url($modulepath .'/image.php?imgp='. base64_encode($absolpath .'/'. $retval[$poct - 1]) .'&amp;imgw='. $imgwbig .'&amp;imgh='. $imghbig);
        //drupal_set_message($absolpath .'/'. $retval[$poct - 1]);
        //drupal_set_message('imgsrc: '.$imgsrc);
        //echo $imgw; flush();exit();
        $bgimgproperties_array = array(
          // This array contains everything that makes the image unique.
          'imgp' => $absolpath . '/' . $retval[$poct - 1],
          //'imgp' => $retval[$poct - 1],
          'imgw' => $imgwbig,
          'imgh' => $imghbig,
          'imgcrop' => 'no', # Not $imgcrop because zoomed images do never need to be cropped (only thumbnails)!
        );
        //$bgimgproperties_base64 = base64_encode(serialize($bgimgproperties_array));
        $bgimgproperties_array_token = brilliant_gallery_image_properties_set($bgimgproperties_array);
        //$bgimgproperties_hash = brilliant_gallery_image_properties_set($bgimgproperties_array);
        //drupal_set_message('bgimgproperties_hash: '.$bgimgproperties_hash);
        //drupal_set_message(file_create_url('brilliant_gallery/image/').$bgimgproperties_hash);
        //$imgsrc = file_create_url('brilliant_gallery/image/').$bgimgproperties_hash.'.bgimg';
        //$imgsrc = file_create_url('brilliant_gallery/image/').$bgimgproperties_base64.'.'.brilliant_gallery_get_extension($retval[$poct - 1]);
        $pcache = variable_get('brilliant_gallery_pcache', BRILLIANT_GALLERY_DEFAULT_CACHE_DIR);
        //brilliant_gallery_check_or_create_dir($pcache);
        $imgsrc = file_create_url(FILE_DIRECTORY_PATH . '/' . $pcache . '/bg_cached_resized_' . $bgimgproperties_array_token . '.' . brilliant_gallery_get_extension($retval[$poct - 1]));
        //$imgsrc = file_create_url(substr(file_directory_path(),1) .'/'. variable_get('brilliant_gallery_pcache', BRILLIANT_GALLERY_DEFAULT_CACHE_DIR) .'/bg_cached_resized_'.$bgimgproperties_base64.'.'.brilliant_gallery_get_extension($retval[$poct - 1]));
        $result .= '<a href="' . $imgsrc . '"';
        //$result .= '<a href="'.$modulepath .'/image.php?imgp='. base64_encode($absolpath .'/'. $retval[$poct - 1]) .'&imgw='. $imgwbig .'&imgh='. $imghbig.'"';
        #}

        switch ($overbrowser) {

          case 'colorbox':
            $result .= ' class="colorbox" rel="bg-' . $setname . '"';
            break;

          case 'lightbox':
            $result .= ' rel="lightbox[' . $setname . ']"';
            break;

          default:
            break;

        }

        if ($showcaption <> '') {
          if ($showcaption <> 'filename') {
            $caption = $showcaption;
          }
          $result .= ' title="' . $caption . '"';
        }
        $result .= '>';
        # Important to begin with the "/" otherwise thumbnails in non-root folders fail. See http://drupal.org/node/175292
        /*
          $modulepath = url(drupal_get_path('module', 'brilliant_gallery'), array('absolute' => TRUE));
          # url() ads i18n codes to the URL ... we need to remove them here...
          if ( $langcode <> '' ) {
          $modulepath = str_replace( '/' . $langcode . '/', '/', $modulepath );
          }
          # Non-clean URLs need removing ?q=
          $modulepath = str_replace( "?q=", "",  $modulepath );
         */

        #$result .= '<img style="display: block;border:0" src="' . $modulepath .'/image.php?imgp=' . base64_encode( $absolpath . '/' . $retval[$poct-1] ) . '&imgw=' . $imgw . '&imgh=' . $imgh . '" />'; # width="' . $imgw . '"
        #$result .= '<img style="border: 0; margin:0px; padding:0px;" alt="" src="' . $modulepath .'/image.php?imgp=' . base64_encode( $absolpath . '/' . $retval[$poct-1] ) . '&amp;imgw=' . $imgw . '&amp;imgh=' . $imgh . '" />'; # width="' . $imgw . '"
        # width="' . $imgw . '"
        #$result .= '<img style="border: 0; margin:0px; padding:0px;" alt="" src="'. $modulepath .'/image.php?imgp='. base64_encode($absolpath .'/'. $retval[$poct - 1]) .'&imgw='. $imgw .'&imgh='. $imgh .'" />';
        /*
          $imgsrc = file_create_url($modulepath .'/image.php?imgp='. base64_encode($absolpath .'/'. $retval[$poct - 1]) .'&amp;imgw='. $imgw .'&amp;tst=3&amp;imgh='. $imgh . '&amp;imgcrop='. $imgcrop);
          $result .= '<img style="border: 0; margin:0px; padding:0px;" alt="" src="'.$imgsrc.'" />';
         */
        $bgimgproperties_array = array(
          // This array contains everything that makes the image unique.
          'imgp' => $absolpath . '/' . $retval[$poct - 1],
          //'imgp' => $retval[$poct - 1],
          'imgw' => $imgw,
          'imgh' => $imgh,
          'imgcrop' => $imgcrop,
        );
        //$bgimgproperties_base64 = base64_encode(serialize($bgimgproperties_array));
        $bgimgproperties_array_token = brilliant_gallery_image_properties_set($bgimgproperties_array);
        //$bgimgproperties_hash = brilliant_gallery_image_properties_set($bgimgproperties_array);
        //drupal_set_message('bgimgproperties_hash: '.$bgimgproperties_hash);
        //drupal_set_message(file_create_url('brilliant_gallery/image/').$bgimgproperties_hash);
        //$imgsrc = file_create_url('brilliant_gallery/image/').$bgimgproperties_hash.'.bgimg';
        //$imgsrc = file_create_url('brilliant_gallery/image/').$bgimgproperties_base64.'.'.brilliant_gallery_get_extension($retval[$poct - 1]);
        $pcache = variable_get('brilliant_gallery_pcache', BRILLIANT_GALLERY_DEFAULT_CACHE_DIR);
        //brilliant_gallery_check_or_create_dir($pcache);
        $imgsrc = file_create_url(FILE_DIRECTORY_PATH . '/' . $pcache . '/bg_cached_resized_' . $bgimgproperties_array_token . '.' . brilliant_gallery_get_extension($retval[$poct - 1]));
        $secur = empty($_SERVER["HTTPS"]) ? '' : ($_SERVER["HTTPS"] == "on") ? "s" : "";
        if ($secur == 's') {
          $imgsrc = str_replace('http://', 'https://', $imgsrc);
        }
        $result .= '<img style="border: 0px; margin: 0px; padding: 0px;" alt="" src="' . $imgsrc . '" />';
        #$result .= '<br>.................................'.$imgsrc;
        #$result .= '</a>';
        $result .= '</a>' . "\n";
      }
      else {
        $fosiz = ceil($imagewidth / 13);
        $style_li = "font-size:" . $fosiz . "px; float: left; width: " . $imagewidth . "px; list-style: none; background: " . $bgcolour . "; height: " . $imagemaxh . "px; padding: " . $padding . "px; text-align: " . $thumbalignincell . "; margin: 0px; border: none;";
        $result .= ( '<li style="' . $style_li . '">' . "\n");
        $result .= '<a href="' . $fullimgpath . '">';
        #$result .= '<center>' . $retval[$poct-1] . '</center>';
        $result .= $retval[$poct - 1];
        #brokenimage("Error loading PNG");
        #$result .= '</a>';
        $result .= '</a>' . "\n";
      }
      #$result .= '</td>';
      $result .= '</li>' . "\n";

      $column_count += 1;
      #if ($column_count == $columns) { $result .= ("</tr>\n"); $column_count = 0; }
    }
    #if ($column_count <> 0) { $result .= ("</tr>"); }
    #if ($column_count <> 0) { $result .= ("</tr>\n"); }
    #$result .= "</table>\n";
    $result .= "</ul>\n";
    $result .= "<div style=\"clear: both;\"></div>\n"; // Add a new line (needed because of the float). See http://stackoverflow.com/questions/2580772/how-do-i-get-a-new-line-after-using-floatleft
    #$result .= '<br style="clear: both;" />';
    #$result .= '<p>';
  }

  return $result;
}
