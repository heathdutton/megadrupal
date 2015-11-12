/*
 * @file
 * js file to load silimar titles on leaving the title field while adding new content.
 */
(function($) {
  Drupal.behaviors.similarTitlesBehavior = {
    attach: function(context, settings) {
      $('#ajax-title-div input').bind('keyup paste blur', function(e) {
        if(e.type == 'keyup'){
          if(e.keyCode != 32){
            return;
          }
        }
        var title = this.value;
        // getting base path defined in alter form
        var basep = $('#ajax-title-div span.base_path').html();
        // getting type of node being created
        var type = $('#ajax-title-div span.type').html();
        $.ajax({
          // ajax callback defined in .module file
          url: basep + "node/ajax/title",
          cache: false,
          type: 'post',
          data: {
            title: title,
            type: type
          },
          success: function(data) {
            $('#ajax-title-div #ajax-title-container').html('');
            $('#ajax-title-div #ajax-title-container').removeClass('ajax-title-container');
            // checking if duplicate content exists
            if (data) {
              $('#ajax-title-div #ajax-title-container').addClass('ajax-title-container');
              $('#ajax-title-div #ajax-title-container').html(data);
              // clicking on close icon
              $('.ajax-title-div-close a').click(function() {
                $('#ajax-title-div #ajax-title-container').removeClass('ajax-title-container');
                $('#ajax-title-div #ajax-title-container').html('');
              });
            }
          }
        });
      });
    }
  };
})(jQuery);
