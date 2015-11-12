<?php

/**
 * @file
 * Default theme implementation to display the basic html structure of a single
 * Drupal page.
 *
 * Variables:
 * - $css: An array of CSS files for the current page.
 * - $language: (object) The language the site is being displayed in.
 *   $language->language contains its textual representation.
 *   $language->dir contains the language direction. It will either be 'ltr' or
 *   'rtl'.
 * - $rdf_namespaces: All the RDF namespace prefixes used in the HTML document.
 * - $grddl_profile: A GRDDL profile allowing agents to extract the RDF data.
 * - $head_title: A modified version of the page title, for use in the TITLE
 *   tag.
 * - $head_title_array: (array) An associative array containing the string parts
 *   that were used to generate the $head_title variable, already prepared to be
 *   output as TITLE tag. The key/value pairs may contain one or more of the
 *   following, depending on conditions:
 *   - title: The title of the current page, if any.
 *   - name: The name of the site.
 *   - slogan: The slogan of the site, if any, and if there is no title.
 * - $head: Markup for the HEAD section (including meta tags, keyword tags, and
 *   so on).
 * - $styles: Style tags necessary to import all CSS files for the page.
 * - $scripts: Script tags necessary to load the JavaScript files and settings
 *   for the page.
 * - $page_top: Initial markup from any modules that have altered the
 *   page. This variable should always be output first, before all other dynamic
 *   content.
 * - $page: The rendered page content.
 * - $page_bottom: Final closing markup from any modules that have altered the
 *   page. This variable should always be output last, after all other dynamic
 *   content.
 * - $classes String of classes that can be used to style contextually through
 *   CSS.
 *
 * @see template_preprocess()
 * @see template_preprocess_html()
 * @see template_process()
 */
?>
<!DOCTYPE html>
<!--[if lt IE 7]> <html version="HTML+RDFa 1.1" lang="<?php print $language->language; ?>" class="no-js ie6 "<?php print $rdf_namespaces; ?>> <![endif]-->
<!--[if IE 7]>    <html version="HTML+RDFa 1.1" lang="<?php print $language->language; ?>" class="no-js ie7 "<?php print $rdf_namespaces; ?>> <![endif]-->
<!--[if IE 8]>    <html version="HTML+RDFa 1.1" lang="<?php print $language->language; ?>" class="no-js ie8 "<?php print $rdf_namespaces; ?>> <![endif]-->
<!--[if gt IE 8]><!--> <html version="HTML+RDFa 1.1" lang="<?php print $language->language; ?>" class="no-js"<?php print $rdf_namespaces; ?>> <!--<![endif]-->

<head>
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <?php print $scripts; ?>

</head>
<body class="<?php print $classes; ?>" <?php print $attributes;?>>

  <?php if (theme_get_setting('cms_theme_browser_ie6')): ?>
  <!-- update ie6 message -->
  <!--[if IE 6]>
      <style type="text/css">
        #update-ie6                       { position:absolute; z-index:999999; top:expression(eval(document.compatMode && document.compatMode=='CSS1Compat') ? documentElement.scrollTop : document.body.scrollTop); background:url(<?php echo base_path() . path_to_theme(); ?>/assets/images/ie6/bg.jpg) repeat-x top;}
        #update-ie6 a.lk-closeie6         { display:inline-block; position:absolute; top:10px; right:10px; padding:3px 5px; text-decoration:none; color:#fff; background:#999;}
        #update-ie6 ul                    { width:950px; height:160px; margin:0 auto; padding:0; overflow:hidden;}
        #update-ie6 ul li                 { float:left; width:230px; height:160px; margin:0; padding:0 10px 0 0; list-style:none; background:url(<?php echo base_path() . path_to_theme(); ?>/assets/images/divider.jpg) no-repeat 234px 30px;}
        #update-ie6 ul li.last            { padding:0; background:none;}
        #update-ie6 ul li h3              { margin:0; padding:25px 0 15px; font-size:16px; line-height:22px; font-weight:bold; color:#fff;}
        #update-ie6 ul li h3 strong       { font-size:22px; color:#3bc0e9;}
        #update-ie6 ul li p               { margin:0; padding:0; font-style:italic; color:#fff;}
        #update-ie6 ul li a, a:visited    { color:#999; text-decoration:none; outline:none; font-size:12px;}
        #update-ie6 ul li a:hover         { color:#fff;}
        #update-ie6 ul li a               { display:block; width:230px; height:70px; line-height:70px; margin:0; padding:90px 0 0; text-align:center; overflow:hidden;}
        #update-ie6 ul li a.lk-ff         { background:url(<?php echo base_path() . path_to_theme(); ?>/assets/images/ie6/lk-ff.jpg) no-repeat 0 0;}
        #update-ie6 ul li a.lk-ff:hover { background:url(<?php echo base_path() . path_to_theme(); ?>/assets/images/ie6/lk-ff.jpg) no-repeat 0 -160px;}
        #update-ie6 ul li a.lk-gc         { background:url(<?php echo base_path() . path_to_theme(); ?>/assets/images/ie6/lk-gc.jpg) no-repeat 0 0;}
        #update-ie6 ul li a.lk-gc:hover { background:url(<?php echo base_path() . path_to_theme(); ?>/assets/images/ie6/lk-gc.jpg) no-repeat 0 -160px;}
        #update-ie6 ul li a.lk-ie         { background:url(<?php echo base_path() . path_to_theme(); ?>/assets/images/ie6/lk-ie.jpg) no-repeat 0 0;}
        #update-ie6 ul li a.lk-ie:hover { background:url(<?php echo base_path() . path_to_theme(); ?>/assets/images/ie6/lk-ie.jpg) no-repeat 0 -160px;}
      </style>
      <div id="update-ie6">
          <a class="lk-closeie6" href='#' onclick='javascript:this.parentNode.style.display="none"; return false;'>close</a>
          <ul>
              <li>
                  <h3>You are using an<br />outdated browser</h3>
                  <p>For a better experience using this site, please upgrade to a modern<br />web browser.</p>
              </li>
              <li><a class="lk-ie" href="http://www.microsoft.com/france/windows/internet-explorer/telecharger-ie8.aspx" title="Download Internet Explorer" target="_blank">Update to Internet Explorer</a></li>
              <li><a class="lk-ff" href="http://www.mozilla-europe.org/fr/firefox/" title="Download Firefox" target="_blank">Update to Mozilla Firefox</a></li>
              <li class="last"><a class="lk-gc" href="http://www.google.ch/chrome?hl=fr" title="Download Google Chrome" target="_blank">Update to Google Chrome</a></li>
          </ul>
      </div>
  <![endif]-->
  <?php endif; ?>

  <?php if (theme_get_setting('cms_theme_browser_nojs')): ?>
  <!-- noscript -->
    <noscript><div class="noscript"><?php echo theme_get_setting('cms_theme_browser_nojs_label'); ?></div></noscript>
  <?php endif; ?>

  <div id="skip-link">
      <a href="#main-content" class="element-invisible element-focusable"><?php print t('Skip to main content'); ?></a>
  </div>
  <?php print $page_top; ?>
  <?php print $page; ?>
  <?php print $page_bottom; ?>
</body>
</html>
