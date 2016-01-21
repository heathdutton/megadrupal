jQuery(document).ready(function($) {
  $('.comment-length-limiter').hide();
  $('textarea[comment_max_length]').bind('textchange',function(event, previousText) {
      if ($(this).val() != '') {
        $('.comment-length-limiter:hidden').fadeIn('slow');
        if ($(this).attr('comment_max_length') - $(this).val().length <= 0) {
          $(this).val($(this).val().substr(0, $(this).attr('comment_max_length')));
        }
        $('.comment-length-limiter-counter').html($(this).attr('comment_max_length') - $(this).val().length + '');
      }
      else {
        $('.comment-length-limiter:visible').fadeOut('slow');
      }
  });
});
