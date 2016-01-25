(function($) {

Drupal.behaviors.webform_steps = {};
Drupal.behaviors.webform_steps.attach = function(context, settings) {

$('.webform-client-form', context).each(function() {
  var $form = $(this);
  var $steps = $form.find('.webform-progressbar .webform-progressbar-page');
  $form.find('.webform-steps-buttons input.step-button').each(function(i) {
    var $button = $(this);
    if ($button.is(':enabled')) {
      $($steps[i]).click(function(event) {
        $button.click();
      }).addClass('clickable').css('cursor', 'pointer');
    }
  });
});

}
})(jQuery);
