(function ($) {

  window.ExceptionNotifier = {};
  $.extend(ExceptionNotifier, {
    notify: function (e) {
      if ( typeof(ExceptionNotifierOptions) !== 'undefined' && (typeof(ExceptionNotifierOptions.logErrors) !== 'undefined') && !ExceptionNotifierOptions.logErrors ) { return; }

      e = this.formatStringError(e);
      e['Browser'] = navigator.userAgent;
      e['Page'] = location.href;

      this.send(e);
      return false;
    },

    send: function(error) {
      if (typeof (Drupal.settings.js2watchdog.token) != 'undefined') {
        
        var params = [];
        for (var attr in error) {
          params.push(attr + "=" + encodeURIComponent(error[attr]) + '');
        }
        js2watchdog.woof(params.join("&"));
      }
    },

    // If error is a string, convert it to a stub of error object
    formatStringError: function (error) {
      if (typeof error == 'string') {
        var old = error;
        error = {
          toString: function () { return old; }
        };
        error.message = old;
      }
      return error;
    },

    // Listens window.onError
    errorHandler: function(msg, url, l) {
      var e = {
        message: msg,
        fileName: url,
        lineNumber: l
      };
      ExceptionNotifier.notify(e);
      return false;
    },

    // Removes listener
    killEvent: function (event) {
      if (!event) { return; }
      event.cancelBubble = true;
      if (event.stopPropagation) { event.stopPropagation(); }
      if (event.preventDefault) { event.preventDefault(); }
    }
  });
  window.onerror = ExceptionNotifier.errorHandler;

})(jQuery);

var  js2watchdog = {};
js2watchdog.woof = function(message, type){
  if (typeof type == 'undefined') {
    type = 'exception';
  }
  message += '&type=' + type;
  message += '&token=' + Drupal.settings.js2watchdog.token;
  
  jQuery.ajax({
    url: Drupal.settings.basePath + 'js2watchdog/exception',
    type: 'POST',
    data: message
  });
}

/**
 * Override default Drupal.attachBehaviors function to wrap all behaviors with try-catch
 * Required to log errors in browsers which not support window.onerror
 */
Drupal.attachBehaviors = function(context, settings) {
  context = context || document;
  settings = settings || Drupal.settings;
  try {
    // Execute all of them.
    jQuery.each(Drupal.behaviors, function () {
      if (jQuery.isFunction(this.attach)) {
        this.attach(context, settings);
      }
    });
  } catch (e) {
    if (/Opera|WebKit/.test(navigator.userAgent)) {
      ExceptionNotifier.notify(e);
    }
    throw(e);
  }
};
