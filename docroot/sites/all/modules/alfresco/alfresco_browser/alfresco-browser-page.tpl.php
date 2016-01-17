<?php
/**
 * @file alfresco-browser-page.tpl.php
 * Default theme implementation for Alfresco Browser page.
 *
 * @see template_preprocess_alfresco_browser_page()
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01//EN">
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
  <title><?php print t('Alfresco Browser for Drupal'); ?></title>
  <link rel="stylesheet" type="text/css" href="<?php print "$extjs_path/resources/css/ext-all.css"; ?>">
  <link rel="stylesheet" type="text/css" href="<?php print "$extjs_path/resources/css/xtheme-gray.css"; ?>">
  <link rel="stylesheet" type="text/css" href="<?php print "$module_path/css/browser.css"; ?>">
  <style type="text/css">
    #loading-mask {position:absolute;left:0;top:0;width:100%;height:100%;z-index:30000;background-color:white}
    #loading {position:absolute;left:45%;top:40%;padding:2px;z-index:30001;height:auto}
    #loading a {color:#258}
    #loading .loading-indicator {background:white;color:#444;font:bold 13px tahoma,arial,helvetica;padding:10px;margin:0;height:auto}
    #loading .loading-indicator img {float:left;margin-right:8px;vertical-align:top}
    #loading-msg {font:normal 10px arial,tahoma,sans-serif}
  </style>
  <?php print $scripts; ?>
  <script type="text/javascript" src="<?php print "$extjs_path/" . ALFRESCO_BROWSER_EXT_ADAPTER; ?>"></script>
  <script type="text/javascript" src="<?php print "$extjs_path/" . ALFRESCO_BROWSER_EXT_ALL; ?>"></script>
  <?php if ($extjs_locale_js): ?>
  <script type="text/javascript" src="<?php print "$extjs_locale_js"; ?>"></script>
  <?php endif; ?>
  <script type="text/javascript">
  <!--//--><![CDATA[//><!--
    Ext.BLANK_IMAGE_URL = '<?php print "$extjs_path/resources/images/default/s.gif"; ?>';
  //--><!]]>
  </script>
  <script type="text/javascript" src="<?php print "$module_path/js/FileUploadField.js"; ?>"></script>
  <script type="text/javascript" src="<?php print "$module_path/js/AlfrescoBrowser.js"; ?>"></script>
</head>
<body>
<div id="loading-mask"></div>
<div id="loading">
  <div class="loading-indicator">
    <?php print $loading_img; ?><?php print $loading_msg; ?>
    <br /><span id="loading-msg"><?php print t('Initializing...'); ?></span>
  </div>
</div>
<div id="header">
<h1><?php print $header; ?></h1>
<div id="search-box" class="x-normal-editor"><input type="text" id="search" /></div>
</div>
</body>
</html>
