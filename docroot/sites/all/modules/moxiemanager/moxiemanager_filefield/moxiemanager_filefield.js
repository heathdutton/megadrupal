(function ($) {
  /**
   * Attached the filemanager opener behavior to CCK filefields.
   */
  Drupal.behaviors.uploadField = {
    attach: function (context) {
      $('.filefield-filemanager-opener', context).once('filefield-filemanager-opener').each(function() {
        // Reference variables.
        var $select_submit = $(this);
        var $wrapper = $select_submit.parents('.form-managed-file').addClass('filefield-filemanager-opener-widget-edit');
        var $path_input = $('input.filemanager-path', $wrapper);
        var $upload_input = $('input[type="file"]', $wrapper);
        var $upload_submit = $('input[name$="upload_button"]', $wrapper);

        var index = $("input[name*='filefield_opener_active_tab']").val();

        // Create the links for the tabs.
        var $link1 = $('<a href="#" />').text(Drupal.t('Upload new file'));
        var $link2 = $('<a href="#" />').text(Drupal.t('Select existing file'));

        // Create the tabs warpper and its tabs.
        var $tabs = $('<ul />')
          .addClass('filefield-filemanager-opener-tabs')
          .append($('<li />').append($link1))
          .append($('<li />').append($link2))
          .appendTo($wrapper);

        // Create the tab content wrappes and place the appropriate contents
        // inside it.
        var $wrapper1 = $('<div />')
          .addClass('tab-content')
          .append($upload_input)
          .append($upload_submit);
        var $wrapper2 = $('<div />')
          .addClass('tab-content')
          .append($path_input.parent())
          .append($select_submit.parent());

        // Place the tab content wrappers inside the widget wrapper.
        $wrapper.append($('<div />').append($wrapper1).append($wrapper2));

        // Create a browse button.
        // This button will open the Moxiecode file/image browser.

        $filemanager_opener = $('<a href="#" />')
          .addClass('filefield-filemanager-opener-link')
          .text(Drupal.t('Browse'))
          .insertAfter($path_input)
          .bind('click', filefieldFileManager.openBrowser);

        // Bind the click events to the tab links and trigger the event for the
        // first tab.
        $link1.bind('click', filefieldFileManager.openTab);
        $link2.bind('click', filefieldFileManager.openTab);
        $('a', $tabs).eq(1).trigger('click');
      });
    }
  };

  var filefieldFileManager = {

    /**
     * Opens the Imagemanager or Filemanager browser window after clicking the
     * browse button.
     */
    openBrowser: function(e) {
      e.preventDefault();

      var $trigger = $(this);
      var $wrapper = $trigger.parents('.tab-content');
      var $widget = $trigger.parents('.widget-edit');
      var field_id = $('input[name*="[filemanager][path]"]', $wrapper).attr('id');
      var rootpath = $('input[name*="[filemanager][file_system_rootpath]"]', $wrapper).attr('value');

  		window.moxman.browse({
		    no_host: true,
		    relative_urls: true,
		    document_base_url: Drupal.settings.basePath,
		    fields: field_id,
  		  path: rootpath,
  		  rootpath: rootpath,
  		  oninsert: function(args) {
      	  //alert(args.files[0].url);
         }
      });
    },

    /**
     * Shows a tabs content after clicking a tab.
     */
    openTab: function(e) {
      e.preventDefault();

      var $trigger = $(this);
      var $widget = $trigger.parents('.widget-edit');
      var $tab_wrapper = $trigger.parents('.filefield-filemanager-opener-widget-edit');
      var index = $('a', $trigger.parents('ul')).index($trigger);

      // Hide all tabs content wrappers before showing the one whith the same
      // index as the clicked tab.
      $('.tab-content', $tab_wrapper).hide();
      $('.tab-content', $tab_wrapper).eq(index).fadeIn('fast');

      // Toggle classes.
      $('li', $trigger.parents('ul')).removeClass('active');
      $trigger.parent().addClass('active');

      // Store the clicked tab index so it can be reused after AHAH refreshes.
      $('input[id~="-filefield-opener-active-tab"]', $widget).val(index);
    },

  };

  // [name] is the name of the event "click", "mouseover", ..
  // same as you'd pass it to bind()
  // [fn] is the handler function
  $.fn.bindFirst = function(name, fn) {
    if (this.size()) {
      // bind as you normally would
      // don't want to miss out on any jQuery magic
      this.bind(name, fn);

      // Thanks to a comment by @Martin, adding support for
      // namespaced events too.
      var handlers = this.data('events')[name.split('.')[0]];
      // take out the handler we just inserted from the end
      var handler = handlers.pop();
      // move it at the beginning
      handlers.splice(0, 0, handler);
    };
  };

})(jQuery);
