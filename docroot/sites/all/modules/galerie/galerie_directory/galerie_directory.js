(function($) {

Drupal.behaviors.galerie_directory = {
  attach: function (context) {
    $('#edit-galerie-create-directory').change(function() {
      if ($(this).attr('checked')) {
        $('#edit-galerie-directory-details').slideUp('fast');
      } else {
        $('#edit-galerie-directory-details').slideDown({
          duration: 'fast',
          easing: 'linear',
        });
      }
    }).change();
  }
};


})(jQuery);
