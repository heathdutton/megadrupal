       <div id="header" class="container_<?php print $wrappedsites->gridType; ?> clearfix">
        
        <?php if ($logo){ ?>
           <div id="logo-wrapper" class="grid_<?php print $wrappedsites->gridType/4; ?>">
            <a href="<?php print $front_page ?>">
            
              <img src="<?php print $logo ?>" alt="<?php print $site_name_and_slogan ?>" title="<?php print $site_name_and_slogan ?>" id="logo" />
            
             
            </a>
           </div>
             <?php } ?>
           
              <?php if ($site_name || $site_slogan){ ?>
        <div id="name-and-slogan" class="grid_<?php print ($wrappedsites->gridType/4)*3; ?>">
          <?php if ($site_name){ ?>
            <?php if ($title){ ?>
              <div id="site-name"><strong>
                <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
              </strong></div>
            <?php } 
            else { /* Use h1 when the content title is empty */ ?>
              <h1 id="site-name">
                <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
              </h1>
            <?php } ?>
          <?php } ?>

          <?php if ($site_slogan){ ?>
            <div id="site-slogan"><?php print $site_slogan; ?></div>
            <?php } ?>
         
        </div> <!-- /#name-and-slogan -->
       <?php } ?>

      <?php print render($page['header']); ?>

      <?php if ($main_menu || $secondary_menu){ ?>
      
        <?php print theme('links__system_main_menu', array('links' => $main_menu, 'attributes' => array('id' => 'main-menu', 'class' => array('links', 'inline', 'grid_'.$wrappedsites->gridType, 'clearfix')))); ?>
        <?php print theme('links__system_secondary_menu', array('links' => $secondary_menu, 'attributes' => array('id' => 'secondary-menu', 'class' => array('links', 'inline', 'grid_'.$wrappedsites->gridType, 'clearfix')))); ?>
      
    <?php } ?>      
       

      </div> <!-- /#header -->
          
       <div id="columns" class="container_<?php print $wrappedsites->gridType; ?> clearfix">

           <div id="main" class="sidebar grid_<?php print $grid_regions['content']->width?> push_<?php print $grid_regions['content']->push?>">
           
              <?php if ($breadcrumb){ ?>
      <div id="breadcrumb"><?php print $breadcrumb; ?></div>
    <?php } ?>
      <?php print $messages; ?>
              <?php if ($page['highlighted']){ ?><div id="highlighted"><?php print render($page['highlighted']); ?></div><?php } ?>
              <?php print render($title_prefix); ?>
        <?php if ($title){ ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php } ?>
        <?php print render($title_suffix); ?>
        <?php if ($tabs){ ?><div class="tabs"><?php print render($tabs); ?></div><?php } ?>
        <?php print render($page['help']); ?>
        <?php if ($action_links){ ?><ul class="action-links"><?php print render($action_links); ?></ul><?php } ?>
              
          
          <?php print render($page['content']); ?>
          <?php print $feed_icons; ?>
           </div>


       <?php foreach ($grid_regions as $key=>$value) { if ($key!='content'){ ?>
            <?php print render($page[$value->region]); ?>
        <?php } } ?>

      </div>
   
     <div id="footer"  class="container_<?php print $wrappedsites->gridType; ?>"><div class="grid_<?php print $wrappedsites->gridType; ?> clearfix">
         <?php print render($page['footer']); ?>
      </div></div>
   
  
    
