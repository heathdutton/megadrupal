<?php // $Id$   ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">

  <head>
    <?php print $head; ?>
    <title><?php print $head_title; ?></title>
    <?php print $styles; ?>
    <?php print $scripts; ?>
       <!--[if lt IE 7.]>
          <style type="text/css" media="screen">
            @import url(<?php print base_path() . path_to_theme() ?>/ie_styles.css);
          </style>
      <![endif]--> 
      <script type="text/javascript">
        <!--//--><![CDATA[//><!--
        Drupal.behaviors.tntIEFixes = function (context) {
            if ($.browser.msie && ($.browser.version < 7)) {
              $('#topmenu li').hover(function() {
                  $(this).addClass('hover');
                }, function() {
                  $(this).removeClass('hover');
              });
            };
        };
        //--><!]]>
      </script>
  </head>
  <body>
    <div id="header">
      <div id="headertitle">
        <h1><a href="<?php echo $front_page;?>" title="<?php echo t('Home') ?>"><?php echo $site_name;?></a></h1>
        <?php if($site_slogan): ?>
          <div class='site-slogan'>
            <?php echo $site_slogan ;?>
          </div>
        <?php endif; ?>
      </div><!-- /headertitle -->
      
      <?php if ($logo): ?>
        <div id="logo">
          <a href="<?php print $front_page ?>" title="<?php print t('Home') ?>"><img src="<?php print($logo) ?>" alt="<?php print t('Home') ?>" /></a>
        </div>
      <?php endif; ?> 
    </div><!-- /header -->
    
    <?php if($messages): echo "<div id=\"messagebox\">" . $messages . "</div>"; endif; ?>       
    <div id="contentcontainer">
      <div id="container">
        <div id="contentleft">
          <div id="page">
            <?php print $breadcrumb; ?>
            <a id="main-content"></a>
            <?php if ($title): ?>
              <h1 class="title" id="page-title"><?php print $title; ?></h1>
            <?php endif; ?>
            <?php if ($tabs): ?>
              <div class="tabs"><?php print $tabs; ?></div>
            <?php endif; ?>
            <?php print $help; ?>
            <?php print $content; ?>
            <div class="feedicons">
              <?php echo $feed_icons ?>
            </div>
          </div><!-- /page -->
          
          <?php if ($footer_block): print "<div id=\"footer\">" . $footer_block . "</div>"; endif;?>
        </div><!-- /contentleft -->
        
        <div id="contentright">
          <?php print $sidebar_second; ?>
        </div><!-- /contentright -->
        
      </div><!-- /container -->
      
    </div><!-- /contentcontainer -->
    
    <div id="bottompage">
      <div id="skyline"></div>
      <div id="bottomtext">
        Theme designed by <a href="http://www.carettedonny.be" title="Donny Carette">Donny Carette</a> - Powered by <a href="http://www.drupal.org" title="Drupal">Drupal</a> - copyright &copy; <?php echo date("Y");?>
      </div>
    </div><!-- /bottompage -->
  </body>
</html>