-- INSTALLATION --

1 Donwload and install wysisyg (https://www.drupal.org/project/wysiwyg) and
  markdown (https://www.drupal.org/project/markdown) module.
2 Install EpicEditor (http://www.webwash.net/tutorials/using-epiceditor-drupal-7
  for instruction) with next steps:
  2.1 Download EpicEditor (http://oscargodson.github.com/EpicEditor/ tested
      with version 0.2.2) and extract the zip file into
      sites/all/libraries/epiceditor.
  2.2 Go to Configuration -> "Wysiwyg profiles" (admin/config/content/wysiwyg).
  2.3 Select "EpicEditor *.*.*" from the editor drop-down within the "Filtered
      HTML" row then click on Save.
  2.4 Go to Configuration -> "Text formats" (admin/config/content/formats) and
      click on the configure link within the "Filtered HTML" row.
  2.5 Remove the "Convert line breaks into HTML" and "Convert URLs into links"
      filters.
  2.6 Add the Markdown filter and make sure that the filter is above the "Limit
      allowed HTML tags" filter.
  2.7 For Markdown to work properly, we need to add a few more tags to the
      "Allowed HTML tags" text field in the "Limit allowed HTML tags" filter
      settings. Copy the tags below into the "Allowed HTML tags" text field:
      <a> <em> <strong> <cite> <blockquote> <code> <ul> <ol> <li> <dl> <dt>
      <dd> <p> <br> <img> <table> <tr> <td> <th> <tbody>
3 Download the highlight script (https://highlightjs.org/download/
  tested with version 8.5) and extract in sites/all/libraries.
4 Enable CHEE module.
