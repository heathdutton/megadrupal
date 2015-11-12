(function ($) {
  // This is a modified version of the core Drupal.ajax.prototype.beforeSend()
  // so that we can control the throbber behaviour better when displaying the
  // reminder messages. See misc/ajax.js.
  //
  // All original behaviour is preserved, meaning we shouldn't break any
  // existing AJAX functionality, we just alter the way the throbber works for
  // specific URLs related to this module.
  Drupal.ajax.prototype.beforeSend = function(xmlhttprequest, options) {
    if (this.form) {
      options.extraData = options.extraData || {};
      options.extraData.ajax_iframe_upload = '1';
      var v = $.fieldValue(this.element);
      if (v !== null) {
        options.extraData[this.element.name] = Drupal.checkPlain(v);
      }
    }

    $(this.element).addClass('progress-disabled').attr('disabled', true);

    if (this.progress.type == 'bar') {
      var progressBar = new Drupal.progressBar('ajax-progress-' + this.element.id, eval(this.progress.update_callback), this.progress.method, eval(this.progress.error_callback));
      if (this.progress.message) {
        progressBar.setProgress(-1, this.progress.message);
      }
      if (this.progress.url) {
        progressBar.startMonitoring(this.progress.url, this.progress.interval || 1500);
      }
      this.progress.element = $(progressBar.element).addClass('ajax-progress ajax-progress-bar');
      this.progress.object = progressBar;
      $(this.element).after(this.progress.element);
    }
    else if (this.progress.type == 'throbber') {
      this.progress.element = $('<div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div></div>');
      if (this.progress.message) {
        $('.throbber', this.progress.element).after('<div class="message">' + this.progress.message + '</div>');
      }
      // Target module defined URLs and alter the throbber placement
      if (/\d+\/reminder\/\d+\/suppress|deactivate\/ajax\//.test(options.url)) {
        $('.reminder-status-wrapper').html(this.progress.element);
      }
      else {
        $(this.element).after(this.progress.element);
      }
    }
  }

})(jQuery);