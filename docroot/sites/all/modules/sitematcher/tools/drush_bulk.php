<?php

require_once dirname (dirname( __FILE__ )) . '/sitematcher.class.inc';

$sm=new DrupalSiteMatcher();

foreach ($sm->getSites() as $site) {
  echo "${site}\n";
}
