(function($){
  $('.form-item-cheetahmail-nl-start-month-day').hide();
  $('.form-item-cheetahmail-nl-start-week-day').hide();
  $('.form-item-cheetahmail-nl-start-hour').hide();
  $(function(){
    $('#edit-cheetahmail-nl-sending-interval').change(function(){
      var interval = parseInt($('#edit-cheetahmail-nl-sending-interval option:selected').val());
      switch (interval) {
        case 1:
          $('.form-item-cheetahmail-nl-start-month-day').hide();
          $('.form-item-cheetahmail-nl-start-week-day').hide();
          $('.form-item-cheetahmail-nl-start-hour').show();
          break;
        case 7:
          $('.form-item-cheetahmail-nl-start-month-day').hide();
          $('.form-item-cheetahmail-nl-start-week-day').show();
          $('.form-item-cheetahmail-nl-start-hour').show();
          break;
        case 30:
          $('.form-item-cheetahmail-nl-start-month-day').show();
          $('.form-item-cheetahmail-nl-start-week-day').hide();
          $('.form-item-cheetahmail-nl-start-hour').show();
          break;
          
        default:
          alert('hello');
          break;
      }      
    }).change();
  });
})(jQuery);
