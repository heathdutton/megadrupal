<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">

<head>
  <title><?php print $head_title; ?></title>
  <?php print $head; ?>
  <?php print $styles; ?>
  <?php print $scripts; ?>
  <!--[if IE]>
    <link type="text/css" rel="stylesheet" media="all" href="<?php print base_path() . path_to_theme(); ?>/ie_styles.css" />
  <![endif]-->
</head>
<body class="<?php print $classes; ?>">
  <div id="header">

    <div id="headerWrapper">
      
      <div id="siteName">
        <?php if ($logo): print '<a href="' . $front_page . '" title="' . t('Home') . '"> <img src="' . check_url($logo) . '" alt="' . t('Home') . '" id="logo" /></a>'; endif; ?>
        <div id="siteInfo">
          <?php if (!empty($site_name)): ?>
            <h1 id="siteTitle">
              <a href="<?php print $front_page ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a>
            </h1>
          <?php endif; ?>
          
          <?php if (!empty($site_slogan)): ?>
            <div id="siteSlogan"><?php print $site_slogan; ?></div>
          <?php endif; ?>
        </div><!-- /siteInfo -->
      </div> <!-- /siteName-->
        
        
    </div><!-- /headerWrapper -->
  </div><!-- /header -->

  <div id="container">
    <div id="inner">  
      <div id="contentWrapper">
        <?php if (!empty($primary_nav)): ?>
          <div id="menuLeft"></div>
          <div id="primaryMenu">
            <?php print $primary_nav; ?>
          </div>
          <div id="menuRight"></div>
        <?php endif; ?> 
        

        
        <?php if($sidebar_first): ?>
          <div id="sidebar_first" class="sidebar">
            <?php print $sidebar_first; ?>
          </div>
        <?php endif; ?>
       
        <div id="center">
          
         <?php print $messages; ?>
         
        
          <div id="content">        
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
          </div>
 
        </div><!-- /center --> 
    
        <?php if($sidebar_second): ?>
          <div id="sidebar_last" class="sidebar">
            <?php print $sidebar_second; ?>
          </div>
        <?php endif; ?>
      
      </div><!-- /contentWrapper -->
      
    </div><!-- /Inner -->
    
  </div><!-- /container -->
  
  <div id="footer">
    <div class="footer-text">
      <?php if($footer_message): ?>
        <?php print $footer_message; ?>
      <?php endif; ?>
    </div>
                    
  </div><!-- /footer -->
  
  </body>
</html>
