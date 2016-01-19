/**
 * @file
 *
 * Implements the trigger called by insert.js to alter the output when a user
 * click on "Insert" beside a valid markdown insert widget.
 */

(function ($) {
  $(document).bind('insertIntoActiveEditor', function(event, options) {

    // Modify to markdown output based on the widget type.
    switch (options.widgetType) {

      // Generic files.
      case 'file_generic':

        // Attributes that are needed for the new markdown file output.
        var href = $(options.content).attr('href') || "";
        var description = options.fields.description || options.filename || "";

        // Update the output in the form [Description](href).
        options.content = "[" + description + "](" + href + ")";

      break;

      // Images
      case 'image_image':

        // Attributes that are needed for the new markdown image output.
        var src = $(options.content).attr('src') || "";
        var alt = options.fields.alt || "";
        var title = options.fields.title? ' "' + options.fields.title + '"' : "";
        var link = options.fields.link || false;

        // Markdown image format is ![Alt Text](/path/to/file "Optional Title").
        var image = "![" + alt + "](" + src + title + ")";

        // Markdown line format is [Link Content](/link/href "Optional Title").
        options.content = link? "[" + image + "](" + link + title + ")" : image;

       break;
    }
  });
})(jQuery);
