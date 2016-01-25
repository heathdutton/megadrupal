/**
 * @file
 * This file contains all JQuery based functionality for inline images.
 */
(function ($) {

  console.log(Drupal.settings);

  tinymce.create('tinymce.plugins.openlucius_inline_image', {

    /**
     * Initializes the plugin, this will be executed after the plugin has been created.
     * This call is done before the editor instance has finished it's initialization so use the onInit event
     * of the editor instance to intercept that event.
     *
     * @param {tinymce.Editor} ed Editor instance that the plugin is initialized in.
     * @param {string} url Absolute URL to where the plugin is located.
     */
    init: function (ed, url) {

      // Register the command so that it can be invoked
      // using tinyMCE.activeEditor.execCommand('openluciusUploadFile');
      ed.addCommand('openluciusUploadFile', function() {

        // Create input element.
        var input = $(document.createElement('input'));

        // Set it to file type.
        input.attr('type', 'file');

        // Bind change event to the input element.
        input.bind('change', function (e) {

          // Add the throbber so people know they must wait.
          $(this).addThrobber();

          var file = this.files[0];
          var xhr = new XMLHttpRequest();
          (xhr.upload || xhr).addEventListener('progress', function (e) {
            var done = e.position || e.loaded
            var total = e.totalSize || e.total;
            console.log('xhr progress: ' + Math.round(done / total * 100) + '%');
          });

          // Respond to the upload complete.
          xhr.addEventListener('load', function (e) {

            // Parse the response.
            var response = JSON.parse(this.responseText);

            // Validate the response.
            if (response.hasOwnProperty('image') && response.image !== 'false' && response.image !== false) {

              // Remove the throbber.
              $(this).removeThrobber();

              // Append that awesome image.
              ed.execCommand('mceInsertContent', false, response.image);
            }
            else if (response.hasOwnProperty('error') && response.error !== 'false' && response.error !== false) {

              // Remove the throbber.
              $(this).removeThrobber();
              alert(response.error);
            }
          });
          var formData = new FormData();
          formData.append("files", file);
          xhr.open('post', Drupal.settings.basePath + 'openlucius-inline-images-attach-file/' + Drupal.settings.openlucius_inline_images.group + '/' + Drupal.settings.openlucius_inline_images.token, true);
          xhr.send(formData);
        });

        // Click it for the dialog.
        input.click();
        return false;
      });

      // Register example button
      ed.addButton('openlucius_inline_image', {
        title: 'OpenLucius Inline Image',
        cmd: 'openluciusUploadFile',
        image: url + '/img/image.gif'
      });
    },

    /**
     * Creates control instances based in the incomming name. This method is normally not
     * needed since the addButton method of the tinymce.Editor class is a more easy way of adding buttons
     * but you sometimes need to create more complex controls like listboxes, split buttons etc then this
     * method can be used to create those.
     *
     * @param {String} n Name of the control to create.
     * @param {tinymce.ControlManager} cm Control manager to use inorder to create new control.
     * @return {tinymce.ui.Control} New control instance or null if no control was created.
     */
    createControl: function (n, cm) {
      return null;
    },

    /**
     * Returns information about the plugin as a name/value array.
     * The current keys are longname, author, authorurl, infourl and version.
     *
     * @return {Object} Name/value array containing information about the plugin.
     */
    getInfo: function () {
      return {
        longname: 'OpenLucius Inline Image plugin',
        author: 'Lucius BV / Erik de Kamps, Webdeveloper',
        authorurl: 'http://openlucius.com',
        infourl: 'mailto:info@openlucius.nl',
        version: '1.0'
      };
    }
  });

  // Register plugin
  tinymce.PluginManager.add('openlucius_inline_image', tinymce.plugins.openlucius_inline_image);

})(jQuery);
