(function (window, $, widgetEnv) {

  /**
   * Replace preloader.
   */
  Drupal.behaviors.amocrmWidgetPreloadReplace = {
    attach: function (context, settings) {
      widgetEnv.amoCrmDrupal.bind('pageLoad', function(data) {
        widgetEnv.data = data.data;

        var options = {
          url: Drupal.settings.basePath + 'amocrm_widget/widget-page',
          submit: {
            "page_type": data.data.card.card_entity
          }
        };
        var ajax = new Drupal.ajax(false, false, options);
        ajax.eventResponse(ajax, {});
      });
    }
  };

})(window, jQuery, widgetEnv);
