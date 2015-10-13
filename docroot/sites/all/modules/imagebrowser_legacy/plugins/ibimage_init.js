// $Id: ib_wysiwyg_init.js,v 1.1.2.4 2010/05/07 05:34:33 blixxxa Exp $
(function ($) {

/**
 * @file ib_wysiwyg_init.js
 *
 * All the javascript for the WYSIWYG API plugin for Image Browser.
 */

var instanceId = null;

/**
 * Open jQuery UI dialog box.
function ib_wysiwyg_dialog_open(instance) {
  instanceId = instance;
  $("body").append("<div id='ib_dialog'><iframe id='ib_dialog_contents' src='"+Drupal.settings.imagebrowser.basepath + "?q=imagebrowser/ib_wysiwyg' width='602' height='446' frameborder='0' /></div>");
  $("#ib_dialog").dialog({ modal: true, height: 476, width: 602, title: 'Image Browser', beforeclose: function(event, ui) { ib_wysiwyg_dialog_close(); } }).show();
}
 */

/**
 * Close jQuery UI dialog box.
function ib_wysiwyg_dialog_close() {
  $("#ib_dialog").remove();
}
 */

/**
 * Define WYSIWYG API plugin.
 */
Drupal.wysiwyg.plugins.ibimage = {

  /**
   * Return whether the passed node belongs to this plugin.
   */
  isNode: function(node) {
    return ($(node).is('img.ibimage'));
  },

  /**
   * Execute the button.
  invoke: function(data, settings, instanceId) {
    ib_wysiwyg_dialog_open(instanceId);
  },
   */

  /**
   * Replace all [ibimage] tags with images.
   * FID == Image Preset == Link Preset == Link Target == Styles
   */
  attach: function(content, settings, instanceId) {
    content = content.replace(/\[ibimage\=\=([^\[\]]+)\]/g, function(orig, match) {
      var node = {}
      return ibimage_decode(match, node);
    });
    return content;
  },

  /**
   * Replace images with [ibimage] tags in content upon detaching editor.
   */
  detach: function(content, settings, instanceId) {
    var $content = $('<div>' + content + '</div>'); // No .outerHTML() in jQuery :(
    $('img.ibimage', $content).each(function(node) {
      var data = this.title.split("==");
      var inlineTag = '[ibimage==' + data[0] + '==' + data[1] + '==' + data[2] + '==' + data[3] + '==' + data[4] + ']';
      $(this).replaceWith(inlineTag);
    });
    return $content.html();
  }
};

/**
 * Decode an [ibimage] tag and wait for response.
 */
function ibimage_decode(tag, node) {
  var ibdata = null;
  $.ajax({
      type: "POST",
      url: Drupal.settings.basePath + 'index.php?q=imagebrowser_legacy/ajax&tag=[ibimage==' + tag + ']',
      contentType: "application/json; charset=utf-8",
      dataType: "json",
      data: {},
      cache: true,
      async: false,
      success: function(data, textStatus) {
        ibdata = data;
      }
  });
  node.alt = ibdata.link_preset;
  node.target = ibdata.link_target;
  node['class'] = ibdata.styles;
  return '<img src="'+ibdata.image_url+'" alt="IB Image" title="'+ibdata.fid+'=='+ibdata.preset+'=='+ibdata.link_preset+'=='+ibdata.link_target+'=='+ibdata.styles+'" class="ibimage '+ibdata.styles+'" />';
} 
})(jQuery);
