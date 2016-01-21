(function (window, $, widgetEnv) {

  // Init widget
  var pageLoadPromise =  pageLoadPromise || jQuery.Deferred();
  widgetEnv.amoCrmDrupal.bind('pageLoad', function(data) {
    pageLoadPromise.resolve(data);

    return '';
  });

  /**
   * Replace preloader.
   */
  Drupal.behaviors.amocrmWidgetPreloadReplace = {
    attach: function (context, settings) {
      $(document).on('click', function() {
        widgetEnv.amoCrmDrupal.call('windowHeight', $('html').height());
      });

      pageLoadPromise.then(function(data) {
        widgetEnv.data = data.data;

        var options = {
          url: Drupal.settings.basePath + 'amocrm_widget/widget-page',
          submit: {
            page_type: data.data.card.card_entity,
            context: data.data.card
          }
        };
        var ajax = new Drupal.ajax(false, false, options);
        ajax.eventResponse(ajax, {});
      });
    }
  };

})(window, jQuery, widgetEnv);
