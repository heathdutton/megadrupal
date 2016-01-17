// $Id$
(function ($) {
  $(document).ready(function() {
    $('#edit-search-block-form--2').click(function() {
      if ($(this).val() == Drupal.t('Search')) {
        $(this).val('');
      }
    });
    $('#edit-search-block-form--2').blur(function() {
      if ($(this).val() == '') {
        $(this).val(Drupal.t('Search'));
      }
    });
  });
})(jQuery);