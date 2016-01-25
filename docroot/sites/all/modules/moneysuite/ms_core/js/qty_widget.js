(function ($) {
  $(function(){
    $(".qty_inc").show();
    $(".qty_dec").show();
    $('.qty_inc').click(function(e){
      e.preventDefault();
      $(':text[name="qty"]').val( Number($(':text[name="qty"]').val()) + 1 );
    });
    $('.qty_dec').click(function(e){
      e.preventDefault();
      if (Number($(':text[name="qty"]').val()) > 1) {
        $(':text[name="qty"]').val( Number($(':text[name="qty"]').val()) - 1 );
      }
    });
  });
})(jQuery);
