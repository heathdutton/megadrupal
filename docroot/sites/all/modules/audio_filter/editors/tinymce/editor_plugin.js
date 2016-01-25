(function ($) {
  tinymce.create('tinymce.plugins.audio_filter', {
    init : function(ed, url) {
      // Register commands
      ed.addCommand('mceAudioFilter', function() {
        ed.windowManager.open({
          file : Drupal.settings.audio_filter.url.wysiwyg_tinymce,
          width : 580,
          height : 480,
          inline : true,
          scrollbars : 1,
          popup_css : false
        }, {
          plugin_url : url
        });
      });

      // Register buttons
      ed.addButton('audio_filter', {
        title : 'Audio filter',
        cmd : 'mceAudioFilter',
        image : url + '/images/audio_filter.png'
      });
    },

    getInfo : function() {
      return {
        longname : 'Audio Filter',
        author : 'Audio Filter',
        authorurl : 'http://drupal.org/project/audio_filter',
        infourl : 'http://drupal.org/project/audio_filter',
        version : tinymce.majorVersion + "." + tinymce.minorVersion
      };
    }
  });
  // Register plugin
  tinymce.PluginManager.add('audio_filter', tinymce.plugins.audio_filter);
})(jQuery);
