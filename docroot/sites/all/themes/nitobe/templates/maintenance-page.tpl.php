<?php
// $Id: maintenance-page.tpl.php,v 1.6 2010/10/23 12:56:27 pl2 Exp $
/**
 * @file
 * Provides a custom maintenance notice page for Nitobe.
 *
 * The maintenance page has the same additional variables that are provided
 * in page.tpl.php
 *
 * TODO Fix maintenance page.
 */
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
	"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
	<head>
		<title><?php print $head_title; ?></title>
		<?php print $head; ?>
		<?php print $styles; ?>
		<?php print $scripts; ?>
		<!--[if lt IE 8]>
			<?php print nitobe_get_ie_styles() . "\n"; ?>
		<![endif]-->
	</head>
	<body class="nitobe <?php print $body_classes; ?>">
    <?php print $page_top; ?>
		<div id="page-wrapper" class="clearfix">
			<div id="header-area" class="container-16">
					<div id="title-group" class="<?php print nitobe_ns('grid-16', $header, 6, $search_box, 4); ?>">
			            <?php if (isset($nitobe_logo)) { print $nitobe_logo; } ?>
			            <?php if (isset($nitobe_title)) { print $nitobe_title; } ?>
			            <?php if (isset($nitobe_slogan)) { print $nitobe_slogan; } ?>
			        </div><!-- /title-group -->
			        <?php if ($page['header']): ?>
    			        <div id="header-region" class="grid-6">
                    <?php print render($page['header']); ?>
    			        </div><!-- /header-region -->
			        <?php endif; ?>
          		<hr/>
          		<hr id="no-menu-rule" class="rule-bottom grid-16"/>
          		<div id="masthead" class="grid-16">
          		    <?php if (isset($masthead)) { print $masthead; } ?>
          		</div><!-- /masthead -->
          		<hr class="rule-top grid-16"/>
			</div><!-- /header-area -->
			<hr/>
			<div id="content-area" class="container-16">
				<div id="content" class="<?php print $nitobe_classes['content']; ?>">
					<?php if ($show_messages && !empty($messages)) { print $messages; } ?>
					<?php print render($page['help']); ?>
					<?php if (!empty($mission)): ?>
						<div id="mission" class="clearfix"><?php print $mission; ?></div>
					<?php endif;?>
					<?php if (!empty($title)): ?>
          				<div id="page-headline" class="clearfix">
    					    <?php print $nitobe_page_title; ?>
          				</div><!-- #page-headline -->
					<?php endif; ?>
					<?php print $content; ?>
				</div><!-- /content -->
				<?php if ($page['sidebar_first']): ?>
					<div id="sidebar-first" class="<?php print $nitobe_classes['sidebar_first']; ?>">
					  <?php print render($page['sidebar_first']); ?>
					</div><!-- /sidebar-first -->
				<?php endif; ?>
				<?php if ($page['sidebar_second']): ?>
					<div id="sidebar-second" class="<?php print $nitobe_classes['sidebar_second']; ?>">
						<?php print render($page['sidebar_second']); ?>
					</div> <!-- /sidebar_second -->
				<?php endif; ?>
			</div><!-- /content-area -->
			<hr/>
			<div id="footer-area" class="container-16">
				<hr class="rule-top grid-16"/>
				<div id="footer" class="grid-16">
					<?php print render($page['footer']); ?>
				</div><!-- /footer -->
			</div><!-- /footer-area -->
			<hr/>
		</div><!-- /page-wrapper -->
		<?php print $page_bottom; ?>
	</body>
</html>
