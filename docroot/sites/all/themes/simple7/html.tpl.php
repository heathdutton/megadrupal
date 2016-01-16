<!DOCTYPE html>

<html lang="<?php print $language->language ?>">

<head>

	<?php print $head ?>
	<?php print $styles ?>
	<?php print $scripts ?>
	<!--[if lt IE 9]>
		<script src="http://html5shiv.googlecode.com/svn/trunk/html5.js"></script>
	<![endif]-->
  <title><?php print $head_title ?></title>

</head>
 
<body class="<?php print $classes ?>">
	
	<?php print $page_top ?>
	
	<?php print $page ?>
	
	<?php print $page_bottom ?>

</body>

</html>