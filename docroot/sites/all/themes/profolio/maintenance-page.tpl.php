<?php
/**
 * @file
 * Default theme implementation to display a single Drupal page while offline.
 *
 * All the available variables are mirrored in html.tpl.php and page.tpl.php.
 * Some may be blank but they are provided for consistency.
 *
 * @see template_preprocess()
 * @see template_preprocess_maintenance_page()
 */

?>
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
  <div id="headcontainer">
   <div id="logowrap">
   
        <?php if ($site_name): ?>
          <?php if ($title): ?>
            <div id="site-name"<?php if ($hide_site_name) : print ' class="element-invisible"'; endif;  ?>>
              <strong>
                <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
              </strong>
            </div>
          <?php else: /* Use h1 when the content title is empty */ ?>
            <h1 id="site-name"<?php if ($hide_site_name) : print ' class="element-invisible"'; endif;  ?>>
              <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
            </h1>
          <?php endif; ?>
        <?php endif; ?>

        <?php if ($site_slogan): ?>
          <div id="site-slogan"<?php if ($hide_site_slogan) : print ' class="element-invisible"'; endif;  ?>>
            <?php print $site_slogan; ?>
          </div>
      </div> <!-- /#name-and-slogan -->
    <?php endif; ?>
    
   <div id="menu">
            <?php print theme('links__system_main_menu', array('links' => $main_menu, 'attributes' => array('id' => 'main-menu', 'class' => array('links', 'inline', 'clearfix')))); ?>
    </div>
  </div>
</div><!--Header-->
<div id="maincontainer">
    <?php if ($page['slider']): ?>
     <div class="scanlines">
       <div class ="innerborder">
         <div class="slider">
           <?php print render($page['slider']); ?>
         </div>
       </div>
     </div>
     <?php endif;?>

    <?php if (($page['top1']) && ($page['top2']) && ($page['top3']) &&  ($page['top4'])):  ?>
    <div id="topcontent">
      <div id="top1" class="small"><?php print render($page['top1']); ?></div>
      <div id="top2" class="small"><?php print render($page['top2']); ?></div>
      <div id="top3" class="small"><?php print render($page['top3']); ?></div>
      <div id="top4" class="small"><?php print render($page['top4']); ?></div>
    </div>
     <?php endif;?>

    <?php if (($page['top1']) && ($page['top2']) && (!$page['top3']) &&  (!$page['top4'])):  ?>
    <div id="topcontent">
      <div id="top1" class="bigger"><?php print render($page['top1']); ?></div>
      <div id="top2" class="bigger"><?php print render($page['top2']); ?></div>
    </div>
     <?php endif;?>
    
    <div id="main">
        <?php if($page['sidebar_first']): ?>
          <div id="sidebar_first" class="sidebar">
            <?php print render($page['sidebar_first']); ?>
          </div>
        <?php endif; ?>
  
      <div id="content">
       <?php print $messages; ?>
	   <?php print render($title_prefix); ?>
         <?php if (!$page): ?>
         <h2 class="title"<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
         <?php endif; ?>
	   <?php print render($title_suffix); ?>
            <?php if ($tabs): ?>
              <div class="tabs"><?php print render($tabs); ?></div>
            <?php endif; ?>
            <?php print render($page['help']); ?>
            <?php if ($action_links): ?>
              <ul class="action-links"><?php print render($action_links); ?></ul>
            <?php endif; ?>
            <?php print render($page['content']); ?>
            <div class="feedicons">
              <?php echo $feed_icons ?>
         </div>

    </div><!--content-->
                <?php if($page['sidebar_second']): ?>
             <div id="sidebar_last" class="sidebar">
               <?php print render($page['sidebar_second']); ?>
             </div>
            <?php endif; ?>
  </div><!--main-->
 </div><!--maincontainer-->

<div id="footer">
   <div id="footcontainer">
      <div id="foot1"><?php print render($page['foot1']); ?></div>
      <div id="foot2"><?php print render($page['foot2']); ?></div>
      <div id="foot3"><?php print render($page['foot3']); ?></div>
   </div>
</div>
<div id="footer2">
   <div id="footcontainer2">
      <div id="foot12">Evad.be</div>
   </div>
</div>
  
  </body>
</html>
