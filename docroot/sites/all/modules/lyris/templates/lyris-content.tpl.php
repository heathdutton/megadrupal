<?php
/**
 * @file
 * Create the bare content display for the iFrame.
 */
?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
  <title>Title of the document</title>
  <script type="text/javascript" src="<?php print base_path(); ?>misc/jquery.js"></script>
  <script type="text/javascript">
    // Maximize the parent iFrame.
    $(document).ready(function() {
      var body = $("body");
      var iframe = $("#lyris-content-frame", window.parent.document);

      iframe.animate({
        'height': body.height() + 50
      }, 200);
    });
  </script>
</head>
<body <?php print (!$is_html ? 'style="font-family: monospace;"' : ''); ?>>
  <?php print $content; ?>
</body>
</html>
