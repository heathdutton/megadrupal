<?php
/**
 * @file ynews-view.tpl.php
 * Views template to display a news list
 */
print '<?xml version="1.0" encoding="UTF-8" ?>';
?>

<rss version="2.0" xmlns:yandex="http://news.yandex.ru">
<channel>
<title><?php print $title; ?></title>
<link><?php print $GLOBALS['base_url'] . $GLOBALS['base_path']; ?></link>
<description><?php print $description; ?></description>
<lastBuildDate><?php print date(DATE_RFC822); ?></lastBuildDate>
<image>
  <url><?php print $image_url; ?></url>
  <title><?php print $title; //print yml_safe(variable_get('site_name', '')); ?></title>
  <link><?php print $GLOBALS['base_url'] . $GLOBALS['base_path']; ?></link>
</image>
<?php foreach ($rows as $id => $row): ?>
<item>
   <title><?php print $row['title']; ?></title>
   <link><?php print $row['link']; ?></link>
   
   <?php if(isset($row['description']) && $row['description']): ?>
   <description><?php print $row['description']; ?></description>
   <?php endif; ?>
   
   <?php if(isset($row['author'])): ?>
   <author><?php print $row['author']; ?></author>
   <?php endif; ?>
   
   <yandex:full-text><?php print $row['body']; ?></yandex:full-text>
   
   <?php if(isset($row['category'])): ?>
   <category><?php print $row['category']; ?></category>
   <?php endif; ?>
   
   <?php if(isset($row['pubdate'])): ?>
   <pubDate><?php print $row['pubdate']; ?></pubDate>
   <?php endif; ?>

   <?php if(isset($row['enclosure'])): ?>
   <?php foreach ($row['enclosure'] as $file): ?>
   <enclosure url="<?php print $file['url']; ?>" type="<?php print $file['ctype']?>" />
   <?php endforeach; ?>
   <?php endif; ?>
   
</item>
<?php endforeach; ?>


</channel>
</rss>