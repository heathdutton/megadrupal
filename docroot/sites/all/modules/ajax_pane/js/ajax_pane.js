(function ($) {

  /**
   * Load Panels panes via ajax.
   */
  Drupal.behaviors.ajaxPaneLoad = {
    attach: function (context, settings) {

      $('.ajax-pane').once('ajax-pane-processed', function() {

        var element = jQuery(this);
        var url = element.attr('src');

        jQuery.ajax({
          url: url,
          type: 'GET',
          dataType: 'html',
          success: function (response) {
            element.replaceWith(response);
          }
        });
      });

    }
  };

})(jQuery);
