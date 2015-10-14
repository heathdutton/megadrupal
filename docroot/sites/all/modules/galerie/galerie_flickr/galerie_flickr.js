
(function($) {

Drupal.behaviors.galerie_flickr = {
  attach: function (context) {
    $('#edit-galerie-tags').keyup(function() {
      if ($('#edit-galerie-type-flickr-user:checked').size()) {
        if ($(this).val() == '') {
          $('.form-item-galerie-tags-operator').slideUp('fast');
        } else {
          $('.form-item-galerie-tags-operator').slideDown({
            duration: 'fast',
            easing: 'linear',
          });
        }
      }
    }).keyup();
  }
};


})(jQuery);

