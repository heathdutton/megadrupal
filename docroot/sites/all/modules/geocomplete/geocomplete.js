/**
 * @file
 * A JavaScript file for the Geocomplete module.
 */
(function ($, Drupal, window, document, undefined) {

  Drupal.behaviors.geocomplete = {
    attach: function (context, settings) {

      $('.geocomplete-input').each(function(){
        $(this).geocomplete({
          details: $(this).parents('form').find('[data-geocomplete-src="' + $(this).attr('name') + '"]'),
          detailsAttribute: "data-geocomplete-type"
        });
      });

    }
  };

})(jQuery, Drupal, this, this.document);