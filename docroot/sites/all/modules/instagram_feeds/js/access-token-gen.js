/**
 * @file
 * Instagram Feeds access-token generation js.
 */

(function($) {
  Drupal.behaviors.instagramAccessTokenGen = {
    attach: function() {
      // Create URL for generating access-token.
      var clientID = $('#edit-instagram-feeds-client-id');
      var form = clientID.closest('form');
      var link = $('#edit-instagram-feeds-access-token').closest('div.form-item').find('a');
      link.attr('target', '');
      if (!url) {
        var url = link.attr('href');
      }
      link.attr('href', url + '&client_id=' + clientID.val());
      clientID.keyup(function() {
        link.attr('href', url + '&client_id=' + $(this).val());
      });
      clientID.mouseup(function() {
        link.attr('href', url + '&client_id=' + $(this).val());
      });

      // Autosubmit form when click on link.
      link.click(function(e) {
        $('#edit-instagram-feeds-access-token').val('');
        form.submit();
        return false;
      });
    }
  };
})(jQuery);
