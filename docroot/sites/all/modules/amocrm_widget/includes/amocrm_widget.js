
var widgetEnv = {
  amoCrmDrupal: new amoCrmDrupal(),
  data: {}
};

widgetEnv.amoCrmDrupal.init(window.parent);

(function ($, widgetEnv) {
  Drupal.ajax.prototype.commands.amocrmWidgetOpenForm = function(ajax, data, status) {
    var form = {
      message: data.form.message,
      buttonConfirm: data.form.button_confirm,
      buttonCancel: data.form.button_cancel,
      fields: data.form.fields
    };

    widgetEnv.amoCrmDrupal.call('modalForm', form, function (formData) {

      if (typeof formData === 'object') {
        formData.callback = data.form.callback;
        formData.context = widgetEnv.data.card;
        var options = {
          url: Drupal.settings.basePath + 'amocrm_widget/form-submit',
          submit: formData
        };
        var ajax = new Drupal.ajax(false, false, options);
        ajax.eventResponse(ajax, {});
      }
    });
  };

  Drupal.ajax.prototype.commands.amocrmWidgetReload = function (ajax, data, status) {
    widgetEnv.amoCrmDrupal.call('reload');
  };

  Drupal.ajax.prototype.commands.amocrmWidgetResize = function (ajax, data, status) {
    widgetEnv.amoCrmDrupal.call('windowHeight', $('html').height());
  }

})(jQuery, widgetEnv);
