/**
 * @file
 * The main javascript file for the region_tokens module
 *
 * @ingroup region_tokens
 * @{
 */

(function ($) {
  Drupal.regionTokens = Drupal.regionTokens || {};

  /**
  * Core behavior for region_tokens.
  */
  Drupal.behaviors.regionTokens = Drupal.behaviors.regionTokens || {};
  Drupal.behaviors.regionTokens.attach = function (context, settings) {
    function hideShowToken($input) {
      var state = $input.find('input').is(':checked');
      if (state) {
        $input.find('code').show();
      }
      else {
        $input.find('code').hide();
      }
    }

    var $form = $('#region-tokens-settings');
    $form.find('.form-type-checkbox')
    .each(function() {
      hideShowToken($(this));
    })
    .click(function(){
      hideShowToken($(this));
    })
  };

  /**
  * @} End of "defgroup region_tokens".
  */

})(jQuery);
