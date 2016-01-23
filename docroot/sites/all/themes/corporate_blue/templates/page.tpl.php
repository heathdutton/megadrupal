<div id="wrapper" class="container-12">
      <div class="header">
        <div class="grid-12">
          <div class="grid-4 alpha logo">
            <?php if ($site_name): ?>
                  <a href="<?php print check_url($front_page); ?>" title="<?php print t('Home'); ?>">
                    <img src="<?php print $logo; ?>" alt="<?php print $site_name; ?>" />
                  </a>
            <?php endif; ?>
          </div>
          <?php if ($page ['login']): ?>
          <div class="grid-8 omega">
            <?php print render($page['login']); ?>
          </div>
          <?php endif; ?>
        </div>
      </div>
      <div class="grid-12  main_nav">
        <?php // if ($page ['primarynav']): ?>
        <div class="grid-8 alpha omega">
          <?php // print render($page['primarynav']); ?>
           <?php 
          if (module_exists('i18n')) { 
            $main_menu_tree = i18n_menu_translated_tree(variable_get('menu_main_links_source', 'main-menu'));
          } else {
            $main_menu_tree = menu_tree(variable_get('menu_main_links_source', 'main-menu'));
          }
          print drupal_render($main_menu_tree);
        ?>
        </div>
        <?php // endif; ?>
        <?php if ($page ['search']): ?>
        <div class="grid-4">
          <?php print render($page['search']); ?>
        </div>
        <?php endif; ?>
      </div>
      <div id="content_section" class="grid-12">
        <div class="<?php print (($page['rightblockone'] || $page['rightblocktwo'] || $page['rightblockthree'] || $page['rightblockfour'] || $page['rightblockfive']) ? 'grid-8' : 'grid-11') ?> region-content">
          <?php if ($tabs): ?>
            <?php print render($tabs); ?>
          <?php endif; ?>
          <?php if ($page ['content']): ?>
    		    <?php print render($page['content']); ?>
    			<?php endif; ?>  
        </div>       
        <?php if ($page['rightblockone'] || $page['rightblocktwo'] || $page['rightblockthree'] || $page['rightblockfour'] || $page['rightblockfive']): ?>
        <div class="grid-3 omega region-rightblock">
          <?php if ($page ['rightblockone']): ?>
            <?php print render($page['rightblockone']); ?>
          <?php endif; ?>
          <?php if ($page ['rightblocktwo']): ?>
            <?php print render($page['rightblocktwo']); ?>
          <?php endif; ?>
          <?php if ($page ['rightblockthree']): ?>
            <?php print render($page['rightblockthree']); ?>
          <?php endif; ?>
          <?php if ($page ['rightblockfour']): ?>
            <?php print render($page['rightblockfour']); ?>
          <?php endif; ?>
          <?php if ($page ['rightblockfive']): ?>
            <?php print render($page['rightblockfive']); ?>
          <?php endif; ?>
        </div>
        <?php endif; ?>
      </div>
      <div class="grid-12 footer_top">
        <?php if ($page ['f-left']): ?>
        <div class="grid-3 leftmargin">
          <?php print render($page['f-left']); ?>
        </div>
        <?php endif; ?>
        <div class="grid-3">
        <?php if ($page ['f-middle']): ?>
        
          <div class="middle_top"></div>
          <div class="middle_middle"></div>
          <div class="middle_bottom"></div>
          <?php print render($page['f-middle']); ?>
        
        <?php endif; ?>
        </div>
        <div class="grid-5">
        <?php if ($page ['f-right']): ?>
          <?php print render($page['f-right']); ?>
        <?php endif; ?>       
        </div>
        <div class="footer_zyxware">Theme by <a href="http://www.zyxware.com/" title="Zyxware" target="_blank">Zyxware</a></div> 
      </div>
      <div class="grid-12 footer_bottom">
        <?php if ($page ['copyright']): ?>
          <?php print render($page['copyright']); ?>
        <?php endif; ?> 
      </div> 
    </div>
