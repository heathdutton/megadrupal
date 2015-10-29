    <div id="head" class="clearfix">

      <div class="clearfix">
        <div id="logo">
          <?php if ($logo || $site_name) {
            print '<a href="'. check_url($base_path) .'" title="'. $site_name .'">';
            if ($logo) {
              print '<img src="'. check_url($logo) .'" alt="'. $site_name .'" />';
          } else {
            print '<span id="sitename">'. $site_name .'</span>';
          }
            print '</a>';
          }
        ?>
        <?php if ($site_slogan): print '<div id="tagline">'. $site_slogan .'</div>'; endif; ?>
        </div>
        <?php if ($page['banner']): ?>
          <div id="banner-region">
            <?php  print render($page['banner']); ?>
          </div>
        <?php endif; ?>
      </div>

      <div id="navbar" class="clearfix">
          <?php if (isset($main_menu)) {
            print arthemia_primary();
          } ?>
      </div>
    </div>

    <div id="page" class="clearfix">
      <?php if ($page['headline']): ?>
	
      <div id="top" class="clearfix">
        <div id="headline" class="<?php print empty($page['featured']) ? 'no' : 'with'?>-featured">
          <?php print render($page['headline']); ?>
        </div>
        <?php if ($page['featured']): ?>
          <div id="featured">
            <?php  print render($page['featured']); ?>
          </div>
        <?php endif; ?>
      </div>
      <?php endif; ?>
      
      <?php if ($page['middle']):?>
        <div id="middle" class="clearfix">
          <?php  print render($page['middle']); ?>
        </div>
      <?php endif; ?>

    

        <?php if ($breadcrumb) { print $breadcrumb; } ?>
        <?php if ($tabs) { print "<div id='tabs-wrapper' class='clearfix'>"; } ?>
        <?php if ($title) { print "<h1". ($tabs ? " class='with-tabs'" : "") .">". $title ."</h1>"; } ?>
        <?php if ($tabs) { print render($tabs) ."</div>"; } ?>
        <?php if (isset($tabs2)) { print render($tabs2); } ?>
        <?php if ($page['help']) {render($page['help']); } ?>
        <?php if ($show_messages && $messages) { print render($messages); } ?>
		
 <div id="content" class="main-content <?php print empty($page['sidebar_first']) ? 'no' : 'with'?>-sidebar">
        <?php if ($page['content']): ?>
            <?php print render($page['content']); ?>
        <?php endif; ?>
      </div>

      <?php if ($page['sidebar_first']):?>
        <div id="sidebar-primary" class="sidebar">
          <?php print render($page['sidebar_first']); ?>
        </div>
      <?php endif; ?>
      
      <?php if ($page['sidebar_second']):?>
        <div id="sidebar-secondary" class="sidebar">
          <?php print render($page['sidebar_second']); ?>
        </div>
      <?php endif; ?>

      <?php if ($page['bottom']):?>
        <div id="bottom">
          <?php print render($page['bottom']); ?>
        </div>
      <?php endif; ?>
    </div>

    <div id="footer-region" class="clearfix">
      <div id="triptych-first" class="clearfix">
        <?php print render($page['triptych_first']); ?>
      </div> 		

      <div id="triptych-second" class="clearfix">
        <?php print render($page['triptych_second']); ?>
      </div>

      <div id="triptych-third" class="clearfix">
        <?php print render($page['triptych_third']); ?>
      </div>
      <?php print render($page['footer']); ?>
    </div>