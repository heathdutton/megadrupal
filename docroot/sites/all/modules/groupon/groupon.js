(function ($) {
  $(document).ready(function(){
    $('.groupon-row div a').each(function () {
      $(this).attr('target', '_blank');
    });
  });
})(jQuery);
