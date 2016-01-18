<?php
/**
 * @file
 * Implementation to display a single Drupal page while offline.
 *
 * All the available variables are mirrored in page.tpl.php.
 *
 * @see template_preprocess()
 * @see template_preprocess_maintenance_page()
 * @see morelesszen_process_maintenance_page()
 */

$html_attributes = "lang=\"{$language->language}\" dir=\"{$language->dir}\" {$rdf->version}{$rdf->namespaces}";
?>

<?php print $doctype; ?>
<!--[if IE 6 ]><html <?php print $html_attributes; ?> class="no-js ie6"><![endif]-->
<!--[if IE 7 ]><html <?php print $html_attributes; ?> class="no-js ie7"><![endif]-->
<!--[if IE 8 ]><html <?php print $html_attributes; ?> class="no-js ie8"><![endif]-->
<!--[if IE 9 ]><html <?php print $html_attributes; ?> class="no-js ie9"><![endif]-->
<!--[if (gt IE 9)|!(IE)]><!--><html <?php print $html_attributes; ?> class="no-js"><!--<![endif]-->
<head<?php print $rdf->profile; ?>>

  <?php print $head; ?>

  <!-- Always force latest IE rendering engine (even in intranet) & Chrome Frame  -->
  <!--[if IE]><meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1"><![endif]-->

  <!--  Mobile viewport optimized: j.mp/bplateviewport -->
  <meta name="viewport" content="width=device-width, initial-scale=1.0">

  <!-- Prevent blocking -->
  <!--[if IE 6]><![endif]-->

  <title><?php print $head_title; ?></title>

  <?php print $styles; ?>

  <?php print $scripts; ?>

</head>

<body class="<?php print $classes; ?>" <?php print $attributes;?>>
  
  <div id="page-wrapper">
    <div id="page">

      <div id="header">
        <div class="section middle clearfix">

          <?php if ($logo): ?>
            <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" class="logo">
              <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
            </a>
          <?php endif; ?>
          
        </div> <!-- /.section -->
      </div> <!-- /#header -->

      <div id="main-wrapper">
        <div id="main" class="clearfix middle">
          <div id="content" class="column"><div class="section">

            <?php if ($title): ?>
              <h1 class="title" id="page-title"><?php print $title; ?></h1>
            <?php endif; ?>

            <?php print $content; ?>

            <?php if ($messages): ?>
              <div id="messages">
                <div class="section clearfix"><?php print $messages; ?></div>
              </div> <!-- /#messages -->
            <?php endif; ?>

          </div></div> <!-- /.section, /#content -->
        </div> <!-- /#main -->
      </div> <!-- /#main-wrapper -->

    </div> <!-- /#page -->
  </div> <!-- /#page-wrapper -->

  <!--[if lt IE 7 ]>
    <script src="<?php print $morelesszen_path;?>/js/libs/dd_belatedpng.min.js"></script>
    <script> DD_belatedPNG.fix('img, .png_bg'); //fix any <img> or .png_bg background-images </script>
  <![endif]-->

  <?php if (morelesszen_ga_enabled()): ?>
  <!-- Google Analytics : mathiasbynens.be/notes/async-analytics-snippet -->
  <script type="text/javascript">
    <!--//--><![CDATA[//><!--
    var _gaq=[['_setAccount','<?php print theme_get_setting('morelesszen_ga_trackingcode');?>'],['_trackPageview']];
    <?php if (theme_get_setting('morelesszen_ga_anonimize')): ?>
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