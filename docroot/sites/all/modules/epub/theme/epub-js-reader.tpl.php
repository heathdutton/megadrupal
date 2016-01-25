<!DOCTYPE html>
<html class="no-js" xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $GLOBALS['language']->language; ?>" xml:lang="<?php print $GLOBALS['language']->language; ?>">
  <head>
    <title><?php //print t('File viewer'); ?></title>
        <meta charset="utf-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
        <title></title>
        <meta name="description" content="">
        <meta name="viewport" content="width=device-width, user-scalable=no">
        <meta name="apple-mobile-web-app-capable" content="yes">

    <?php
      $epubjs = $GLOBALS['base_url'] . '/' . libraries_get_path('epub.js');
      $filename = $variables['file']->filename;
      $file_url = file_create_url($variables['file']->uri);
      $path = file_create_url('public://epub_content/' . $variables['file']->fid);
      if (file_entity_access('download', $variables['file'])) {
        $download_link = t('Download: ') . l($filename, $file_url);
      }
      if ($variables['unzipped']) {
        $epub = $path . '/';
      }
      else {
        $epub = $file_url;
      }
      if (module_exists('admin_menu')) {
        admin_menu_suppress();
      }
      print drupal_get_js('header');
    ?>
    <link rel="stylesheet" href="<?php print $epubjs?>/reader/css/normalize.css">
    <link rel="stylesheet" href="<?php print $epubjs?>/reader/css/main.css">
    <link rel="stylesheet" href="<?php print $epubjs?>/reader/css/popup.css">
    <script src="<?php print $epubjs?>/libs/jquery/jquery-2.1.0.min.js"></script>
    <script src="<?php print $epubjs?>/build/libs/zip.min.js"></script>
    <script src="<?php print $epubjs?>/build/epub.min.js"></script>
    <script src="<?php print $epubjs?>/build/hooks.min.js"></script>
    <script src="<?php print $epubjs?>/build/reader.js"></script>
    <script src="<?php print $epubjs?>/build/libs/screenfull.min.js"></script>
    <script>
        "use strict";
        document.onreadystatechange = function () {
          if (document.readyState == "complete") {
            EPUBJS.filePath = "<?php print $epubjs?>/build/libs/";
            EPUBJS.cssPath = "<?php print $epubjs?>/demo/css/";
            // fileStorage.filePath = EPUBJS.filePath;
            var reader = ePubReader('<?php print $epub; ?>', {sidebarReflow: false});
          }
        };
    </script>
  </head>
    <body>
      <div id="sidebar">
        <div id="panels">
          <!--input id="searchBox" placeholder="search" type="search">
          <a id="show-Search" class="show_view icon-search" data-view="Search">Search</a-->
          <a id="show-Toc" class="show_view icon-list-1 active" data-view="Toc">TOC</a>
          <a id="show-Bookmarks" class="show_view icon-bookmark" data-view="Bookmarks">Bookmarks</a>
          <!--a id="show-Notes" class="show_view icon-edit" data-view="Notes">Notes</a-->
        </div>
        <div id="tocView" class="view">
        </div>
        <div id="searchView" class="view">
          <ul id="searchResults"></ul>
        </div>
        <div id="bookmarksView" class="view">
          <ul id="bookmarks"></ul>
        </div>
      </div>
      <div id="main">
        <div id="titlebar">
          <div id="opener">
            <a id="slider" class="icon-menu">Menu</a>
          </div>
          <div id="metainfo">
            <span id="book-title"></span>
            <span id="title-seperator">&nbsp;&nbsp;–&nbsp;&nbsp;</span>
            <span id="chapter-title"></span>
          </div>
          <div id="title-controls">
            <a id="bookmark" class="icon-bookmark-empty">Bookmark</a>
            <a id="setting" class="icon-cog">Settings</a>
            <a id="fullscreen" class="icon-resize-full">Fullscreen</a>
          </div>
        </div>
        <div id="divider"></div>
        <div id="prev" class="arrow">‹</div>
        <div id="viewer"></div>
        <div id="next" class="arrow">›</div>
        <div id="loader"><img src="<?php print $epubjs?>/reader/img/loader.gif"></div>
      </div>
      <div class="modal md-effect-1" id="settings-modal">
          <div class="md-content">
              <h3>Settings</h3>
              <div>
                  <p>
                    <input type="checkbox" id="sidebarReflow" name="sidebarReflow">Reflow text when sidebars are open.</input>
                  </p>
              </div>
              <div class="closer icon-cancel-circled"></div>
          </div>
      </div>
      <div class="overlay"></div>
      <?php print drupal_get_js('footer'); ?>
    </body>
</html>
