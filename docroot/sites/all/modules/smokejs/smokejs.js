(function ($) {

/**
 * Command to provide an smoke.js signal.
 */
Drupal.ajax.prototype.commands.smokeSignal = function (ajax, response, status) {
  smoke.signal(response.text, function(e){}, response.settings);
}

Drupal.behaviors.smokeJs = {
  attach: function (context, settings) {
    for (key in settings.ajax) {
      /**
       * Override Drupal ajax error
       */
      Drupal.ajax[key].error = function (response, uri) {
        smoke.alert(Drupal.ajaxError(response, uri));
        // Remove the progress element.
        if (this.progress.element) {
          $(this.progress.element).remove();
        }
        if (this.progress.object) {
          this.progress.object.stopMonitoring();
        }
        // Undo hide.
        $(this.wrapper).show();
        // Re-enable the element.
        $(this.element).removeClass('progress-disabled').removeAttr('disabled');
        // Reattach behaviors, if they were detached in beforeSerialize().
        if (this.form) {
          var settings = response.settings || this.settings || Drupal.settings;
          Drupal.attachBehaviors(this.form, settings);
        }
      }

      /**
       * Override Drupal ajax command alter
       */
      Drupal.ajax[key].commands.alert = function (ajax, response, status) {
        smoke.alert(response.text);
      }
    };
  }
};

})(jQuery);
