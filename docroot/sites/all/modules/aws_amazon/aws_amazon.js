(function($) {
  Drupal.behaviors.aws_amazon_search = {
    attach: function(context) {
      if($('#search').length > 0) {
        $('#search').bind('keyup', searchText);
        // $('#search').bind('change', searchText);
      }
    }
  };
  Drupal.behaviors.aws_amazon_browse = {
    attach: function(context) {
      if($('#edit-btn-browse').length > 0) {
        $('#edit-btn-browse').bind('click', function(){
          if ($('#edit-bucket').val() != '') {
            window.location.href = Drupal.settings.aws_amazon['url'] + $('#edit-bucket').val();
            return false; 
          }
        });
      }
    }
  };
  var searchText = function() {
    var val = $(this).val().toLocaleLowerCase();
    if (val == '') {
      $('li.hidden').removeClass('hidden');
    }
    else {
      $('#bucket-browser a.search:not([key*="' + val + '"])').parent().addClass('hidden');
      $('#bucket-browser a.search[key*="' + val + '"]').parent().removeClass('hidden');
    }
  };
})(jQuery);
