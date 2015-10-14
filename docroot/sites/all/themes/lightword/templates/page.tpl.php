<div id="wrapper">
	<?php if ($logo): ?>
	<div id="top_cufon">
		<h1 id="logo">
			<a  title="<?php print t('Home'); ?>" href="<?php print $front_page; ?>" >
				<?php print $site_name; ?>
			</a> 
			<small><?php print $site_slogan; ?></small>
		</h1>
	</div>
	<?php endif; ?>
	<div style="width:700px;overflow:hidden;">
		<?php print render($page['header']); ?>	
	</div>
	<div style="clear:both;" > </div>
	<div id="header">	
		<a id="rss-feed" title="Syndicate this site using RSS" href="?q=rss.xml">Subscribe via RSS</a>
		<div id="top_bar">
			<?php if ($main_menu): ?>
		    <!--<div class="center_menu">
		        <?php print theme('links__system_main_menu', array(
		          'links' => $main_menu,
		          'attributes' => array(
		          'id' => 'front_menu',
			  'class' => array('links', 'clearfix'),
		          ),
		          'heading' => array(
		            'text' => t('Main menu'),
		            'level' => 'h2',
		            'class' => array('element-invisible'),
		          ),
		        )); ?>
		     </div> <!-- /#main-menu -->
		             <div id="nav" class="grid-12 clearfix alpha omega">
          <?php // if ($page ['primarynav']): ?>
            <div class="grid-7 alpha">
            <?php //print render($page['primarynav']); ?> 
                       <?php 
          if (module_exists('i18n')) { 
            $main_menu_tree = i18n_menu_translated_tree(variable_get('menu_main_links_source', 'main-menu'));
          } else {
            $main_menu_tree = menu_tree(variable_get('menu_main_links_source', 'main-menu'));
          }
          print drupal_render($main_menu_tree);
        ?>
            </div>
			    <?php endif; ?>
			    <div class="ser_box">
			       <?php print render($page['search_box']); ?>
			    </div>
		</div>
	</div>
	 
	<div id="<?php print (($page['sidebar_second']) ? 'content' : 'content-full') ?>">
		<div id="<?php print (($page['sidebar_second']) ? 'content-body' : 'content-body-full') ?>">
		<a id="main-content"></a>
        <?php print render($title_prefix); ?>
        <?php if ($title): ?>
          <h1 class="title" id="page-title">
            <?php print $title; ?>
          </h1>
        <?php endif; ?>
        <?php print render($title_suffix); ?>
		
		  <?php if (!empty($tabs)): ?>
			<div class="nwwrap clearfix">
				<?php print render($tabs); ?>
			</div>
		  <?php endif; ?>
			<?php if ($page ['content']): ?>
			<?php print render($page['content']); ?>
			<?php endif; ?>
		</div>		
		<?php if ($page ['sidebar_second']): ?>
		<div class="content-sidebar">
		<!--[if gte IE 9]> <script type="text/javascript"> Cufon.set('engine', 'canvas'); </script> <![endif]--> 
			<?php print render($page['sidebar_second']); ?>		
		</div>
		<?php endif; ?>
		<div class="clear"></div>
	</div>
	<div id="footer">
			<div class="text">
			    <?php if ($page['footer']): ?>
			    		      <?php print render($page['footer']); ?>
			    <?php endif; ?>
				Designed by <a href="http://www.lightword-design.com/" title="Lightword" target="_blank">Lightword Theme</a> <br/>
				Developed by <a href="http://www.zyxware.com/" title="Zyxware" target="_blank">Zyxware</a> 
			</div>
	</div>
		
</div>


