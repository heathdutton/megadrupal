<?php

/**
 * @file
 * Provides Drupal context around ClickHeat interface.
 *
 * Copyright 2008-2009 by Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 * Portions copyright 2012-2013 by John Franklin (https://drupal.org/user/683430)
 */
?><html>
  <head>
    <title>ClickHeat</title>
  </head>
  <body style="margin: 0; padding: 0;">
    <div style="height: 3%; background-color: #eeeeee; padding: 0 5px;">
      <a href=".."><?php print t('Back to Drupal'); ?></a>
    </div>
    <iframe src="<?php print $click_heatmap_library; ?>" style="width: 100%; height: 97%; border: 0;"></iframe>
  </body>
</html>