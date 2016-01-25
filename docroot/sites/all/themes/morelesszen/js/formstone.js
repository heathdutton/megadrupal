(function($) {
Drupal.behaviors.formstone = {};
Drupal.behaviors.formstone.attach = function(context, settings) {
  if ($.fn.selecter) {
    $('.webform-client-form select', context).selecter();
  }
  if ($.fn.picker) {
    $('.webform-client-form input[type=checkbox]', context).picker();
    $('.webform-client-form input[type=radio]', context).picker();
  }
  if ($.fn.filer) {
    $('.webform-client-form input[type=file]', context).filer();
  }
};
})(jQuery);
