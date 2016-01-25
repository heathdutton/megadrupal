(function ($) {

  Drupal.behaviors.amocrmWidgetMailForm = {
    attach: function (context, settings) {
      $('#drupal-send-mail').on('click', function(element) {
        var options = {
          url: Drupal.settings.basePath + 'amocrm-widget-mail/email-form',
          submit: {
            card_id: widgetEnv.data.card.card_id,
            card_entity: widgetEnv.data.card.card_entity,
            context: widgetEnv.data.card
          }
        };
        var ajax = new Drupal.ajax(false, false, options);
        ajax.eventResponse(ajax, {});

        return false;
      });
    }
  };

})(jQuery);
