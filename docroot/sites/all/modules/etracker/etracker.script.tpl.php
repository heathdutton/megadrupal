<?php

/**
 * @file
 * Theme the more link.
 *
 * - $view: The view object.
 * - $more_url: the url for the more link.
 * - $link_text: the text for the more link.
 *
 * @ingroup views_templates
 */
?>
<!-- Copyright (c) 2000-2014 etracker GmbH. All rights reserved. -->
<!-- This material may not be reproduced, displayed, modified or distributed -->
<!-- without the express prior written permission of the copyright holder. -->
<!-- etracker tracklet 4.0 -->
<script type="text/javascript">
  <?php
    foreach ($query as $key => $value) {
      print 'var ' . $key . ' = "' . $value . '";' . PHP_EOL;
    }
  ?>
</script>
<script id="_etLoader" type="text/javascript" charset="UTF-8" data-secure-code="<?php print $account_key_1; ?>" src="//static.etracker.com/code/e.js"></script>
<!-- etracker tracklet 4.0 end -->
