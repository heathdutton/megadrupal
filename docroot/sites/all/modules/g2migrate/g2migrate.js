/**
 * Prettify slect forms in the breadcrumb.
 */
(function ($) {
  $(document).ready(function() {
    $(".breadcrumb form input.form-submit").hide();
    $(".breadcrumb form select").change(function() {
      window.location.href = $(this).val();
    });
  });
})(jQuery);
