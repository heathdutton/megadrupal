<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML+RDFa 1.0//EN"
  "http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" version="XHTML+RDFa 1.0" dir="<?php print $language->dir; ?>"<?php print $rdf_namespaces; ?>>

<head profile="<?php print $grddl_profile; ?>">
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<title><?php print $head_title ?></title>
<?php print $head ?>
<?php print $styles ?>
<style type="text/css" media="all">@import "<?php print base_path() . path_to_theme() ?>/master_reset.css";</style>
<!--[if lt IE 7]>
    <style type="text/css" media="all">@import "<?php print base_path() . path_to_theme() ?>/fix-ie6.css";</style>
	<![endif]-->
<?php print $scripts ?>
<script type="text/javascript"><?php /* Needed to avoid Flash of Unstyle Content in IE */ ?> </script>
</head>
<body class="<?php print  $classes; ; print ' mainbody'; print get_sidebar_state($page['sidebar_first'], $page['sidebar_last'], $page['right_dark']); ?>">
  <?php print $page_top; ?>
  <?php print $page; ?>
  <?php print $page_bottom; ?>
</body>
</html>