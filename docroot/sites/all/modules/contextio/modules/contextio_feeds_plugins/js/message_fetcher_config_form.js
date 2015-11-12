/**
 * @file
 * Custom scripts for ContextIOMessageFetcher's config form.
 */

(function($) {

  Drupal.behaviors.contextIOMessageFetcherConfigForm = {
    attach: function (context, settings) {

      // Show warning message about the add new mailbox functionality.
      $('input[name="mailbox[add_new_mailbox]"]').change(function(){
        $('#add-new-mailbox-message').toggle(0);
      });

    }
  };

})(jQuery);
