(function ($) {

  /**
   * Attaches our custom command to Drupal.ajax.prototype.commands
   */
  if (Drupal.ajax) {
    Drupal.ajax.prototype.commands.pagerViewFade = function (ajax, response, status) {

      // The parent selector of the content we're replacing
      var viewsDomId = ajax.selector;
      // The data being requested.
      var newContent  = response.data;
      // Duration for fadeTo
      var duration = response.settings.fade_duration;

      // When an ajax call is made we fadeOut the 'old' content
      // and then replace it with the 'new' content with a fadeIn.
      $(viewsDomId).fadeTo(duration, 0, function(){
        $(this).replaceWith(function() {
          return $(newContent).hide().fadeTo(duration, 1);
          // Attach our current command to the content that
          // has been loaded via Ajax.
          Drupal.attachBehaviors(this);
        });
      });

    };
  }

}) (jQuery);
