var page = require('webpage').create();

<?php if (!empty($paper_size)): ?>
page.paperSize = <?php echo json_encode($paper_size); ?>;
<?php endif; ?>
<?php if (!empty($viewport_size)): ?>
page.viewportSize = <?php echo json_encode($viewport_size); ?>;
<?php endif; ?>

// For TypeKit suppport.
page.settings.userAgent = 'Mozilla/5.0 (Windows NT 6.0; WOW64) AppleWebKit/535.7 (KHTML, like Gecko) Chrome/16.0.912.75 Safari/535.7';
page.customHeaders = { 'Referer': <?php echo json_encode($url); ?> };

// Add cookies.
<?php if (!empty($cookies)): ?>
  try {
    <?php foreach ($cookies as $cookie): ?>
      phantom.addCookie(<?php echo json_encode($cookie); ?>);
    <?php endforeach; ?>
  }
  catch (ex) {
    phantom.exit();
  }
<?php endif; ?>

// Open and render the page.
page.open('<?php echo $url; ?>', function () {
  page.render('/dev/stdout', { format: 'pdf' });
  phantom.exit();
});
