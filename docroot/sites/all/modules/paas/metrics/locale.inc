<?php

function paas_metric_locale_call($value) {
  if (!module_exists('locale')) {
    return;
  }

  global $language;
  $langcode = $language->language;
  
  switch ($value) {
    case 'summary':
      $sql = 'SELECT SUBSTRING_INDEX(SUBSTRING_INDEX(location, \'/\', 2), \'?\', 1) as location, COUNT(*) as count
      FROM locales_source s
      LEFT JOIN locales_target t ON s.lid = t.lid AND t.language = :language
      WHERE s.textgroup = \'default\' AND s.version = :version AND LENGTH(s.source) < 75
      AND s.location LIKE \'/%%\'
      GROUP BY 1';
      paas_dump_csv($sql, array(
        ':language' => $langcode,
        ':version' => VERSION,
      ));
      break;
    case 'strings':
      $sql = 'SELECT s.lid, s.location, s.source
      FROM locales_source s
      WHERE s.textgroup = \'default\' AND s.version = :version AND LENGTH(s.source) < 75
      AND s.location LIKE \'/%\'
      AND s.source NOT REGEXP \'[\\%\\!\\@]\'
      GROUP BY 2
      HAVING COUNT(*) = 1';
      paas_dump_csv($sql, array(
        ':version' => VERSION,
      ));
      break;
  }
}
