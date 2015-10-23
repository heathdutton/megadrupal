<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
  <head>
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
<body class="<?php print  $classes;  print ' mainbody'; ?>" >

<div class="container">
<?php print $messages; ?>
<?php print $help; ?>
</div>
<div style="font-size:18px; margin:auto; margin-top:150px; display:block; width: 800px;" > <?php if ($title): ?><h2><?php print $title ?></h2><?php endif; ?><?php print $content ?></div>
   
 
 </body>
</html>
