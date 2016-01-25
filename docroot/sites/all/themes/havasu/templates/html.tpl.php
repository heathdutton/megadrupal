<!DOCTYPE html>
<html version="HTML+RDFa 1.0" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>" xmlns="http://www.w3.org/1999/xhtml"<?php print $rdf_namespaces; ?>>

	<head profile="<?php print $grddl_profile; ?>">
	  <?php print $head; ?>
	  <title><?php print $head_title; ?></title>
	  <?php print $styles; ?>
	  <?php print $scripts; ?>
		<script src="misc/jquery.js"></script>
		<script src="sites/all/themes/havasu/js/havasu.js"></script>
	</head>
	<body class="<?php print $classes; ?>" <?php print $attributes;?>>
	  <div id="skip-link">
	    <a href="#content" title="<?php print t('Jump to the main content of this page'); ?>" class="element-invisible"><?php print t('Jump to Content'); ?></a>
	  </div>
	  <?php print $page_top; ?>
	  <?php print $page; ?>
	  <?php print $page_bottom; ?>
	</body>
</html>