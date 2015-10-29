<?php
/**
 * @file
 * Implementation to display a single Drupal page while offline.
 *
 * All the available variables are mirrored in page.tpl.php.
 *
 * @see template_preprocess()
 * @see template_preprocess_maintenance_page()
 * @see ninesixtyfive_process_maintenance_page()
 */
$html_attributes = "lang=\"{$language->language}\" dir=\"{$language->dir}\" {$rdf->version}{$rdf->namespaces}";
?>
<?php print $doctype; ?>
<!--[if IE 6]><![endif]-->
<!--[if IE 7 ]><html <?php print $html_attributes; ?> class="no-js ie7"><![endif]-->
<!--[if IE 8 ]><html <?php print $html_attributes; ?> class="no-js ie8"><![endif]-->
<!--[if IE 9 ]><html <?php print $html_attributes; ?> class="no-js ie9"><![endif]-->
<!--[if (gt IE 9)|!(IE)]><!--><html <?php print $html_attributes; ?> class="no-js"><!--<![endif]-->
<head<?php print $rdf->profile; ?>>

  <?php print $head; ?>

  <!-- Always force latest IE rendering engine (even in intranet) & Chrome Frame  -->
  <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">

  <!--  Mobile viewport optimized: j.mp/bplateviewport -->
  <meta name="viewport" content="width=device-width, initial-scale=1.0">

  <!-- Prevent blocking -->
  <!--[if IE 6]><![endif]-->

  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>

  <!-- Uncomment if you are specifically targeting less enabled mobile browsers
  <link rel="stylesheet" media="handheld" href="<?php print $ninesixtyfive_path;?>/css/handheld.css">  -->

  <?php print $scripts; ?>

</head>
<body class="<?php print $classes; ?>" <?php print $attributes;?>>

  <div id="skip-link">
    <a href="#main-content" class="element-invisible element-focusable"><?php print t('Skip to main content'); ?></a>
  </div>

  <?php print $page_top; ?>

  <div id="page-wrapper"><div id="page">

      <header id="header" role="banner">
        <div class="section clearfix">
          <?php if ($site_name || $site_slogan): ?>
            <div id="name-and-slogan">
              <?php if ($site_name): ?>
                <div id="site-name">
                  <strong>
                    <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
                  </strong>
                </div>
              <?php endif; ?>
              <?php if ($site_slogan): ?>
                <div id="site-slogan">
                  <?php print $site_slogan; ?>
                </div>
              <?php endif; ?>
            </div> <!-- /#name-and-slogan -->
          <?php endif; ?>
        </div><!-- /.section -->
      </header><!-- /#header -->

      <div id="main-wrapper">
        <div id="main" class="clearfix">
          <div id="content" class="column" role="main">
            <div class="section">
              <a id="main-content"></a>
              <?php if ($title): ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?>
              <?php print $content; ?>
              <?php if ($messages): ?>
                <div id="messages">
                  <div class="section clearfix">
                    <?php print $messages; ?>
                  </div><!-- /.section -->
                </div><!-- /#messages -->
              <?php endif; ?>
            </div><!-- /.section -->
          </div><!-- /#content -->
        </div><!-- /#main -->
      </div><!-- /#main-wrapper -->

      </div><!-- /#page -->
    </div><!-- /#page-wrapper -->

  <?php print $page_bottom; ?>

  <?php if (ninesixtyfive_ga_enabled()) :?>
  <!-- Google Analytics : mathiasbynens.be/notes/async-analytics-snippet -->
  <script type="text/javascript">
    <!--//--><![CDATA[//><!--
    var _gaq=[['_setAccount','<?php echo theme_get_setting('ga_trackingcode');?>'],['_trackPageview']];
    <?php if (theme_get_settings('ninesixtyfive_ga_anonimize')): ?>
      _gaq.push (['_gat._anonymizeIp']);
    <?php endif; ?>
    (function(d,t){var g=d.createElement(t),s=d.getElementsByTagName(t)[0];g.async=1;
    g.src=('https:'==location.protocol?'//ssl':'//www')+'.google-analytics.com/ga.js';
    s.parentNode.insertBefore(g,s)}(document,'script'));
    //--><!]]>
  </script>
  <?php endif; ?>

</body>
</html>
