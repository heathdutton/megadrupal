/**
 * @file
 * Show/hide functionality for SimpleMeta on-page form.
 */

(function ($) {
  var formHidden = false;
  Drupal.behaviors.simplemeta = {
    attach: function (context) {
      var forms = $('#simplemeta-meta-form.simplemeta-meta-form-ajax:not(.simplemeta-processed)', context);
      forms.each(function (index) {

        var form = $(this),
            close = $('<span class="form-close"></span>').prependTo(form);
        close.text(Drupal.t('Meta'));
        if (!formHidden) {
          form.addClass('simplemeta-hidden').css({left: (-form.outerWidth()) + 'px'});
          formHidden = true;
        }

        close.click(function (event) {
          var $this = $(this);
          if (form.hasClass('simplemeta-hidden')) {
            form.stop(true).animate({left: 0});
            $this.text(Drupal.t('Close'));
          }
          else {
            form.stop(true).animate({left: -form.outerWidth()});
            $this.text(Drupal.t('Meta'));
          }
          form.toggleClass('simplemeta-hidden');
        });
      });
      forms.addClass('simplemeta-processed');
    }
  };
})(jQuery);
