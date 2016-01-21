/**
 * @file
 * jQuery part of RC4 email decrypts email address in a.rc4_email elements.
 *
 * The mailto link and its link text are encrypted in attributes data-emailurl
 * and data-textenc in all a.rc4_email elements. This uses Javascript implementation
 * of RC4 to decrypt and restore the mailto link and text to its original
 * plaintext.
 */
(function ($) {
  Drupal.behaviors.rc4_email = {
    attach:function () {
      if (typeof Drupal.settings.rc4_email !== 'undefined') {
        if (!window.btoa) {
          window.btoa = base64.encode;
        }
        if (!window.atob) {
          window.atob = base64.decode;
        }

        $("a.rc4_email").each(function (i,e) {
          var $this = $(this);
          var emailurl = $this.attr("data-emailurl");
          var textenc = $this.attr("data-textenc");
          $this.removeAttr("data-emailurl");
          $this.removeAttr("data-textenc");
          $this.removeClass("rc4_email");
          $this.attr("href", _rc4_email_rc4(Drupal.settings.rc4_email.key, window.atob(emailurl)));
          $this.html(_rc4_email_rc4(Drupal.settings.rc4_email.key, window.atob(textenc)));
        });
      }
    }
  }
})(jQuery);
